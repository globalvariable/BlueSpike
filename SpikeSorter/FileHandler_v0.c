#include "FileHandler_v0.h"



int write_template_matching_data_v0(int num, ...)
{
	FILE *fp;	
	char  *path_folder;
	char  path[600];
	TemplateMatchingData	*template_matching_data;
	char message[600];
	int i,j,k,m,n;

  	va_list arguments;
	va_start ( arguments, num );   
    	path_folder = va_arg ( arguments, char*); 
      	template_matching_data = va_arg ( arguments, TemplateMatchingData*); 	
	va_end ( arguments );

	strcpy(path, path_folder);
	strcat(path, "/TemplateMatching");

	if ((fp = fopen(path, "w")) == NULL)  { sprintf(message, "Couldn' t create %s.", path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", message); return 0; }
	
	fprintf(fp, "----------TemplateMatching Data----------\n");	
	fprintf(fp,"VERSION\t%u\n", 0);
	fprintf(fp,"MAX_NUM_OF_MWA\t%d\n", MAX_NUM_OF_MWA);
	fprintf(fp,"MAX_NUM_OF_CHAN_PER_MWA\t%d\n",MAX_NUM_OF_CHAN_PER_MWA);
	fprintf(fp,"MAX_NUM_OF_UNIT_PER_CHAN\t%d\n",MAX_NUM_OF_UNIT_PER_CHAN);
	fprintf(fp,"NUM_OF_SAMP_PER_SPIKE\t%d\n",NUM_OF_SAMP_PER_SPIKE);		
	fprintf(fp, "--------- Templates ------\n");
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN; k++)
			{
				fprintf(fp, "---------\tMWA:\t%d\tChannel:\t%d\tUnit:\t%d\t------\n", i,j, k);																								
				for (m=0; m<NUM_OF_SAMP_PER_SPIKE; m++)
				{
					fprintf(fp, "%.20f\n", (*template_matching_data)[i][j][k].template[m]);
				}
			}
		}
	}																					
	fprintf(fp, "--------- Inverted Covariance Matrix ------\n");																					
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN; k++)
			{
				fprintf(fp, "---------\tMWA:\t%d\tChannel:\t%d\tUnit:\t%d\t------\n", i,j, k);																								
				for (m=0; m<NUM_OF_SAMP_PER_SPIKE; m++)
				{
					for (n=0; n<NUM_OF_SAMP_PER_SPIKE; n++)
					{	
						fprintf(fp, "%.20f\n", (*template_matching_data)[i][j][k].inv_S[m][n]);
					}
				}
			}
		}
	}
	fprintf(fp, "--------- sqrt(det(S)) / log(det(S)) / probability_threshold / sorting_on_off / include_unit ------\n");																					
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		fprintf(fp, "--------- MWA: %d\n", i);																							
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN; k++)
			{
				fprintf(fp, "%.20f\n", (*template_matching_data)[i][j][k].sqrt_det_S);
				fprintf(fp, "%.20f\n", (*template_matching_data)[i][j][k].log_det_S);	
				fprintf(fp, "%E\n", (*template_matching_data)[i][j][k].probability_thres);
				fprintf(fp, "%d\n", (*template_matching_data)[i][j][k].sorting_on);		
				fprintf(fp, "%d\n", (*template_matching_data)[i][j][k].include_unit);	
			}
		}
	}
	fprintf(fp, "----------End of TemplateMatching Data----------\n");				
	fclose(fp);

	if ((fp = fopen("path_initial_directory", "r")) == NULL)  { print_message(ERROR_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", "Couldn' t read ./path_initial_directory."); return 0; }
	fclose(fp);
	if ((fp = fopen("path_initial_directory", "w")) == NULL)  { print_message(ERROR_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", "Couldn' t write to ./path_initial_directory."); return 0; }
	fprintf(fp, "%s", path); 
	fclose (fp);	
	print_message(INFO_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", "Succesuflly saved TemplateMatching data file.");	
	return 1;
}
int read_template_matching_data_v0(int num, ...)
{
	TempMat2KrnlSpkMsg *template_matching_2_kernel_spike_msgs = NULL;
	TemplateMatchingData	*template_matching_data;
	char *path;	
	char message[500];
	char line[300];
	char word[100];
	int i, j, k, m, n, line_cntr = 0;
	int max_num_of_mwa, max_num_of_channel_per_mwa, max_num_of_unit_per_chan, num_of_samp_per_spike;
	FILE *fp=NULL;

  	va_list arguments;
	va_start ( arguments, num );   
    	path = va_arg ( arguments, char*); 
      	template_matching_2_kernel_spike_msgs = va_arg ( arguments, TempMat2KrnlSpkMsg*); 	
      	template_matching_data = va_arg ( arguments, TemplateMatchingData*); 	
	va_end ( arguments );

	if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  ----------TemplateMatching Data----------
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  VERSION	XX
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  MAX_NUM_OF_MWA	XX
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_mwa = (int)atof(word);
	if (MAX_NUM_OF_MWA != max_num_of_mwa)
	{
		printf("ERROR: TemplateMatching file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_MWA = %d\n", MAX_NUM_OF_MWA);	
		fclose(fp);
		return 0;
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_mwa = (int)atof(word);	
	if (MAX_NUM_OF_CHAN_PER_MWA != max_num_of_channel_per_mwa)
	{
		printf("ERROR: TemplateMatching file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
		fclose(fp);
		return 0;
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_unit_per_chan = (int)atof(word);	
	if (MAX_NUM_OF_UNIT_PER_CHAN != max_num_of_unit_per_chan)
	{
		printf("ERROR: TemplateMatching file was saved when MAX_NUM_OF_UNIT_PER_CHAN = %d\n", max_num_of_unit_per_chan);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_UNIT_PER_CHAN);	
		fclose(fp);
		return 0;
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	num_of_samp_per_spike = (int)atof(word);	
	if (NUM_OF_SAMP_PER_SPIKE != num_of_samp_per_spike)
	{
		printf("ERROR: TemplateMatching file was saved when NUM_OF_SAMP_PER_SPIKE = %d\n", num_of_samp_per_spike);
		printf("ERROR: Now it is NUM_OF_SAMP_PER_SPIKE = %d\n", NUM_OF_SAMP_PER_SPIKE);	
		fclose(fp);
		return 0;
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} ///  "--------- Templates ------\n"
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN; k++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} /// "---------\tMWA:\t%d\tChannel:\t%d\tUnit:\t%d\t------\n"
				for (m=0; m<NUM_OF_SAMP_PER_SPIKE; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
					if (!(get_word_in_line('\t', 0, word, line, TRUE))) { fclose(fp); return 0; }	
					(*template_matching_data)[i][j][k].template[m] = atof(word);																			
				}
			}
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} ///  "--------- Inverted Covariance Matrix ------\n"
	for (i=0; i<max_num_of_mwa; i++)
	{
		for (j=0; j<max_num_of_channel_per_mwa; j++)
		{
			for (k=0; k<max_num_of_unit_per_chan; k++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} ///  "---------\tMWA:\t%d\tChannel:\t%d\tUnit:\t%d\t------\n"
				for (m=0; m < num_of_samp_per_spike; m++)
				{
					for (n=0; n<num_of_samp_per_spike; n++)
					{
						if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
						if (!(get_word_in_line('\t', 0, word, line, TRUE))) {	fclose(fp); return 0; }
						(*template_matching_data)[i][j][k].inv_S[m][n] = atof(word);																						
					}
				}
			}
		}
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} /// "--------- sqrt(det(S)) / log(det(S)) / probability_threshold / sorting_on_off / include_unit ------\n"
	for (i=0; i<max_num_of_mwa; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} /// "--------- MWA: %d\n"
		for (j=0; j<max_num_of_channel_per_mwa; j++)
		{
			for (k=0; k<max_num_of_unit_per_chan; k++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
				if (!(get_word_in_line('\t', 0, word, line, TRUE))) {	fclose(fp); return 0; }
				(*template_matching_data)[i][j][k].sqrt_det_S = atof(word);	

				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
				if (!(get_word_in_line('\t', 0, word, line, TRUE))) {	fclose(fp); return 0; }
				(*template_matching_data)[i][j][k].log_det_S = atof(word);	

				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
				if (!(get_word_in_line('\t', 0, word, line, TRUE))) {	fclose(fp); return 0; }
				(*template_matching_data)[i][j][k].probability_thres = atof(word);	

				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
				if (!(get_word_in_line('\t', 0, word, line, TRUE))) {	fclose(fp); return 0; }
				if ((bool)atof(word))
				{
					if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_UNIT_SORTING_ON, i, j, k, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
						return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "read_template_matching_data_v0", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
				}
				else
				{
					if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_UNIT_SORTING_OFF, i, j, k, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
						return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "read_template_matching_data_v0", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
				}

				if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
				if (!(get_word_in_line('\t', 0, word, line, TRUE))) {	fclose(fp); return 0; }
				if ((bool)atof(word))
				{
					if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_INCLUDE_UNIT, i, j, k, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
						return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "read_template_matching_data_v0", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
				}
				else
				{
					if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_EXCLUDE_UNIT, i, j, k, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
						return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "read_template_matching_data_v0", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
				}
			}
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
	if (strcmp(line, "----------End of TemplateMatching Data----------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", "strcmp(line, ----------End of TemplateMatching Data----------) != 0.");	
	}  		

	fclose(fp);

	print_message(INFO_MSG ,"SpikeSorter", "FileHandler", "read_template_matching_data_v0", "Succesuflly read TemplateMatching data file.");	
	return 1;
}





