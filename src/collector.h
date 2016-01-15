#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/delay.h> 
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/time.h>
#include <linux/limits.h>

#include "recorder.h"

#define SET_WRITE write_cr0( read_cr0() & (~0x10000) )
#define SET_READ write_cr0( read_cr0() | (0x10000) )

/* @brief: find syscall table */
unsigned long **COLLECTOR_find_table( void );

/* OPEN hook stuff */
inline void COLLECTOR_hook_open( unsigned long **table );
inline void COLLECTOR_unhook_open( unsigned long **table );
asmlinkage int COLLECTOR_hooked_open(const char * file, int flags, int mode);
static void COLLECTOR_write_open_log( const char *file, unsigned int uid, int mode );
inline unsigned long long COLLECTOR_get_files_opened( void );

#endif