CC=g++
CFLAGS=-Wall -Wextra -std=c++17 -g

client: client.cpp helpers.cpp buffer.cpp requests.cpp
	$(CC) -o client client.cpp helpers.cpp buffer.cpp requests.cpp -Wall

run: client
	./client

clean:
	rm -f *.o client
