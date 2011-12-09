prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
COMEDI_FLAGS = -lcomedi -lm 
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: Recorder

Recorder: Recorder.c DataFormat/DataFormat.o DataFormat/v0/DataFormat_v0.o
	$(CC) $(LXRT_CFLAGS) -o $@ $< DataFormat/DataFormat.o DataFormat/v0/DataFormat_v0.o $(LXRT_LDFLAGS) $(COMEDI_FLAGS) $(GTK_FLAGS) 


DataFormat/DataFormat.o: DataFormat/DataFormat.c 
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $<   $(GTK_FLAGS) 

DataFormat/v0/DataFormat_v0.o: DataFormat/v0/DataFormat_v0.c
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $< $(GTK_FLAGS) 

clean:
	rm -f *.o Recorder DataFormat/DataFormat.o DataFormat/v0/DataFormat_v0.o

.PHONY: clean
