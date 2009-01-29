-include makefile.local

all: run-it

status-display-test: test.c status_display.c
	$(CC) -g $(CFLAGS) -o $@ $^

.PHONY: run-it
run-it: status-display-test
	@./status-display-test


# Try "COLUMNS=14 ./status-display-test" to test ellipsization.

