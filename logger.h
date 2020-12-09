 /**
  * @brief Initialize the logger module
  *
  * This functions initialized all the data of the logger module and prepares the
  * module to run, it is called only once on system initialization
  */
void logger_init( void );

/**
 * @brief Open the logger
 *
 * This function opens the logger for a read or write operation, every call to this
 * function must be enclosed with a call to logger_close(). No nesting is allowed in the
 * calls
 *
 * @param mode The logger open mode : 0 = read , 1 = write
 * @return 0 = ok, -1 = argument error, -2 = fail
 */
int logger_open( int mode );

/**
 * @brief Read data from the logger
 *
 * This function reads a buffer of data from the logger, after a call to logger_open()
 * the read starts from the beginning of the logger and each call to this function
 * advances the read pointer by the amount of data read.
 *
 * @param data Pointer to a data buffer to be filled with the data
 * @param size Should be initialized to the maximum data to be read,
 * when the function returns it will hold how many bytes actually read
 * @return 0 = ok, -1 = argument error, -2 = fail
 */
int logger_read( unsigned char * data , unsigned int * size );

/**
 * @brief Write data to the logger
 *
 * This function writes a buffer of data to the logger. The data is always appended
 * to the end of the logger.
 *
 * @param data The data buffer to be appended to the log
 * @param size The size of the data
 * @return 0 = ok, -1 = argument error, -2 = fail
 */
int logger_write( unsigned char * data , unsigned int size );

/**
 * @brief Close the logger session
 *
 * This function terminates the current logger session and should be called
 * for every logger_open() call
 */
void logger_close( void );
