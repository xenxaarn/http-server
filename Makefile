server: server.c
	$(CC) server.c -o server -Wall -Wextra -pedantic -std=c99
clean:
	rm server
