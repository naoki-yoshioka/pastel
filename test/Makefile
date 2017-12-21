.PHONY: all
all: tuple_test
all: geometry
all: particles
all: fixed_neighbor_list
all: simple_system
all: system
all: euler
all: vverlet
all: gear

.PHONY: test
test: all
	./tuple_test
	./geometry
	./particles
	./fixed_neighbor_list
	./simple_system
	./system
	./euler
	./vverlet
	./gear

tuple_test: tuple_test.cpp
	g++ tuple_test.cpp -Wall -I../include -o tuple_test

geometry: geometry.cpp
	g++ geometry.cpp -Wall -I../include -o geometry

particles: particles.cpp
	g++ particles.cpp -Wall -I../include -o particles

fixed_neighbor_list: fixed_neighbor_list.cpp
	g++ fixed_neighbor_list.cpp -Wall -I../include -o fixed_neighbor_list

simple_system: simple_system.cpp
	g++ simple_system.cpp -Wall -I../include -o simple_system

system: system.cpp
	g++ system.cpp -Wall -I../include -o system

euler: euler.cpp
	g++ euler.cpp -Wall -I../include -o euler

vverlet: vverlet.cpp
	g++ vverlet.cpp -Wall -I../include -o vverlet

gear: gear.cpp
	g++ gear.cpp -Wall -I../include -o gear

.PHONY: clean
clean: 
	$(RM) tuple_test geometry particles fixed_neighbor_list simple_system system euler vverlet gear

