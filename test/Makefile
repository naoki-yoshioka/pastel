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
	./bin/tuple_test
	./bin/geometry
	./bin/particles
	./bin/fixed_neighbor_list
	./bin/simple_system
	./bin/system
	./bin/euler
	./bin/vverlet
	./bin/gear

tuple_test: tuple_test.cpp
	g++ tuple_test.cpp -Wall -I../include -o bin/tuple_test

geometry: geometry.cpp
	g++ geometry.cpp -Wall -I../include -o bin/geometry

particles: particles.cpp
	g++ particles.cpp -Wall -I../include -o bin/particles

fixed_neighbor_list: fixed_neighbor_list.cpp
	g++ fixed_neighbor_list.cpp -Wall -I../include -o bin/fixed_neighbor_list

simple_system: simple_system.cpp
	g++ simple_system.cpp -Wall -I../include -o bin/simple_system

system: system.cpp
	g++ system.cpp -Wall -I../include -o bin/system

euler: euler.cpp
	g++ euler.cpp -Wall -I../include -o bin/euler

vverlet: vverlet.cpp
	g++ vverlet.cpp -Wall -I../include -o bin/vverlet

gear: gear.cpp
	g++ gear.cpp -Wall -I../include -o bin/gear

.PHONY: clean
clean: 
	$(RM) bin/tuple_test bin/geometry bin/particles bin/fixed_neighbor_list bin/simple_system bin/system bin/euler bin/vverlet bin/gear

