.PHONY: all
all: bin/tuple_test
all: bin/geometry
all: bin/particles
all: bin/fixed_neighbor_list
all: bin/simple_system
all: bin/system
all: bin/euler
all: bin/vverlet
all: bin/gear
all: bin/lennard_jones_collisions

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
	./bin/lennard_jones_collisions

bin/tuple_test: tuple_test.cpp
	g++ tuple_test.cpp -Wall -I../include -o bin/tuple_test

bin/geometry: geometry.cpp
	g++ geometry.cpp -Wall -I../include -o bin/geometry

bin/particles: particles.cpp
	g++ particles.cpp -Wall -I../include -o bin/particles

bin/fixed_neighbor_list: fixed_neighbor_list.cpp
	g++ fixed_neighbor_list.cpp -Wall -I../include -o bin/fixed_neighbor_list

bin/simple_system: simple_system.cpp
	g++ simple_system.cpp -Wall -I../include -o bin/simple_system

bin/system: system.cpp
	g++ system.cpp -Wall -I../include -o bin/system

bin/euler: euler.cpp
	g++ euler.cpp -Wall -I../include -o bin/euler

bin/vverlet: vverlet.cpp
	g++ vverlet.cpp -Wall -I../include -o bin/vverlet

bin/gear: gear.cpp
	g++ gear.cpp -Wall -I../include -o bin/gear

bin/lennard_jones_collisions: lennard_jones_collisions.cpp
	g++ lennard_jones_collisions.cpp -Wall -I../include -o bin/lennard_jones_collisions

.PHONY: clean
clean: 
	$(RM) bin/tuple_test bin/geometry bin/particles bin/fixed_neighbor_list bin/simple_system bin/system bin/euler bin/vverlet bin/gear bin/lennard_jones_collisions

