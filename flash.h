/**
 * @brief Write a full page to the flash device
 *
 * This function writes a full page (512 bytes) to the flash device.
 *
 * @param page The page number to write
 * @param data The data buffer to be written
 * @return 0 = ok , -1 = argument error
 */
int flash_write(unsigned int page, unsigned char * data);

/**
* @brief Read a full page from the flash device
*
* This function reads a full page (512 bytes) from the flash device.
*
* @param page The page number to read
* @param data Pointer to the buffer to be filled with the data
* @return 0 = ok , -1 = argument error
*/
int flash_read(unsigned int page, unsigned char * data);

/**
 * @brief Erase a sector in the flash device
 *
 * This function erases a full sector (16K bytes) from the flash device. After a sector
 * is erased all its bits are set to logic 1
 *
 * @param sector The sector number to erase
 * @return 0 = ok , -1 = argument error
 */
int flash_erase(unsigned int sector);
