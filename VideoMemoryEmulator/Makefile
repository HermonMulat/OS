terminal: main.o terminal.o
	gcc -o terminal main.o terminal.o -lncurses

%.o: %.c
	gcc -c -o $@ $<

clean:
	rm -f main.o terminal.o terminal
