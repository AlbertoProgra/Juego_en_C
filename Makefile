CC=gcc
CFLAGS=-Wall -I/usr/local/include -L/usr/local/lib
ALLEGRO_CFLAGS=-Wall -I/usr/local/include -L/usr/local/lib -lallegro_image -lallegro -lallegro_primitives

program: program.c
	$(CC) program.c -o program $(CFLAGS)

eccentric: eccentric.c
	$(CC) eccentric.c -o eccentric

bit_ops: bit_ops.c
	$(CC) bit_ops.c -o bit_ops

bit_count: bit_count.c
	$(CC) bit_count.c -o bit_count

editor: editor.c
	$(CC) editor.c -o editor

juego: juego.c
	$(CC) juego.c -o juego $(ALLEGRO_CFLAGS)

fs: fs.c
	$(CC) fs.c -o fs $(ALLEGRO_CFLAGS)

.PHONY: clean
clean:
	$(RM) program eccentric bit_ops bit_count editor juego
