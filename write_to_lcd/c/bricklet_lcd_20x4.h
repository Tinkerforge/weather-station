/* ***********************************************************
 * This file was automatically generated on 2012-12-14.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

#ifndef BRICKLET_LCD_20X4_H
#define BRICKLET_LCD_20X4_H

#include "ip_connection.h"

/**
 * \defgroup BrickletLCD20x4 LCD20x4 Bricklet
 */

/**
 * \ingroup BrickletLCD20x4
 *
 * Device for controlling a LCD with 4 lines a 20 characters
 */
typedef Device LCD20x4;

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_WRITE_LINE 1

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_CLEAR_DISPLAY 2

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_BACKLIGHT_ON 3

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_BACKLIGHT_OFF 4

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_IS_BACKLIGHT_ON 5

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_SET_CONFIG 6

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_GET_CONFIG 7

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_IS_BUTTON_PRESSED 8

/**
 * \ingroup BrickletLCD20x4
 */
#define LCD_20X4_FUNCTION_GET_IDENTITY 255

/**
 * \ingroup BrickletLCD20x4
 *
 * This callback is triggered when a button is pressed. The parameter is
 * the number of the button (0 to 2).
 */
#define LCD_20X4_CALLBACK_BUTTON_PRESSED 9

/**
 * \ingroup BrickletLCD20x4
 *
 * This callback is triggered when a button is released. The parameter is
 * the number of the button (0 to 2).
 */
#define LCD_20X4_CALLBACK_BUTTON_RELEASED 10

/**
 * \ingroup BrickletLCD20x4
 *
 * The device identifier is used to identify a device in the enumerate callback
 * of the IPConnection.
 */
#define LCD_20X4_DEVICE_IDENTIFIER 212

/**
 * \ingroup BrickletLCD20x4
 *
 * Creates the device object \u lcd_20x4 with the unique device ID \c uid and adds
 * it to the IPConnection \c ipcon.
 */
void lcd_20x4_create(LCD20x4 *lcd_20x4, const char *uid, IPConnection *ipcon);

/**
 * \ingroup BrickletLCD20x4
 *
 * Removes the device object \u lcd_20x4 from its IPConnection and destroys it.
 * The device object cannot be used anymore afterwards.
 */
void lcd_20x4_destroy(LCD20x4 *lcd_20x4);

int lcd_20x4_get_response_expected(LCD20x4 *lcd_20x4, uint8_t function_id);

void lcd_20x4_set_response_expected(LCD20x4 *lcd_20x4, uint8_t function_id, bool response_expected);

void lcd_20x4_set_response_expected_all(LCD20x4 *lcd_20x4, bool response_expected);

/**
 * \ingroup BrickletLCD20x4
 *
 * Registers a callback with ID \c id to the function \c callback.
 */
void lcd_20x4_register_callback(LCD20x4 *lcd_20x4, uint8_t id, void *callback, void *user_data);

/**
 * \ingroup BrickletLCD20x4
 *
 * Returns the name (including the hardware version), the firmware version
 * and the binding version of the device. The firmware and binding versions are
 * given in arrays of size 3 with the syntax [major, minor, revision].
 */
int lcd_20x4_get_api_version(LCD20x4 *lcd_20x4, uint8_t ret_api_version[3]);

/**
 * \ingroup BrickletLCD20x4
 *
 * Writes text to a specific line (0 to 3) with a specific position 
 * (0 to 19). The text can have a maximum of 20 characters.
 * 
 * For example: (0, 7, "Hello") will write *Hello* in the middle of the
 * first line of the display.
 * 
 * The display uses a special charset that includes all ASCII characters except
 * backslash and tilde. The LCD charset also includes several other non-ASCII characters, see
 * the `charset specification <https://github.com/Tinkerforge/lcd-20x4-bricklet/raw/master/datasheets/standard_charset.pdf>`__
 * for details. The Unicode example above shows how to specify non-ASCII characters
 * and how to translate from Unicode to the LCD charset.
 */
int lcd_20x4_write_line(LCD20x4 *lcd_20x4, uint8_t line, uint8_t position, char text[20]);

/**
 * \ingroup BrickletLCD20x4
 *
 * Deletes all characters from the display.
 */
int lcd_20x4_clear_display(LCD20x4 *lcd_20x4);

/**
 * \ingroup BrickletLCD20x4
 *
 * Turns the backlight on.
 */
int lcd_20x4_backlight_on(LCD20x4 *lcd_20x4);

/**
 * \ingroup BrickletLCD20x4
 *
 * Turns the backlight off.
 */
int lcd_20x4_backlight_off(LCD20x4 *lcd_20x4);

/**
 * \ingroup BrickletLCD20x4
 *
 * Returns *true* if the backlight is on and *false* otherwise.
 */
int lcd_20x4_is_backlight_on(LCD20x4 *lcd_20x4, bool *ret_backlight);

/**
 * \ingroup BrickletLCD20x4
 *
 * Configures if the cursor (shown as "_") should be visible and if it
 * should be blinking (shown as a blinking block). The cursor position
 * is one character behind the the last text written with 
 * {@link lcd_20x4_write_line}.
 * 
 * The default is (false, false).
 */
int lcd_20x4_set_config(LCD20x4 *lcd_20x4, bool cursor, bool blinking);

/**
 * \ingroup BrickletLCD20x4
 *
 * Returns the configuration as set by {@link lcd_20x4_set_config}.
 */
int lcd_20x4_get_config(LCD20x4 *lcd_20x4, bool *ret_cursor, bool *ret_blinking);

/**
 * \ingroup BrickletLCD20x4
 *
 * Returns *true* if the button (0 to 2) is pressed. If you want to react
 * on button presses and releases it is recommended to use the
 * {@link LCD_20X4_CALLBACK_BUTTON_PRESSED} and {@link LCD_20X4_CALLBACK_BUTTON_RELEASED} callbacks.
 */
int lcd_20x4_is_button_pressed(LCD20x4 *lcd_20x4, uint8_t button, bool *ret_pressed);

/**
 * \ingroup BrickletLCD20x4
 *
 * Returns the UID, the UID where the Bricklet is connected to, 
 * the position, the hardware and firmware version as well as the
 * device identifier.
 * 
 * The position can be 'a', 'b', 'c' or 'd'.
 * 
 * The device identifiers are:
 * 
 * \verbatim
 *  "Device Identifier", "Device"
 * 
 *  "11", "Brick DC"
 *  "12", "Brick Debug"
 *  "13", "Brick Master 2.0"
 *  "14", "Brick Servo"
 *  "15", "Brick Stepper"
 *  "16", "Brick IMU"
 *  "", ""
 *  "21", "Bricklet Ambient Light"
 *  "22", "Bricklet Breakout"
 *  "23", "Bricklet Current12"
 *  "24", "Bricklet Current25"
 *  "25", "Bricklet Distance IR"
 *  "26", "Bricklet Dual Relay"
 *  "27", "Bricklet Humidity"
 *  "28", "Bricklet IO-16"
 *  "29", "Bricklet IO-4"
 *  "210", "Bricklet Joystick"
 *  "211", "Bricklet LCD 16x2"
 *  "212", "Bricklet LCD 20x4"
 *  "213", "Bricklet Linear Poti"
 *  "214", "Bricklet Piezo Buzzer"
 *  "215", "Bricklet Rotary Poti"
 *  "216", "Bricklet Temperature"
 *  "217", "Bricklet Temperature IR"
 *  "218", "Bricklet Voltage"
 *  "219", "Bricklet Analog In"
 *  "220", "Bricklet Analog Out"
 *  "221", "Bricklet Barometer"
 *  "222", "Bricklet GPS"
 *  "223", "Bricklet Industrial Digital In 4"
 *  "224", "Bricklet Industrial Digital Out 4"
 *  "225", "Bricklet Industrial Quad Relay"
 *  "226", "Bricklet PTC"
 * \endverbatim
 * 
 * .. versionadded:: 2.0.0~(Plugin)
 */
int lcd_20x4_get_identity(LCD20x4 *lcd_20x4, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier);

#endif
