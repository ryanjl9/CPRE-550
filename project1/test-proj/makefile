deps = test.h config.h
client_objects = client.o test_clnt.o test_xdr.o
server_objects = server.o test_svc.o test_xdr.o
objects = $(client_objects) $(server_objects)

.PHONY: all
all: client server test

%.o: %.c $(deps)
	gcc -c -o $@ $< -ggdb $(CFLAGS)

client: $(client_objects)
	gcc -o $@ $^

server: $(server_objects)
	gcc -o $@ $^

.PHONY: test
test:
	make -f TestMakefile

.PHONY: clean
clean: 
	@rm -f client server $(objects)
	@make -f TestMakefile realclean

.PHONY: clean-verbose
clean-verbose: 
	rm -f client server $(objects)
	@make -f TestMakefile realclean