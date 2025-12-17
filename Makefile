all: comm_bus station

comm_bus: comm_bus.c
	gcc -o comm_bus comm_bus.c -Wall

station: station.c
	gcc -o station station.c -Wall

clean:
	rm -f comm_bus station *.o logs/*
	killall comm_bus || true