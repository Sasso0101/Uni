a.out: main.o library.o
	g++ -o a.out main.o library.o

library.o: library.cc
	g++ -c library.cc

main.o: main.cc
	g++ -c main.cc

.PHONY: clean
clean:
	rm *.o

.PHONY: run
run: a.out
	./a.out