bin/mips_simulator : src/simulator.cpp src/mem.cpp src/registers.cpp src/pc.cpp src/headers/*.hpp
	mkdir -p bin
	g++ -g -Wall -std=c++11 src/simulator.cpp src/registers.cpp  src/mem.cpp src/pc.cpp -o bin/mips_simulator
simulator : bin/mips_simulator

bin/write_csv: src/write_csv.cpp
	mkdir -p bin
	g++ -g -Wall -std=c++11 src/write_csv.cpp -o bin/write_csv
testbench : bin/write_csv
