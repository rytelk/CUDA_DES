des-brute-force: *.cu *.h
	nvcc -o des-brute-force kernel.cu
clean:
	rm -f *.o des-brute-force