/* test.x: Testing Program */
struct set_params {
	unsigned index;
	int setval;
};

program TESTPROG {
	version TESTVERS {
		int test_get(unsigned) = 1;
		bool test_set(struct set_params) = 2; 
	} = 1;
} = 0x20000001;