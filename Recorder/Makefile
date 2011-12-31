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

Recorder: Recorder.c ../DataHandling/DataHandling.o ../DataHandling/v0/DataFormat_v0.o ../DataHandling/v0/Buffer_v0.o ../Misc/Misc.o ../Interrogate/Interrogate.o
	$(CC) $(LXRT_CFLAGS) -o $@ $< ../DataHandling/DataHandling.o ../DataHandling/v0/DataFormat_v0.o ../DataHandling/v0/Buffer_v0.o ../Misc/Misc.o ../Interrogate/Interrogate.o $(LXRT_LDFLAGS) $(COMEDI_FLAGS) $(GTK_FLAGS) 

../DataHandling/DataHandling.o: ../DataHandling/DataHandling.c 
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $<   $(GTK_FLAGS) 
	
../DataHandling/v0/Buffer_v0.o: ../DataHandling/v0/Buffer_v0.c 
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $<   $(GTK_FLAGS) 	

../DataHandling/v0/DataFormat_v0.o: ../DataHandling/v0/DataFormat_v0.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 
	
../Misc/Misc.o: ../Misc/Misc.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 	

../Interrogate/Interrogate.o: ../Interrogate/Interrogate.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 	
	

clean:
	rm -f *.o Recorder ../DataHandling/*.o ../DataHandling/v0/*.o ../Misc/*.o ../Interrogate/*.o

.PHONY: clean
