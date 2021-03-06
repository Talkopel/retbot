#include "recorder.h"

char RECORDER_buffer[RECORDER_BUFFER_SIZE];
static unsigned short RECORDER_buffer_current_size = 0;
static char *write_ptr = RECORDER_buffer;

inline void RECORDER_add_message( const char *message, unsigned char m_size ) {

	if ( RECORDER_buffer_current_size + m_size + 1 < RECORDER_BUFFER_SIZE) {
		/* Write at write ptr */
		/* memcpy( &write_ptr[0], &m_size, sizeof(unsigned char) ); --> depracated: 
		 * heavier processing required in python to parse this */
		*write_ptr = 1; /* Set separator as SOH */
		write_ptr += 1;
		strncpy( write_ptr, message, m_size );
		write_ptr += m_size;
		RECORDER_buffer_current_size += m_size + 1;
	}

}

inline void RECORDER_clean_buffer( void ) {
	
	memset( RECORDER_buffer, 0, RECORDER_BUFFER_SIZE );
	RECORDER_buffer_current_size = 0;
	write_ptr = RECORDER_buffer;
}

inline char *RECORDER_get_buffer( void ) {
	
	return RECORDER_buffer;
}

inline unsigned short RECORDER_get_buffer_size( void ) {
	return RECORDER_buffer_current_size;
}