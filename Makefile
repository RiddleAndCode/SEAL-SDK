BIN = build

OBJS = build/object

SOURCEDIR = \
tests \

SOURCES =

OUTPUT_LIB_DIR =

LIB_DIR = \
libs/

CC = /usr/bin/gcc

LIBS = -lrt -sea -dl

DEFINES = \

INCLUDE = \
-I./inc \
-I./tests/Unity

FLAGS = -std=c99 $(INCLUDE) $(LIBS) $(DEFINES) -std)


.PHONY: all lib debug clean

v1: $(SOURCEDIR)/v1_tests.c $(SOURCEDIR)/ses_tests.c $(LIB_DIR)/v1/libseal.a $(LIB_DIR)/v1/libsedyn.so
	OUTPUT_LIB_DIR += libs/v1
	SOURCES += $(SOURCEDIR)/v1_tests.c \
	$(SOURCEDIR)/Unity/unity.c

	$(CC) $(FLAGS) -L$(OUTPUT_LIB_DIR) $(LIBS)  -o $@


v2: $(SOURCEDIR)/v2_tests.c $(SOURCEDIR)/ses_tests.c $(LIB_DIR)/v2/libseal.a $(LIB_DIR)/v2/libsedyn.so
	INCLUDE += -I./tests/micro-ecc/include
	OUTPUT_LIB_DIR += libs/v2
	SOURCES += $(SOURCEDIR)/v2_tests.c \
	$(SOURCEDIR)/Unity/unity.c \
	$(SOURCEDIR)/micro-ecc/src/uECC.c


	$(CC) $(FLAGS) -L$(OUTPUT_LIB_DIR) $(LIBS)  -o $@

v3: $(SOURCEDIR)/v3_tests.c $(SOURCEDIR)/ses_tests.c $(LIB_DIR)/v3/libseal.a $(LIB_DIR)/v3/libsedyn.so
	OUTPUT_LIB_DIR += libs/v3
	SOURCES += $(SOURCEDIR)/v3_tests.c \
	$(SOURCEDIR)/Unity/unity.c
	$(CC) $(FLAGS) -L$(OUTPUT_LIB_DIR) $(LIBS)  -o $@

ses:
debug:
	@echo  $(SOURCE_C) "\n\n" $(SOURCE_OBJ)

clean:
	rm -rf build/