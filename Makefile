all: example

CCFLAGS = -m64 -O2 -std=gnu11

example.o: src/example/example.c src/example/mini_dlg.h src/mini_dlg_flat.h 
	$(CC) $(CCFLAGS) -o $@ -c src/example/example.c

mini_dlg.o: src/example/mini_dlg.c src/example/mini_dlg.h src/mini_dlg_flat.h 
	$(CC) $(CCFLAGS) -o $@ -c src/example/mini_dlg.c

example:  example.o mini_dlg.o
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f example