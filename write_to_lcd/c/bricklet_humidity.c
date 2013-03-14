/* ***********************************************************
 * This file was automatically generated on 2012-12-14.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/


#define IPCON_EXPOSE_INTERNALS

#include "bricklet_humidity.h"

#include <string.h>



typedef void (*HumidityCallbackFunction)(uint16_t, void *);

typedef void (*AnalogValueCallbackFunction)(uint16_t, void *);

typedef void (*HumidityReachedCallbackFunction)(uint16_t, void *);

typedef void (*AnalogValueReachedCallbackFunction)(uint16_t, void *);

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
} ATTRIBUTE_PACKED GetHumidity_;

typedef struct {
	PacketHeader header;
	uint16_t humidity;
} ATTRIBUTE_PACKED GetHumidityResponse_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAnalogValue_;

typedef struct {
	PacketHeader header;
	uint16_t value;
} ATTRIBUTE_PACKED GetAnalogValueResponse_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED SetHumidityCallbackPeriod_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetHumidityCallbackPeriod_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED GetHumidityCallbackPeriodResponse_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED SetAnalogValueCallbackPeriod_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAnalogValueCallbackPeriod_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED GetAnalogValueCallbackPeriodResponse_;

typedef struct {
	PacketHeader header;
	char option;
	int16_t min;
	int16_t max;
} ATTRIBUTE_PACKED SetHumidityCallbackThreshold_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetHumidityCallbackThreshold_;

typedef struct {
	PacketHeader header;
	char option;
	int16_t min;
	int16_t max;
} ATTRIBUTE_PACKED GetHumidityCallbackThresholdResponse_;

typedef struct {
	PacketHeader header;
	char option;
	uint16_t min;
	uint16_t max;
} ATTRIBUTE_PACKED SetAnalogValueCallbackThreshold_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetAnalogValueCallbackThreshold_;

typedef struct {
	PacketHeader header;
	char option;
	uint16_t min;
	uint16_t max;
} ATTRIBUTE_PACKED GetAnalogValueCallbackThresholdResponse_;

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
	uint16_t humidity;
} ATTRIBUTE_PACKED HumidityCallback_;

typedef struct {
	PacketHeader header;
	uint16_t value;
} ATTRIBUTE_PACKED AnalogValueCallback_;

typedef struct {
	PacketHeader header;
	uint16_t humidity;
} ATTRIBUTE_PACKED HumidityReachedCallback_;

typedef struct {
	PacketHeader header;
	uint16_t value;
} ATTRIBUTE_PACKED AnalogValueReachedCallback_;

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

static void humidity_callback_wrapper_humidity(Humidity *humidity, Packet *packet) {
	HumidityCallbackFunction callback_function = (HumidityCallbackFunction)humidity->registered_callbacks[HUMIDITY_CALLBACK_HUMIDITY];
	void *user_data = humidity->registered_callback_user_data[HUMIDITY_CALLBACK_HUMIDITY];
	HumidityCallback_ *callback = (HumidityCallback_ *)packet;

	callback->humidity = leconvert_uint16_from(callback->humidity);

	if (callback_function != NULL) {
		callback_function(callback->humidity, user_data);
	}
}

static void humidity_callback_wrapper_analog_value(Humidity *humidity, Packet *packet) {
	AnalogValueCallbackFunction callback_function = (AnalogValueCallbackFunction)humidity->registered_callbacks[HUMIDITY_CALLBACK_ANALOG_VALUE];
	void *user_data = humidity->registered_callback_user_data[HUMIDITY_CALLBACK_ANALOG_VALUE];
	AnalogValueCallback_ *callback = (AnalogValueCallback_ *)packet;

	callback->value = leconvert_uint16_from(callback->value);

	if (callback_function != NULL) {
		callback_function(callback->value, user_data);
	}
}

static void humidity_callback_wrapper_humidity_reached(Humidity *humidity, Packet *packet) {
	HumidityReachedCallbackFunction callback_function = (HumidityReachedCallbackFunction)humidity->registered_callbacks[HUMIDITY_CALLBACK_HUMIDITY_REACHED];
	void *user_data = humidity->registered_callback_user_data[HUMIDITY_CALLBACK_HUMIDITY_REACHED];
	HumidityReachedCallback_ *callback = (HumidityReachedCallback_ *)packet;

	callback->humidity = leconvert_uint16_from(callback->humidity);

	if (callback_function != NULL) {
		callback_function(callback->humidity, user_data);
	}
}

static void humidity_callback_wrapper_analog_value_reached(Humidity *humidity, Packet *packet) {
	AnalogValueReachedCallbackFunction callback_function = (AnalogValueReachedCallbackFunction)humidity->registered_callbacks[HUMIDITY_CALLBACK_ANALOG_VALUE_REACHED];
	void *user_data = humidity->registered_callback_user_data[HUMIDITY_CALLBACK_ANALOG_VALUE_REACHED];
	AnalogValueReachedCallback_ *callback = (AnalogValueReachedCallback_ *)packet;

	callback->value = leconvert_uint16_from(callback->value);

	if (callback_function != NULL) {
		callback_function(callback->value, user_data);
	}
}

void humidity_create(Humidity *humidity, const char *uid, IPConnection *ipcon) {
	device_create(humidity, uid, ipcon);

	humidity->api_version[0] = 1;
	humidity->api_version[1] = 0;
	humidity->api_version[2] = 0;

	humidity->response_expected[HUMIDITY_FUNCTION_GET_HUMIDITY] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	humidity->response_expected[HUMIDITY_FUNCTION_GET_ANALOG_VALUE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	humidity->response_expected[HUMIDITY_FUNCTION_SET_HUMIDITY_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	humidity->response_expected[HUMIDITY_FUNCTION_GET_HUMIDITY_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	humidity->response_expected[HUMIDITY_FUNCTION_SET_ANALOG_VALUE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	humidity->response_expected[HUMIDITY_FUNCTION_GET_ANALOG_VALUE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	humidity->response_expected[HUMIDITY_FUNCTION_SET_HUMIDITY_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	humidity->response_expected[HUMIDITY_FUNCTION_GET_HUMIDITY_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	humidity->response_expected[HUMIDITY_FUNCTION_SET_ANALOG_VALUE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	humidity->response_expected[HUMIDITY_FUNCTION_GET_ANALOG_VALUE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	humidity->response_expected[HUMIDITY_FUNCTION_SET_DEBOUNCE_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	humidity->response_expected[HUMIDITY_FUNCTION_GET_DEBOUNCE_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	humidity->response_expected[HUMIDITY_CALLBACK_HUMIDITY] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	humidity->response_expected[HUMIDITY_CALLBACK_ANALOG_VALUE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	humidity->response_expected[HUMIDITY_CALLBACK_HUMIDITY_REACHED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	humidity->response_expected[HUMIDITY_CALLBACK_ANALOG_VALUE_REACHED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	humidity->response_expected[HUMIDITY_FUNCTION_GET_IDENTITY] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;

	humidity->callback_wrappers[HUMIDITY_CALLBACK_HUMIDITY] = (void *)humidity_callback_wrapper_humidity;
	humidity->callback_wrappers[HUMIDITY_CALLBACK_ANALOG_VALUE] = (void *)humidity_callback_wrapper_analog_value;
	humidity->callback_wrappers[HUMIDITY_CALLBACK_HUMIDITY_REACHED] = (void *)humidity_callback_wrapper_humidity_reached;
	humidity->callback_wrappers[HUMIDITY_CALLBACK_ANALOG_VALUE_REACHED] = (void *)humidity_callback_wrapper_analog_value_reached;
}

void humidity_destroy(Humidity *humidity) {
	device_destroy(humidity);
}


int humidity_get_response_expected(Humidity *humidity, uint8_t function_id) {
	return device_get_response_expected(humidity, function_id);
}

void humidity_set_response_expected(Humidity *humidity, uint8_t function_id, bool response_expected) {
	device_set_response_expected(humidity, function_id, response_expected);
}

void humidity_set_response_expected_all(Humidity *humidity, bool response_expected) {
	device_set_response_expected_all(humidity, response_expected);
}

void humidity_register_callback(Humidity *humidity, uint8_t id, void *callback, void *user_data) {
	humidity->registered_callbacks[id] = callback;
	humidity->registered_callback_user_data[id] = user_data;
}

int humidity_get_api_version(Humidity *humidity, uint8_t ret_api_version[3]) {
	ret_api_version[0] = humidity->api_version[0];
	ret_api_version[1] = humidity->api_version[1];
	ret_api_version[2] = humidity->api_version[2];

	return E_OK;
}

int humidity_get_humidity(Humidity *humidity, uint16_t *ret_humidity) {
	GetHumidity_ request;
	GetHumidityResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_HUMIDITY, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetHumidityResponse_ *)&humidity->response_packet;
	*ret_humidity = leconvert_uint16_from(response->humidity);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_get_analog_value(Humidity *humidity, uint16_t *ret_value) {
	GetAnalogValue_ request;
	GetAnalogValueResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_ANALOG_VALUE, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetAnalogValueResponse_ *)&humidity->response_packet;
	*ret_value = leconvert_uint16_from(response->value);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_set_humidity_callback_period(Humidity *humidity, uint32_t period) {
	SetHumidityCallbackPeriod_ request;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_SET_HUMIDITY_CALLBACK_PERIOD, humidity->ipcon, humidity);

	request.period = leconvert_uint32_to(period);

	ret = device_send_request(humidity, (Packet *)&request);

	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_get_humidity_callback_period(Humidity *humidity, uint32_t *ret_period) {
	GetHumidityCallbackPeriod_ request;
	GetHumidityCallbackPeriodResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_HUMIDITY_CALLBACK_PERIOD, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetHumidityCallbackPeriodResponse_ *)&humidity->response_packet;
	*ret_period = leconvert_uint32_from(response->period);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_set_analog_value_callback_period(Humidity *humidity, uint32_t period) {
	SetAnalogValueCallbackPeriod_ request;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_SET_ANALOG_VALUE_CALLBACK_PERIOD, humidity->ipcon, humidity);

	request.period = leconvert_uint32_to(period);

	ret = device_send_request(humidity, (Packet *)&request);

	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_get_analog_value_callback_period(Humidity *humidity, uint32_t *ret_period) {
	GetAnalogValueCallbackPeriod_ request;
	GetAnalogValueCallbackPeriodResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_ANALOG_VALUE_CALLBACK_PERIOD, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetAnalogValueCallbackPeriodResponse_ *)&humidity->response_packet;
	*ret_period = leconvert_uint32_from(response->period);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_set_humidity_callback_threshold(Humidity *humidity, char option, int16_t min, int16_t max) {
	SetHumidityCallbackThreshold_ request;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_SET_HUMIDITY_CALLBACK_THRESHOLD, humidity->ipcon, humidity);

	request.option = option;
	request.min = leconvert_int16_to(min);
	request.max = leconvert_int16_to(max);

	ret = device_send_request(humidity, (Packet *)&request);

	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_get_humidity_callback_threshold(Humidity *humidity, char *ret_option, int16_t *ret_min, int16_t *ret_max) {
	GetHumidityCallbackThreshold_ request;
	GetHumidityCallbackThresholdResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_HUMIDITY_CALLBACK_THRESHOLD, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetHumidityCallbackThresholdResponse_ *)&humidity->response_packet;
	*ret_option = response->option;
	*ret_min = leconvert_int16_from(response->min);
	*ret_max = leconvert_int16_from(response->max);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_set_analog_value_callback_threshold(Humidity *humidity, char option, uint16_t min, uint16_t max) {
	SetAnalogValueCallbackThreshold_ request;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_SET_ANALOG_VALUE_CALLBACK_THRESHOLD, humidity->ipcon, humidity);

	request.option = option;
	request.min = leconvert_uint16_to(min);
	request.max = leconvert_uint16_to(max);

	ret = device_send_request(humidity, (Packet *)&request);

	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_get_analog_value_callback_threshold(Humidity *humidity, char *ret_option, uint16_t *ret_min, uint16_t *ret_max) {
	GetAnalogValueCallbackThreshold_ request;
	GetAnalogValueCallbackThresholdResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_ANALOG_VALUE_CALLBACK_THRESHOLD, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetAnalogValueCallbackThresholdResponse_ *)&humidity->response_packet;
	*ret_option = response->option;
	*ret_min = leconvert_uint16_from(response->min);
	*ret_max = leconvert_uint16_from(response->max);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_set_debounce_period(Humidity *humidity, uint32_t debounce) {
	SetDebouncePeriod_ request;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_SET_DEBOUNCE_PERIOD, humidity->ipcon, humidity);

	request.debounce = leconvert_uint32_to(debounce);

	ret = device_send_request(humidity, (Packet *)&request);

	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_get_debounce_period(Humidity *humidity, uint32_t *ret_debounce) {
	GetDebouncePeriod_ request;
	GetDebouncePeriodResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_DEBOUNCE_PERIOD, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetDebouncePeriodResponse_ *)&humidity->response_packet;
	*ret_debounce = leconvert_uint32_from(response->debounce);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}

int humidity_get_identity(Humidity *humidity, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
	GetIdentity_ request;
	GetIdentityResponse_ *response;
	int ret;

	mutex_lock(&humidity->request_mutex);

	packet_header_create(&request.header, sizeof(request), HUMIDITY_FUNCTION_GET_IDENTITY, humidity->ipcon, humidity);


	ret = device_send_request(humidity, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&humidity->request_mutex);

		return ret;
	}

	response = (GetIdentityResponse_ *)&humidity->response_packet;
	strncpy(ret_uid, response->uid, 8);
	strncpy(ret_connected_uid, response->connected_uid, 8);
	*ret_position = response->position;
	memcpy(ret_hardware_version, response->hardware_version, 3 * sizeof(uint8_t));
	memcpy(ret_firmware_version, response->firmware_version, 3 * sizeof(uint8_t));
	*ret_device_identifier = leconvert_uint16_from(response->device_identifier);


	mutex_unlock(&humidity->request_mutex);

	return ret;
}
