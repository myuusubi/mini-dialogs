all: build/example

CCFLAGS = -m64 -O2 -std=c11

build/example.o: src/example/example.c src/example/mini_dlg.h src/releases/mini_dlg_flat.h 
	$(CC) $(CCFLAGS) -o $@ -c src/example/example.c

build/mini_dlg.o: src/example/mini_dlg.c src/example/mini_dlg.h src/releases/mini_dlg_flat.h 
	$(CC) $(CCFLAGS) -o $@ -c src/example/mini_dlg.c

build/example: build/example.o build/mini_dlg.o
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm -f build/*.o
	rm -f build/example