CC=g++
CFLAGS=-c -Wall -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE
LDFLAGS=
SOURCES1 = asm.v2/assembler.cpp asm.v2/asm_func.cpp 
OBJECTS1=$(SOURCES1:.cpp=.o)
EXECUTABLE1 = asm.v2/asm.exe

SOURCES2 = cpu/CPU_main.cpp cpu/CPU_func.cpp cpu/Stack/Stack.cpp
OBJECTS2=$(SOURCES2:.cpp=.o)
EXECUTABLE2 = cpu/CPU.exe

all: asm cpu
	
asm: $(SOURCES1) $(EXECUTABLE1)
	
$(EXECUTABLE1): $(OBJECTS1) 
	$(CC) $(LDFLAGS) $(OBJECTS1) -o $@

cpu: $(SOURCES2) $(EXECUTABLE2)
	
$(EXECUTABLE2): $(OBJECTS2) 
	$(CC) $(LDFLAGS) $(OBJECTS2) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $< -o $@