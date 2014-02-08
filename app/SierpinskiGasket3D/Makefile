BINARIES  = bin
SOURCE    = src
TEMPORARY = tmp
VARIABLE  = var

#PROJECT = GLUTmain
PROJECT = GLFWmain
BUILDER = $(TEMPORARY)/$(PROJECT).o $(SOURCE)/shader.h
#LIBRARY = -lglut -lGLEW -lGL
LIBRARY = -lglfw3 -lGLEW -lGL -lX11 -lpthread -lXi -lXrandr -lXxf86vm

COMPILE = g++ -std=gnu++11
#OPTIONS = -g -O -Wall -Weffc++ -pedantic -pedantic-errors -Wextra -Wall -Waggregate-return -Wcast-align -Wcast-qual -Wchar-subscripts -Wcomment -Wconversion -Wdisabled-optimization -Werror -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wlong-long -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wpadded -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings

build: all clean
all: $(BINARIES)/$(PROJECT)
again: new all
new: clean
	-rm $(BINARIES)/*
	-rm $(PROJECT).tgz
clean:
	-rm $(TEMPORARY)/*
export: $(PROJECT).tgz

$(PROJECT).tgz: new
	tar -czvf $(PROJECT).tgz *
$(BINARIES)/$(PROJECT): $(BUILDER)
	$(COMPILE) $(OPTIONS) -o $@ $(BUILDER) $(LIBRARY)
$(TEMPORARY)/%.o: $(SOURCE)/%.cpp
	$(COMPILE) $(OPTIONS) -o $@ -c $<
