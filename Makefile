cc = gcc
LDIR = /usr/local/lib
LIB = mosquitto
test: src/test.c src/mqttsub.c dep/parson.c src/config.c
	$(cc) -L$(LDIR) -l$(LIB) -o test src/test.c src/mqttsub.c src/config.c dep/parson.c
