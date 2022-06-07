all: main.cpp
	g++ -o main_test.exe -std=c++17 main.cpp 
clean:
	rm *.o
