# _*_ Makefile _*_

CC = gcc
all = cracker

cracker: csrf.o main.o memcallback.o post.o
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` csrf.o main.o post.o memcallback.o -o cracker

csrf.o: csrf.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c csrf.c 

post.o: post.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c post.c

main.o: main.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c main.c

memcallback.o: memcallback.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c memcallback.c

clean: 
	rm -f *.o 