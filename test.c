#include "status_display.h"
#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
	char str[128];
	static const char* value_names[] = {
		"One", "Two", "Three", "Four"
		};

	int counter = 0;
	int outer, inner;
	for (outer = 0; outer < 4; ++outer) {
		set_status("Outer", value_names[outer]);
		for (inner = 0; inner < 4; ++inner) {
			set_status("Inner", value_names[inner]);

			counter += 1;
			sprintf(str, "%d%%", counter * 100 / 16);
			set_status("Done", str);

			usleep(500000);
			}
		}
	status_display_end();

	return 0;
}

