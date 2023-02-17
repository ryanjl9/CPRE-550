/**
 * File test_client.c
 * Author: Ryan Lanciloti
 * Date of Creation: 2/14/2023
 * Description: This file will hold all of the unit tests for the client.c file.
*/
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

/* CPPUTESTS */
TEST_GROUP_C_TEARDOWN(client_tests){
	mock_c()->checkExpectations();
	mock_c()->clear();
}
