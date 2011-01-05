#include "SpikeSorter.h"

int main( int argc, char *argv[])
{
	buff = (buff_data_struct *)rtai_malloc(nam2num(BUFFNAME), SHMSIZ);
  	if (buff == NULL) 
	{
		printf("rtai_malloc() failed (maybe /dev/rtai_shm is missing)!\n");
		return -1;
   	}
	gtk_init(&argc, &argv);
	create_gui(); 	
	gtk_main();

	return 0;
}
