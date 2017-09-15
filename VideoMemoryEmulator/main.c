#include <curses.h>
#include "terminal.h"

int main(int argc, char **argv) {
	terminal_initialize();

	// Set foreground color to black, background color to green
	// Clears the screen, which fills up the screen with the chosen colors
	// PHASE 1: Implement the terminal_setcolor function, and you should see a green background
	terminal_setcolor(BLACK, GREEN);
	terminal_clear();

	// PHASE2: Implement terminal_write_string and test it with those commands

	terminal_write_string("Hello colorful world!");
	getch();

	terminal_write_string(" This is a very long string that should wrap to the next line in your output. Of course, when you skip a line, you should move to the first character of the next line. This will also print a new line in the end, so the cursor must be in the first character of the new line.\n");
	getch();

	terminal_write_string("Now\nlets\ntest\nsome\nscrolling\nin\nyour\nterminal\nla\nla\nla\nla\nla\nla\nla\nla\nla\nla\nla\nla\nla\nla\n");
	getch();

	terminal_setcolor(WHITE, RED);

	terminal_write_string("This should print with different foreground and background colors...");
	getch();

	terminal_write_string("and this too! Note that newlines are now created with the new colors!\nPRESS ENTER TO FINISH THE PROGRAM");

	terminal_shutdown();
}
