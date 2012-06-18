prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm -lmeschach
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: SpikeSorter

SpikeSorter: SpikeSorter.c ../Library/Misc/Misc.o ../Library/Messages/TempMat2KrnlSpk.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< ../Library/Misc/Misc.o ../Library/Messages/TempMat2KrnlSpk.o

../Library/Misc/Misc.o: ../Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../Library/Misc/Misc	

../Library/Messages/TempMat2KrnlSpk.o: ../Library/Messages/TempMat2KrnlSpk.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../Library/Messages/TempMat2KrnlSpk

clean:
	rm -f *.o SpikeSorter ../Library/Misc/*.o ../Library/Messages/*.o

.PHONY: clean
