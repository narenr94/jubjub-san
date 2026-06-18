g++ ./sim.cpp -L ../build/lib -I ../build/include/jubjub -lJubjub -o sim

export LD_LIBRARY_PATH=../build/lib:$LD_LIBRARY_PATH
./sim