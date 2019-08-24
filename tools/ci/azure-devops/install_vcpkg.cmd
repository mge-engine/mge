@SET VCPKG_CLONE_DIR="%1\vcpkg"
@SET VCPKG_GITHASH=%2
@SET VCPKG_TRIPLET=%3
@ECHO OFF

IF "%1" EQU "" (
    ECHO Missing clone directory argument
    EXIT /B 1
)

SHIFT
SHIFT
SHIFT
SET VCPKG_PACKAGES=

:MORE_TO_PROCESS
IF "%1" EQU "" (
    GOTO :FINISHED
) ELSE (
    SET VCPKG_PACKAGES=%VCPKG_PACKAGES% %1
    SHIFT
    GOTO :MORE_TO_PROCESS
)

:FINISHED

ECHO %VCPKG_PACKAGES%

IF NOT DEFINED VCPKG_GITHASH (
    ECHO Missing githash argument
    EXIT /B 1
)

IF NOT EXIST %VCPKG_CLONE_DIR% (
    git clone https://github.com/mge-engine/vcpkg.git %VCPKG_CLONE_DIR%
) ELSE (
    CD %VCPKG_CLONE_DIR%
    git fetch origin
)

CD %VCPKG_CLONE_DIR%
git reset --hard %2

IF NOT EXIST .\vcpkg.exe CALL .\bootstrap-vcpkg.bat

ECHO Installing %VCPKG_PACKAGES%
.\vcpkg.exe install --triplet %VCPKG_TRIPLET% %VCPKG_PACKAGES%

CD ..