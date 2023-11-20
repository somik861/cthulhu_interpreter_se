Remove-Item -Path bin -Force -Recurse
New-Item -Path bin -ItemType Directory

Set-Location ..
Remove-Item -Path build -Force -Recurse
New-Item -Path build -ItemType Directory
Set-Location build

cmake ..
cmake --build . --config Release

Copy-Item Release\cthulhu.exe ..\benchmark\bin -Force
