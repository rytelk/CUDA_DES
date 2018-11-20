all: *.cu *.cuh
	nvcc -w -std=c++11 main.cu -o main
clean:
	rm -f *.o des-brute-force