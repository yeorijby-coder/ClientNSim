# Bin

빌드 산출물 폴더다. **실행 폴더로 쓰지 마라.**

## 왜

시뮬레이터는 데이터 파일을 **상대경로**로 읽는다. `.\SC.xml`, `.\ScLayout1.XML`,
`.\Logic.xml` 처럼. 즉 **작업 디렉터리(working directory)** 에 있는 것을 집는다.
`ScSim.ini` 같은 ini 도 `g_strEcsPath = GetCurrentDirectory()` 를 앞에 붙여 찾는다.

그런데 **이름은 같고 내용이 다른 파일이 프로젝트마다 있다.**

| 파일 | CvSim | ScSim | HostSim |
| --- | --- | --- | --- |
| `SC.XML` | 루트 `<SC>` 인 옛 한 대짜리 정의 | 루트 `<ECS>` + 장비 16개 | 안 씀 |
| `Logic.xml` | 컨베이어 로직 | 안 씀 | 작업 로직 |
| `EcsDefine.xml` | CvSim 것 | ScSim 것 | HostSim 것 |

한 폴더에 섞으면 어느 한쪽은 반드시 남의 파일을 읽는다.
실제로 그래서 사고가 났다 — ScSim 을 `Bin\Debug` 에서 돌리니 CvSim 의 `SC.XML` 을
읽었고, 자식 요소에 `TYPE` 속성이 없어 `XmlDom.cpp` 의 `ASSERT` 에서 죽었다.
개발툴이 없는 테스트 서버라 디버거도 못 붙였다.

## 그래서 어떻게 실행하나

**방법 1 — 배포 폴더를 만든다 (다른 PC 로 가져갈 때)**

```
Tools\Deploy.ps1 -Config Debug
```

`Deploy\Debug\CvSim`, `Deploy\Debug\ScSim`, `Deploy\Debug\HostSim` 이 생긴다.
각 폴더에 exe + 공용 DLL + **그 시뮬레이터 자기 데이터** + 런타임 DLL 이 다 들어간다.
폴더째 복사해서 그 안의 exe 를 실행하면 된다.

**방법 2 — 개발 PC 에서 그냥 돌릴 때**

`Bin\Debug` 의 exe 를 **각 프로젝트 폴더를 작업 디렉터리로 삼아** 실행한다.
바로가기라면 "시작 위치" 를 `...\ClientNSim\ScSim` 으로 잡는다.

## 디버그 런타임

Debug 빌드는 `mfc140ud.dll` / `vcruntime140d.dll` / `ucrtbased.dll` 을 쓴다.
이것들은 **재배포 대상이 아니라** 개발툴이 깔린 PC 에만 있다.
`Tools\Deploy.ps1` 이 이 PC 에서 찾아 같이 담아 주기는 하지만,
사외로 넘길 것이면 Release 로 만들어야 한다.

`Bin\Debug` 에 있는 `mfc100ud.dll` / `msvcr100d.dll` 은 VS2010(v100) 시절 잔재다.
지금 빌드는 v143 이라 저것들은 쓰이지 않는다.

## Bin\Debug 에 남아 있는 데이터 파일

`*.ini` 는 일부러 넣어 둔 것이다. `SC.XML` 과 `ScLayout*.XML` 도 ScSim 이
여기서도 뜨도록 맞춰 두었다.

`EcsDefine.xml`, `EcsLayout1~3.xml`, `Logic.xml`, `DeviceMap*.xml` 은
어느 프로젝트 폴더의 것과도 내용이 다르다. 여기서 직접 고쳐 쓴 흔적으로 보인다.
어느 실행파일이 쓰는지 확인되기 전까지는 지우지 않고 둔다.
**새로 뭔가를 돌릴 때 이 파일들을 믿지 마라.** 위의 배포 폴더를 써라.
