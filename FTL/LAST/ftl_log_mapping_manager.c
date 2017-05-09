// File: ftl_log_mapping_manager.c
// Data: 2013. 05. 06.
// Author: Jinsoo Yoo (jedisty@hanyang.ac.kr)
// Copyright(c)2013
// Hanyang University, Seoul, Korea
// Embedded Software Systems Laboratory. All right reserved

#include "common.h"

#ifdef DEBUG_MODE2
FILE* fp_dbg2_seq;
FILE* fp_dbg2_ran;
#endif
#ifdef DEBUG_MODE3
extern FILE* fp_dbg3_ran;
extern FILE* fp_dbg3_1_ran;
unsigned int count_dbg3=0;
#endif
#ifdef DEBUG_MODE4
extern FILE* fp_dbg4_seq;
extern FILE* fp_dbg4_ran;
#endif
#ifdef DEBUG_MODE5
extern FILE* fp_dbg5_seq;
extern FILE* fp_dbg5_ran;
#endif
#ifdef DEBUG_MODE6
extern FILE* fp_dbg6_seq;
extern FILE* fp_dbg6_ran;
unsigned int seq_erase_cnt = 0;
unsigned int seq_erase_add = 0;
unsigned int ran_erase_cnt = 0;
unsigned int ran_erase_add = 0;
#endif
#ifdef DEBUG_MODE8
extern FILE* fp_dbg8_seq;
extern FILE* fp_dbg8_ran;
unsigned int seq_data_dbg8 = 0;
unsigned int ran_log_dbg8 = 0;
unsigned int ran_data_dbg8 = 0;
unsigned int ran_total_dbg8 = 0;
#endif

void* seq_block_mapping_table_start;
void* ran_cold_block_mapping_table_start;
void* ran_hot_block_mapping_table_start;

void* seq_log_mapping_table_start;
void* ran_cold_log_mapping_table_start;
void* ran_hot_log_mapping_table_start;

hot_page_entry* hot_page_list_head;
hot_page_entry* hot_page_list_tail;
unsigned int hot_page_nb;

unsigned int alloc_seq_block_index;
unsigned int alloc_ran_cold_block_index;
unsigned int alloc_ran_hot_block_index;

int32_t last_seq_sector_nb;
int ran_cold_log_cnt;

void INIT_SEQ_BLOCK_MAPPING(int caller)
{

	int i;
	unsigned int new_phy_flash_nb;
	unsigned int new_phy_block_nb;

	if(caller == INIT){
		/* Allocation Memory for Mapping Table */
		seq_block_mapping_table_start = (void*)calloc(SEQ_MAPPING_ENTRY_NB, sizeof(seq_block_mapping_entry));
		if(seq_block_mapping_table_start == NULL){
			printf("ERROR[FTL_INIT_SEQ_BLOCK_MAPPING] Calloc mapping table fail\n");
			return;
		}

		/* If mapping_table.dat file exists */
		FILE* fp = fopen("./data/seq_block_mapping.dat","r");
		if(fp != NULL){
			fread(seq_block_mapping_table_start, sizeof(seq_block_mapping_entry), SEQ_MAPPING_ENTRY_NB, fp);
		}
		else{	
			seq_block_mapping_entry* curr_mapping_entry = (seq_block_mapping_entry*)seq_block_mapping_table_start;
			for(i=0;i<SEQ_MAPPING_ENTRY_NB;i++){

				/* Allocation New Sequential Log Block */
				GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

				curr_mapping_entry->phy_flash_nb = new_phy_flash_nb;
				curr_mapping_entry->phy_block_nb = new_phy_block_nb;

				UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, FLASH_NB, 0, EMPTY_SEQ_BLOCK);
				curr_mapping_entry += 1;
			}
		}
	}
	else if(caller == SEQ_MERGE){
		seq_block_mapping_entry* curr_mapping_entry = (seq_block_mapping_entry*)seq_block_mapping_table_start;

		for(i=0;i<SEQ_MAPPING_ENTRY_NB;i++){

			/* Allocation New Sequential Log Block */
			GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

			curr_mapping_entry->phy_flash_nb = new_phy_flash_nb;
			curr_mapping_entry->phy_block_nb = new_phy_block_nb;

			UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, FLASH_NB, 0, EMPTY_SEQ_BLOCK);

			curr_mapping_entry += 1;
		}
	}
}

void INIT_RAN_COLD_BLOCK_MAPPING(int caller)
{

	int i;
	unsigned int new_phy_flash_nb;
	unsigned int new_phy_block_nb;

	if(caller == INIT){
		/* Allocation Memory for Mapping Table */
		ran_cold_block_mapping_table_start = (void*)calloc(RAN_MAPPING_ENTRY_NB, sizeof(ran_block_mapping_entry));
		if(ran_cold_block_mapping_table_start == NULL){
			printf("ERROR[FTL_INIT_RAN_COLD_BLOCK_MAPPING] Calloc mapping table fail\n");
			return;
		}

		/* Initialization Mapping Table */

		/* If mapping_table.dat file exists */
		FILE* fp = fopen("./data/ran_cold_block_mapping.dat","r");
		if(fp != NULL){
			fread(ran_cold_block_mapping_table_start, sizeof(ran_block_mapping_entry), RAN_MAPPING_ENTRY_NB, fp);
		}
		else{
			ran_block_mapping_entry* curr_mapping_entry = (ran_block_mapping_entry*)ran_cold_block_mapping_table_start;
			for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){

				/* Allocation New Random Log Block */
				GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

				curr_mapping_entry->phy_flash_nb = new_phy_flash_nb;
				curr_mapping_entry->phy_block_nb = new_phy_block_nb;

				UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, FLASH_NB, 0, EMPTY_RAN_COLD_BLOCK);

				curr_mapping_entry += 1;
			}
		}
	}
	else if(caller == RAN_MERGE){
		ran_block_mapping_entry* curr_mapping_entry = (ran_block_mapping_entry*)ran_cold_block_mapping_table_start;
		for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){

			/* Allocation New Random Log Block */
			GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

			curr_mapping_entry->phy_flash_nb = new_phy_flash_nb;
			curr_mapping_entry->phy_block_nb = new_phy_block_nb;

			UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, FLASH_NB, 0, EMPTY_RAN_COLD_BLOCK);

			curr_mapping_entry += 1;
		}
	}
}

void INIT_RAN_HOT_BLOCK_MAPPING(int caller)
{
	int i;
	unsigned int new_phy_flash_nb;
	unsigned int new_phy_block_nb;

	if(caller == INIT){
		/* Allocation Memory for Mapping Table */
		ran_hot_block_mapping_table_start = (void*)calloc(RAN_MAPPING_ENTRY_NB, sizeof(ran_block_mapping_entry));
		if(ran_hot_block_mapping_table_start == NULL){
			printf("ERROR[FTL_INIT_RAN_HOT_BLOCK_MAPPING] Calloc mapping table fail\n");
			return;
		}

		/* Initialization Mapping Table */

		/* If mapping_table.dat file exists */
		FILE* fp = fopen("./data/ran_hot_block_mapping.dat","r");
		if(fp != NULL){
			fread(ran_hot_block_mapping_table_start, sizeof(ran_block_mapping_entry), RAN_MAPPING_ENTRY_NB, fp);
		}
		else{
			ran_block_mapping_entry* curr_mapping_entry = (ran_block_mapping_entry*)ran_hot_block_mapping_table_start;
			for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){

				/* Allocation New Random Log Block */
				GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

				curr_mapping_entry->phy_flash_nb = new_phy_flash_nb;
				curr_mapping_entry->phy_block_nb = new_phy_block_nb;

				UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, FLASH_NB, 0, EMPTY_RAN_HOT_BLOCK);

				curr_mapping_entry += 1;
			}
		}
	}	
	else if(caller == RAN_MERGE){
		ran_block_mapping_entry* curr_mapping_entry = (ran_block_mapping_entry*)ran_hot_block_mapping_table_start;
		for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){

			/* Allocation New Random Log Block */
			GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

			curr_mapping_entry->phy_flash_nb = new_phy_flash_nb;
			curr_mapping_entry->phy_block_nb = new_phy_block_nb;

			UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, FLASH_NB, 0, EMPTY_RAN_HOT_BLOCK);

			curr_mapping_entry += 1;
		}
	}
}

void INIT_SEQ_LOG_MAPPING(int caller)
{
	int i;
	if(caller == INIT){
		/* Allocation Memory for Seq Log Mapping Table */
		seq_log_mapping_table_start = (void*)calloc(SEQ_MAPPING_ENTRY_NB, sizeof(seq_log_mapping_entry));
		if(seq_log_mapping_table_start == NULL){
			printf("ERROR[FTL_INIT_SEQ_LOG_MAPPING] Calloc mapping table fail\n");
			return;
		}

		/* If mapping_table.dat file exists */
		FILE* fp = fopen("./data/seq_log_mapping.dat","r");
		if(fp != NULL){
			fread(seq_log_mapping_table_start, sizeof(seq_log_mapping_entry), SEQ_MAPPING_ENTRY_NB, fp);
		}
		else{

			seq_log_mapping_entry* curr_mapping_entry = (seq_log_mapping_entry*)seq_log_mapping_table_start;
			for(i=0;i<SEQ_MAPPING_ENTRY_NB;i++){

				curr_mapping_entry->start_page_nb = 0;
				curr_mapping_entry->page_length = 0;
				curr_mapping_entry->data_flash_nb = FLASH_NB;
				curr_mapping_entry->data_block_nb = 0;
				curr_mapping_entry->log_flash_nb = FLASH_NB;
				curr_mapping_entry->log_block_nb = 0;

				curr_mapping_entry += 1;
			}
		}
	}
	else if(caller == SEQ_MERGE){
		seq_log_mapping_entry* curr_mapping_entry = (seq_log_mapping_entry*)seq_log_mapping_table_start;
		for(i=0;i<SEQ_MAPPING_ENTRY_NB;i++){

			curr_mapping_entry->start_page_nb = 0;
			curr_mapping_entry->page_length = 0;
			curr_mapping_entry->data_flash_nb = FLASH_NB;
			curr_mapping_entry->data_block_nb = 0;
			curr_mapping_entry->log_flash_nb = FLASH_NB;
			curr_mapping_entry->log_block_nb = 0;

			curr_mapping_entry += 1;
		}
	}
}

void INIT_RAN_COLD_LOG_MAPPING(int caller)
{
	int i;

	if(caller == INIT){

		/* Allocation Memory for Ran Log Mapping Table */
		ran_cold_log_mapping_table_start = (void*)calloc(RAN_LOG_MAPPING_ENTRY_NB, sizeof(ran_log_mapping_entry));
		if(ran_cold_log_mapping_table_start == NULL){
			printf("ERROR[FTL_INIT_RAN_COLD_LOG_MAPPING] Calloc mapping table fail\n");
			return;
		}

		/* Initialization Ran Log Mapping Table */

		FILE* fp = fopen("./data/ran_cold_log_mapping.dat","r");
		if(fp != NULL){
			fread(ran_cold_log_mapping_table_start, sizeof(ran_log_mapping_entry), RAN_LOG_MAPPING_ENTRY_NB, fp);
		}
		else{
			ran_log_mapping_entry* curr_mapping_entry = (ran_log_mapping_entry*)ran_cold_log_mapping_table_start;
			for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i++){

				curr_mapping_entry->data_flash_nb = FLASH_NB;
				curr_mapping_entry->data_block_nb = 0;

				curr_mapping_entry->log_flash_nb = FLASH_NB;
				curr_mapping_entry->log_block_nb = 0;
				curr_mapping_entry->log_page_nb = 0;

				curr_mapping_entry->valid = 0;

				curr_mapping_entry += 1;
			}
		}
	}
	else if(caller == RAN_MERGE){
		ran_log_mapping_entry* curr_mapping_entry = (ran_log_mapping_entry*)ran_cold_log_mapping_table_start;
		for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i++){

			curr_mapping_entry->data_flash_nb = FLASH_NB;
			curr_mapping_entry->data_block_nb = 0;

			curr_mapping_entry->log_flash_nb = FLASH_NB;
			curr_mapping_entry->log_block_nb = 0;
			curr_mapping_entry->log_page_nb = 0;

			curr_mapping_entry->valid = 0;

			curr_mapping_entry += 1;
		}
	}
}

void INIT_RAN_HOT_LOG_MAPPING(int caller)
{
	int i;

	if(caller == INIT){

		/* Allocation Memory for Ran Log Mapping Table */
		ran_hot_log_mapping_table_start = (void*)calloc(RAN_LOG_MAPPING_ENTRY_NB, sizeof(ran_log_mapping_entry));
		if(ran_hot_log_mapping_table_start == NULL){
			printf("ERROR[FTL_INIT_RAN_HOT_LOG_MAPPING] Calloc mapping table fail\n");
			return;
		}

		/* Initialization Ran Log Mapping Table */

		FILE* fp = fopen("./data/ran_hot_log_mapping.dat","r");
		if(fp != NULL){
			fread(ran_hot_log_mapping_table_start, sizeof(ran_log_mapping_entry), RAN_LOG_MAPPING_ENTRY_NB, fp);
		}
		else{
			ran_log_mapping_entry* curr_mapping_entry = (ran_log_mapping_entry*)ran_hot_log_mapping_table_start;
			for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i++){

				curr_mapping_entry->data_flash_nb = FLASH_NB;
				curr_mapping_entry->data_block_nb = 0;

				curr_mapping_entry->log_flash_nb = FLASH_NB;
				curr_mapping_entry->log_block_nb = 0;
				curr_mapping_entry->log_page_nb = 0;

				curr_mapping_entry->valid = 0;

				curr_mapping_entry += 1;
			}
		}
	}
	else if(caller == RAN_MERGE){
		ran_log_mapping_entry* curr_mapping_entry = (ran_log_mapping_entry*)ran_hot_log_mapping_table_start;
		for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i++){

			curr_mapping_entry->data_flash_nb = FLASH_NB;
			curr_mapping_entry->data_block_nb = 0;

			curr_mapping_entry->log_flash_nb = FLASH_NB;
			curr_mapping_entry->log_block_nb = 0;
			curr_mapping_entry->log_page_nb = 0;

			curr_mapping_entry->valid = 0;

			curr_mapping_entry += 1;
		}
	}
}

void INIT_LOG_BLOCK_INDEX(void)
{
	FILE* fp = fopen("./data/log_block_index.dat","r");
	if(fp != NULL){
		fread(&alloc_seq_block_index, sizeof(unsigned int), 1, fp);
		fread(&alloc_ran_cold_block_index, sizeof(unsigned int), 1, fp);
		fread(&alloc_ran_hot_block_index, sizeof(unsigned int), 1, fp);
		fread(&ran_cold_log_cnt, sizeof(int), 1, fp);
	}
	else{
		alloc_seq_block_index = 0;
		alloc_ran_cold_block_index = 0;
		alloc_ran_hot_block_index = 0;
		ran_cold_log_cnt = 1;
	}
}

void INIT_HOT_PAGE_LIST(void)
{
	hot_page_nb = 0;
	hot_page_list_head = NULL;
	hot_page_list_tail = NULL;
}

void TERM_SEQ_BLOCK_MAPPING(void)
{
	FILE* fp = fopen("./data/seq_block_mapping.dat","w");
	if(fp==NULL){
		printf("ERROR[TERM_SEQ_BLOCK_MAPPING] File open fail\n");
		return;
	}

	/* Write the mapping table to file */
	fwrite(seq_block_mapping_table_start, sizeof(seq_block_mapping_entry),SEQ_MAPPING_ENTRY_NB,fp);

	/* Free memory for mapping table */
	free(seq_block_mapping_table_start);
}

void TERM_RAN_COLD_BLOCK_MAPPING(void)
{
	FILE* fp = fopen("./data/ran_cold_block_mapping.dat","w");
	if(fp==NULL){
		printf("ERROR[TERM_RAN_COLD_BLOCK_MAPPING] File open fail\n");
		return;
	}

	/* Write the block table to file */
	fwrite(ran_cold_block_mapping_table_start, sizeof(ran_block_mapping_entry), RAN_MAPPING_ENTRY_NB, fp);

	/* Free the block table memory */
	free(ran_cold_block_mapping_table_start);
}

void TERM_RAN_HOT_BLOCK_MAPPING(void)
{
	FILE* fp = fopen("./data/ran_hot_block_mapping.dat","w");
	if(fp==NULL){
		printf("ERROR[TERM_RAN_HOT_BLOCK_MAPPING] File open fail\n");
		return;
	}

	/* Write the block table to file */
	fwrite(ran_hot_block_mapping_table_start, sizeof(ran_block_mapping_entry), RAN_MAPPING_ENTRY_NB, fp);

	/* Free the block table memory */
	free(ran_hot_block_mapping_table_start);
}

void TERM_LOG_BLOCK_INDEX(void)
{
	FILE* fp = fopen("./data/log_block_index.dat","w");
	if(fp==NULL){
		printf("ERROR[TERM_LOG_BLOCK_INDEX] File open fail \n");
		return;
	}

	/* Write The log block index to file */
	fwrite(&alloc_seq_block_index, sizeof(unsigned int), 1, fp);
	fwrite(&alloc_ran_cold_block_index, sizeof(unsigned int), 1, fp);
	fwrite(&alloc_ran_hot_block_index, sizeof(unsigned int), 1, fp);
	fwrite(&ran_cold_log_cnt, sizeof(int), 1, fp);
}

void TERM_SEQ_LOG_MAPPING(void)
{
	FILE* fp = fopen("./data/seq_log_mapping.dat","w");
	if(fp==NULL){
		printf("ERROR[TERM_SEQ_LOG_MAPPING] File open fail\n");
		return;
	}

	/* Write the mapping table to file */
	fwrite(seq_log_mapping_table_start, sizeof(seq_log_mapping_entry),SEQ_MAPPING_ENTRY_NB,fp);

	/* Free the mapping table memory */
	free(seq_log_mapping_table_start);
}

void TERM_RAN_COLD_LOG_MAPPING(void)
{
	FILE* fp = fopen("./data/ran_cold_log_mapping.dat","w");
	if(fp==NULL){
		printf("ERROR[TERM_RAN_COLD_LOG_MAPPING] File open fail\n");
		return;
	}

	/* Write the mapping table to file */
	fwrite(ran_cold_log_mapping_table_start, sizeof(ran_log_mapping_entry),RAN_LOG_MAPPING_ENTRY_NB, fp);

	/* Free the mapping table memory */
	free(ran_cold_log_mapping_table_start);
}

void TERM_RAN_HOT_LOG_MAPPING(void)
{
	FILE* fp = fopen("./data/ran_hot_log_mapping.dat","w");
	if(fp==NULL){
		printf("ERROR[TERM_RAN_HOT_LOG_MAPPING] File open fail\n");
		return;
	}

	/* Write the mapping table to file */
	fwrite(ran_hot_log_mapping_table_start, sizeof(ran_log_mapping_entry),RAN_LOG_MAPPING_ENTRY_NB, fp);

	/* Free the mapping table memory */
	free(ran_hot_log_mapping_table_start);
}

void TERM_HOT_PAGE_LIST(void)
{
	int i;

	hot_page_entry* curr_entry = hot_page_list_head;
	hot_page_entry* prev_entry;
	for(i=0;i<hot_page_nb;i++){
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
		free(prev_entry);
	}
}

int SET_WRITE_TYPE(int32_t sector_nb, unsigned int length)
{
#ifdef FAST_SEQ_COND
	unsigned int log_page_nb_i = sector_nb / (int32_t)SECTORS_PER_PAGE;
        unsigned int log_page_nb_r = log_page_nb_i % (int32_t)PAGE_NB;
#endif
	if(last_seq_sector_nb + 1 == sector_nb){
		return CONT_SEQ_WRITE;
	}
#ifndef FAST_SEQ_COND
	else if(length >= SEQ_THRESHOLD){
		return NEW_SEQ_WRITE;
	}
#else
	else if(log_page_nb_r == 0){
		return NEW_SEQ_WRITE;
	}
#endif

	int ran_write_type = FIND_IN_HOT_PAGE_LIST(sector_nb);

	if(ran_write_type == HOT_RAN_WRITE){
		return HOT_RAN_WRITE;
	}
	else if(ran_write_type == COLD_RAN_WRITE){
		return COLD_RAN_WRITE;
	}
	else{
		printf("ERROR[SET_WRITE_TYPE] Wrong write type\n");
		return FAIL;
	}		
}

int FIND_IN_HOT_PAGE_LIST(int32_t sector_nb)
{
	int i = 0;
	int write_type = COLD_RAN_WRITE;
//temp
	int32_t log_page_nb_i = sector_nb / (int32_t)SECTORS_PER_PAGE;
	hot_page_entry* curr_entry = hot_page_list_head;

//	printf("\t[%s] sector_nb  %d, log_page_nb %d\n",__FUNCTION__, sector_nb, log_page_nb_i);

	for(i=0;i<hot_page_nb;i++){
		if(curr_entry->hot_page_nb == log_page_nb_i){
			write_type = HOT_RAN_WRITE;
			break;
		}
		curr_entry = curr_entry->next;
	}

	INSERT_HOT_PAGE(log_page_nb_i);

	return write_type;
}

void INSERT_HOT_PAGE(int32_t log_page_nb)
{
	hot_page_entry* curr_entry = (hot_page_entry*)calloc(1, sizeof(hot_page_entry));
	hot_page_entry* prev_entry;

	if(curr_entry == NULL){
		printf("ERROR[INSERT_HOT_PAGE] Calloc fail\n");
		return;
	}

	curr_entry->hot_page_nb = log_page_nb;
	curr_entry->next = NULL;

	if(hot_page_nb == 0){
		hot_page_list_head = curr_entry;
		hot_page_list_tail = curr_entry;
		hot_page_nb++;
	}
	else if(hot_page_nb < HOT_PAGE_NB_THRESHOLD){
		hot_page_list_tail->next = curr_entry;
		hot_page_list_tail = curr_entry;
		hot_page_nb++;
	}
	else{
		hot_page_list_tail->next = curr_entry;
		hot_page_list_tail = curr_entry;

		prev_entry = hot_page_list_head;
		hot_page_list_head = hot_page_list_head->next;
		free(prev_entry);
	}
}


seq_block_mapping_entry* GET_SEQ_BLOCK_MAPPING_ENTRY(int seq_mapping_index)
{
	if(seq_mapping_index >= SEQ_MAPPING_ENTRY_NB){
		printf("ERROR[GET_SEQ_BLOCK_MAPPING_ENTRY] Exceed seq_mapping_index\n");
		return FAIL;
	}

	seq_block_mapping_entry* seq_block_entry = (seq_block_mapping_entry*)seq_block_mapping_table_start + seq_mapping_index;
	return seq_block_entry;
}

seq_log_mapping_entry* GET_SEQ_LOG_MAPPING_ENTRY(int seq_mapping_index)
{
	if(seq_mapping_index >= SEQ_MAPPING_ENTRY_NB){
		printf("ERROR[GET_SEQ_LOG_MAPPING_ENTRY] Exceed seq_mapping_index\n");
		return FAIL;
	}

	seq_log_mapping_entry* seq_log_entry = (seq_log_mapping_entry*)seq_log_mapping_table_start + seq_mapping_index;
	return seq_log_entry;
}

ran_block_mapping_entry* GET_RAN_BLOCK_MAPPING_ENTRY(int ran_mapping_index, int type)
{
	if(ran_mapping_index >= RAN_LOG_MAPPING_ENTRY_NB){
		printf("ERROR[GET_RAN_BLOCK_MAPPING_ENTRY] Exceed ran_mapping_index\n");
		return FAIL;
	}

	int mapping_index = ran_mapping_index / PAGE_NB;
	ran_block_mapping_entry* ran_block_entry = NULL;

	if(type == COLD_RAN){
		ran_block_entry = (ran_block_mapping_entry*)ran_cold_block_mapping_table_start + mapping_index;
	}
	else if(type == HOT_RAN){
		ran_block_entry = (ran_block_mapping_entry*)ran_hot_block_mapping_table_start + mapping_index;
	}
	else{
		printf("ERROR[GET_RAN_BLOCK_MAPPING_ENTRY] Wrong type input\n");
	}

	return ran_block_entry;
}

ran_log_mapping_entry* GET_RAN_LOG_MAPPING_ENTRY(int ran_mapping_index, int type)
{
	if(ran_mapping_index >= RAN_LOG_MAPPING_ENTRY_NB){
		printf("ERROR[GET_RAN_LOG_MAPPING_ENTRY] Exceed ran_mapping_index\n");
		return FAIL;
	}

	ran_log_mapping_entry* ran_log_entry = NULL;
	
	if(type == COLD_RAN){
		ran_log_entry = (ran_log_mapping_entry*)ran_cold_log_mapping_table_start + ran_mapping_index;
	}
	else if(type == HOT_RAN){
		ran_log_entry = (ran_log_mapping_entry*)ran_hot_log_mapping_table_start + ran_mapping_index;
	}
	else{
		printf("ERROR[GET_RAN_LOG_MAPPING_ENTRY] Wrong type input\n");
	}

	return ran_log_entry;
}

int WRITE_TO_SEQ_BLOCK(int32_t sector_nb, unsigned int length, int write_type){

#ifdef FTL_DEBUG
	printf("[WRITE_TO_SEQ_BLOCK] Start\n");
#endif

#ifdef DEBUG_MODE4
	int64_t start_dbg4, end_dbg4;
	start_dbg4 = get_usec();
#endif

	int i, ret;

	unsigned int remain_sector = length;
	unsigned int lba = sector_nb;
	unsigned int left_skip = sector_nb % SECTORS_PER_PAGE;
	unsigned int left_skip_sec = sector_nb % SECTORS_PER_PAGE;
	unsigned int right_skip;
	unsigned int write_sects;

	unsigned int old_phy_flash_nb;
	unsigned int old_phy_block_nb;

	unsigned int page_length = 0;

	int write_page_offset = 0;
	int io_page_nb;

	io_alloc_overhead = ALLOC_IO_REQUEST(sector_nb, length, WRITE, &io_page_nb);

	/* Count the number of page to write */

	while(remain_sector > 0){

		if(remain_sector > SECTORS_PER_PAGE - left_skip){
			right_skip = 0;
		}
		else{
			right_skip = SECTORS_PER_PAGE - left_skip - remain_sector;
		}
		write_sects = SECTORS_PER_PAGE - left_skip - right_skip;	

		page_length++;

		lba += write_sects;
		remain_sector -= write_sects;
		left_skip = 0;
	}

	int32_t log_page_nb_i = sector_nb / (int32_t)SECTORS_PER_PAGE;
	int32_t log_block_nb_i;

	unsigned int log_flash_nb_r;
        unsigned int log_block_nb_r;
        unsigned int log_page_nb_r;

	unsigned int seq_flash_nb;
	unsigned int seq_block_nb;

	unsigned int remain_pages = page_length;
//	unsigned int remain_pages = io_page_nb;
	unsigned int write_pages;

	seq_block_mapping_entry* seq_block_entry;
	seq_log_mapping_entry* seq_log_entry;


	left_skip = (unsigned int)(log_page_nb_i % (int32_t)PAGE_NB);
	
	if(left_skip != 0 && write_type == CONT_SEQ_WRITE){
		alloc_seq_block_index--;
	}
	else if(write_type == NEW_SEQ_WRITE){
		last_seq_sector_nb = -1;
	}

	while(remain_pages > 0){

		if(alloc_seq_block_index == SEQ_MAPPING_ENTRY_NB){
			MERGE_SEQ_BLOCK();
		}

		if(remain_pages > (PAGE_NB-left_skip)){
			right_skip = 0;
		}
		else{
			right_skip = PAGE_NB - left_skip - remain_pages;
		}
		write_pages = PAGE_NB - left_skip - right_skip;

		/* Calculate Logical Address */
		log_block_nb_i = log_page_nb_i / (int32_t)PAGE_NB;

                log_flash_nb_r = (unsigned int)(log_block_nb_i / (int32_t)BLOCK_NB);
                log_block_nb_r = (unsigned int)(log_block_nb_i % (int32_t)BLOCK_NB);
                log_page_nb_r  = (unsigned int)(log_page_nb_i % (int32_t)PAGE_NB);

		/* Find Data block */
		GET_DATA_BLOCK_MAPPING_INFO(log_flash_nb_r, log_block_nb_r, &old_phy_flash_nb, &old_phy_block_nb);

		/* Get New Sequential Log Block To Write */
		seq_block_entry = GET_SEQ_BLOCK_MAPPING_ENTRY(alloc_seq_block_index);

		seq_flash_nb = seq_block_entry->phy_flash_nb;
		seq_block_nb = seq_block_entry->phy_block_nb;

		/* Write Pages to Cell */
		for(i=0;i<write_pages;i++){
			ret = SSD_PAGE_WRITE(seq_flash_nb, seq_block_nb, log_page_nb_r+i, write_page_offset, SEQ_WRITE, io_page_nb);
			write_page_offset++;
		}

		/* Update Sequential Log Mapping */
		seq_log_entry = GET_SEQ_LOG_MAPPING_ENTRY(alloc_seq_block_index);

		if(left_skip != 0 && write_type == CONT_SEQ_WRITE){
			if(left_skip_sec != 0){
				seq_log_entry->page_length += (write_pages-1);
			}
			else{
				seq_log_entry->page_length += write_pages;
			}
		}
		else{
			seq_log_entry->start_page_nb = log_page_nb_r;
			seq_log_entry->page_length = write_pages;
			seq_log_entry->data_flash_nb = old_phy_flash_nb;
			seq_log_entry->data_block_nb = old_phy_block_nb;
			seq_log_entry->log_flash_nb = log_flash_nb_r;
			seq_log_entry->log_block_nb = log_block_nb_r;
		}

		/* Update Inverse Block Mapping */
		UPDATE_INVERSE_BLOCK_MAPPING(seq_flash_nb, seq_block_nb, log_flash_nb_r, log_block_nb_r, SEQ_BLOCK);
		for(i=0;i<write_pages;i++){
			UPDATE_INVERSE_BLOCK_VALIDITY(seq_flash_nb, seq_block_nb, log_page_nb_r+i, VALID);
		}
		if(old_phy_flash_nb != FLASH_NB){
			for(i=0;i<write_pages;i++){
				UPDATE_INVERSE_BLOCK_VALIDITY(old_phy_flash_nb, old_phy_block_nb, log_page_nb_r+i, INVALID);
			}
		}

		/* Update for Next Write Loop */
		log_page_nb_i += write_pages;
		remain_pages -= write_pages;
		left_skip = 0;
		left_skip_sec = 0;
		alloc_seq_block_index++;
		write_type = NEW_SEQ_WRITE;
	}

	last_seq_sector_nb = sector_nb + length - 1;

	INCREASE_IO_REQUEST_SEQ_NB();


#ifdef DEBUG_MODE4
	end_dbg4 = get_usec();
	fprintf(fp_dbg4_seq,"SEQ\t%d\t%ld\n", page_length, end_dbg4 - start_dbg4);
#endif

#ifdef FTL_DEBUG
	printf("[WRITE_TO_SEQ_BLOCK] Complete\n");
#endif

	return SUCCESS;
}

int WRITE_TO_RAN_COLD_BLOCK(int32_t sector_nb, unsigned int length){

#ifdef FTL_DEBUG
	printf("[WRITE_TO_RAN_COLD_BLOCK] Start\n");
#endif

#ifdef DEBUG_MODE4
	int64_t start_dbg4, end_dbg4;
	start_dbg4 = get_usec();
#endif

	int ret;
	unsigned int remain_sector = length;
	unsigned int lba = sector_nb;
	unsigned int left_skip = sector_nb % SECTORS_PER_PAGE;
	unsigned int right_skip;
	unsigned int write_sects;

	unsigned int old_phy_flash_nb;
	unsigned int old_phy_block_nb;

	ran_block_mapping_entry* ran_cold_block_entry;
	ran_log_mapping_entry* ran_cold_log_entry;

	unsigned int log_flash_nb_r;
	unsigned int log_block_nb_r;
	unsigned int log_page_nb_r;

	unsigned int log_block_nb_i;
	unsigned int log_page_nb_i;

	unsigned int ran_flash_nb;
	unsigned int ran_block_nb;
	unsigned int ran_page_nb;

	unsigned int write_pages = 0;
	int write_page_offset = 0;
	int io_page_nb;

	io_alloc_overhead = ALLOC_IO_REQUEST(sector_nb, length, WRITE, &io_page_nb);

	while(remain_sector > 0){

		if(alloc_ran_cold_block_index == RAN_LOG_MAPPING_ENTRY_NB){
			MERGE_RAN_COLD_BLOCK();
		}

		if(remain_sector > SECTORS_PER_PAGE - left_skip){
			right_skip = 0;
		}
		else{
			right_skip = SECTORS_PER_PAGE - left_skip - remain_sector;
		}
		write_sects = SECTORS_PER_PAGE - left_skip - right_skip;

		/* Calculate Lofical Address */
		log_page_nb_i = lba / (int32_t)SECTORS_PER_PAGE;
		log_block_nb_i = log_page_nb_i / PAGE_NB;

		log_flash_nb_r = (unsigned int)(log_block_nb_i / (int32_t)BLOCK_NB);
		log_block_nb_r = (unsigned int)(log_block_nb_i % (int32_t)BLOCK_NB);
		log_page_nb_r  = (unsigned int)(log_page_nb_i % (int32_t)PAGE_NB);

	 	/* Update Ran Log Block Validity */
                UPDATE_RAN_LOG_MAPPING_VALID(log_flash_nb_r, log_block_nb_r, log_page_nb_r, alloc_ran_cold_block_index, COLD_RAN);

		/* Find Data Block */
		GET_DATA_BLOCK_MAPPING_INFO(log_flash_nb_r, log_block_nb_r, &old_phy_flash_nb, &old_phy_block_nb);

		/* Get Ran Log Block Page To Write */
		ran_cold_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(alloc_ran_cold_block_index, COLD_RAN);

		ran_flash_nb = ran_cold_block_entry->phy_flash_nb;
		ran_block_nb = ran_cold_block_entry->phy_block_nb;
		ran_page_nb = alloc_ran_cold_block_index % (unsigned int)PAGE_NB;

		ret = SSD_PAGE_WRITE(ran_flash_nb, ran_block_nb, ran_page_nb, write_page_offset, RAN_COLD_WRITE, io_page_nb);
		write_pages++;
		write_page_offset++;

		/* Update Ran Log Mapping */
		ran_cold_log_entry = GET_RAN_LOG_MAPPING_ENTRY(alloc_ran_cold_block_index, COLD_RAN);

		ran_cold_log_entry->data_flash_nb = old_phy_flash_nb;
		ran_cold_log_entry->data_block_nb = old_phy_block_nb;
		ran_cold_log_entry->log_flash_nb = log_flash_nb_r;
		ran_cold_log_entry->log_block_nb = log_block_nb_r;
		ran_cold_log_entry->log_page_nb = log_page_nb_r;
		ran_cold_log_entry->valid = VALID;

		/* Update Inverse Block Mapping */
		UPDATE_INVERSE_BLOCK_MAPPING(ran_flash_nb, ran_block_nb, FLASH_NB, 0, RAN_COLD_BLOCK);
		UPDATE_INVERSE_BLOCK_VALIDITY(ran_flash_nb, ran_block_nb, ran_page_nb, VALID);

                if(old_phy_flash_nb != FLASH_NB){
			UPDATE_INVERSE_BLOCK_VALIDITY(old_phy_flash_nb, old_phy_block_nb, log_page_nb_r, INVALID);
                }

		/* Update for Next Write Loop */
		lba += write_sects;
		remain_sector -= write_sects;
		left_skip = 0;
//		alloc_ran_cold_block_index++;

		if(alloc_ran_cold_block_index == (PARAL_COUNT * ran_cold_log_cnt -1)){
			alloc_ran_cold_block_index++;
			ran_cold_log_cnt++;
		}
		else{
			alloc_ran_cold_block_index += PAGE_NB;
			if(alloc_ran_cold_block_index >= PARAL_COUNT*ran_cold_log_cnt){
				alloc_ran_cold_block_index = alloc_ran_cold_block_index % PARAL_COUNT + PARAL_COUNT * (ran_cold_log_cnt-1) + 1;
			}
		}
	}

	INCREASE_IO_REQUEST_SEQ_NB();

#ifdef DEBUG_MODE4
	end_dbg4 = get_usec();
	fprintf(fp_dbg4_ran,"CLD\t%ld\t%d\n", end_dbg4 - start_dbg4, write_pages);
#endif

#ifdef FTL_DEBUG
	printf("[WRITE_TO_RAN_COLD_BLOCK] Complete\n");
#endif

        return SUCCESS;
}

int WRITE_TO_RAN_HOT_BLOCK(int32_t sector_nb, unsigned int length){

#ifdef FTL_DEBUG
	printf("[WRITE_TO_RAN_HOT_BLOCK] Start\n");
#endif

#ifdef DEBUG_MODE4
	int64_t start_dbg4, end_dbg4;
	start_dbg4 = get_usec();
#endif

	int ret;
	unsigned int remain_sector = length;
	unsigned int lba = sector_nb;
	unsigned int left_skip = sector_nb % SECTORS_PER_PAGE;
	unsigned int right_skip;
	unsigned int write_sects;

	unsigned int old_phy_flash_nb;
	unsigned int old_phy_block_nb;

	ran_block_mapping_entry* ran_hot_block_entry;
	ran_log_mapping_entry* ran_hot_log_entry;

	unsigned int log_flash_nb_r;
	unsigned int log_block_nb_r;
	unsigned int log_page_nb_r;

	unsigned int log_block_nb_i;
	unsigned int log_page_nb_i;

	unsigned int ran_flash_nb;
	unsigned int ran_block_nb;
	unsigned int ran_page_nb;

	unsigned int write_pages = 0;
	int write_page_offset = 0;
	int io_page_nb;

	io_alloc_overhead = ALLOC_IO_REQUEST(sector_nb, length, WRITE, &io_page_nb);

	while(remain_sector > 0){

		if(alloc_ran_hot_block_index == RAN_LOG_MAPPING_ENTRY_NB){
			MERGE_RAN_HOT_BLOCK();
		}

		if(remain_sector > SECTORS_PER_PAGE - left_skip){
			right_skip = 0;
		}
		else{
			right_skip = SECTORS_PER_PAGE - left_skip - remain_sector;
		}
		write_sects = SECTORS_PER_PAGE - left_skip - right_skip;

		/* Calculate Lofical Address */
		log_page_nb_i = lba / (int32_t)SECTORS_PER_PAGE;
		log_block_nb_i = log_page_nb_i / PAGE_NB;

		log_flash_nb_r = (unsigned int)(log_block_nb_i / (int32_t)BLOCK_NB);
		log_block_nb_r = (unsigned int)(log_block_nb_i % (int32_t)BLOCK_NB);
		log_page_nb_r  = (unsigned int)(log_page_nb_i % (int32_t)PAGE_NB);

		/* Update Ran Log Block Validity */
		UPDATE_RAN_LOG_MAPPING_VALID(log_flash_nb_r, log_block_nb_r, log_page_nb_r, alloc_ran_hot_block_index, HOT_RAN);

		/* Find Data Block */
		GET_DATA_BLOCK_MAPPING_INFO(log_flash_nb_r, log_block_nb_r, &old_phy_flash_nb, &old_phy_block_nb);

		/* Get Ran Log Block Page To Write */
		ran_hot_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(alloc_ran_hot_block_index, HOT_RAN);

		ran_flash_nb = ran_hot_block_entry->phy_flash_nb;
		ran_block_nb = ran_hot_block_entry->phy_block_nb;
		ran_page_nb = alloc_ran_cold_block_index % (unsigned int)PAGE_NB;

		ret = SSD_PAGE_WRITE(ran_flash_nb, ran_block_nb, ran_page_nb, write_page_offset, RAN_HOT_WRITE, io_page_nb);
		write_pages++;
		write_page_offset++;

		/* Update Ran Log Mapping */
		ran_hot_log_entry = GET_RAN_LOG_MAPPING_ENTRY(alloc_ran_hot_block_index, HOT_RAN);

		ran_hot_log_entry->data_flash_nb = old_phy_flash_nb;
		ran_hot_log_entry->data_block_nb = old_phy_block_nb;
		ran_hot_log_entry->log_flash_nb = log_flash_nb_r;
		ran_hot_log_entry->log_block_nb = log_block_nb_r;
		ran_hot_log_entry->log_page_nb = log_page_nb_r;
		ran_hot_log_entry->valid = VALID;

		/* Update Inverse Block Mapping */
		UPDATE_INVERSE_BLOCK_MAPPING(ran_flash_nb, ran_block_nb, FLASH_NB, 0, RAN_HOT_BLOCK);
		UPDATE_INVERSE_BLOCK_VALIDITY(ran_flash_nb, ran_block_nb, ran_page_nb, VALID);

		if(old_phy_flash_nb != FLASH_NB){
			UPDATE_INVERSE_BLOCK_VALIDITY(old_phy_flash_nb, old_phy_block_nb, log_page_nb_r, INVALID);
		}

		/* Update for Next Write Loop */
		lba += write_sects;
		remain_sector -= write_sects;
		left_skip = 0;
		alloc_ran_hot_block_index++;
	}

	INCREASE_IO_REQUEST_SEQ_NB();

#ifdef DEBUG_MODE4
	end_dbg4 = get_usec();
	fprintf(fp_dbg4_ran,"HOT\t%ld\t%d\n", end_dbg4 - start_dbg4, write_pages);
#endif

#ifdef FTL_DEBUG
	printf("[WRITE_TO_RAN_HOT_BLOCK] Complete\n");
#endif

	return SUCCESS;
}

int MERGE_SEQ_BLOCK(void)
{
#ifdef FTL_DEBUG
	printf("[MERGE_SEQ_BLOCK] Start\n");
#endif

#ifdef DEBUG_MODE5
        int64_t start_dbg5, end_dbg5;
        start_dbg5 = get_usec();
#endif
	/* Debugging Variable */
	unsigned int write_amp_pages=0;
	unsigned int switch_merge_cnt=0;
	unsigned int partial_merge_cnt=0;

	int i, j, ret;

	seq_block_mapping_entry* seq_block_entry;
	seq_log_mapping_entry* seq_log_entry;
	inverse_block_mapping_entry* inverse_block_entry;

	unsigned int data_flash_nb;
	unsigned int data_block_nb;

	unsigned int seq_flash_nb;
	unsigned int seq_block_nb;

	unsigned int log_flash_nb;
	unsigned int log_block_nb;

	char* valid_array;

	int copy_page_nb = 0;

	double seq_block_util = 0;

	for(i=0;i<SEQ_MAPPING_ENTRY_NB;i++){
		seq_block_entry = GET_SEQ_BLOCK_MAPPING_ENTRY(i);
		seq_log_entry = GET_SEQ_LOG_MAPPING_ENTRY(i);

		seq_flash_nb = seq_block_entry->phy_flash_nb;
		seq_block_nb = seq_block_entry->phy_block_nb;

		data_flash_nb = seq_log_entry->data_flash_nb;
		data_block_nb = seq_log_entry->data_block_nb;
		
		log_flash_nb = seq_log_entry->log_flash_nb;
		log_block_nb = seq_log_entry->log_block_nb;

		seq_block_util += seq_log_entry->page_length;
		
#ifdef DEBUG_MODE2
		fp_dbg2_seq = fopen("./data/l_dbg2_seq.txt","a");
                if(data_flash_nb == FLASH_NB){
                        fprintf(fp_dbg2_seq,"SEQ\t%d\t%d\t%d\t0\n",seq_flash_nb, seq_block_nb,seq_log_entry->page_length);
                }
                else{
       
                        inverse_block_entry = GET_INVERSE_BLOCK_MAPPING_ENTRY(data_flash_nb, data_block_nb);
	                fprintf(fp_dbg2_seq,"SEQ\t%d\t%d\t%d\t%d\n",seq_flash_nb, seq_block_nb, seq_log_entry->page_length, inverse_block_entry->valid_page_nb);
                }
		fclose(fp_dbg2_seq);
#endif
		/* If There is no associated data block, */
		if(data_flash_nb == FLASH_NB){

			/* Switching Merge */
			UPDATE_INVERSE_BLOCK_MAPPING(seq_flash_nb, seq_block_nb, log_flash_nb, log_block_nb, DATA_BLOCK);

			UPDATE_DATA_BLOCK_MAPPING(log_flash_nb, log_block_nb, seq_flash_nb, seq_block_nb);
			switch_merge_cnt++;
#ifdef DEBUG_MODE6
			seq_erase_add++;
#endif

		}
		else{
			/* Get Data Block Inverse Block Entry */
			inverse_block_entry = GET_INVERSE_BLOCK_MAPPING_ENTRY(data_flash_nb, data_block_nb);

			if(inverse_block_entry->valid_page_nb == 0){
				/* Switching Merge */
				UPDATE_INVERSE_BLOCK_MAPPING(seq_flash_nb, seq_block_nb, log_flash_nb, log_block_nb, DATA_BLOCK);
				UPDATE_INVERSE_BLOCK_MAPPING(data_flash_nb, data_block_nb, FLASH_NB, 0, EMPTY_BLOCK);
				UPDATE_DATA_BLOCK_MAPPING(log_flash_nb, log_block_nb, seq_flash_nb, seq_block_nb);
				/* Erase The Data Block */
				ret = SSD_BLOCK_ERASE(data_flash_nb, data_block_nb);

				/* Added The Data Block To Empty Block List */
				INSERT_EMPTY_BLOCK(data_flash_nb, data_block_nb);
			
				switch_merge_cnt++;
#ifdef DEBUG_MODE6
				seq_erase_cnt++;
#endif

			}
			else if(inverse_block_entry->valid_page_nb != 0){

				valid_array = inverse_block_entry->valid_array;

				/* Partial Merge */
				for(j=0;j<PAGE_NB;j++){
					if(valid_array[j]=='V'){
						ret = SSD_PAGE_READ(data_flash_nb, data_block_nb, j, 0, SEQ_MERGE_READ, -1);
						ret = SSD_PAGE_WRITE(seq_flash_nb, seq_block_nb, j, 0, SEQ_MERGE_WRITE, -1);

						UPDATE_INVERSE_BLOCK_VALIDITY(seq_flash_nb, seq_block_nb, j, VALID);
						copy_page_nb++;
						write_amp_pages++;
#ifdef DEBUG_MODE8
						seq_data_dbg8++;
#endif
					}
				}
				UPDATE_INVERSE_BLOCK_MAPPING(seq_flash_nb, seq_block_nb, log_flash_nb, log_block_nb, DATA_BLOCK);
				UPDATE_INVERSE_BLOCK_MAPPING(data_flash_nb, data_block_nb, FLASH_NB, 0, EMPTY_BLOCK);
				UPDATE_DATA_BLOCK_MAPPING(log_flash_nb, log_block_nb, seq_flash_nb, seq_block_nb);
				/* Erase The Data Block */
				ret = SSD_BLOCK_ERASE(data_flash_nb, data_block_nb);

				/* Added The Data Block To Empty Block List */
				INSERT_EMPTY_BLOCK(data_flash_nb, data_block_nb);

				partial_merge_cnt++;
#ifdef DEBUG_MODE6
				seq_erase_cnt++;
#endif
			}
		}
	}

	INIT_SEQ_BLOCK_MAPPING(SEQ_MERGE);
	INIT_SEQ_LOG_MAPPING(SEQ_MERGE);
	alloc_seq_block_index = 0;

#ifdef DEBUG_MODE5
	end_dbg5 = get_usec();
	fprintf(fp_dbg5_seq,"SEQ\t%ld\t%d\n", end_dbg5 - start_dbg5, switch_merge_cnt + partial_merge_cnt);
#endif

#ifdef DEBUG_MODE6
        fprintf(fp_dbg6_seq,"SEQ\t%d\t%d\n",seq_erase_cnt, seq_erase_add);
#endif

#ifdef DEBUG_MODE8
	fprintf(fp_dbg8_seq,"SEQ\t%d\n",seq_data_dbg8);
#endif

#ifdef MONITOR_ON
	char szTemp[1024];
	sprintf(szTemp,"EXCHANGE %d", switch_merge_cnt);
	WRITE_LOG(szTemp);
	sprintf(szTemp,"MERGE SEQ %d", partial_merge_cnt);
	WRITE_LOG(szTemp);
	sprintf(szTemp,"WB AMP %d", copy_page_nb);
	WRITE_LOG(szTemp);
#endif

#ifdef FTL_DEBUG
        printf("[MERGE_SEQ_LOG] Complete\n");
#endif
	return SUCCESS;
}

int MERGE_RAN_COLD_BLOCK(void)
{
#ifdef FTL_DEBUG
        printf("[MERGE_RAN_COLD_BLOCK] Start \n");
#endif

	int i, j, ret;
	unsigned int ran_merge_cnt=0;
	unsigned int ran_merge[RAN_MAPPING_ENTRY_NB];
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		ran_merge[i]=0;
	}

#ifdef DEBUG_MODE3
        unsigned int block_arr_dbg3[RAN_MAPPING_ENTRY_NB];
	unsigned int page_arr_dbg3[RAN_MAPPING_ENTRY_NB];
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		block_arr_dbg3[i]=0;
		page_arr_dbg3[i]=0;
	}
#endif
#ifdef DEBUG_MODE5
	int64_t start_dbg5, end_dbg5;
	start_dbg5 = get_usec();
#endif

	ran_log_mapping_entry* ran_cold_log_entry;
	ran_block_mapping_entry* ran_cold_block_entry;

	ran_log_mapping_entry* curr_log_entry;
	inverse_block_mapping_entry* inverse_block_entry;

	unsigned int data_flash_nb;
	unsigned int data_block_nb;

	unsigned int ran_flash_nb;
	unsigned int ran_block_nb;

	unsigned int log_flash_nb;
	unsigned int log_block_nb;

	unsigned int block_nb;

	unsigned int new_phy_flash_nb;
	unsigned int new_phy_block_nb;
	char* valid_array;

	unsigned int curr_phy_page_nb;

	int copy_page_nb;

	/* Debugging Variable */
	unsigned int asso_block_nb=0;
	unsigned int ran_copy_page_nb=0;
	unsigned int exist_data_block_nb=0;

#ifdef DEBUG_MODE2
	fp_dbg2_ran = fopen("./data/l_dbg2_ran.txt","a");
        for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i+=PAGE_NB){
                ran_cold_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(i, COLD_RAN);
                inverse_block_entry = GET_INVERSE_BLOCK_MAPPING_ENTRY(ran_cold_block_entry->phy_flash_nb, ran_cold_block_entry->phy_block_nb);
                fprintf(fp_dbg2_ran,"CLD\t%d\t%d\t%d\n",ran_cold_block_entry->phy_flash_nb, ran_cold_block_entry->phy_block_nb,inverse_block_entry->valid_page_nb);
        }
	fclose(fp_dbg2_ran);
#endif

	for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i++){

		/* Full merge */
		ran_cold_log_entry = GET_RAN_LOG_MAPPING_ENTRY(i, COLD_RAN);
		if(ran_cold_log_entry->valid == INVALID){
			continue;
		}

		data_flash_nb = ran_cold_log_entry->data_flash_nb;
		data_block_nb = ran_cold_log_entry->data_block_nb;

		log_flash_nb = ran_cold_log_entry->log_flash_nb;
		log_block_nb = ran_cold_log_entry->log_block_nb;

		/* Get New Data Block */
		GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

		UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, log_flash_nb, log_block_nb, EMPTY_DATA_BLOCK);

		UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, log_flash_nb, log_block_nb, DATA_BLOCK);
		UPDATE_DATA_BLOCK_MAPPING(log_flash_nb, log_block_nb, new_phy_flash_nb, new_phy_block_nb);

		/* Write Data Block Page to New Empty Block */
		if(data_flash_nb != FLASH_NB){

			inverse_block_entry = GET_INVERSE_BLOCK_MAPPING_ENTRY(data_flash_nb, data_block_nb);
			valid_array = inverse_block_entry->valid_array;

			for(j=0;j<PAGE_NB;j++){
				if(valid_array[j]=='V'){
					ret = SSD_PAGE_READ(data_flash_nb, data_block_nb, j, 0, RAN_COLD_MERGE_READ, -1);
					ret = SSD_PAGE_WRITE(new_phy_flash_nb, new_phy_block_nb, j, 0, RAN_COLD_MERGE_WRITE, -1);
				
					UPDATE_INVERSE_BLOCK_VALIDITY(new_phy_flash_nb, new_phy_block_nb, j, VALID);
					copy_page_nb++;
#ifdef DEBUG_MODE8
					ran_data_dbg8++;
					ran_total_dbg8++;
#endif
				}
			}
			UPDATE_INVERSE_BLOCK_MAPPING(data_flash_nb, data_block_nb, FLASH_NB, 0, EMPTY_BLOCK);

			/* Erase The Data Block */
			ret = SSD_BLOCK_ERASE(data_flash_nb, data_block_nb);
			
			/* Added The Data Block To Empty Block List */
			INSERT_EMPTY_BLOCK(data_flash_nb, data_block_nb);

			exist_data_block_nb++;
#ifdef DEBUG_MODE6
			ran_erase_cnt++;
#endif
		}
#ifdef DEBUG_MODE6
		else{
			ran_erase_add++;
		}
#endif


#ifdef DEBUG_MODE3
		/* Count Block Associativity */
		unsigned int block_arr[RAN_MAPPING_ENTRY_NB];
		for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
			block_arr[j]=0;
		}
		for(j=0;j<RAN_LOG_MAPPING_ENTRY_NB;j++){
			curr_log_entry = GET_RAN_LOG_MAPPING_ENTRY(j, COLD_RAN);
		
			block_nb = j/PAGE_NB;
			if(curr_log_entry->log_flash_nb == log_flash_nb && curr_log_entry->log_block_nb == log_block_nb && curr_log_entry->valid == VALID){
				block_arr[block_nb]=1;
			}
		}
		for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
			if(block_arr[j]==1){
				asso_block_nb++;
			}
		}
		for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
			if(block_arr[j]==1){
				block_arr_dbg3[j] += 1;
			}
		}
#endif

		/* Write Log Block Page to New Empty Block */
		for(j=i;j<RAN_LOG_MAPPING_ENTRY_NB;j++){
			curr_log_entry = GET_RAN_LOG_MAPPING_ENTRY(j, COLD_RAN);

			if(curr_log_entry->log_flash_nb == log_flash_nb && curr_log_entry->log_block_nb == log_block_nb && curr_log_entry->valid == VALID){
				
				curr_phy_page_nb = (unsigned int)(j % PAGE_NB);
				ran_cold_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(j, COLD_RAN);

				ret = SSD_PAGE_READ(ran_cold_block_entry->phy_flash_nb, ran_cold_block_entry->phy_block_nb, curr_phy_page_nb, 0, RAN_COLD_MERGE_READ, -1);
				ret = SSD_PAGE_WRITE(new_phy_flash_nb, new_phy_block_nb, curr_log_entry->log_page_nb, 0, RAN_COLD_MERGE_WRITE, -1);

				UPDATE_INVERSE_BLOCK_VALIDITY(new_phy_flash_nb, new_phy_block_nb, curr_log_entry->log_page_nb, VALID);
				curr_log_entry->valid = INVALID;

				copy_page_nb++;
				ran_copy_page_nb++;

				ran_merge[j/PAGE_NB] = 1;
#ifdef DEBUG_MODE3
				page_arr_dbg3[j/PAGE_NB] += 1;
#endif
#ifdef DEBUG_MODE8
				ran_log_dbg8++;
				ran_total_dbg8++;
#endif
			}
		}
#ifdef DEBUG_MODE3
		for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
			if(page_arr_dbg3[j]!=0){
				fprintf(fp_dbg3_1_ran,"CLD\t%d\t%d\n",page_arr_dbg3[j], count_dbg3);
				page_arr_dbg3[j]=0;
			}
		}
#endif
	}

	/* Erase Random Log Block */
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		ran_cold_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(i, COLD_RAN);

		ret = SSD_BLOCK_ERASE(ran_cold_block_entry->phy_flash_nb, ran_cold_block_entry->phy_block_nb);

		UPDATE_INVERSE_BLOCK_MAPPING(ran_cold_block_entry->phy_flash_nb, ran_cold_block_entry->phy_block_nb, FLASH_NB, 0, EMPTY_BLOCK);
		INSERT_EMPTY_BLOCK(ran_cold_block_entry->phy_flash_nb, ran_cold_block_entry->phy_block_nb);

#ifdef DEBUG_MODE6
		ran_erase_cnt++;
#endif
	}

	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		if(ran_merge[i]==1){
			ran_merge_cnt++;
		}
	}

	/* Init Random Log Block */
	INIT_RAN_COLD_BLOCK_MAPPING(RAN_MERGE);
	INIT_RAN_COLD_LOG_MAPPING(RAN_MERGE);
	alloc_ran_cold_block_index = 0;
	ran_cold_log_cnt = 1;

#ifdef DEBUG_MODE3
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		fprintf(fp_dbg3_ran,"CLD\t%d\t%d\t%d\n",block_arr_dbg3[i], ran_copy_page_nb,count_dbg3);
	}
	count_dbg3++;
#endif

#ifdef DEBUG_MODE5
	end_dbg5 = get_usec();
	fprintf(fp_dbg5_ran,"CLD\t%ld\t%d\n", end_dbg5 - start_dbg5, ran_merge_cnt);
#endif

#ifdef DEBUG_MODE6
        fprintf(fp_dbg6_ran,"RAN\t%d\t%d\n",ran_erase_cnt, ran_erase_add);
#endif

#ifdef DEBUG_MODE8
	fprintf(fp_dbg8_ran,"CLD\t%d\t%d\t%d\n",ran_log_dbg8,ran_data_dbg8,ran_total_dbg8);
#endif

#ifdef MONITOR_ON
	char szTemp[1024];
	sprintf(szTemp, "MERGE RAND %d", ran_merge_cnt);
	WRITE_LOG(szTemp);
	sprintf(szTemp, "WB AMP %d", copy_page_nb);
	WRITE_LOG(szTemp);
#endif

#ifdef FTL_DEBUG
	printf("[MERGE_RAN_COLD_BLOCK] Complete\n");
#endif

	return SUCCESS;
}

int MERGE_RAN_HOT_BLOCK(void)
{
#ifdef FTL_DEBUG
	printf("[MERGE_RAN_HOT_BLOCK] Start \n");
#endif

        int i, j, ret;
	unsigned int ran_merge_cnt=0;
	unsigned int ran_merge[RAN_MAPPING_ENTRY_NB];
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		ran_merge[i]=0;
	}

#ifdef DEBUG_MODE3
	unsigned int block_arr_dbg3[RAN_MAPPING_ENTRY_NB];
	unsigned int page_arr_dbg3[RAN_MAPPING_ENTRY_NB];
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		block_arr_dbg3[i]=0;
		page_arr_dbg3[i]=0;
	}
#endif
#ifdef DEBUG_MODE5
	int64_t start_dbg5, end_dbg5;
	start_dbg5 = get_usec();
#endif

	ran_log_mapping_entry* ran_hot_log_entry;
	ran_block_mapping_entry* ran_hot_block_entry;

	ran_log_mapping_entry* curr_log_entry;
	inverse_block_mapping_entry* inverse_block_entry;

	unsigned int data_flash_nb;
	unsigned int data_block_nb;

	unsigned int ran_flash_nb;
	unsigned int ran_block_nb;

	unsigned int log_flash_nb;
	unsigned int log_block_nb;

	unsigned int block_nb;

	unsigned int new_phy_flash_nb;
	unsigned int new_phy_block_nb;
	char* valid_array;

	unsigned int curr_phy_page_nb;

	int copy_page_nb=0;

	/* Debugging Variable */
	unsigned int asso_block_nb=0;
	unsigned int ran_copy_page_nb=0;
	unsigned int exist_data_block_nb=0;

#ifdef DEBUG_MODE2
	fp_dbg2_ran = fopen("./data/l_dbg2_ran.txt","a");
        for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i+=PAGE_NB){
                ran_hot_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(i, HOT_RAN);
                inverse_block_entry = GET_INVERSE_BLOCK_MAPPING_ENTRY(ran_hot_block_entry->phy_flash_nb, ran_hot_block_entry->phy_block_nb);
                fprintf(fp_dbg2_ran,"HOT\t%d\t%d\t%d\n",ran_hot_block_entry->phy_flash_nb, ran_hot_block_entry->phy_block_nb,inverse_block_entry->valid_page_nb);
        }
	fclose(fp_dbg2_ran);
#endif

	for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i++){

		/* Full merge */
		ran_hot_log_entry = GET_RAN_LOG_MAPPING_ENTRY(i, HOT_RAN);
		if(ran_hot_log_entry->valid == INVALID){
			continue;
		}

		ran_merge_cnt++;

		data_flash_nb = ran_hot_log_entry->data_flash_nb;
		data_block_nb = ran_hot_log_entry->data_block_nb;

		log_flash_nb = ran_hot_log_entry->log_flash_nb;
		log_block_nb = ran_hot_log_entry->log_block_nb;

		/* Get New Data Block */
		GET_EMPTY_BLOCK(&new_phy_flash_nb, &new_phy_block_nb);

		UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, log_flash_nb, log_block_nb, EMPTY_DATA_BLOCK);

		UPDATE_INVERSE_BLOCK_MAPPING(new_phy_flash_nb, new_phy_block_nb, log_flash_nb, log_block_nb, DATA_BLOCK);
		UPDATE_DATA_BLOCK_MAPPING(log_flash_nb, log_block_nb, new_phy_flash_nb, new_phy_block_nb);

		/* Write Data Block Page to New Empty Block */
		if(data_flash_nb != FLASH_NB){

			inverse_block_entry = GET_INVERSE_BLOCK_MAPPING_ENTRY(data_flash_nb, data_block_nb);
			valid_array = inverse_block_entry->valid_array;

			for(j=0;j<PAGE_NB;j++){
				if(valid_array[j]=='V'){
					ret = SSD_PAGE_READ(data_flash_nb, data_block_nb, j, 0, RAN_HOT_MERGE_READ, -1);
					ret = SSD_PAGE_WRITE(new_phy_flash_nb, new_phy_block_nb, j, 0, RAN_HOT_MERGE_WRITE, -1);
					
					UPDATE_INVERSE_BLOCK_VALIDITY(new_phy_flash_nb, new_phy_block_nb, j, VALID);
					copy_page_nb++;
#ifdef DEBUG_MODE8
					ran_data_dbg8++;
					ran_total_dbg8++;
#endif
				}
			}
			UPDATE_INVERSE_BLOCK_MAPPING(data_flash_nb, data_block_nb, FLASH_NB, 0, EMPTY_BLOCK);

			/* Erase The Data Block */
			ret = SSD_BLOCK_ERASE(data_flash_nb, data_block_nb);

			/* Added The Data Block To Empty Block List */
			INSERT_EMPTY_BLOCK(data_flash_nb, data_block_nb);

			exist_data_block_nb++;
#ifdef DEBUG_MODE6
			ran_erase_cnt++;
#endif
		}
#ifdef DEBUG_MODE6
		else{
			ran_erase_add++;
		}
#endif


#ifdef DEBUG_MODE3
                /* Count Block Associativity */
                unsigned int block_arr[RAN_MAPPING_ENTRY_NB];
                for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
                        block_arr[j]=0;
                }
                for(j=0;j<RAN_LOG_MAPPING_ENTRY_NB;j++){
                        curr_log_entry = GET_RAN_LOG_MAPPING_ENTRY(j, COLD_RAN);

                        block_nb = j/PAGE_NB;
                        if(curr_log_entry->log_flash_nb == log_flash_nb && curr_log_entry->log_block_nb == log_block_nb && curr_log_entry->valid == VALID){
                                block_arr[block_nb]=1;
                        }
                }
                for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
                        if(block_arr[j]==1){
                                asso_block_nb++;
                        }
                }
		for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
			if(block_arr[j]==1){
				block_arr_dbg3[j] += 1;
			}
		}
#endif

		/* Write Log Block Page to New Empty Block */
		for(j=i;j<RAN_LOG_MAPPING_ENTRY_NB;j++){
			curr_log_entry = GET_RAN_LOG_MAPPING_ENTRY(j, HOT_RAN);

			if(curr_log_entry->log_flash_nb == log_flash_nb && curr_log_entry->log_block_nb == log_block_nb && curr_log_entry->valid == VALID){

                                curr_phy_page_nb = (unsigned int)(j % PAGE_NB);
                                ran_hot_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(j, HOT_RAN);

				ret = SSD_PAGE_READ(ran_hot_block_entry->phy_flash_nb, ran_hot_block_entry->phy_block_nb, curr_phy_page_nb, 0, RAN_HOT_MERGE_READ, -1);
				ret = SSD_PAGE_WRITE(new_phy_flash_nb, new_phy_block_nb, curr_log_entry->log_page_nb, 0, RAN_HOT_MERGE_WRITE, -1);
				
				UPDATE_INVERSE_BLOCK_VALIDITY(new_phy_flash_nb, new_phy_block_nb, curr_log_entry->log_page_nb, VALID);
				curr_log_entry->valid = INVALID;

				copy_page_nb++;
				ran_copy_page_nb++;

				ran_merge[j/PAGE_NB]=1;
#ifdef DEBUG_MODE3
				page_arr_dbg3[j/PAGE_NB] += 1;
#endif
#ifdef DEBUG_MODE8
				ran_log_dbg8++;
				ran_total_dbg8++;
#endif
			}
		}
#ifdef DEBUG_MODE3
		for(j=0;j<RAN_MAPPING_ENTRY_NB;j++){
			if(page_arr_dbg3[j]!=0){
				fprintf(fp_dbg3_1_ran,"HOT\t%d\t%d\n",page_arr_dbg3[j], count_dbg3);
				page_arr_dbg3[j]=0;
			}
		}
#endif
	}

	/* Erase Random Log Block */
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		ran_hot_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(i, HOT_RAN);

		ret = SSD_BLOCK_ERASE(ran_hot_block_entry->phy_flash_nb, ran_hot_block_entry->phy_block_nb);

		UPDATE_INVERSE_BLOCK_MAPPING(ran_hot_block_entry->phy_flash_nb, ran_hot_block_entry->phy_block_nb, FLASH_NB, 0, EMPTY_BLOCK);
		INSERT_EMPTY_BLOCK(ran_hot_block_entry->phy_flash_nb, ran_hot_block_entry->phy_block_nb);

#ifdef DEBUG_MODE6
		ran_erase_cnt++;
#endif
	}

	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		if(ran_merge[i]==1){
			ran_merge_cnt++;
		}
	}

	/* Init Random Log Block */
	INIT_RAN_HOT_BLOCK_MAPPING(RAN_MERGE);
	INIT_RAN_HOT_LOG_MAPPING(RAN_MERGE);
	alloc_ran_hot_block_index = 0;

#ifdef DEBUG_MODE3_1
	for(i=0;i<RAN_MAPPING_ENTRY_NB;i++){
		fprintf(fp_dbg3_ran,"HOT\t%d\t%d\t%d\n",block_arr_dbg3[i], ran_copy_page_nb,count_dbg3);
	}
	count_dbg3++;
#endif

#ifdef DEBUG_MODE5
	end_dbg5 = get_usec();
	fprintf(fp_dbg5_ran,"HOT\t%ld\t%d\n", end_dbg5 - start_dbg5, ran_merge_cnt);
#endif

#ifdef DEBUG_MODE6
        fprintf(fp_dbg6_ran,"RAN\t%d\t%d\n",ran_erase_cnt, ran_erase_add);
#endif

#ifdef DEBUG_MODE8
	fprintf(fp_dbg8_ran,"HOT\t%d\t%d\t%d\n",ran_log_dbg8,ran_data_dbg8,ran_total_dbg8);
#endif

#ifdef MONITOR_ON
	char szTemp[1024];
	sprintf(szTemp, "MERGE RAND %d", ran_merge_cnt);
	WRITE_LOG(szTemp);
	sprintf(szTemp, "WB AMP %d", copy_page_nb);
	WRITE_LOG(szTemp);
#endif

#ifdef FTL_DEBUG
	printf("[MERGE_RAN_HOT_BLOCK] Complete\n");
#endif

	return SUCCESS;
}

int UPDATE_RAN_LOG_MAPPING_VALID(unsigned int log_flash_nb, unsigned int log_block_nb, unsigned int log_page_nb, int ran_mapping_index, int type)
{
	int i;
	ran_log_mapping_entry* curr_mapping_entry = NULL;
	ran_block_mapping_entry* ran_block_entry;

	if(type == COLD_RAN){
		curr_mapping_entry = (ran_log_mapping_entry*)ran_cold_log_mapping_table_start;
	}
	else if(type == HOT_RAN){
		curr_mapping_entry = (ran_log_mapping_entry*)ran_hot_log_mapping_table_start;
	}

	for(i=0;i<RAN_LOG_MAPPING_ENTRY_NB;i++){

		if(i != ran_mapping_index && curr_mapping_entry->log_flash_nb == log_flash_nb && curr_mapping_entry->log_block_nb == log_block_nb && curr_mapping_entry->log_page_nb == log_page_nb && curr_mapping_entry->valid == VALID){

			curr_mapping_entry->valid = INVALID;
			ran_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(i, type);

			UPDATE_INVERSE_BLOCK_VALIDITY(ran_block_entry->phy_flash_nb, ran_block_entry->phy_block_nb, i%(PAGE_NB), INVALID);

                }
		curr_mapping_entry += 1;
	}

	return SUCCESS;	
}

int FIND_PAGE_IN_SEQ_LOG(int32_t sector_nb, unsigned int* phy_flash_nb, unsigned int* phy_block_nb, unsigned int* phy_page_nb){
	int i;

	int32_t log_page_nb_i = sector_nb / (int32_t)SECTORS_PER_PAGE;
	int32_t log_block_nb_i = log_page_nb_i / (int32_t)PAGE_NB;

	unsigned int log_flash_nb_r;
	unsigned int log_block_nb_r;
	unsigned int log_page_nb_r;

	log_flash_nb_r = (unsigned int)(log_block_nb_i / (int32_t)BLOCK_NB);
	log_block_nb_r = (unsigned int)(log_block_nb_i % (int32_t)BLOCK_NB);
	log_page_nb_r  = (unsigned int)(log_page_nb_i % (int32_t)PAGE_NB);

	seq_log_mapping_entry* seq_log_entry = seq_log_mapping_table_start;
	seq_block_mapping_entry* seq_block_entry;

	inverse_block_mapping_entry* inverse_block_entry;

	for(i=0; i<SEQ_MAPPING_ENTRY_NB;i++){
	
		if(seq_log_entry->log_flash_nb == log_flash_nb_r && seq_log_entry->log_block_nb == log_block_nb_r){

			seq_block_entry = GET_SEQ_BLOCK_MAPPING_ENTRY(i);
			inverse_block_entry = GET_INVERSE_BLOCK_MAPPING_ENTRY(seq_block_entry->phy_flash_nb, seq_block_entry->phy_block_nb);
			if(inverse_block_entry->valid_array[log_page_nb_r] == 'V'){

				*phy_flash_nb = seq_block_entry->phy_flash_nb;
				*phy_block_nb = seq_block_entry->phy_block_nb;
				*phy_page_nb = log_page_nb_r;

				return SUCCESS;
			}
		}
		seq_log_entry += 1;
	}	

	return FAIL;
}
int FIND_PAGE_IN_RAN_COLD_LOG(int32_t sector_nb, unsigned int* phy_flash_nb, unsigned int* phy_block_nb, unsigned int* phy_page_nb){
	int i;

	int32_t log_page_nb_i = sector_nb / (int32_t)SECTORS_PER_PAGE;
	int32_t log_block_nb_i = log_page_nb_i / (int32_t)PAGE_NB;

	unsigned int log_flash_nb_r;
	unsigned int log_block_nb_r;
	unsigned int log_page_nb_r;
	unsigned int curr_phy_page_nb;

	log_flash_nb_r = (unsigned int)(log_block_nb_i / (int32_t)BLOCK_NB);
	log_block_nb_r = (unsigned int)(log_block_nb_i % (int32_t)BLOCK_NB);
	log_page_nb_r  = (unsigned int)(log_page_nb_i % (int32_t)PAGE_NB);

	ran_log_mapping_entry* ran_cold_log_entry = ran_cold_log_mapping_table_start;
	ran_block_mapping_entry* ran_cold_block_entry;

	for(i=0; i<RAN_LOG_MAPPING_ENTRY_NB;i++){

		if(ran_cold_log_entry->log_flash_nb == log_flash_nb_r && ran_cold_log_entry->log_block_nb == log_block_nb_r && ran_cold_log_entry->log_page_nb == log_page_nb_r && ran_cold_log_entry->valid == VALID){
                        
			curr_phy_page_nb = (unsigned int)(i % PAGE_NB);
			ran_cold_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(i, COLD_RAN);

			*phy_flash_nb = ran_cold_block_entry->phy_flash_nb;
			*phy_block_nb = ran_cold_block_entry->phy_block_nb;
			*phy_page_nb = curr_phy_page_nb;

			return SUCCESS;
		}
		ran_cold_log_entry += 1;
	} 

        return FAIL;
}

int FIND_PAGE_IN_RAN_HOT_LOG(int32_t sector_nb, unsigned int* phy_flash_nb, unsigned int* phy_block_nb, unsigned int* phy_page_nb){
int i;

	int32_t log_page_nb_i = sector_nb / (int32_t)SECTORS_PER_PAGE;
	int32_t log_block_nb_i = log_page_nb_i / (int32_t)PAGE_NB;

	unsigned int log_flash_nb_r;
	unsigned int log_block_nb_r;
	unsigned int log_page_nb_r;
	unsigned int curr_phy_page_nb;

	log_flash_nb_r = (unsigned int)(log_block_nb_i / (int32_t)BLOCK_NB);
	log_block_nb_r = (unsigned int)(log_block_nb_i % (int32_t)BLOCK_NB);
	log_page_nb_r  = (unsigned int)(log_page_nb_i % (int32_t)PAGE_NB);

	ran_log_mapping_entry* ran_hot_log_entry = ran_hot_log_mapping_table_start;
	ran_block_mapping_entry* ran_hot_block_entry;

	for(i=0; i<RAN_LOG_MAPPING_ENTRY_NB;i++){

		if(ran_hot_log_entry->log_flash_nb == log_flash_nb_r && ran_hot_log_entry->log_block_nb == log_block_nb_r && ran_hot_log_entry->log_page_nb == log_page_nb_r && ran_hot_log_entry->valid == VALID){

			curr_phy_page_nb = (unsigned int)(i % PAGE_NB);
			ran_hot_block_entry = GET_RAN_BLOCK_MAPPING_ENTRY(i, HOT_RAN);

			*phy_flash_nb = ran_hot_block_entry->phy_flash_nb;
			*phy_block_nb = ran_hot_block_entry->phy_block_nb;
			*phy_page_nb = curr_phy_page_nb;

			return SUCCESS;
		}
		ran_hot_log_entry += 1;
	}

	return FAIL;
}
