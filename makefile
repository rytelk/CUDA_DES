all: *.cu *.cuh
	nvcc -w -Wall -Wextra -Werror -std=c++11 main.cu -o main
clean:
	rm -f *.o des-brute-force