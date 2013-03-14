/* ***********************************************************
 * This file was automatically generated on 2012-12-14.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

#ifndef BRICKLET_AMBIENT_LIGHT_H
#define BRICKLET_AMBIENT_LIGHT_H

#include "ip_connection.h"

/**
 * \defgroup BrickletAmbientLight AmbientLight Bricklet
 */

/**
 * \ingroup BrickletAmbientLight
 *
 * Device for sensing Ambient Light
 */
typedef Device AmbientLight;

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE 1

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE 2

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_SET_ILLUMINANCE_CALLBACK_PERIOD 3

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE_CALLBACK_PERIOD 4

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_SET_ANALOG_VALUE_CALLBACK_PERIOD 5

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE_CALLBACK_PERIOD 6

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_SET_ILLUMINANCE_CALLBACK_THRESHOLD 7

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE_CALLBACK_THRESHOLD 8

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_SET_ANALOG_VALUE_CALLBACK_THRESHOLD 9

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE_CALLBACK_THRESHOLD 10

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_SET_DEBOUNCE_PERIOD 11

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_DEBOUNCE_PERIOD 12

/**
 * \ingroup BrickletAmbientLight
 */
#define AMBIENT_LIGHT_FUNCTION_GET_IDENTITY 255

/**
 * \ingroup BrickletAmbientLight
 *
 * This callback is triggered periodically with the period that is set by
 * {@link ambient_light_set_illuminance_callback_period}. The parameter is the illuminance of the
 * ambient light sensor.
 * 
 * {@link AMBIENT_LIGHT_CALLBACK_ILLUMINANCE} is only triggered if the illuminance has changed since the
 * last triggering.
 */
#define AMBIENT_LIGHT_CALLBACK_ILLUMINANCE 13

/**
 * \ingroup BrickletAmbientLight
 *
 * This callback is triggered periodically with the period that is set by
 * {@link ambient_light_set_analog_value_callback_period}. The parameter is the analog value of the
 * ambient light sensor.
 * 
 * {@link AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE} is only triggered if the analog value has changed since the
 * last triggering.
 */
#define AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE 14

/**
 * \ingroup BrickletAmbientLight
 *
 * This callback is triggered when the threshold as set by
 * {@link ambient_light_set_illuminance_callback_threshold} is reached.
 * The parameter is the illuminance of the ambient light sensor.
 * 
 * If the threshold keeps being reached, the callback is triggered periodically
 * with the period as set by {@link ambient_light_set_debounce_period}.
 */
#define AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED 15

/**
 * \ingroup BrickletAmbientLight
 *
 * This callback is triggered when the threshold as set by
 * {@link ambient_light_set_analog_value_callback_threshold} is reached.
 * The parameter is the analog value of the ambient light sensor.
 * 
 * If the threshold keeps being reached, the callback is triggered periodically
 * with the period as set by {@link ambient_light_set_debounce_period}.
 */
#define AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE_REACHED 16

/**
 * \ingroup BrickletAmbientLight
 *
 * The device identifier is used to identify a device in the enumerate callback
 * of the IPConnection.
 */
#define AMBIENT_LIGHT_DEVICE_IDENTIFIER 21

/**
 * \ingroup BrickletAmbientLight
 *
 * Creates the device object \u ambient_light with the unique device ID \c uid and adds
 * it to the IPConnection \c ipcon.
 */
void ambient_light_create(AmbientLight *ambient_light, const char *uid, IPConnection *ipcon);

/**
 * \ingroup BrickletAmbientLight
 *
 * Removes the device object \u ambient_light from its IPConnection and destroys it.
 * The device object cannot be used anymore afterwards.
 */
void ambient_light_destroy(AmbientLight *ambient_light);

int ambient_light_get_response_expected(AmbientLight *ambient_light, uint8_t function_id);

void ambient_light_set_response_expected(AmbientLight *ambient_light, uint8_t function_id, bool response_expected);

void ambient_light_set_response_expected_all(AmbientLight *ambient_light, bool response_expected);

/**
 * \ingroup BrickletAmbientLight
 *
 * Registers a callback with ID \c id to the function \c callback.
 */
void ambient_light_register_callback(AmbientLight *ambient_light, uint8_t id, void *callback, void *user_data);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the name (including the hardware version), the firmware version
 * and the binding version of the device. The firmware and binding versions are
 * given in arrays of size 3 with the syntax [major, minor, revision].
 */
int ambient_light_get_api_version(AmbientLight *ambient_light, uint8_t ret_api_version[3]);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the illuminance of the ambient light sensor. The value
 * has a range of 0 to 9000 and is given in Lux/10, i.e. a value
 * of 4500 means that an illuminance of 450 Lux is measured.
 * 
 * If you want to get the illuminance periodically, it is recommended to use the
 * callback {@link AMBIENT_LIGHT_CALLBACK_ILLUMINANCE} and set the period with 
 * {@link ambient_light_set_illuminance_callback_period}.
 */
int ambient_light_get_illuminance(AmbientLight *ambient_light, uint16_t *ret_illuminance);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the value as read by a 12-bit analog-to-digital converter.
 * The value is between 0 and 4095.
 * 
 * \note
 *  The value returned by {@link ambient_light_get_illuminance} is averaged over several samples
 *  to yield less noise, while {@link ambient_light_get_analog_value} gives back raw
 *  unfiltered analog values. The only reason to use {@link ambient_light_get_analog_value} is,
 *  if you need the full resolution of the analog-to-digital converter.
 * 
 *  Also, the analog-to-digital converter covers three different ranges that are
 *  set dynamically depending on the light intensity. It is impossible to
 *  distinguish between these ranges with the analog value.
 * 
 * If you want the analog value periodically, it is recommended to use the 
 * callback {@link AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE} and set the period with 
 * {@link ambient_light_set_analog_value_callback_period}.
 */
int ambient_light_get_analog_value(AmbientLight *ambient_light, uint16_t *ret_value);

/**
 * \ingroup BrickletAmbientLight
 *
 * Sets the period in ms with which the {@link AMBIENT_LIGHT_CALLBACK_ILLUMINANCE} callback is triggered
 * periodically. A value of 0 turns the callback off.
 * 
 * {@link AMBIENT_LIGHT_CALLBACK_ILLUMINANCE} is only triggered if the illuminance has changed since the
 * last triggering.
 * 
 * The default value is 0.
 */
int ambient_light_set_illuminance_callback_period(AmbientLight *ambient_light, uint32_t period);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the period as set by {@link ambient_light_set_illuminance_callback_period}.
 */
int ambient_light_get_illuminance_callback_period(AmbientLight *ambient_light, uint32_t *ret_period);

/**
 * \ingroup BrickletAmbientLight
 *
 * Sets the period in ms with which the {@link AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE} callback is triggered
 * periodically. A value of 0 turns the callback off.
 * 
 * {@link AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE} is only triggered if the analog value has changed since the
 * last triggering.
 * 
 * The default value is 0.
 */
int ambient_light_set_analog_value_callback_period(AmbientLight *ambient_light, uint32_t period);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the period as set by {@link ambient_light_set_analog_value_callback_period}.
 */
int ambient_light_get_analog_value_callback_period(AmbientLight *ambient_light, uint32_t *ret_period);

/**
 * \ingroup BrickletAmbientLight
 *
 * Sets the thresholds for the {@link AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED} callback. 
 * 
 * The following options are possible:
 * 
 * \verbatim
 *  "Option", "Description"
 * 
 *  "'x'",    "Callback is turned off"
 *  "'o'",    "Callback is triggered when the illuminance is *outside* the min and max values"
 *  "'i'",    "Callback is triggered when the illuminance is *inside* the min and max values"
 *  "'<'",    "Callback is triggered when the illuminance is smaller than the min value (max is ignored)"
 *  "'>'",    "Callback is triggered when the illuminance is greater than the min value (max is ignored)"
 * \endverbatim
 * 
 * The default value is ('x', 0, 0).
 */
int ambient_light_set_illuminance_callback_threshold(AmbientLight *ambient_light, char option, int16_t min, int16_t max);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the threshold as set by {@link ambient_light_set_illuminance_callback_threshold}.
 */
int ambient_light_get_illuminance_callback_threshold(AmbientLight *ambient_light, char *ret_option, int16_t *ret_min, int16_t *ret_max);

/**
 * \ingroup BrickletAmbientLight
 *
 * Sets the thresholds for the {@link AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE_REACHED} callback. 
 * 
 * The following options are possible:
 * 
 * \verbatim
 *  "Option", "Description"
 * 
 *  "'x'",    "Callback is turned off"
 *  "'o'",    "Callback is triggered when the analog value is *outside* the min and max values"
 *  "'i'",    "Callback is triggered when the analog value is *inside* the min and max values"
 *  "'<'",    "Callback is triggered when the analog value is smaller than the min value (max is ignored)"
 *  "'>'",    "Callback is triggered when the analog value is greater than the min value (max is ignored)"
 * \endverbatim
 * 
 * The default value is ('x', 0, 0).
 */
int ambient_light_set_analog_value_callback_threshold(AmbientLight *ambient_light, char option, uint16_t min, uint16_t max);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the threshold as set by {@link ambient_light_set_analog_value_callback_threshold}.
 */
int ambient_light_get_analog_value_callback_threshold(AmbientLight *ambient_light, char *ret_option, uint16_t *ret_min, uint16_t *ret_max);

/**
 * \ingroup BrickletAmbientLight
 *
 * Sets the period in ms with which the threshold callbacks
 * 
 *  {@link AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED}, {@link AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE_REACHED}
 * 
 * are triggered, if the thresholds
 * 
 *  {@link ambient_light_set_illuminance_callback_threshold}, {@link ambient_light_set_analog_value_callback_threshold}
 * 
 * keep being reached.
 * 
 * The default value is 100.
 */
int ambient_light_set_debounce_period(AmbientLight *ambient_light, uint32_t debounce);

/**
 * \ingroup BrickletAmbientLight
 *
 * Returns the debounce period as set by {@link ambient_light_set_debounce_period}.
 */
int ambient_light_get_debounce_period(AmbientLight *ambient_light, uint32_t *ret_debounce);

/**
 * \ingroup BrickletAmbientLight
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
int ambient_light_get_identity(AmbientLight *ambient_light, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier);

#endif
