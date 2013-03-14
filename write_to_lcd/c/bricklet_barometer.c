/* ***********************************************************
 * This file was automatically generated on 2012-12-14.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/


#define IPCON_EXPOSE_INTERNALS

#include "bricklet_barometer.h"

#include <string.h>



typedef void (*AirPressureCallbackFunction)(int32_t, void *);

typedef void (*AltitudeCallbackFunction)(int32_t, void *);

typedef void (*AirPressureReachedCallbackFunction)(int32_t, void *);

typedef void (*AltitudeReachedCallbackFunction)(int32_t, void *);

#if defined _MSC_VER || defined __BORLANDC__
	#pragma pack(push)
	#pragma pack(1)
	#define ATTRIBUTE_PACKED
#elif defined __GNUC__
	#define ATTRIBUTE_PACKED __attribute__((packed))
#else
	#error unknown compiler, do not know how to enable struct packing
#endif

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAirPressure_;

typedef struct {
	PacketHeader header;
	int32_t air_pressure;
} ATTRIBUTE_PACKED GetAirPressureResponse_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAltitude_;

typedef struct {
	PacketHeader header;
	int32_t altitude;
} ATTRIBUTE_PACKED GetAltitudeResponse_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED SetAirPressureCallbackPeriod_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAirPressureCallbackPeriod_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED GetAirPressureCallbackPeriodResponse_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED SetAltitudeCallbackPeriod_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAltitudeCallbackPeriod_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED GetAltitudeCallbackPeriodResponse_;

typedef struct {
	PacketHeader header;
	char option;
	int32_t min;
	int32_t max;
} ATTRIBUTE_PACKED SetAirPressureCallbackThreshold_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAirPressureCallbackThreshold_;

typedef struct {
	PacketHeader header;
	char option;
	int32_t min;
	int32_t max;
} ATTRIBUTE_PACKED GetAirPressureCallbackThresholdResponse_;

typedef struct {
	PacketHeader header;
	char option;
	int32_t min;
	int32_t max;
} ATTRIBUTE_PACKED SetAltitudeCallbackThreshold_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAltitudeCallbackThreshold_;

typedef struct {
	PacketHeader header;
	char option;
	int32_t min;
	int32_t max;
} ATTRIBUTE_PACKED GetAltitudeCallbackThresholdResponse_;

typedef struct {
	PacketHeader header;
	uint32_t debounce;
} ATTRIBUTE_PACKED SetDebouncePeriod_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetDebouncePeriod_;

typedef struct {
	PacketHeader header;
	uint32_t debounce;
} ATTRIBUTE_PACKED GetDebouncePeriodResponse_;

typedef struct {
	PacketHeader header;
	int32_t air_pressure;
} ATTRIBUTE_PACKED SetReferenceAirPressure_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetChipTemperature_;

typedef struct {
	PacketHeader header;
	int16_t temperature;
} ATTRIBUTE_PACKED GetChipTemperatureResponse_;

typedef struct {
	PacketHeader header;
	int32_t air_pressure;
} ATTRIBUTE_PACKED AirPressureCallback_;

typedef struct {
	PacketHeader header;
	int32_t altitude;
} ATTRIBUTE_PACKED AltitudeCallback_;

typedef struct {
	PacketHeader header;
	int32_t air_pressure;
} ATTRIBUTE_PACKED AirPressureReachedCallback_;

typedef struct {
	PacketHeader header;
	int32_t altitude;
} ATTRIBUTE_PACKED AltitudeReachedCallback_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetReferenceAirPressure_;

typedef struct {
	PacketHeader header;
	int32_t air_pressure;
} ATTRIBUTE_PACKED GetReferenceAirPressureResponse_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetIdentity_;

typedef struct {
	PacketHeader header;
	char uid[8];
	char connected_uid[8];
	char position;
	uint8_t hardware_version[3];
	uint8_t firmware_version[3];
	uint16_t device_identifier;
} ATTRIBUTE_PACKED GetIdentityResponse_;

#if defined _MSC_VER || defined __BORLANDC__
	#pragma pack(pop)
#endif
#undef ATTRIBUTE_PACKED

static void barometer_callback_wrapper_air_pressure(Barometer *barometer, Packet *packet) {
	AirPressureCallbackFunction callback_function = (AirPressureCallbackFunction)barometer->registered_callbacks[BAROMETER_CALLBACK_AIR_PRESSURE];
	void *user_data = barometer->registered_callback_user_data[BAROMETER_CALLBACK_AIR_PRESSURE];
	AirPressureCallback_ *callback = (AirPressureCallback_ *)packet;

	callback->air_pressure = leconvert_int32_from(callback->air_pressure);

	if (callback_function != NULL) {
		callback_function(callback->air_pressure, user_data);
	}
}

static void barometer_callback_wrapper_altitude(Barometer *barometer, Packet *packet) {
	AltitudeCallbackFunction callback_function = (AltitudeCallbackFunction)barometer->registered_callbacks[BAROMETER_CALLBACK_ALTITUDE];
	void *user_data = barometer->registered_callback_user_data[BAROMETER_CALLBACK_ALTITUDE];
	AltitudeCallback_ *callback = (AltitudeCallback_ *)packet;

	callback->altitude = leconvert_int32_from(callback->altitude);

	if (callback_function != NULL) {
		callback_function(callback->altitude, user_data);
	}
}

static void barometer_callback_wrapper_air_pressure_reached(Barometer *barometer, Packet *packet) {
	AirPressureReachedCallbackFunction callback_function = (AirPressureReachedCallbackFunction)barometer->registered_callbacks[BAROMETER_CALLBACK_AIR_PRESSURE_REACHED];
	void *user_data = barometer->registered_callback_user_data[BAROMETER_CALLBACK_AIR_PRESSURE_REACHED];
	AirPressureReachedCallback_ *callback = (AirPressureReachedCallback_ *)packet;

	callback->air_pressure = leconvert_int32_from(callback->air_pressure);

	if (callback_function != NULL) {
		callback_function(callback->air_pressure, user_data);
	}
}

static void barometer_callback_wrapper_altitude_reached(Barometer *barometer, Packet *packet) {
	AltitudeReachedCallbackFunction callback_function = (AltitudeReachedCallbackFunction)barometer->registered_callbacks[BAROMETER_CALLBACK_ALTITUDE_REACHED];
	void *user_data = barometer->registered_callback_user_data[BAROMETER_CALLBACK_ALTITUDE_REACHED];
	AltitudeReachedCallback_ *callback = (AltitudeReachedCallback_ *)packet;

	callback->altitude = leconvert_int32_from(callback->altitude);

	if (callback_function != NULL) {
		callback_function(callback->altitude, user_data);
	}
}

void barometer_create(Barometer *barometer, const char *uid, IPConnection *ipcon) {
	device_create(barometer, uid, ipcon);

	barometer->api_version[0] = 1;
	barometer->api_version[1] = 1;
	barometer->api_version[2] = 0;

	barometer->response_expected[BAROMETER_FUNCTION_GET_AIR_PRESSURE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_ALTITUDE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_SET_AIR_PRESSURE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_AIR_PRESSURE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_SET_ALTITUDE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_ALTITUDE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_SET_AIR_PRESSURE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_AIR_PRESSURE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_SET_ALTITUDE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_ALTITUDE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_SET_DEBOUNCE_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_DEBOUNCE_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_SET_REFERENCE_AIR_PRESSURE] = DEVICE_RESPONSE_EXPECTED_FALSE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_CHIP_TEMPERATURE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_CALLBACK_AIR_PRESSURE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	barometer->response_expected[BAROMETER_CALLBACK_ALTITUDE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	barometer->response_expected[BAROMETER_CALLBACK_AIR_PRESSURE_REACHED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	barometer->response_expected[BAROMETER_CALLBACK_ALTITUDE_REACHED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_REFERENCE_AIR_PRESSURE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	barometer->response_expected[BAROMETER_FUNCTION_GET_IDENTITY] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;

	barometer->callback_wrappers[BAROMETER_CALLBACK_AIR_PRESSURE] = (void *)barometer_callback_wrapper_air_pressure;
	barometer->callback_wrappers[BAROMETER_CALLBACK_ALTITUDE] = (void *)barometer_callback_wrapper_altitude;
	barometer->callback_wrappers[BAROMETER_CALLBACK_AIR_PRESSURE_REACHED] = (void *)barometer_callback_wrapper_air_pressure_reached;
	barometer->callback_wrappers[BAROMETER_CALLBACK_ALTITUDE_REACHED] = (void *)barometer_callback_wrapper_altitude_reached;
}

void barometer_destroy(Barometer *barometer) {
	device_destroy(barometer);
}


int barometer_get_response_expected(Barometer *barometer, uint8_t function_id) {
	return device_get_response_expected(barometer, function_id);
}

void barometer_set_response_expected(Barometer *barometer, uint8_t function_id, bool response_expected) {
	device_set_response_expected(barometer, function_id, response_expected);
}

void barometer_set_response_expected_all(Barometer *barometer, bool response_expected) {
	device_set_response_expected_all(barometer, response_expected);
}

void barometer_register_callback(Barometer *barometer, uint8_t id, void *callback, void *user_data) {
	barometer->registered_callbacks[id] = callback;
	barometer->registered_callback_user_data[id] = user_data;
}

int barometer_get_api_version(Barometer *barometer, uint8_t ret_api_version[3]) {
	ret_api_version[0] = barometer->api_version[0];
	ret_api_version[1] = barometer->api_version[1];
	ret_api_version[2] = barometer->api_version[2];

	return E_OK;
}

int barometer_get_air_pressure(Barometer *barometer, int32_t *ret_air_pressure) {
	GetAirPressure_ request;
	GetAirPressureResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_AIR_PRESSURE, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetAirPressureResponse_ *)&barometer->response_packet;
	*ret_air_pressure = leconvert_int32_from(response->air_pressure);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_altitude(Barometer *barometer, int32_t *ret_altitude) {
	GetAltitude_ request;
	GetAltitudeResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_ALTITUDE, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetAltitudeResponse_ *)&barometer->response_packet;
	*ret_altitude = leconvert_int32_from(response->altitude);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_set_air_pressure_callback_period(Barometer *barometer, uint32_t period) {
	SetAirPressureCallbackPeriod_ request;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_SET_AIR_PRESSURE_CALLBACK_PERIOD, barometer->ipcon, barometer);

	request.period = leconvert_uint32_to(period);

	ret = device_send_request(barometer, (Packet *)&request);

	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_air_pressure_callback_period(Barometer *barometer, uint32_t *ret_period) {
	GetAirPressureCallbackPeriod_ request;
	GetAirPressureCallbackPeriodResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_AIR_PRESSURE_CALLBACK_PERIOD, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetAirPressureCallbackPeriodResponse_ *)&barometer->response_packet;
	*ret_period = leconvert_uint32_from(response->period);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_set_altitude_callback_period(Barometer *barometer, uint32_t period) {
	SetAltitudeCallbackPeriod_ request;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_SET_ALTITUDE_CALLBACK_PERIOD, barometer->ipcon, barometer);

	request.period = leconvert_uint32_to(period);

	ret = device_send_request(barometer, (Packet *)&request);

	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_altitude_callback_period(Barometer *barometer, uint32_t *ret_period) {
	GetAltitudeCallbackPeriod_ request;
	GetAltitudeCallbackPeriodResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_ALTITUDE_CALLBACK_PERIOD, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetAltitudeCallbackPeriodResponse_ *)&barometer->response_packet;
	*ret_period = leconvert_uint32_from(response->period);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_set_air_pressure_callback_threshold(Barometer *barometer, char option, int32_t min, int32_t max) {
	SetAirPressureCallbackThreshold_ request;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_SET_AIR_PRESSURE_CALLBACK_THRESHOLD, barometer->ipcon, barometer);

	request.option = option;
	request.min = leconvert_int32_to(min);
	request.max = leconvert_int32_to(max);

	ret = device_send_request(barometer, (Packet *)&request);

	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_air_pressure_callback_threshold(Barometer *barometer, char *ret_option, int32_t *ret_min, int32_t *ret_max) {
	GetAirPressureCallbackThreshold_ request;
	GetAirPressureCallbackThresholdResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_AIR_PRESSURE_CALLBACK_THRESHOLD, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetAirPressureCallbackThresholdResponse_ *)&barometer->response_packet;
	*ret_option = response->option;
	*ret_min = leconvert_int32_from(response->min);
	*ret_max = leconvert_int32_from(response->max);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_set_altitude_callback_threshold(Barometer *barometer, char option, int32_t min, int32_t max) {
	SetAltitudeCallbackThreshold_ request;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_SET_ALTITUDE_CALLBACK_THRESHOLD, barometer->ipcon, barometer);

	request.option = option;
	request.min = leconvert_int32_to(min);
	request.max = leconvert_int32_to(max);

	ret = device_send_request(barometer, (Packet *)&request);

	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_altitude_callback_threshold(Barometer *barometer, char *ret_option, int32_t *ret_min, int32_t *ret_max) {
	GetAltitudeCallbackThreshold_ request;
	GetAltitudeCallbackThresholdResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_ALTITUDE_CALLBACK_THRESHOLD, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetAltitudeCallbackThresholdResponse_ *)&barometer->response_packet;
	*ret_option = response->option;
	*ret_min = leconvert_int32_from(response->min);
	*ret_max = leconvert_int32_from(response->max);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_set_debounce_period(Barometer *barometer, uint32_t debounce) {
	SetDebouncePeriod_ request;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_SET_DEBOUNCE_PERIOD, barometer->ipcon, barometer);

	request.debounce = leconvert_uint32_to(debounce);

	ret = device_send_request(barometer, (Packet *)&request);

	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_debounce_period(Barometer *barometer, uint32_t *ret_debounce) {
	GetDebouncePeriod_ request;
	GetDebouncePeriodResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_DEBOUNCE_PERIOD, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetDebouncePeriodResponse_ *)&barometer->response_packet;
	*ret_debounce = leconvert_uint32_from(response->debounce);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_set_reference_air_pressure(Barometer *barometer, int32_t air_pressure) {
	SetReferenceAirPressure_ request;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_SET_REFERENCE_AIR_PRESSURE, barometer->ipcon, barometer);

	request.air_pressure = leconvert_int32_to(air_pressure);

	ret = device_send_request(barometer, (Packet *)&request);

	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_chip_temperature(Barometer *barometer, int16_t *ret_temperature) {
	GetChipTemperature_ request;
	GetChipTemperatureResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_CHIP_TEMPERATURE, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetChipTemperatureResponse_ *)&barometer->response_packet;
	*ret_temperature = leconvert_int16_from(response->temperature);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_reference_air_pressure(Barometer *barometer, int32_t *ret_air_pressure) {
	GetReferenceAirPressure_ request;
	GetReferenceAirPressureResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_REFERENCE_AIR_PRESSURE, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetReferenceAirPressureResponse_ *)&barometer->response_packet;
	*ret_air_pressure = leconvert_int32_from(response->air_pressure);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}

int barometer_get_identity(Barometer *barometer, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
	GetIdentity_ request;
	GetIdentityResponse_ *response;
	int ret;

	mutex_lock(&barometer->request_mutex);

	packet_header_create(&request.header, sizeof(request), BAROMETER_FUNCTION_GET_IDENTITY, barometer->ipcon, barometer);


	ret = device_send_request(barometer, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&barometer->request_mutex);

		return ret;
	}

	response = (GetIdentityResponse_ *)&barometer->response_packet;
	strncpy(ret_uid, response->uid, 8);
	strncpy(ret_connected_uid, response->connected_uid, 8);
	*ret_position = response->position;
	memcpy(ret_hardware_version, response->hardware_version, 3 * sizeof(uint8_t));
	memcpy(ret_firmware_version, response->firmware_version, 3 * sizeof(uint8_t));
	*ret_device_identifier = leconvert_uint16_from(response->device_identifier);


	mutex_unlock(&barometer->request_mutex);

	return ret;
}
