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

#########################################
# 				  FILES					#
#########################################
INCLUDE_FILES = $(shell find $(INCLUDE_DIR) -name '*.h')
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(SRC_FILES:%=$(OBJ_DIR)/%.o)

#########################################
#				 BINARIES				#
#########################################
SERVER_BIN = $(BIN_DIR)/server
CLIENT_BIN = $(BIN_DIR)/client

#########################################
#			BUILD PLACEHOLDER			#
#########################################
client_objects = 
server_objects = 

#########################################
#			 COMPILER FLAGS				#
#########################################
# Enable to generate symbols for GDB
CFLAGS += -ggdb

#########################################
#				TARGETS					#
#########################################

.PHONY: all
all:
	@echo ALL

.PHONY: test
test:
	@make -f tests/testingmakefile

.PHONY: clean
clean: 
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@make -f tests/testingmakefile realclean

.PHONY: clean-verbose
clean-verbose: 
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	make -f tests/testingmakefile realclean
