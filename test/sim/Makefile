.PHONY: all
all: linear_spring

.PHONY: test
test: all
	./bin/linear_spring > test1.dat

linear_spring: linear_spring.cpp
	g++ linear_spring.cpp -O3 -DNDEBUG -Wall -I../../include -o bin/linear_spring

.PHONY: clean
clean: 
	$(RM) bin/linear_spring *.dat

