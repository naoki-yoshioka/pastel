.PHONY: all
all: bin/linear_spring
all: bin/lennard_jones
all: bin/sph

bin/linear_spring: linear_spring.cpp
	mkdir -p bin
	g++ linear_spring.cpp -O3 -DNDEBUG -Wall -I../../include -o bin/linear_spring

bin/lennard_jones: lennard_jones.cpp
	mkdir -p bin
	g++ lennard_jones.cpp -Wall -I../../include -o bin/lennard_jones
#	g++ lennard_jones.cpp -O3 -DNDEBUG -Wall -I../../include -o bin/lennard_jones

bin/sph: sph.cpp
	mkdir -p bin
	g++ sph.cpp -O3 -DNDEBUG -Wall -I../../include -o bin/sph

.PHONY: clean
clean: 
	$(RM) bin/linear_spring bin/lennard_jones bin/sph
#	$(RM) bin/linear_spring bin/lennard_jones bin/sph *.dat

