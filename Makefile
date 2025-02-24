LDLIBS=-lncurses

main: particle.o main.o
	gcc -g -o main particle.o main.o $(LDLIBS)

main.o: main.c
	gcc -c -g -o main.o main.c

particle.o: particle.c
	gcc -c -g -o particle.o particle.c

clean:
	$(RM) *.o main

.PHONY: clean