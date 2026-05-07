# Clear INCLUDE and LIB before invoking vcvars
$env:INCLUDE = ''
$env:LIB = ''

# Invoke vcvars64.bat and import the resulting environment into this session
$vcvarsPath = 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat'
$envLines = cmd /c "`"$vcvarsPath`" && set"
foreach ($line in $envLines) {
    if ($line -match '^([^=]+)=(.*)$') {
        Set-Item -Path "Env:$($Matches[1])" -Value $Matches[2]
    }
}

$env:CC = 'cl'
$env:CXX = 'cl'
$env:HOME = "$env:SYSTEMDRIVE$env:HOMEPATH"

$uvPythonRoot = Join-Path $env:APPDATA 'uv\python'
$uvPythonInstall = $null

if (Test-Path $uvPythonRoot) {
    $uvPythonInstall = Get-ChildItem -Path $uvPythonRoot -Directory |
        Where-Object { $_.Name -like 'cpython-*+freethreaded-windows-x86_64-none' } |
        Sort-Object -Property @{ Expression = {
                                      if ($_.Name -match '^cpython-\d+\.\d+\.\d+\+') {
                                          1
                                      } else {
                                          0
                                      }
                                  }; Descending = $true },
                              @{ Expression = { $_.Name }; Descending = $true } |
        Select-Object -First 1
}

if ($uvPythonInstall) {
    $uvPythonExecutable = Join-Path $uvPythonInstall.FullName 'python.exe'

    if (Test-Path $uvPythonExecutable) {
        $env:Python3_EXECUTABLE = $uvPythonExecutable
        $env:Python3_ROOT_DIR = $uvPythonInstall.FullName
        $env:PATH = "$($uvPythonInstall.FullName);$env:PATH"
    }
}

# Set VCPKG_ROOT and prepend to PATH
$env:VCPKG_ROOT = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\vcpkg'))
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
