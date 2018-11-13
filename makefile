all: *.cpp *.h
	g++ -w -Wall -Wextra -Werror -o main main.cpp
clean:
	rm -f *.o des-brute-force