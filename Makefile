# _*_ Makefile _*_

CC = gcc
all = test

test: csrf.o arg.o parsing.o main.o memcallback.o post.o
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` csrf.o arg.o parsing.o main.o post.o memcallback.o -o test

csrf.o: csrf.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c csrf.c 

post.o: post.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c post.c

arg.o: arg.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c arg.c

parsing.o: parsing.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c parsing.c

main.o: main.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c main.c

memcallback.o: memcallback.c headers.h
	gcc -Wall -Wextra -lcurl `xml2-config --cflags --libs` -c memcallback.c
