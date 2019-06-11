
COMPILER = g++
LINKER = g++
BIN_FILE = teabot
LIBS = -lpthread -lcurl
SOURCE_DIR = src/
ROOT_DEPDIR = .deps

ifeq (${RELEASE_MODE},1)
	LINKER_FLAGS  = -Wall -fno-stack-protector -s -O3 -o
	COMPILER_FLAGS = -Wall -fno-stack-protector -Iinclude/ -s -O3 -c -o
else
	LINKER_FLAGS  = -Wall -fno-stack-protector -g3 -ggdb -Og -o
	COMPILER_FLAGS = -Wall -fno-stack-protector -Iinclude/ -ggdb3 -Og -c -o
endif

SOURCES = $(shell find ${SOURCE_DIR} -name '*.c')
OBJECTS = $(SOURCES:%.c=%.c.o)
SOURCES_DIR = $(shell find ${SOURCE_DIR} -type d)

DEPDIR = ${SOURCES_DIR:%=${ROOT_DEPDIR}/%}
DEPFLAGS = -MT $@ -MMD -MP -MF ${ROOT_DEPDIR}/$*.d
DEPFILES = ${SOURCES:%=${ROOT_DEPDIR}/%.d}

all: ${BIN_FILE}

${ROOT_DEPDIR}:
	mkdir -p $@

${DEPDIR}: | ${ROOT_DEPDIR}
	mkdir -p $@

${OBJECTS}: | ${DEPDIR}
	${COMPILER} ${DEPFLAGS} ${COMPILER_FLAGS} $@ ${@:%.o=%}

${BIN_FILE}: ${OBJECTS}
	${LINKER} ${LINKER_FLAGS} ${BIN_FILE} ${OBJECTS} ${LIBS}

-include ${DEPFILES}

release:
	make clean
	make RELEASE_MODE=1
	strip -s ${BIN_FILE}

clean:
	rm -rf ${DEPFILES}
	rm -rf ${OBJECTS}
	rm -rf ${BIN_FILE}
