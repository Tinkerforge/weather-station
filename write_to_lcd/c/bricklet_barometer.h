/* ***********************************************************
 * This file was automatically generated on 2012-12-14.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

#ifndef BRICKLET_BAROMETER_H
#define BRICKLET_BAROMETER_H

#include "ip_connection.h"

/**
 * \defgroup BrickletBarometer Barometer Bricklet
 */

/**
 * \ingroup BrickletBarometer
 *
 * Device for sensing air pressure and altitude changes
 */
typedef Device Barometer;

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_AIR_PRESSURE 1

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_ALTITUDE 2

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_SET_AIR_PRESSURE_CALLBACK_PERIOD 3

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_AIR_PRESSURE_CALLBACK_PERIOD 4

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_SET_ALTITUDE_CALLBACK_PERIOD 5

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_ALTITUDE_CALLBACK_PERIOD 6

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_SET_AIR_PRESSURE_CALLBACK_THRESHOLD 7

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_AIR_PRESSURE_CALLBACK_THRESHOLD 8

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_SET_ALTITUDE_CALLBACK_THRESHOLD 9

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_ALTITUDE_CALLBACK_THRESHOLD 10

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_SET_DEBOUNCE_PERIOD 11

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_DEBOUNCE_PERIOD 12

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_SET_REFERENCE_AIR_PRESSURE 13

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_CHIP_TEMPERATURE 14

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_REFERENCE_AIR_PRESSURE 19

/**
 * \ingroup BrickletBarometer
 */
#define BAROMETER_FUNCTION_GET_IDENTITY 255

/**
 * \ingroup BrickletBarometer
 *
 * This callback is triggered periodically with the period that is set by
 * {@link barometer_set_air_pressure_callback_period}. The parameter is the air pressure of the
 * air pressure sensor.
 * 
 * {@link BAROMETER_CALLBACK_AIR_PRESSURE} is only triggered if the air pressure has changed since the
 * last triggering.
 */
#define BAROMETER_CALLBACK_AIR_PRESSURE 15

/**
 * \ingroup BrickletBarometer
 *
 * This callback is triggered periodically with the period that is set by
 * {@link barometer_set_altitude_callback_period}. The parameter is the altitude of the
 * air pressure sensor.
 * 
 * {@link BAROMETER_CALLBACK_ALTITUDE} is only triggered if the altitude has changed since the
 * last triggering.
 */
#define BAROMETER_CALLBACK_ALTITUDE 16

/**
 * \ingroup BrickletBarometer
 *
 * This callback is triggered when the threshold as set by
 * {@link barometer_set_air_pressure_callback_threshold} is reached.
 * The parameter is the air pressure of the air pressure sensor.
 * 
 * If the threshold keeps being reached, the callback is triggered periodically
 * with the period as set by {@link barometer_set_debounce_period}.
 */
#define BAROMETER_CALLBACK_AIR_PRESSURE_REACHED 17

/**
 * \ingroup BrickletBarometer
 *
 * This callback is triggered when the threshold as set by
 * {@link barometer_set_altitude_callback_threshold} is reached.
 * The parameter is the altitude of the air pressure sensor.
 * 
 * If the threshold keeps being reached, the callback is triggered periodically
 * with the period as set by {@link barometer_set_debounce_period}.
 */
#define BAROMETER_CALLBACK_ALTITUDE_REACHED 18

/**
 * \ingroup BrickletBarometer
 *
 * The device identifier is used to identify a device in the enumerate callback
 * of the IPConnection.
 */
#define BAROMETER_DEVICE_IDENTIFIER 221

/**
 * \ingroup BrickletBarometer
 *
 * Creates the device object \u barometer with the unique device ID \c uid and adds
 * it to the IPConnection \c ipcon.
 */
void barometer_create(Barometer *barometer, const char *uid, IPConnection *ipcon);

/**
 * \ingroup BrickletBarometer
 *
 * Removes the device object \u barometer from its IPConnection and destroys it.
 * The device object cannot be used anymore afterwards.
 */
void barometer_destroy(Barometer *barometer);

int barometer_get_response_expected(Barometer *barometer, uint8_t function_id);

void barometer_set_response_expected(Barometer *barometer, uint8_t function_id, bool response_expected);

void barometer_set_response_expected_all(Barometer *barometer, bool response_expected);

/**
 * \ingroup BrickletBarometer
 *
 * Registers a callback with ID \c id to the function \c callback.
 */
void barometer_register_callback(Barometer *barometer, uint8_t id, void *callback, void *user_data);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the name (including the hardware version), the firmware version
 * and the binding version of the device. The firmware and binding versions are
 * given in arrays of size 3 with the syntax [major, minor, revision].
 */
int barometer_get_api_version(Barometer *barometer, uint8_t ret_api_version[3]);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the air pressure of the air pressure sensor. The value
 * has a range of 10000 to 1200000 and is given in mbar/1000, i.e. a value
 * of 1001092 means that an air pressure of 1001.092 mbar is measured.
 * 
 * If you want to get the air pressure periodically, it is recommended to use the
 * callback {@link BAROMETER_CALLBACK_AIR_PRESSURE} and set the period with
 * {@link barometer_set_air_pressure_callback_period}.
 */
int barometer_get_air_pressure(Barometer *barometer, int32_t *ret_air_pressure);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the relative altitude of the air pressure sensor. The value is given in
 * cm and is caluclated based on the difference between the current air pressure
 * and the reference air pressure that can be set with {@link barometer_set_reference_air_pressure}.
 * 
 * If you want to get the altitude periodically, it is recommended to use the
 * callback {@link BAROMETER_CALLBACK_ALTITUDE} and set the period with
 * {@link barometer_set_altitude_callback_period}.
 */
int barometer_get_altitude(Barometer *barometer, int32_t *ret_altitude);

/**
 * \ingroup BrickletBarometer
 *
 * Sets the period in ms with which the {@link BAROMETER_CALLBACK_AIR_PRESSURE} callback is triggered
 * periodically. A value of 0 turns the callback off.
 * 
 * {@link BAROMETER_CALLBACK_AIR_PRESSURE} is only triggered if the air pressure has changed since the
 * last triggering.
 * 
 * The default value is 0.
 */
int barometer_set_air_pressure_callback_period(Barometer *barometer, uint32_t period);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the period as set by {@link barometer_set_air_pressure_callback_period}.
 */
int barometer_get_air_pressure_callback_period(Barometer *barometer, uint32_t *ret_period);

/**
 * \ingroup BrickletBarometer
 *
 * Sets the period in ms with which the {@link BAROMETER_CALLBACK_ALTITUDE} callback is triggered
 * periodically. A value of 0 turns the callback off.
 * 
 * {@link BAROMETER_CALLBACK_ALTITUDE} is only triggered if the altitude has changed since the
 * last triggering.
 * 
 * The default value is 0.
 */
int barometer_set_altitude_callback_period(Barometer *barometer, uint32_t period);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the period as set by {@link barometer_set_altitude_callback_period}.
 */
int barometer_get_altitude_callback_period(Barometer *barometer, uint32_t *ret_period);

/**
 * \ingroup BrickletBarometer
 *
 * Sets the thresholds for the {@link BAROMETER_CALLBACK_AIR_PRESSURE_REACHED} callback.
 * 
 * The following options are possible:
 * 
 * \verbatim
 *  "Option", "Description"
 * 
 *  "'x'",    "Callback is turned off"
 *  "'o'",    "Callback is triggered when the air pressure is *outside* the min and max values"
 *  "'i'",    "Callback is triggered when the air pressure is *inside* the min and max values"
 *  "'<'",    "Callback is triggered when the air pressure is smaller than the min value (max is ignored)"
 *  "'>'",    "Callback is triggered when the air pressure is greater than the min value (max is ignored)"
 * \endverbatim
 * 
 * The default value is ('x', 0, 0).
 */
int barometer_set_air_pressure_callback_threshold(Barometer *barometer, char option, int32_t min, int32_t max);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the threshold as set by {@link barometer_set_air_pressure_callback_threshold}.
 */
int barometer_get_air_pressure_callback_threshold(Barometer *barometer, char *ret_option, int32_t *ret_min, int32_t *ret_max);

/**
 * \ingroup BrickletBarometer
 *
 * Sets the thresholds for the {@link BAROMETER_CALLBACK_ALTITUDE_REACHED} callback.
 * 
 * The following options are possible:
 * 
 * \verbatim
 *  "Option", "Description"
 * 
 *  "'x'",    "Callback is turned off"
 *  "'o'",    "Callback is triggered when the altitude is *outside* the min and max values"
 *  "'i'",    "Callback is triggered when the altitude is *inside* the min and max values"
 *  "'<'",    "Callback is triggered when the altitude is smaller than the min value (max is ignored)"
 *  "'>'",    "Callback is triggered when the altitude is greater than the min value (max is ignored)"
 * \endverbatim
 * 
 * The default value is ('x', 0, 0).
 */
int barometer_set_altitude_callback_threshold(Barometer *barometer, char option, int32_t min, int32_t max);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the threshold as set by {@link barometer_set_altitude_callback_threshold}.
 */
int barometer_get_altitude_callback_threshold(Barometer *barometer, char *ret_option, int32_t *ret_min, int32_t *ret_max);

/**
 * \ingroup BrickletBarometer
 *
 * Sets the period in ms with which the threshold callbacks
 * 
 *  {@link BAROMETER_CALLBACK_AIR_PRESSURE_REACHED}, {@link BAROMETER_CALLBACK_ALTITUDE_REACHED}
 * 
 * are triggered, if the thresholds
 * 
 *  {@link barometer_set_air_pressure_callback_threshold}, {@link barometer_set_altitude_callback_threshold}
 * 
 * keep being reached.
 * 
 * The default value is 100.
 */
int barometer_set_debounce_period(Barometer *barometer, uint32_t debounce);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the debounce period as set by {@link barometer_set_debounce_period}.
 */
int barometer_get_debounce_period(Barometer *barometer, uint32_t *ret_debounce);

/**
 * \ingroup BrickletBarometer
 *
 * Sets the reference air pressure in mbar/1000 for the altitude calculation.
 * Setting the reference to the current air pressure results in a calculated
 * altitude of 0cm. Passing 0 is a shortcut for passing the current air pressure as
 * reference.
 * 
 * Well known reference values are the Q codes
 * `QNH <http://en.wikipedia.org/wiki/QNH>`__ and
 * `QFE <http://en.wikipedia.org/wiki/Mean_sea_level_pressure#Mean_sea_level_pressure>`__
 * used in aviation.
 * 
 * The default value is 1013.25mbar.
 * 
 * .. versionadded:: 1.1.0~(Plugin)
 */
int barometer_set_reference_air_pressure(Barometer *barometer, int32_t air_pressure);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the temperature of the air pressure sensor. The value
 * has a range of -4000 to 8500 and is given in °C/100, i.e. a value
 * of 2007 means that a temperature of 20.07 °C is measured.
 * 
 * This temperature is used internally for temperature compensation of the air
 * pressure measurement. It is not as accurate as the temperature measured by the
 * :ref:`temperature_bricklet` or the :ref:`temperature_ir_bricklet`.
 */
int barometer_get_chip_temperature(Barometer *barometer, int16_t *ret_temperature);

/**
 * \ingroup BrickletBarometer
 *
 * Returns the reference air pressure as set by {@link barometer_set_reference_air_pressure}.
 * 
 * .. versionadded:: 1.1.0~(Plugin)
 */
int barometer_get_reference_air_pressure(Barometer *barometer, int32_t *ret_air_pressure);

/**
 * \ingroup BrickletBarometer
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
int barometer_get_identity(Barometer *barometer, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier);

#endif
