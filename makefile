all: *.cu *.h
	nvcc main.cu -o main
clean:
	rm -f *.o des-brute-force