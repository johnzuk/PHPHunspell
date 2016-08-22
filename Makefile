NAME                =   hunspellphp

UBUNTU_MAJOR  := $(shell /usr/bin/lsb_release -r -s | cut -f1 -d.)
OVER_SIXTEEN  := $(shell echo "${UBUNTU_MAJOR} >= 16" | bc)
OVER_FOURTEEN := $(shell echo "${UBUNTU_MAJOR} >= 14" | bc)

#ifeq (${OVER_SIXTEEN}, 1)
#    INI_DIR     =   /etc/php/7.0/mods-available/
#else ifeq (${OVER_FOURTEEN}, 1)
#    INI_DIR     =   /etc/php5/mods-available/
#else
#    INI_DIR     =   /etc/php5/conf.d/
#endif
INI_DIR = /etc/php/7.0/cli/conf.d/

EXTENSION_DIR       =   $(shell php-config7.0 --extension-dir)


EXTENSION           =   ${NAME}.so
INI                 =   20-${NAME}.ini

COMPILER            =   g++
LINKER              =   g++

COMPILER_FLAGS      =   -Wall -c -O2 -std=c++11 -fpic -o
LINKER_FLAGS        =   -shared
LINKER_DEPENDENCIES =   -lphpcpp -lhunspell

RM                  =   rm -f
CP                  =   cp -f
MKDIR               =   mkdir -p


SOURCES             =   $(wildcard *.cpp)
OBJECTS             =   $(SOURCES:%.cpp=%.o)


all:                    ${OBJECTS} ${EXTENSION}

${EXTENSION}:           ${OBJECTS}
			${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

${OBJECTS}:
			${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

install:        
			${CP} ${EXTENSION} ${EXTENSION_DIR}
			${CP} ${INI} ${INI_DIR}

clean:
			${RM} ${EXTENSION} ${OBJECTS}
