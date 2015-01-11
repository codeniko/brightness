PROGRAM=brightness
CFLAGS=-W -Wall -g -Wextra -std=c99
CFLAGS_PRODUCTION=-s -O2 -flto -march=native -std=c99
INSTALL_PATH=/usr/local/bin

all: $(PROGRAM)
.PHONY: all
.PHONY: clean
.PHONY: install
.PHONY: uninstall

$(PROGRAM): $(PROGRAM).o
	gcc $(CFLAGS_PRODUCTION) -o $(PROGRAM) $(PROGRAM).o

$(PROGRAM).o: $(PROGRAM).c
	gcc $(CFLAGS_PRODUCTION) -c $(PROGRAM).c

install: 
	cp $(PROGRAM) $(INSTALL_PATH) || exit 1
	chown root:staff $(INSTALL_PATH)/$(PROGRAM) || exit 1
	chmod 4755 $(INSTALL_PATH)/$(PROGRAM) || exit 1

uninstall:
	rm $(INSTALL_PATH)/$(PROGRAM) || exit 1

clean:
	rm -f $(PROGRAM) *.o
