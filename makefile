HEADERS = Program.h
OBJECTS = Program.o

default: Program

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

program: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f Program

run: program
	./Program