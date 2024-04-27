if [[ "$MGE_BUILDENV_SET" == "1" ]]; then
    echo "Environment has already been set"
    exit 0
fi
export VCPKG_ROOT=`pwd`/../vcpkg
export MGE_BUILDENV_SET=1