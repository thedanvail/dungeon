build:
	clang++ --std=c++11 fixme_fixed.cpp -o dungeon.o

run:
	./dungeon.o

fresh:
	make build
	make run

