# File: makefile
# Author: Ryan Lanciloti
# Date of Creation: 2/12/2023
# Description: This makefile will be used to generate all object files and binaries used
# in the sys_monitor project.

#########################################
#				DIRECTORIES				#
#########################################
INCLUDE_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./objs
BIN_DIR = ./bins
LIB_DIR = ./lib

#########################################
# 				  FILES					#
#########################################
INCLUDE_FILES = $(shell find $(INCLUDE_DIR) -name '*.h')
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')

#########################################
#				 BINARIES				#
#########################################
SERVER_BIN = $(BIN_DIR)/server
CLIENT_BIN = $(BIN_DIR)/client

#########################################
#			BUILD PLACEHOLDER			#
#########################################
_CLIENT_OBJECTS = client clnt_commands console_helper sys_monitor_clnt sys_monitor_xdr
CLIENT_OBJECTS = $(_CLIENT_OBJECTS:%=$(OBJ_DIR)/%.o)
CLIENT_SRCS = $(_CLIENT_OBJECTS:%=$(SRC_DIR)/%.c)

_SERVER_OBJECTS = server sys_monitor_svc sys_monitor_xdr
SERVER_OBJECTS = $(_SERVER_OBJECTS:%=$(OBJ_DIR)/%.o)
SERVER_SRCS = $(_SERVER_OBJECTS:%=$(SRC_DIR)/%.c)

#########################################
#			 COMPILER FLAGS				#
#########################################
# Enable to generate symbols for GDB
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror=format-security
CFLAGS += -Werror=implicit-function-declaration
CFLAGS += -Wfloat-equal
CFLAGS += -Wundef
CFLAGS += -Wshadow
CFLAGS += -Wpointer-arith
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wstrict-overflow=5
CFLAGS += -Wwrite-strings
CFLAGS += -Wformat=2

#########################################
#				TARGETS					#
#########################################

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INCLUDE_FILES) | objs_dir
	gcc -c -o $@ $< $(CFLAGS) -I$(INCLUDE_DIR)

client: $(CLIENT_OBJECTS) $(INCLUDE_FILES) | bin_dir
	gcc -o $(BIN_DIR)/$@ $^ -I$(INCLUDE_DIR) $(CFLAGS) -O2

client_dbg: $(CLIENT_SRCS) $(INCLUDE_FILES) | bin_dir
	gcc -o $(BIN_DIR)/$@ $^ -I$(INCLUDE_DIR) $(CFLAGS) -ggdb

server: $(SERVER_OBJECTS) $(INCLUDE_FILES) | bin_dir
	gcc -o $(BIN_DIR)/$@ $^ -I$(INCLUDE_DIR) $(CFLAGS) -O2

server_dbg: $(SERVER_SRCS) $(INCLUDE_FILES) | bin_dir
	gcc -o $(BIN_DIR)/$@ $^ -I$(INCLUDE_DIR) $(CFLAGS) -ggdb

.PHONY: bin_dir
bin_dir:
	mkdir -p $(BIN_DIR)

.PHONY: objs_dir
objs_dir:
	mkdir -p $(OBJ_DIR)

.PHONY: all
all: client server

.PHONY: test
test:
ifneq "$(CPPUTEST_HOME)" ""
	@make -f tests/testingmakefile
else
$(error The environment variable CPPUTEST_HOME is not set. \
Set it to where cpputest is installed)
endif

.PHONY: clean
clean: 
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)
	
ifneq "$(CPPUTEST_HOME)" ""
	@make -f tests/testingmakefile realclean
endif

.PHONY: clean-verbose
clean-verbose: 
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)
	
ifneq "$(CPPUTEST_HOME)" ""
	make -f tests/testingmakefile realclean
endif
