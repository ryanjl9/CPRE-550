/**
 * This file will be used for testing server.c
*/
//#define _UNIT_TEST
#define _UNIT_TEST
#include <rpc/rpc.h>
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"
#include "config.h"
#include "test.h"

TEST_GROUP_C_SETUP(server_tests){
	memset(TEST_VARS, 0, SIZE * sizeof(int));
}

TEST_GROUP_C_TEARDOWN(server_tests){
	mock_c()->checkExpectations();
	mock_c()->clear();
}

TEST_C(server_tests, test_single_value_get){
	u_int index = 3;
	int retval = 18;

	TEST_VARS[index] = retval;
	int actual = *test_get_1_svc(&index, NULL);

	CHECK_EQUAL_C_INT(actual, retval);
}

TEST_C(server_tests, test_single_value_set){
	int retval = 1;
	u_int index = 5;
	int val = 30;
	struct set_params params = {index, val};
	
	int actual = *test_set_1_svc(&params, NULL);
	CHECK_EQUAL_C_INT(actual, retval);
	CHECK_EQUAL_C_INT(TEST_VARS[index], val);
}
