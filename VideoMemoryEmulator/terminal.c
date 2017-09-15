#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <curses.h>

#include "terminal.h"

#define BUFFER_POSITION(ROW, COL) (ROW * VGA_WIDTH + COL)

static uint16_t terminal_buffer[VGA_HEIGHT * VGA_WIDTH];

int position_col = 0, position_row = 0;

// Default color attribute is white on black
// The color number is 16 bits here, but only the low 8 bits are used
uint16_t color = (BLACK << 4) | WHITE;

// Draw function, declared here, defined below
void draw(void);

void terminal_setcolor(uint8_t foreground, uint8_t background) {
	// TODO: Your one-liner code here
}

void move_cursor(void) {
	move(position_row, position_col);
}

void terminal_write(char character) {
	// TODO:
	// This is stub code: just write everything in the first line, using a default color combination
	// Your function is much richer than this (for reference, my implementation is 50-60 lines)
	terminal_buffer[BUFFER_POSITION(position_row, position_col)] = character | (0x70 << 8);
	position_col++;

	if(position_col >= VGA_WIDTH) {
		position_col = 0;
	}

	// Keep those below
	move_cursor();
	draw();
}

void terminal_write_string(char *text) {
	for(int i = 0; i < strlen(text); i++) {
		terminal_write(text[i]);
	}
}

void terminal_clear(void) {
	// ASCII for 'space' + color parameters
	uint16_t blank = ' ' | (color << 8);

	for(int i = 0; i < VGA_HEIGHT; i++) {
		for(int j = 0; j < VGA_WIDTH; j++) {
			terminal_buffer[BUFFER_POSITION(i, j)] = blank;
		}
	}

	position_col = 0;
	position_row = 0;

	move_cursor();
	draw();
}

void terminal_initialize(void) {
	initscr();
	start_color();

	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			// Define color pair #i such that:
			//   High-end 4 bits indicate the background color
			//   Low-end 4 bits indicate the foreground color
			init_pair((j << 4) | i, i, j);
		}
	}

	terminal_clear();
}

void terminal_shutdown(void) {
	getch();
	endwin();
}

void draw(void) {
	// Move around and print the characters in the buffer

	for(int i = 0; i < VGA_HEIGHT; i++) {
		for(int j = 0; j < VGA_WIDTH; j++) {
			uint8_t color = (terminal_buffer[BUFFER_POSITION(i, j)] >> 8) & 0xFF;

			uint8_t letter = terminal_buffer[BUFFER_POSITION(i, j)]  & 0xFF;

			attron(COLOR_PAIR(color));
			move(i, j);
			addch(letter);
			attroff(COLOR_PAIR(color));
		}
	}

	// Move back to the original position
	move_cursor();

	// Refresh the screen
	refresh();
}
