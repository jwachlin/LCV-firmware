/**
 * @file lvc_fram.h
 * @brief FRAM non-volatile storage interface for Low Cost Ventilator
 */

#ifndef LCV_FRAM_h
#define LCV_FRAM_h

#define FRAM_SPI_SPEED          (4000000)

#define FRAM_SIZE_BYTES         (2048)

#define FRAM_OPCODE_WREN        (0x06)
#define FRAM_OPCODE_WRDI        (0x04)
#define FRAM_OPCODE_RDSR        (0x05)
#define FRAM_OPCODE_WRSR        (0x01)
#define FRAM_OPCODE_READ        (0x03)
#define FRAM_OPCODE_WRITE       (0x02)

#define FRAM_TEST_ADDRESS       (0x0000)

bool fram_init(void);
bool fram_write_data(uint16_t address, uint8_t * data, uint16_t length);
bool fram_read_data(uint16_t address, uint8_t * data, uint16_t length);

#endif