CC     = gcc -std=c99
DEBUG  = -Wall -g

CFLAGS = `pkg-config --cflags --libs glib-2.0` -lm
GFLAGS = `pkg-config --cflags --libs gtk+-3.0 gmodule-export-2.0` -lm

COMMON = -Isrc/main/ src/main/matrix.c src/main/utils.c src/main/latex.c src/main/graphviz.c
GUICOMMON = src/main/dialogs.c

# Rules
all: clean bin/main bin/mwis

test: clean bin/test/mwis

# Algorithms
mwis: clean bin/mwis bin/test/mwis

# Main binaries
bin/main: src/main/main.c
	$(CC) $(DEBUG) -o $@ $? $(COMMON) $(GUICOMMON) $(GFLAGS)

bin/mwis: src/mwis/main.c src/mwis/mwis.c src/mwis/report.c
	$(CC) $(DEBUG) -o $@ $? $(COMMON) $(GUICOMMON) $(GFLAGS)

# Test binaries
bin/test/mwis: src/mwis/test.c src/mwis/mwis.c src/mwis/report.c
	$(CC) $(DEBUG) -o $@ $? $(COMMON) $(CFLAGS)

# Clean
clean:
	rm -f `find bin/ -executable -type f`
	rm -f `find bin/test/ -executable -type f`
	rm -f reports/graph.*
	rm -f reports/mwis.*
	rm -f reports/*.log
