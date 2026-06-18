cd ../

bash install_jubjub.sh

cd sim

g++ ./sim.cpp -L ../build/lib -I ../build/include/jubjub -lJubjub -o sim

g++ ./client.cpp -o client -I ../build/include/jubjub

export LD_LIBRARY_PATH=../build/lib:$LD_LIBRARY_PATH

./sim