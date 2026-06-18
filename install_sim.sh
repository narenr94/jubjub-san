if [ ! -d "./build" ]; then
    mkdir build
fi

cd build

cmake -DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=Debug ../

make

make install

