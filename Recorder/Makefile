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

Recorder: Recorder.c ../Library/DataHandling/DataHandling.o ../Library/DataHandling/v0/Buffer_v0.o ../Library/DataHandling/v0/DataFormat_v0.o ../Library/Misc/Misc.o ../Library/Interrogate/Interrogate.o
	$(CC) $(LXRT_CFLAGS) -o $@ $< ../Library/DataHandling/DataHandling.o ../Library/DataHandling/v0/Buffer_v0.o ../Library/DataHandling/v0/DataFormat_v0.o ../Library/Misc/Misc.o ../Library/Interrogate/Interrogate.o $(LXRT_LDFLAGS) $(COMEDI_FLAGS) $(GTK_FLAGS) 

../Library/DataHandling/DataHandling.o: ../Library/DataHandling/DataHandling.c 
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $<   $(GTK_FLAGS) 
	
../Library/DataHandling/v0/Buffer_v0.o: ../Library/DataHandling/v0/Buffer_v0.c 
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $<   $(GTK_FLAGS) 	

../Library/DataHandling/v0/DataFormat_v0.o: ../Library/DataHandling/v0/DataFormat_v0.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 
	
../Library/Misc/Misc.o: ../Library/Misc/Misc.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 	

../Library/Interrogate/Interrogate.o: ../Library/Interrogate/Interrogate.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 	
	

clean:
	rm -f *.o Recorder ../Library/DataHandling/*.o ../Library/DataHandling/v0/*.o ../Library/Misc/*.o ../Library/Interrogate/*.o

.PHONY: clean
