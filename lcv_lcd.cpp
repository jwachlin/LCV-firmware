/**
 * @file lcv_lcd.cpp
 * @brief LCD driver for Low Cost Ventilator
 * 
 * For NHD-0420D3Z-FL-GBW-V3 LCD 
 */


#include <Arduino.h>
#include <string.h>

#include "LCV.h"
#include "lcv_i2c_interface.h"
#include "lcv_lcd.h"

static char * intro_screen = "Low Cost Ventilator";

bool lcd_init(void)
{
    bool success = true;
    // Turn on display
    uint8_t disp_on_command = LCD_COMMAND_DISPLAY_ON; 
    if(!i2c_write_registers(LCD_I2C_ADDRESS, 
            LCD_PREFIX, &disp_on_command, 1, 20))
    {
        success = false;
    }

    if(!set_backlight(2))
    {
        success = false;
    }
    
    if(!set_contrast(40))
    {
        success = false;
    }

    // Show welcome screen
    set_string(1, 1, intro_screen, strlen(intro_screen));

    return success;
}

bool set_string(uint8_t row, uint8_t column, char * c, uint8_t length)
{
    bool success = true;
    if(row == 0 || row > 4 || column == 0 || column > 20)
    {
       success = false;
       return success;
    }

    for(int i = 0; i < length; i++)
    {
        set_character(row, column, (c+i));
        column++;
        if(column > 20)
        {
            column = 1;
            row+=1;
        }
    }
    return success;
}

bool set_character(uint8_t row, uint8_t column, char * c)
{
    /*
                    Column 1    Column 20
        Line 1      0x00        0x13
        Line 2      0x40        0x53
        Line 3      0x14        0x27
        Line 4      0x54        0x67
    */
    bool success = true;
    if(row == 0 || row > 4 || column == 0 || column > 20)
    {
       success = false;
       return success;
    }
    // Calculate index
    uint8_t index = 0;
    switch (row)
    {
    case 1:
        index = 0x00 + (column-1);
        break;

    case 2:
        index = 0x40 + (column-1);
        break;

    case 3:
        index = 0x14 + (column-1);
        break;

    case 4:
        index = 0x54 + (column-1);
        break;
    
    default:
        break;
    }

    uint8_t command[2] = {LCD_COMMAND_SET_CURSOR, index};
    if(!i2c_write_registers(LCD_I2C_ADDRESS, 
            LCD_PREFIX, &command[0], 2, 20))
    {
        success = false;
    }

    uint8_t this_character = 0;
    memcpy(&this_character, c, 1);
    if(!i2c_write_registers(LCD_I2C_ADDRESS, 
            this_character, &command[0], 0, 20))
    {
        success = false;
    }
    return success;
}

bool set_character_index(uint8_t panel_index, char * c)
{
    if(panel_index > 80 || panel_index < 1)
    {
        return false;
    }
    uint8_t row = 1;
    uint8_t column = 1;
    if(panel_index < 20)
    {
        row = 1;
        column = panel_index;
    }
    else if(panel_index < 40)
    {
        row = 2;
        column = panel_index - 20;
    }
    else if(panel_index < 60)
    {
        row = 3;
        column = panel_index - 40;
    }
    else if(panel_index < 80)
    {
        row = 4;
        column = panel_index - 60;
    }
    
    set_character(row, column, c);
}

bool set_screen(uint8_t * buffer)
{
    // Data should already be reformatted based on line breaks
    // Otherwise it will be split up 
    // Wire library can only do 32 byte chunks at a time
    bool success = true;

    uint8_t command[2] = {LCD_COMMAND_SET_CURSOR, 0x00};
    if(!i2c_write_registers(LCD_I2C_ADDRESS, 
            LCD_PREFIX, &command[0], 2, 5))
    {
        success = false;
    }
    
    if( success && !i2c_write_data(LCD_I2C_ADDRESS, (buffer+0),32,10) )
    {
        success = false;
    }
    if( success && !i2c_write_data(LCD_I2C_ADDRESS, (buffer+32),8,10) )
    {
        success = false;
    }

    command[0] = LCD_COMMAND_SET_CURSOR;
    command[1] = 0x40;
    if(success && !i2c_write_registers(LCD_I2C_ADDRESS, 
            LCD_PREFIX, &command[0], 2, 5))
    {
        success = false;
    }
    

    if( success && !i2c_write_data(LCD_I2C_ADDRESS, (buffer+40),32,30) )
    {
        success = false;
    }
    if( success && !i2c_write_data(LCD_I2C_ADDRESS, (buffer+72),8,30) )
    {
        success = false;
    }
    return success;
}

bool set_contrast(uint8_t level)
{
    if(level < 1 || level > 50)
    {
        return false;
    }
    uint8_t data_to_send[3] = {LCD_PREFIX, LCD_COMMAND_SET_CONTRAST, level};
    return i2c_write_data(LCD_I2C_ADDRESS, data_to_send, 3, 10);
}

bool set_backlight(uint8_t level)
{
    if(level < 1 || level > 8)
    {
        return false;
    }
    uint8_t data_to_send[3] = {LCD_PREFIX, LCD_COMMAND_SET_BRIGHTNESS, level};
    return i2c_write_data(LCD_I2C_ADDRESS, data_to_send, 3, 10);
}