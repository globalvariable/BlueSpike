#include "Main.h"


int main( int argc, char *argv[])
{
	cpu_set_t  mask;


	CPU_ZERO(&mask);
	CPU_SET(BLUESPIKE_USER_SPACE_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU+BLUESPIKE_USER_SPACE_CPU_THREAD_ID, &mask);
	if (sched_setaffinity(0, sizeof(mask), &mask))
		return print_message(ERROR_MSG ,"PCIe6259", "Main", "main","! sched_setaffinity(0, sizeof(mask), &mask).");		

	initialize_daq_mwa_map_data();
	start_periodic_task();
	
	gtk_init(&argc, &argv);
	create_gui();
	gtk_main();
	return 0;
}
