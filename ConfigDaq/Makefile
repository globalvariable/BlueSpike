prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
COMEDI_FLAGS = -lcomedi -lm 
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: ConfigDaq

ConfigDaq: ConfigDaq.c ../Library/Misc/Misc.o Interrogate.o ../Library/Messages/DaqCon2KrnlSpk.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(COMEDI_FLAGS) $(GTK_FLAGS)  -o $@ $< ../Library/Misc/Misc.o Interrogate.o ../Library/Messages/DaqCon2KrnlSpk.o 

../Library/Misc/Misc.o: ../Library/Misc/Misc.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 	

../Library/Messages/DaqCon2KrnlSpk.o: ../Library/Messages/DaqCon2KrnlSpk.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 	
	
Interrogate.o: Interrogate.c
	$(CC)  $(LXRT_CFLAGS)  -c -o $@ $< $(GTK_FLAGS) 		

clean:
	rm -f *.o ConfigDaq ../Library/Misc/*.o ../Library/Messages/*.o

.PHONY: clean
