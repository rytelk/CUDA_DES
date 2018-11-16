all: *.cu *.cuh
	nvcc -std=c++11 main.cu -o main
clean:
	rm -f *.o des-brute-force