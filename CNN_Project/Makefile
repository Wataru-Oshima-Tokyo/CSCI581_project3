run: main.o  cnn.o
	g++ -o run main.o  cnn.o
cnn.o: cnn.cpp cnn.h  
	g++ -c -Wall -pedantic -g -std=c++11 cnn.cpp
main.o: main.cpp  cnn.h 
	g++ -c -Wall -pedantic -g -std=c++11 main.cpp
clean: 
	rm main.o cnn.o run
