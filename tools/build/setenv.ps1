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

# Set VCPKG_ROOT and prepend to PATH
$env:VCPKG_ROOT = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..\..\vcpkg'))
$env:PATH = "$env:VCPKG_ROOT;$env:PATH"
