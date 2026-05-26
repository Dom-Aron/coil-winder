$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$buildDir = Join-Path $repoRoot "build"
$testExe = Join-Path $buildDir "test_winder_core.exe"

New-Item -ItemType Directory -Force -Path $buildDir | Out-Null

g++ `
  -std=c++11 `
  -Wall `
  -Wextra `
  -Werror `
  -I "$repoRoot\Source\OpenCoilWinder" `
  "$repoRoot\tests\test_winder_core.cpp" `
  "$repoRoot\Source\OpenCoilWinder\WinderCore.cpp" `
  -o $testExe

& $testExe
