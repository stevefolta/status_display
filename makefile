all: status-display-test

status-display-test: test.c status_display.c
	$(CC) -g -o $@ $^


# Try "COLUMNS=14 ./status-display-test" to test ellipsization.

