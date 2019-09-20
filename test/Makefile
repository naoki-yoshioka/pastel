.PHONY: all
all: bin/tuple_test
all: bin/geometry
all: bin/particles
all: bin/fixed_neighbor_list
all: bin/simple_system
#all: bin/system
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
#	./bin/system
	./bin/euler
	./bin/vverlet
	./bin/gear
	./bin/lennard_jones_collisions

bin/tuple_test: tuple_test.cpp
	mkdir -p bin
	g++ tuple_test.cpp -Wall -I../include -o bin/tuple_test

bin/geometry: geometry.cpp
	mkdir -p bin
	g++ geometry.cpp -Wall -I../include -o bin/geometry

bin/particles: particles.cpp
	mkdir -p bin
	g++ particles.cpp -Wall -I../include -o bin/particles

bin/fixed_neighbor_list: fixed_neighbor_list.cpp
	mkdir -p bin
	g++ fixed_neighbor_list.cpp -Wall -I../include -o bin/fixed_neighbor_list

bin/simple_system: simple_system.cpp
	mkdir -p bin
	g++ simple_system.cpp -Wall -I../include -o bin/simple_system

#bin/system: system.cpp
#	mkdir -p bin
#	g++ system.cpp -Wall -I../include -o bin/system

bin/euler: euler.cpp
	mkdir -p bin
	g++ euler.cpp -Wall -I../include -o bin/euler

bin/vverlet: vverlet.cpp
	mkdir -p bin
	g++ vverlet.cpp -Wall -I../include -o bin/vverlet

bin/gear: gear.cpp
	mkdir -p bin
	g++ gear.cpp -Wall -I../include -o bin/gear

bin/lennard_jones_collisions: lennard_jones_collisions.cpp
	mkdir -p bin
	g++ lennard_jones_collisions.cpp -Wall -I../include -o bin/lennard_jones_collisions

.PHONY: clean
clean: 
#	$(RM) bin/tuple_test bin/geometry bin/particles bin/fixed_neighbor_list bin/simple_system bin/system bin/euler bin/vverlet bin/gear bin/lennard_jones_collisions
	$(RM) bin/tuple_test bin/geometry bin/particles bin/fixed_neighbor_list bin/simple_system bin/euler bin/vverlet bin/gear bin/lennard_jones_collisions

