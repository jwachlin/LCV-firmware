/**
 * @file lvc_fram.cpp
 * @brief FRAM non-volatile storage interface for Low Cost Ventilator
 */

#include <Arduino.h>

#include "LCV.h"
#include "lcv_fram.h"

bool fram_init(void)
{
    SPI.begin();
    digitalWrite(FRAM_CS_PIN, HIGH);
}

bool fram_write_data(uint16_t address, uint8_t * data, uint16_t length)
{
    if(address + length >= FRAM_SIZE_BYTES)
    {
        return false;
    }
    // NOTE: WREN must be issued before any write
    digitalWrite(FRAM_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(FRAM_SPI_SPEED, MSBFIRST, SPI_MODE0));
    SPI.transfer(FRAM_OPCODE_WREN);
    SPI.endTransaction();
    digitalWrite(FRAM_CS_PIN, HIGH);

    digitalWrite(FRAM_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(FRAM_SPI_SPEED, MSBFIRST, SPI_MODE0));
    // Write address first
    SPI.transfer( (address >> 8) & 0x07 ); // Only send first 11 bits
    SPI.transfer( (address & 0xFF) );
    SPI.transfer(data, length);
    SPI.endTransaction();
    digitalWrite(FRAM_CS_PIN, HIGH);
    return true;
}

bool fram_read_data(uint16_t address, uint8_t * data, uint16_t length)
{
    if(address + length >= FRAM_SIZE_BYTES)
    {
        return false;
    }
    digitalWrite(FRAM_CS_PIN, LOW);
    SPI.beginTransaction(SPISettings(FRAM_SPI_SPEED, MSBFIRST, SPI_MODE0));
    // Write address first
    SPI.transfer( (address >> 8) & 0x07 ); // Only send first 11 bits
    SPI.transfer( (address & 0xFF) );
    for(int i = 0; i < length; i++)
    {
        *(data++) = SPI.transfer(0x00);
    }
    SPI.endTransaction();
    digitalWrite(FRAM_CS_PIN, HIGH);
    return true;
}