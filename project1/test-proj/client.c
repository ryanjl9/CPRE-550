#include <stdio.h>
#include <rpc/rpc.h>
#include "test.h"
#include "config.h"

const char* BOOL_MATRIX[2] = {"FALSE", "TRUE"};

int main(int argc, char** argv){
	CLIENT* clnt;

	if ((clnt = clnt_create(HOSTNAME, TESTPROG, TESTVERS, "udp")) == NULL){
		clnt_pcreateerror(HOSTNAME);
		exit(2);
	}
	
	int index;
	int setval;
	bool_t* test_set_1_retval;
	int* test_get_1_retval;

	for(index = 0; index < SIZE + 5; index++){
		setval = (index*4 + 3) % 13;
		struct set_params params = {index, setval};
		test_set_1_retval = test_set_1(&params, clnt);

		if(test_set_1_retval == NULL || (*test_set_1_retval == FALSE && index < SIZE)){
			clnt_perror(clnt, HOSTNAME);
			exit(3);
		}

		printf("Setting index %u to %d - %s\n", index, setval, BOOL_MATRIX[*test_set_1_retval]);
	}

	printf("\n");

	for(index = 0; index < SIZE + 5; index++){
		test_get_1_retval = test_get_1(&index, clnt);
		if(test_get_1_retval == NULL){
			clnt_perror(clnt, HOSTNAME);
			exit(4);
		}
		printf("Value at index %u: %d\n", index, *test_get_1_retval);
	}

	clnt_destroy(clnt);
	return 0;
}
