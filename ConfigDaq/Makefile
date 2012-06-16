prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: ConfigDaq

ConfigDaq: ConfigDaq.c ../Library/Misc/Misc.o Interrogate.o ../Library/Messages/DaqCon2KrnlSpk.o FileHandler.o  FileHandler_v0.o 
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< ../Library/Misc/Misc.o Interrogate.o ../Library/Messages/DaqCon2KrnlSpk.o FileHandler.o  FileHandler_v0.o 

../Library/Misc/Misc.o: ../Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../Library/Misc/Misc

../Library/Messages/DaqCon2KrnlSpk.o: ../Library/Messages/DaqCon2KrnlSpk.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../Library/Messages/DaqCon2KrnlSpk
	
Interrogate.o: Interrogate.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Interrogate

FileHandler.o: FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### FileHandler

FileHandler_v0.o: FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### FileHandler_v0

clean:
	rm -f *.o ConfigDaq ../Library/Misc/*.o ../Library/Messages/*.o

.PHONY: clean
