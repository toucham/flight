all: build run_server run_client tiny
	rm rio.o
	rm run_server.o server.o
	rm run_client.o	client.o
	rm tiny.o

run_server: server.o rio.o
	g++ -std=c++17 -c ./socket/run_server.cpp -o run_server.o
	g++ -std=c++17 rio.o server.o run_server.o -o run_server
	mv run_server ./build

run_client: client.o rio.o
	g++ -std=c++17 -c ./socket/run_client.cpp -o run_client.o
	g++ -std=c++17 rio.o client.o run_client.o -o run_client
	mv run_client ./build

tiny: server.o rio.o
	g++ -std=c++17 -c ./webserver/tiny.cpp -o tiny.o
	g++ -std=c++17 tiny.o rio.o server.o -o tiny
	mv tiny ./build

rio.o:
	g++ -std=c++17 -c ./io/rio.cpp -o rio.o

server.o:
	g++ -std=c++17 -c ./socket/server.cpp -o server.o

client.o:
	g++ -std=c++17 -c ./socket/client.cpp -o client.o

cleanErr:
	@if [ -f rio.o ]; then\
		rm *.o;\
	fi

clean:
	@if [ -d build ]; then\
		rm -rf build;\
	fi

read_dir:
	cd build; g++ -std=c++17 ../io/read_dir.cpp -o read_dir

read_stat:
	cd build; g++ -std=c++17 ../io/read_stat.cpp -o read_stat

build:
	[ ! -d ./build ] && mkdir build
