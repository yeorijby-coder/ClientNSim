# 시뮬레이터를 다른 PC 로 가져갈 수 있게 한 대씩 따로 담는다.
#
#   Tools\Deploy.ps1 [-Config Debug|Release] [-Out <폴더>] [-Sim CvSim,ScSim,HostSim]
#
# 왜 한 대씩 따로 담나
#   Bin\Debug 는 빌드 산출물 폴더지 실행 폴더가 아니다. 시뮬레이터는 데이터 파일을
#   상대경로(".\SC.xml")로 읽으므로 작업 디렉터리에 있는 것을 집는다. 그런데 이름은
#   같고 내용이 다른 파일이 프로젝트마다 있다.
#     SC.XML     CvSim 것은 루트가 <SC> 인 옛 한 대짜리, ScSim 것은 <ECS> + 장비 16개
#     Logic.xml  CvSim 것은 컨베이어 로직, HostSim 것은 작업 로직
#     EcsDefine.xml  프로젝트마다 다르다
#   한 폴더에 섞으면 어느 한쪽은 반드시 남의 파일을 읽는다.
#
# 이 파일은 UTF-8 BOM 으로 저장한다. BOM 이 없으면 PowerShell 5.1 이 ANSI 로 읽어
# 위 주석과 아래 메시지가 깨진다.
param(
    [ValidateSet('Debug','Release')]
    [string]$Config = 'Debug',
    [string]$Out = '',
    [string[]]$Sim = @('CvSim','ScSim','HostSim')
)

$ErrorActionPreference = 'Stop'

$Root = Split-Path -Parent $PSScriptRoot          # ClientNSim
$BinDir = Join-Path $Root "Bin\$Config"
if (-not (Test-Path $BinDir)) { throw "빌드 산출물이 없습니다: $BinDir" }
if ($Out -eq '') { $Out = Join-Path $Root "Deploy\$Config" }

# 시뮬레이터마다 자기 폴더에서 가져갈 데이터. 와일드카드를 쓴다.
$MANIFEST = @(
    @{ Name = 'CvSim';   Src = 'CvSim';
       Data = @('CvSim.ini','EcsDefine.xml','EcsLayout1.xml','EcsLayout2.xml','EcsLayout3.xml',
                'DeviceMap.xml','DeviceMap0?.xml','KeyWord.xml','Logic.xml') }
    @{ Name = 'ScSim';   Src = 'ScSim';
       Data = @('ScSim.ini','SC.XML','ScLayout*.XML') }
    @{ Name = 'HostSim'; Src = 'HostSim';
       Data = @('HostSim.ini','EcsLog.ini','EcsDefine.xml','EcsLayout.xml','Logic.xml') }
)

# 세 시뮬레이터가 같이 쓰는 것
$SHARED_DLL = @('XmlLib.dll','DciLib.dll','EcsLib.dll')

function Get-RuntimeDll {
    # Debug 빌드는 디버그 CRT/MFC 를 쓴다. 이것들은 재배포 대상이 아니라 개발툴이
    # 깔린 PC 에만 있다. 그래서 여기서 찾아 같이 담는다.
    param([string]$Config)
    if ($Config -eq 'Release') {
        return @('mfc140.dll','vcruntime140.dll','msvcp140.dll')  | ForEach-Object {
            $p = Join-Path $env:WINDIR "SysWOW64\$_"
            if (Test-Path $p) { $p }
        }
    }
    $found = @()
    $vc = Get-ChildItem 'C:\Program Files\Microsoft Visual Studio\*\*\VC\Redist\MSVC\*\debug_nonredist\x86' -Directory -ErrorAction SilentlyContinue |
          Sort-Object FullName -Descending | Select-Object -First 1
    if ($vc) {
        $found += Get-ChildItem (Join-Path $vc.FullName 'Microsoft.VC143.DebugCRT\*.dll') -ErrorAction SilentlyContinue
        $found += Get-ChildItem (Join-Path $vc.FullName 'Microsoft.VC143.DebugMFC\mfc140ud.dll') -ErrorAction SilentlyContinue
        $found += Get-ChildItem (Join-Path $vc.FullName 'Microsoft.VC143.DebugMFC\mfcm140ud.dll') -ErrorAction SilentlyContinue
    }
    $ucrt = Get-ChildItem 'C:\Program Files (x86)\Windows Kits\10\bin\*\x86\ucrt\ucrtbased.dll' -ErrorAction SilentlyContinue |
            Sort-Object FullName -Descending | Select-Object -First 1
    if ($ucrt) { $found += $ucrt }
    return $found | ForEach-Object { $_.FullName }
}

$runtime = @(Get-RuntimeDll -Config $Config)
Write-Host ""
Write-Host "산출물 : $BinDir"
Write-Host "내보낼 곳 : $Out"
if ($Config -eq 'Debug') {
    Write-Host "런타임 : 디버그 CRT/MFC $($runtime.Count) 개를 같이 담습니다." -ForegroundColor Yellow
    Write-Host "         디버그 런타임은 재배포 대상이 아닙니다. 외부에 넘길 것이면 Release 로 만드세요." -ForegroundColor Yellow
}
Write-Host ""

$problem = 0

foreach ($m in $MANIFEST) {
    if ($Sim -notcontains $m.Name) { continue }

    $dst = Join-Path $Out $m.Name
    $srcData = Join-Path $Root $m.Src
    New-Item -ItemType Directory -Force -Path $dst | Out-Null

    Write-Host ("[{0}]" -f $m.Name) -ForegroundColor Cyan

    # 실행파일 + 공용 DLL
    $exe = Join-Path $BinDir ($m.Name + '.exe')
    if (-not (Test-Path $exe)) {
        Write-Host ("   실행파일 없음 : {0}  (빌드하지 않았습니다)" -f $exe) -ForegroundColor Red
        $problem++
        continue
    }
    Copy-Item $exe $dst -Force
    foreach ($d in $SHARED_DLL) {
        $p = Join-Path $BinDir $d
        if (Test-Path $p) { Copy-Item $p $dst -Force }
        else { Write-Host ("   공용 DLL 없음 : {0}" -f $d) -ForegroundColor Red; $problem++ }
    }
    foreach ($p in $runtime) { Copy-Item $p $dst -Force }

    # 데이터
    $n = 0
    foreach ($pat in $m.Data) {
        $hit = @(Get-ChildItem (Join-Path $srcData $pat) -File -ErrorAction SilentlyContinue)
        if ($hit.Count -eq 0) {
            Write-Host ("   데이터 없음 : {0}\{1}" -f $m.Src, $pat) -ForegroundColor Red
            $problem++
            continue
        }
        $hit | Copy-Item -Destination $dst -Force
        $n += $hit.Count
    }
    Write-Host ("   실행파일 1 + 공용DLL {0} + 런타임 {1} + 데이터 {2}  ->  {3}" -f $SHARED_DLL.Count, $runtime.Count, $n, $dst)
}

Write-Host ""
if ($problem -eq 0) {
    Write-Host "다 담았습니다. 폴더째 복사해서 그 폴더 안의 exe 를 실행하면 됩니다." -ForegroundColor Green
} else {
    Write-Host ("빠진 것이 {0} 개 있습니다. 위 빨간 줄을 보세요." -f $problem) -ForegroundColor Red
}
Write-Host ""
