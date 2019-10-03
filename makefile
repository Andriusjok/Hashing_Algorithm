CC=g++
main:
	$(CC) -o main Hash.cpp
gen:
	$(CC) -o ketvirtas 4testas.cpp 
clean:
	rm *.o *.exe
