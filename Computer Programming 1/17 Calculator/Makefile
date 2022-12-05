a.out: main.o stack.o calculator.o
	g++ -o a.out main.o stack.o calculator.o

stack.o: stack.cc
	g++ -c stack.cc

calculator.o: calculator.cc
	g++ -c calculator.cc

main.o: main.cc
	g++ -c main.cc

.PHONY: clean
clean:
	rm *.o

.PHONY: run
run: a.out
	./a.out