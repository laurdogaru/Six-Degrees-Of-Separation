all : movies.c

build: movies.c
	gcc -Wall -o movies movies.c 
	
clean:
	rm movies
