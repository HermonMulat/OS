#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

#define VGA_HEIGHT 25
#define VGA_WIDTH  80

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

void terminal_setcolor(uint8_t foreground, uint8_t background);
void terminal_write(char character);
void terminal_write_string(char *text);
void terminal_initialize(void);
void terminal_shutdown(void);
void terminal_clear(void);

#endif /* TERMINAL_H */
