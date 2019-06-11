
COMPILER = g++
LINKER = g++
BIN_FILE = teabot
LIBS = -lpthread

ifeq (${RELEASE_MODE},1)
	LINKER_FLAGS  = -s -O3
	COMPILER_FLAGS = -Iinclude/ -s -O3 -c -o
else
	LINKER_FLAGS  = -g3 -ggdb -O0 -o
	COMPILER_FLAGS = -Iinclude/ -ggdb3 -O0 -c -o
endif

SOURCES = $(shell find src -name '*.c')
OBJECTS = $(SOURCES:%.c=%.c.o)
SOURCES_DIR = $(shell find src -type d)

ROOT_DEPDIR = .deps
DEPDIR = ${SOURCES_DIR:%=${ROOT_DEPDIR}/%}
DEPFLAGS = -MT $@ -MMD -MP -MF ${ROOT_DEPDIR}/$*.d
DEPFILES = ${SOURCES:%=${ROOT_DEPDIR}/%.d}

all: ${BIN_FILE}

${ROOT_DEPDIR}:
	mkdir -p $@

${DEPDIR}: ${ROOT_DEPDIR}
	mkdir -p $@

${DEPFILES}: ${DEPDIR}

${OBJECTS}: ${DEPFILES}
	${COMPILER} ${DEPFLAGS} ${COMPILER_FLAGS} $@ ${@:%.o=%}

${BIN_FILE}: ${OBJECTS}
	${LINKER} ${LINKER_FLAGS} ${BIN_FILE} ${OBJECTS} ${LIBS}

-include ${DEPFILES}

clean:
	rm -rf ${DEPFILES}
	rm -rf ${OBJECTS}
	rm -rf ${BIN}
