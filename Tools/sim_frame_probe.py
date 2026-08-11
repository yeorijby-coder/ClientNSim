# -*- coding: utf-8 -*-
"""
시뮬레이터가 이상한 통신 프레임에 죽지 않는지 확인하는 시험.

    CvSim / ScSim 이 통신 중에 반복해서 죽던 일이 있었다.
      - 온전하지 않은 프레임을 길이 확인 없이 짚어 배열 밖을 건드림
      - 프레임을 받는 사이에 상대가 끊으면 NULL 이 된 소켓을 건드림
      - 온전한 요청을 받고 답을 안 보내면 소켓 계층이 무너짐
    고친 뒤에도 계속 버티는지 이걸로 확인한다.

쓰는 법
    python sim_frame_probe.py 9201 60 CvSim      CvSim PLC02
    python sim_frame_probe.py 8104 60 ScSim      ScSim 904호기

    포트  CvSim 은 9n01 (PLC n), ScSim 은 81nn (900+nn 호기)
    횟수  각 경우를 몇 번씩 던질지 (기본 60)
"""
import socket, struct, subprocess, sys, time

sys.stdout.reconfigure(encoding='utf-8')
HOST = '127.0.0.1'
PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 9201
N = int(sys.argv[2]) if len(sys.argv) > 2 else 60
EXE = (sys.argv[3] if len(sys.argv) > 3 else 'CvSim') + '.exe'
# 두들긴 뒤 정상 읽기를 확인할 주소. 시뮬레이터마다 살아 있는 자리가 다르다.
PROBE_ADDR = 101 if EXE.startswith('ScSim') else 557


def frame(cmd, addr, npts, payload=b''):
    body = struct.pack('<H', 0x0010) + struct.pack('<H', cmd) + struct.pack('<H', 0)
    body += struct.pack('<I', addr)[:3] + bytes([0xA8]) + struct.pack('<H', npts)
    body += payload
    return b'\x50\x00\x00\xFF\xFF\x03\x00' + struct.pack('<H', len(body)) + body


def hit(data, cut=None, linger0=False):
    try:
        s = socket.create_connection((HOST, PORT), timeout=2)
        if linger0:      # RST 로 즉시 끊는다
            s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER, struct.pack('ii', 1, 0))
        s.sendall(data if cut is None else data[:cut])
        s.close()
    except Exception:
        pass


full_read = frame(0x0401, 557, 1)
full_write = frame(0x1401, 557, 1, struct.pack('<H', 127))

cases = [
    ('헤더가 덜 온 조각 (9바이트)', lambda: hit(full_read, cut=9)),
    ('헤더만 보내고 끊기 (15바이트)', lambda: hit(full_read, cut=15)),
    ('쓰기 헤더만, 본문 없이 끊기', lambda: hit(full_write, cut=21)),
    ('보내자마자 RST 로 끊기', lambda: hit(full_read, linger0=True)),
    ('길이만 크게 적은 쓰기 (300워드)', lambda: hit(frame(0x1401, 557, 300, b'\x01\x00'))),
    ('범위 밖 주소 읽기 (D9999 x 500)', lambda: hit(frame(0x0401, 9999, 500))),
    ('음수처럼 보이는 주소', lambda: hit(frame(0x0401, 0xFFFFFF, 1))),
    ('빈 연결 후 즉시 끊기', lambda: hit(b'')),
]

print('%s 포트 %d 에 이상 프레임을 %d 회씩 던진다.' % (EXE, PORT, N))
for name, fn in cases:
    for _ in range(N):
        fn()
    alive = subprocess.run(['tasklist', '/FI', 'IMAGENAME eq ' + EXE],
                           capture_output=True, text=True).stdout
    ok = EXE in alive
    print('  %-34s %s' % (name, '살아 있음' if ok else '>>> 죽었다 <<<'))
    if not ok:
        raise SystemExit(1)
    time.sleep(0.3)

# 두들겨 맞은 뒤에도 정상 프레임이 먹는지 확인한다.
#   앞의 마지막 경우가 연결을 곧바로 끊는 것이라 바로 뒤 한 번은
#   응답이 짧게 올 수 있다. 몇 번 다시 해 본다.
for attempt in range(5):
    try:
        s = socket.create_connection((HOST, PORT), timeout=5)
        s.sendall(frame(0x0401, PROBE_ADDR, 1))
        rsp = s.recv(4096)
        s.close()
    except Exception:
        rsp = b''
    if len(rsp) >= 13:
        print('\n정상 읽기 응답 %d 바이트, 종료코드 0x%04X, D%d = %d'
              % (len(rsp), struct.unpack('<H', rsp[9:11])[0], PROBE_ADDR,
                 struct.unpack('<H', rsp[11:13])[0]))
        print('버텼다.')
        raise SystemExit(0)
    time.sleep(1)

print('\n살아는 있는데 정상 읽기에 답하지 않는다. (마지막 응답 %d 바이트)' % len(rsp))
raise SystemExit(1)
