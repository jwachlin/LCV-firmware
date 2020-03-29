/**
 * @file lcv_lcd.h
 * @brief LCD driver for Low Cost Ventilator
 * 
 * For NHD-0420D3Z-FL-GBW-V3 LCD 
 */

#ifndef LCV_LCD_h
#define LCV_LCD_h

#define LCD_I2C_ADDRESS                     (0x28) // not counting R/W bit

#define LCD_WAKEUP_TIME_MS                  (150)
#define LCD_I2C_MAX_RATE                    (50000)

/* LCD supports 0x20 to 0x7F ASCII characters */
#define LCD_PREFIX                          (0xFE)

#define LCD_COMMAND_DISPLAY_ON              (0x41)
#define LCD_COMMAND_DISPLAY_OFF             (0x42)
#define LCD_COMMAND_SET_CURSOR              (0x45)
#define LCD_COMMAND_CURSOR_HOME             (0x46)
#define LCD_COMMAND_UNDERLINE_CURSOR_ON     (0x47)
#define LCD_COMMAND_UNDERLINE_CURSOR_OFF    (0x48)
#define LCD_COMMAND_MOVE_CURSOR_LEFT        (0x49)
#define LCD_COMMAND_MOVE_CURSOR_RIGHT       (0x4A)
#define LCD_COMMAND_BLINK_CURSOR_ON         (0x4B)
#define LCD_COMMAND_BLINK_CURSOR_OFF        (0x4C)
#define LCD_COMMAND_BACKSPACE               (0x4E)
#define LCD_COMMAND_CLEAR_SPACE             (0x51)
#define LCD_COMMAND_SET_CONTRAST            (0x52)
#define LCD_COMMAND_SET_BRIGHTNESS          (0x53)
#define LCD_COMMAND_LOAD_CUSTOM_CHAR        (0x54)
#define LCD_COMMAND_MOVE_DISP_LEFT          (0x55)
#define LCD_COMMAND_MOVE_DISP_RIGHT         (0x56)
#define LCD_COMMAND_RS232_BAUDRATE          (0x61)
#define LCD_COMMAND_I2C_ADDRESS             (0x62)
#define LCD_COMMAND_DISP_FIRMWARE_VERSION   (0x70)
#define LCD_COMMAND_DISP_RS232_BAUDRATE     (0x71)
#define LCD_COMMAND_DISP_I2C_ADDRESS        (0x72)

bool lcd_init(void);
bool set_character(uint8_t row, uint8_t column, char * c);

#endif