SET PROJECT_ROOT=%1
SET INCLUDE=
SET LIB=
call "d:\sw\vc2017\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
set CC=cl
set CXX=cl
SET HOME=%SYSTEMDRIVE%%HOMEPATH%
SET PATH=D:\sw\gs\bin;D:\sw\dynamicrio\bin64;%PATH%
ECHO %PROJECT_ROOT%
CD /D "%PROJECT_ROOT%"
MKDIR build
CD build
cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
