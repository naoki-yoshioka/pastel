.PHONY: all
all: linear_spring

.PHONY: test
test: all
	./linear_spring > test1.dat

linear_spring: linear_spring.cpp
	g++ linear_spring.cpp -O3 -DNDEBUG -Wall -I../../include -o linear_spring

.PHONY: clean
clean: 
	$(RM) linear_spring *.dat

