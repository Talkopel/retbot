#ifndef RECORDER_H
#define RECORDER_H

#include <linux/string.h>

#define RECORDER_BUFFER_SIZE 65535
#define RECORDER_MESSAGE_SIZE 256

/* @brief: Adds message to buffer */
void RECORDER_add_message( const char *message, unsigned char m_size );
void RECORDER_clean_buffer( void );
char *RECORDER_get_buffer( void );

#endif