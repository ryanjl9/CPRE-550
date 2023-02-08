deps = test.h config.h
client_objects = client.o test_clnt.o test_xdr.o
server_objects = server.o test_svc.o test_xdr.o
objects = $(client_objects) $(server_objects)

all: client server

%.o: %.c $(deps)
	gcc -c -o $@ $< -ggdb

client: $(client_objects)
	gcc -o $@ $^

server: $(server_objects)
	gcc -o $@ $^

clean: 
	rm -f client server $(objects)