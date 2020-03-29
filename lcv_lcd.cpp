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

    set_character(1,1,"H");
    set_character(1,2,"e");
    set_character(1,3,"l");
    set_character(1,4,"l");
    set_character(1,5,"0");
    set_character(1,6," ");
    set_character(1,7,"W");
    set_character(1,8,"o");
    set_character(1,9,"r");
    set_character(1,10,"l");
    set_character(1,11,"d");

    // Show welcome screen TODO
    
    /*uint8_t row = 1;
    uint8_t column = 1;
    for(int i = 0; i < 19; i++)
    {
        debug_println(i);
        set_character(row, column, (intro_screen+i));
        if((i-1) % 20 == 0 && i > 1)
        {
            column = 0;
            row+=1;
        }
    }*/

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