PROJECT = ogbox

COMPILE = g++ -std=gnu++11
#OPTIONS = -g -O -Wall -Weffc++ -pedantic -pedantic-errors -Wextra -Wall -Waggregate-return -Wcast-align -Wcast-qual -Wchar-subscripts -Wcomment -Wconversion -Wdisabled-optimization -Werror -Wfloat-equal -Wformat -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winline -Winvalid-pch -Wunsafe-loop-optimizations -Wlong-long -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wpadded -Wparentheses -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunreachable-code -Wunused -Wunused-function -Wunused-label -Wunused-parameter -Wunused-value -Wunused-variable -Wvariadic-macros -Wvolatile-register-var -Wwrite-strings
BUILDER = tmp/main.o tmp/ppm_canvas.o
LIBRARY = -lglfw -lGLEW -lGL -lGLU -lX11 -lpthread -lXxf86vm

all: bin/$(PROJECT)
clean:
	-rm -rf tmp
build: all clean
new: clean
	-rm -rf bin
	-rm $(PROJECT).tgz
again: new all
export: $(PROJECT).tgz

$(PROJECT).tgz: new
	tar -czvf $(PROJECT).tgz *
bin/$(PROJECT): $(BUILDER)
	mkdir -p bin
	$(COMPILE) $(OPTIONS) -o $@ $(BUILDER) $(LIBRARY)
tmp/%.o: src/%.cpp
	mkdir -p tmp
	$(COMPILE) $(OPTIONS) -o $@ -c $<
