a.out: main.o dictionary.o
	g++ -o a.out main.o dictionary.o

dictionary.o: dictionary.cc
	g++ -c dictionary.cc

main.o: main.cc
	g++ -c main.cc

.PHONY: clean
clean:
	rm *.o

.PHONY: run
run: a.out
	./a.out product_file.txt order_file.txt