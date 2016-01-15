#ifndef RECORDER_H
#define RECORDER_H

#include <linux/string.h>

#define RECORDER_BUFFER_SIZE 65535
#define RECORDER_MESSAGE_SIZE 256

/* @brief: Adds message to buffer */
inline void RECORDER_add_message( const char *message, unsigned char m_size );
inline void RECORDER_clean_buffer( void );
inline char *RECORDER_get_buffer( void );
inline unsigned short RECORDER_get_buffer_size( void );

#endif