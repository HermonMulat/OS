#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <curses.h>

#include "terminal.h"

#define BUFFER_POSITION(ROW, COL) ((ROW) * VGA_WIDTH + (COL))

static uint16_t terminal_buffer[VGA_HEIGHT * VGA_WIDTH];

int position_col = 0, position_row = 0;

// Default color attribute is white on black
// The color number is 16 bits here, but only the low 8 bits are used
uint16_t color = (BLACK << 4) | WHITE;

// Draw function, declared here, defined below
void draw(void);

void terminal_setcolor(uint8_t foreground, uint8_t background) {
	color = (foreground | background << 4);
}

void move_cursor(void) {
	move(position_row, position_col);
}

void write_chr(char chr){
	if (chr == '\n' || chr == '\t'){
		terminal_buffer[BUFFER_POSITION(position_row, position_col)] = ' ' | color << 8;
	}
	else{
		terminal_buffer[BUFFER_POSITION(position_row, position_col)] = chr | color << 8;
	}
}

void terminal_write(char character) {
	if (position_row < VGA_HEIGHT) write_chr(character);
	position_col++;
	if (character == '\t'){
		position_col = ((position_col/8) + 1)*8;
	}
	if(position_col >= VGA_WIDTH || character == '\n') {
		position_col = 0;
		position_row++;
	}
	if (position_row >= VGA_HEIGHT){
		position_row = VGA_HEIGHT-1;
		for(int r = 0; r < VGA_HEIGHT-1; r++) {
			for(int c = 0; c < VGA_WIDTH; c++) {
				terminal_buffer[BUFFER_POSITION(r, c)] = terminal_buffer[BUFFER_POSITION((r+1), c)];
			}
		}
		// clear the last row
		uint16_t blank = ' ' | (color << 8);
		for (int i = 0; i < VGA_WIDTH; i++){
			terminal_buffer[BUFFER_POSITION((VGA_HEIGHT-1), i)] = blank;
		}

	}

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
