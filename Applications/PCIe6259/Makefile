prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm -lmeschach
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)


all: bluespike

bluespike: Main.c Gui.o RtTask.o ConfigDaq/ConfigDaqGui.o ../../../BlueSpike/Library/Misc/Misc.o ConfigDaq/DaqCard.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ConfigDaq/FileHandler.o ConfigDaq/FileHandler_v0.o  RecordingData.o ConfigDaq/DaqMwaData.o SpikeViewer/SpikeViewerGui.o ButterFilter.o SpikeViewer/FileHandler.o SpikeViewer/FileHandler_v0.o SpikeViewer/DataHandling/DataHandling.o SpikeViewer/DataHandling/v0/DataFormat_v0.o TemplateMatching.o SpikeSorting.o SpikeSorter/SpikeSorterGui.o SpikeSorter/FileHandler.o SpikeSorter/FileHandler_v0.o CubicInterpolation.o 
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< Gui.o RtTask.o ConfigDaq/ConfigDaqGui.o  ../../../BlueSpike/Library/Misc/Misc.o ConfigDaq/DaqCard.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ConfigDaq/FileHandler.o ConfigDaq/FileHandler_v0.o RecordingData.o  ConfigDaq/DaqMwaData.o  SpikeViewer/SpikeViewerGui.o ButterFilter.o SpikeViewer/FileHandler.o  SpikeViewer/FileHandler_v0.o SpikeViewer/DataHandling/DataHandling.o SpikeViewer/DataHandling/v0/DataFormat_v0.o TemplateMatching.o SpikeSorting.o SpikeSorter/SpikeSorterGui.o SpikeSorter/FileHandler.o SpikeSorter/FileHandler_v0.o  CubicInterpolation.o ### bluespike

Gui.o: Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

RtTask.o: RtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### RtTask

RecordingData.o:RecordingData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### RecordingData

ButterFilter.o:ButterFilter.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ButterFilter

SpikeSorting.o: SpikeSorting.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SpikeSorting

TemplateMatching.o: TemplateMatching.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### TemplateMatching

CubicInterpolation.o: CubicInterpolation.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### CubicInterpolation

#################  ConfigDaq  ###########################3
ConfigDaq/ConfigDaqGui.o: ConfigDaq/ConfigDaqGui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ConfigDaq/ConfigDaqGui

ConfigDaq/DaqCard.o: ConfigDaq/DaqCard.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### ConfigDaq/DaqCard

ConfigDaq/DaqMwaData.o:ConfigDaq/DaqMwaData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DaqMwaData

ConfigDaq/FileHandler.o: ConfigDaq/FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### ConfigDaq/FileHandler

ConfigDaq/FileHandler_v0.o: ConfigDaq/FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### ConfigDaq/FileHandler_v0


#################  SpikeViewer  ############################

SpikeViewer/SpikeViewerGui.o: SpikeViewer/SpikeViewerGui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SpikeViewer/SpikeViewerGui

SpikeViewer/FileHandler.o: SpikeViewer/FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### SpikeViewer/FileHandler

SpikeViewer/FileHandler_v0.o: SpikeViewer/FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### SpikeViewer/FileHandler_v0

SpikeViewer/DataHandling/DataHandling.o: SpikeViewer/DataHandling/DataHandling.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### SpikeViewer/DataHandling/DataHandling.o

SpikeViewer/DataHandling/v0/DataFormat_v0.o: SpikeViewer/DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		### SpikeViewer/DataHandling/v0/DataFormat_v0.o

#################  SpikeSorter  ############################
SpikeSorter/SpikeSorterGui.o: SpikeSorter/SpikeSorterGui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SpikeViewer/SpikeSorterGui

SpikeSorter/FileHandler.o: SpikeSorter/FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SpikeViewer/FileHandler

SpikeSorter/FileHandler_v0.o: SpikeSorter/FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SpikeViewer/FileHandler_v0

######################################################################

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c	
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o


clean:
	rm -f bluespike *.o ConfigDaq/*.o ../../../BlueSpike/Library/Misc/*.o ../../../BlueSpike/Library/RtTaskStats/*.o SpikeViewer/*.o SpikeViewer/DataHandling/*.o SpikeViewer/DataHandling/v0/*.o SpikeSorter/*o

.PHONY: clean

