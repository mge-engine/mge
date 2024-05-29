if [[ "$MGE_BUILDENV_SET" == "1" ]]; then
    echo "Environment has already been set"
    exit 0
fi
export VCPKG_ROOT=`pwd`/../vcpkg
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++"
export LDFLAGS="$LDFLAGS -L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
export CC=clang
export CXX=clang++
export MGE_BUILDENV_SET=1
