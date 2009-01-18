/* status_display.c */

#include "status_display.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Line types.
enum {
	StatusLine,
	LiteralLine
};

typedef struct status_line {
	char*	title;
	int  	type;
} status_line;

static status_line* lines;
static int num_lines, allocated_lines;
static int cur_line;
static int max_title_length;
static int num_columns;
static char* status_buffer;

// Tweex.
enum {
	divider_width = 2
};


static void redraw_all()
{
	//*** Will not redraw the values, as we don't store those.

	// Go to the first line.
	if (cur_line > 0)
		printf("\e[%dA", cur_line);

	// Clear everything.
	printf("\e[J");

	// Draw the titles.
	status_line* stopper = &lines[num_lines];
	status_line* line = lines;
	for (; line < stopper; ++line) {
		printf("%s", line->title);
		if (line->type == StatusLine)
			printf(":");
		printf("\n");
		}

	// Put the cursor on the last line.
	if (num_lines > 0) {
		printf("\e[A");
		cur_line = num_lines - 1;
		}
	else
		cur_line = 0;
}


void status_display_start()
{
	// Have we started already?
	if (lines)
		return;

	num_lines = 0;
	allocated_lines = 8;
	lines = calloc(allocated_lines, sizeof(status_line));
	cur_line = 0;
	max_title_length = 0;

	// Figure out how many columns we have.
	num_columns = 80; 	// By default.
	char* columns_str = getenv("COLUMNS");
	if (columns_str)
		num_columns = atoi(columns_str);
	status_buffer = malloc(num_columns + 1);
}


void status_display_end()
{
	if (!lines)
		return;

	// Send the cursor to the end.
	printf("\e[%dB\n", num_lines - cur_line - 1);

	status_line* stopper = &lines[num_lines];
	status_line* line = lines;
	for (; line < stopper; ++line)
		free(line->title);
	free(lines);
	free(status_buffer);
	line = NULL;
}


static status_line* add_line(const char* title, int type)
{
	// Grow "lines" if needed.
	if ((num_lines + 1) >= allocated_lines) {
		int new_allocated_lines = allocated_lines + 8;
		status_line* new_lines = calloc(allocated_lines, sizeof(status_line));
		memcpy(new_lines, lines, new_allocated_lines * sizeof(status_line));
		free(lines);
		lines = new_lines;
		allocated_lines = new_allocated_lines;
		}

	// Add the line.
	status_line* line = &lines[num_lines];
	num_lines += 1;
	line->title = strdup(title);
	line->type = type;

	// If the column divider has moved, we need to redraw everything.
	int title_length = strlen(title);
	if (title_length > max_title_length) {
		max_title_length = title_length;
		redraw_all();
		}

	// Otherwise, just draw the new line.
	else {
		// Go to the (previous) last line.
		int old_last_line = line - lines - 1;
		if (cur_line < old_last_line) {
			printf("\e[%dE", old_last_line - cur_line);
			cur_line = old_last_line;
			}

		// Draw the new line.
		printf("\n%s", line->title);
		if (line->type == StatusLine)
			printf(":");
		cur_line = old_last_line + 1;
		}

	return line;
}


void set_status(const char* title, const char* value)
{
	status_display_start();

	// Find the line.
	status_line* stopper = &lines[num_lines];
	status_line* line = lines;
	for (; line < stopper; ++line) {
		if (strcmp(line->title, title) == 0)
			break;
		}

	// If that line doesn't exist, add it.
	if (line >= stopper) {
		line = add_line(title, StatusLine);
		}

	// Go to the line.
	int line_number = line - lines;
	if (cur_line > line_number)
		printf("\e[%dA", cur_line - line_number);
	else if (cur_line < line_number)
		printf("\e[%dB", line_number - cur_line);
	cur_line = line_number;
	
	// Print the status.
	int value_column = max_title_length + 1 + divider_width + 1;
	printf("\e[%dG\e[K", value_column);
	int status_width = strlen(value);
	int value_width = num_columns - value_column - 1;
	if (status_width > value_width) {
		// Need to ellipsize the value.  We do it at the front.
		strcpy(status_buffer, "...");
		strcat(status_buffer, value + (status_width - value_width) + 3);
		printf("%s", status_buffer);
		}
	else
		printf("%s", value);
	fflush(stdout);
}


void append_status_line(const char* title)
{
	status_display_start();

	add_line(title, StatusLine);
}


void append_nonstatus_line(const char* title)
{
	status_display_start();

	add_line(title, LiteralLine);
}


/*

Copyright (c) 2009 Steve Folta

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

