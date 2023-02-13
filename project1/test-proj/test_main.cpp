#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_WRAPPER(server_tests){
	TEST_GROUP_C_SETUP_WRAPPER(server_tests);
	TEST_GROUP_C_TEARDOWN_WRAPPER(server_tests);
};

TEST_C_WRAPPER(server_tests, test_single_value_get)
TEST_C_WRAPPER(server_tests, test_single_value_set)

int main(int argc, char** argv){
	return RUN_ALL_TESTS(argc, argv);
}
