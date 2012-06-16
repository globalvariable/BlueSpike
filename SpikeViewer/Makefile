prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: SpikeViewer

SpikeViewer: SpikeViewer.c ../Library/Misc/Misc.o ../Library/Messages/FiltCtrl2KrnlSpk.o ../Library/Messages/SpkThres2KrnlSpk.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< ../Library/Misc/Misc.o ../Library/Messages/FiltCtrl2KrnlSpk.o ../Library/Messages/SpkThres2KrnlSpk.o

../Library/Misc/Misc.o: ../Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../Library/Misc/Misc

../Library/Messages/FiltCtrl2KrnlSpk.o: ../Library/Messages/FiltCtrl2KrnlSpk.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../Library/Messages/FiltCtrl2KrnlSpk

../Library/Messages/SpkThres2KrnlSpk.o: ../Library/Messages/SpkThres2KrnlSpk.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../Library/Messages/SpkThres2KrnlSpk

clean:
	rm -f *.o SpikeViewer ../Library/Misc/*.o ../Library/Messages/*.o

.PHONY: clean
