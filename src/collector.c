#include "collector.h"

asmlinkage int (*original_open) (const char * , int , int );
static unsigned long long files_opened = 0;
struct timeval time = {NULL};
extern unsigned long long RETBOT_up_time;


/* Define original syscall pointers here */
unsigned long **COLLECTOR_find_table( void ) { 

	unsigned long ptr = 0;
	unsigned long *p = NULL;

	for (ptr = (unsigned long) &sys_close; ptr < (unsigned long) &loops_per_jiffy; ptr += sizeof(void *)) {

		p = (unsigned long *)ptr;

		if (p[__NR_close] == (unsigned long)sys_close) {

			return (unsigned long **)p;
		}
	}

	return NULL;

}

inline void COLLECTOR_hook_open( unsigned long **table ) {

	SET_WRITE;
	original_open = (void *)table[__NR_open];
	table[__NR_open] = (unsigned long *)COLLECTOR_hooked_open;
	SET_READ;
}

inline void COLLECTOR_unhook_open( unsigned long **table ) {

	SET_WRITE;
	table[__NR_open] = (unsigned long *)original_open;
	SET_READ;
}

asmlinkage int COLLECTOR_hooked_open(const char * file, int flags, int mode) {

	unsigned int uid = 0;

	uid = (unsigned int) get_current_user()->uid.val;

	if (files_opened + 1 == ULONG_MAX) {
		files_opened = 1;
		do_gettimeofday( &time );
		RETBOT_up_time = time.tv_usec;
	}

	files_opened++;

	COLLECTOR_write_open_log( file, uid, mode );
	return (*original_open)(file, flags, mode);
}

inline static void COLLECTOR_write_open_log( const char *file, unsigned int uid, int mode ) {

	char log[RECORDER_MESSAGE_SIZE];

	do_gettimeofday( &time );

	snprintf( log, RECORDER_MESSAGE_SIZE, "FILE:%s USER:%u MODE:%d TIME:%d", file, uid, mode, time.tv_sec );
	RECORDER_add_message( log, strlen( log ) ); 
}

inline unsigned long long COLLECTOR_get_files_opened( void ) {
	return files_opened;
}