#include <rpc/rpc.h>
#include "test.h"
#include "config.h"

static int BAD_INDEX = -1;
static bool_t _TRUE = TRUE;
static bool_t _FALSE = FALSE;

int TEST_VARS[SIZE] = {0};

int* test_get_1_svc(u_int* index_ptr, struct svc_req * req){
	u_int index = *index_ptr;
	
	if(index >= SIZE) return &BAD_INDEX;
	return &(TEST_VARS[index]);
}

bool_t* test_set_1_svc(struct set_params* params_ptr, struct svc_req * req){
	if(params_ptr->index >= SIZE) return &_FALSE;
	TEST_VARS[params_ptr->index] = params_ptr->setval;
	return &_TRUE;
}