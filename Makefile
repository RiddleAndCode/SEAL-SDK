BIN = ./binary

SOURCEDIR = ./tests

SOURCES =

SES_SOURCES = $(SOURCEDIR)/ses_tests.c \
$(SOURCEDIR)/Unity/unity.c

OUTPUT_LIB_DIR =

LIB_DIR = ./libs/raspberry

CC = /usr/bin/gcc

LIBS = -lrt -lsea -ldl

DEFINES = \

INCLUDE = \
-I./inc \
-I./tests/Unity

FLAGS = -std=c99 $(DEFINES)

ifeq ($(target),v1)
	OUTPUT_LIB_DIR += $(LIB_DIR)/v1

	SOURCES += $(SOURCEDIR)/v1_tests.c \
	$(SOURCEDIR)/Unity/unity.c

else ifeq ($(target),v2)
	INCLUDE += -I./tests/micro-ecc/include
	LIBS += -lpthread
	OUTPUT_LIB_DIR += $(LIB_DIR)/v2

	SOURCES += $(SOURCEDIR)/v2_tests.c \
	$(SOURCEDIR)/Unity/unity.c \
	$(SOURCEDIR)/micro-ecc/src/uECC.c

else ifeq ($(target),v3)
	OUTPUT_LIB_DIR += $(LIB_DIR)/v3

	SOURCES += $(SOURCEDIR)/v3_tests.c \
	$(SOURCEDIR)/Unity/unity.c
endif

.PHONY: all debug clean


$(target): $(SOURCEDIR)/$(target)_tests.c  $(LIB_DIR)/$(target)/libsea.a  $(BIN)/ses 
	@mkdir -p $(BIN)

	$(CC) $(FLAGS) $(SOURCES) $(INCLUDE) -L$(OUTPUT_LIB_DIR) $(LIBS)  -o $(BIN)/$@

$(BIN)/ses: $(SOURCEDIR)/ses_tests.c

	@mkdir -p $(BIN)

	$(CC) $(FLAGS) $(SES_SOURCES) $(INCLUDE) -L$(OUTPUT_LIB_DIR) $(LIBS)  -o $@



debug:
	@echo "\n Sources are" $(SOURCES) "\n Includes are" $(INCLUDE) "\n"

clean:
	rm -rf $(BIN)/v1 $(BIN)/v2 $(BIN)/v3 $(BIN)/ses