/* ***********************************************************
 * This file was automatically generated on 2012-12-14.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/


#define IPCON_EXPOSE_INTERNALS

#include "bricklet_ambient_light.h"

#include <string.h>



typedef void (*IlluminanceCallbackFunction)(uint16_t, void *);

typedef void (*AnalogValueCallbackFunction)(uint16_t, void *);

typedef void (*IlluminanceReachedCallbackFunction)(uint16_t, void *);

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
} ATTRIBUTE_PACKED GetIlluminance_;

typedef struct {
	PacketHeader header;
	uint16_t illuminance;
} ATTRIBUTE_PACKED GetIlluminanceResponse_;

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
} ATTRIBUTE_PACKED SetIlluminanceCallbackPeriod_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetIlluminanceCallbackPeriod_;

typedef struct {
	PacketHeader header;
	uint32_t period;
} ATTRIBUTE_PACKED GetIlluminanceCallbackPeriodResponse_;

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
} ATTRIBUTE_PACKED SetIlluminanceCallbackThreshold_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetIlluminanceCallbackThreshold_;

typedef struct {
	PacketHeader header;
	char option;
	int16_t min;
	int16_t max;
} ATTRIBUTE_PACKED GetIlluminanceCallbackThresholdResponse_;

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
	uint16_t illuminance;
} ATTRIBUTE_PACKED IlluminanceCallback_;

typedef struct {
	PacketHeader header;
	uint16_t value;
} ATTRIBUTE_PACKED AnalogValueCallback_;

typedef struct {
	PacketHeader header;
	uint16_t illuminance;
} ATTRIBUTE_PACKED IlluminanceReachedCallback_;

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

static void ambient_light_callback_wrapper_illuminance(AmbientLight *ambient_light, Packet *packet) {
	IlluminanceCallbackFunction callback_function = (IlluminanceCallbackFunction)ambient_light->registered_callbacks[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE];
	void *user_data = ambient_light->registered_callback_user_data[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE];
	IlluminanceCallback_ *callback = (IlluminanceCallback_ *)packet;

	callback->illuminance = leconvert_uint16_from(callback->illuminance);

	if (callback_function != NULL) {
		callback_function(callback->illuminance, user_data);
	}
}

static void ambient_light_callback_wrapper_analog_value(AmbientLight *ambient_light, Packet *packet) {
	AnalogValueCallbackFunction callback_function = (AnalogValueCallbackFunction)ambient_light->registered_callbacks[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE];
	void *user_data = ambient_light->registered_callback_user_data[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE];
	AnalogValueCallback_ *callback = (AnalogValueCallback_ *)packet;

	callback->value = leconvert_uint16_from(callback->value);

	if (callback_function != NULL) {
		callback_function(callback->value, user_data);
	}
}

static void ambient_light_callback_wrapper_illuminance_reached(AmbientLight *ambient_light, Packet *packet) {
	IlluminanceReachedCallbackFunction callback_function = (IlluminanceReachedCallbackFunction)ambient_light->registered_callbacks[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED];
	void *user_data = ambient_light->registered_callback_user_data[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED];
	IlluminanceReachedCallback_ *callback = (IlluminanceReachedCallback_ *)packet;

	callback->illuminance = leconvert_uint16_from(callback->illuminance);

	if (callback_function != NULL) {
		callback_function(callback->illuminance, user_data);
	}
}

static void ambient_light_callback_wrapper_analog_value_reached(AmbientLight *ambient_light, Packet *packet) {
	AnalogValueReachedCallbackFunction callback_function = (AnalogValueReachedCallbackFunction)ambient_light->registered_callbacks[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE_REACHED];
	void *user_data = ambient_light->registered_callback_user_data[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE_REACHED];
	AnalogValueReachedCallback_ *callback = (AnalogValueReachedCallback_ *)packet;

	callback->value = leconvert_uint16_from(callback->value);

	if (callback_function != NULL) {
		callback_function(callback->value, user_data);
	}
}

void ambient_light_create(AmbientLight *ambient_light, const char *uid, IPConnection *ipcon) {
	device_create(ambient_light, uid, ipcon);

	ambient_light->api_version[0] = 1;
	ambient_light->api_version[1] = 0;
	ambient_light->api_version[2] = 0;

	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_SET_ILLUMINANCE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_SET_ANALOG_VALUE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE_CALLBACK_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_SET_ILLUMINANCE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_SET_ANALOG_VALUE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE_CALLBACK_THRESHOLD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_SET_DEBOUNCE_PERIOD] = DEVICE_RESPONSE_EXPECTED_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_DEBOUNCE_PERIOD] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	ambient_light->response_expected[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE_REACHED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	ambient_light->response_expected[AMBIENT_LIGHT_FUNCTION_GET_IDENTITY] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;

	ambient_light->callback_wrappers[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE] = (void *)ambient_light_callback_wrapper_illuminance;
	ambient_light->callback_wrappers[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE] = (void *)ambient_light_callback_wrapper_analog_value;
	ambient_light->callback_wrappers[AMBIENT_LIGHT_CALLBACK_ILLUMINANCE_REACHED] = (void *)ambient_light_callback_wrapper_illuminance_reached;
	ambient_light->callback_wrappers[AMBIENT_LIGHT_CALLBACK_ANALOG_VALUE_REACHED] = (void *)ambient_light_callback_wrapper_analog_value_reached;
}

void ambient_light_destroy(AmbientLight *ambient_light) {
	device_destroy(ambient_light);
}


int ambient_light_get_response_expected(AmbientLight *ambient_light, uint8_t function_id) {
	return device_get_response_expected(ambient_light, function_id);
}

void ambient_light_set_response_expected(AmbientLight *ambient_light, uint8_t function_id, bool response_expected) {
	device_set_response_expected(ambient_light, function_id, response_expected);
}

void ambient_light_set_response_expected_all(AmbientLight *ambient_light, bool response_expected) {
	device_set_response_expected_all(ambient_light, response_expected);
}

void ambient_light_register_callback(AmbientLight *ambient_light, uint8_t id, void *callback, void *user_data) {
	ambient_light->registered_callbacks[id] = callback;
	ambient_light->registered_callback_user_data[id] = user_data;
}

int ambient_light_get_api_version(AmbientLight *ambient_light, uint8_t ret_api_version[3]) {
	ret_api_version[0] = ambient_light->api_version[0];
	ret_api_version[1] = ambient_light->api_version[1];
	ret_api_version[2] = ambient_light->api_version[2];

	return E_OK;
}

int ambient_light_get_illuminance(AmbientLight *ambient_light, uint16_t *ret_illuminance) {
	GetIlluminance_ request;
	GetIlluminanceResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetIlluminanceResponse_ *)&ambient_light->response_packet;
	*ret_illuminance = leconvert_uint16_from(response->illuminance);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_get_analog_value(AmbientLight *ambient_light, uint16_t *ret_value) {
	GetAnalogValue_ request;
	GetAnalogValueResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetAnalogValueResponse_ *)&ambient_light->response_packet;
	*ret_value = leconvert_uint16_from(response->value);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_set_illuminance_callback_period(AmbientLight *ambient_light, uint32_t period) {
	SetIlluminanceCallbackPeriod_ request;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_SET_ILLUMINANCE_CALLBACK_PERIOD, ambient_light->ipcon, ambient_light);

	request.period = leconvert_uint32_to(period);

	ret = device_send_request(ambient_light, (Packet *)&request);

	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_get_illuminance_callback_period(AmbientLight *ambient_light, uint32_t *ret_period) {
	GetIlluminanceCallbackPeriod_ request;
	GetIlluminanceCallbackPeriodResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE_CALLBACK_PERIOD, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetIlluminanceCallbackPeriodResponse_ *)&ambient_light->response_packet;
	*ret_period = leconvert_uint32_from(response->period);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_set_analog_value_callback_period(AmbientLight *ambient_light, uint32_t period) {
	SetAnalogValueCallbackPeriod_ request;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_SET_ANALOG_VALUE_CALLBACK_PERIOD, ambient_light->ipcon, ambient_light);

	request.period = leconvert_uint32_to(period);

	ret = device_send_request(ambient_light, (Packet *)&request);

	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_get_analog_value_callback_period(AmbientLight *ambient_light, uint32_t *ret_period) {
	GetAnalogValueCallbackPeriod_ request;
	GetAnalogValueCallbackPeriodResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE_CALLBACK_PERIOD, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetAnalogValueCallbackPeriodResponse_ *)&ambient_light->response_packet;
	*ret_period = leconvert_uint32_from(response->period);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_set_illuminance_callback_threshold(AmbientLight *ambient_light, char option, int16_t min, int16_t max) {
	SetIlluminanceCallbackThreshold_ request;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_SET_ILLUMINANCE_CALLBACK_THRESHOLD, ambient_light->ipcon, ambient_light);

	request.option = option;
	request.min = leconvert_int16_to(min);
	request.max = leconvert_int16_to(max);

	ret = device_send_request(ambient_light, (Packet *)&request);

	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_get_illuminance_callback_threshold(AmbientLight *ambient_light, char *ret_option, int16_t *ret_min, int16_t *ret_max) {
	GetIlluminanceCallbackThreshold_ request;
	GetIlluminanceCallbackThresholdResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_ILLUMINANCE_CALLBACK_THRESHOLD, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetIlluminanceCallbackThresholdResponse_ *)&ambient_light->response_packet;
	*ret_option = response->option;
	*ret_min = leconvert_int16_from(response->min);
	*ret_max = leconvert_int16_from(response->max);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_set_analog_value_callback_threshold(AmbientLight *ambient_light, char option, uint16_t min, uint16_t max) {
	SetAnalogValueCallbackThreshold_ request;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_SET_ANALOG_VALUE_CALLBACK_THRESHOLD, ambient_light->ipcon, ambient_light);

	request.option = option;
	request.min = leconvert_uint16_to(min);
	request.max = leconvert_uint16_to(max);

	ret = device_send_request(ambient_light, (Packet *)&request);

	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_get_analog_value_callback_threshold(AmbientLight *ambient_light, char *ret_option, uint16_t *ret_min, uint16_t *ret_max) {
	GetAnalogValueCallbackThreshold_ request;
	GetAnalogValueCallbackThresholdResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_ANALOG_VALUE_CALLBACK_THRESHOLD, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetAnalogValueCallbackThresholdResponse_ *)&ambient_light->response_packet;
	*ret_option = response->option;
	*ret_min = leconvert_uint16_from(response->min);
	*ret_max = leconvert_uint16_from(response->max);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_set_debounce_period(AmbientLight *ambient_light, uint32_t debounce) {
	SetDebouncePeriod_ request;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_SET_DEBOUNCE_PERIOD, ambient_light->ipcon, ambient_light);

	request.debounce = leconvert_uint32_to(debounce);

	ret = device_send_request(ambient_light, (Packet *)&request);

	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_get_debounce_period(AmbientLight *ambient_light, uint32_t *ret_debounce) {
	GetDebouncePeriod_ request;
	GetDebouncePeriodResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_DEBOUNCE_PERIOD, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetDebouncePeriodResponse_ *)&ambient_light->response_packet;
	*ret_debounce = leconvert_uint32_from(response->debounce);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}

int ambient_light_get_identity(AmbientLight *ambient_light, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
	GetIdentity_ request;
	GetIdentityResponse_ *response;
	int ret;

	mutex_lock(&ambient_light->request_mutex);

	packet_header_create(&request.header, sizeof(request), AMBIENT_LIGHT_FUNCTION_GET_IDENTITY, ambient_light->ipcon, ambient_light);


	ret = device_send_request(ambient_light, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&ambient_light->request_mutex);

		return ret;
	}

	response = (GetIdentityResponse_ *)&ambient_light->response_packet;
	strncpy(ret_uid, response->uid, 8);
	strncpy(ret_connected_uid, response->connected_uid, 8);
	*ret_position = response->position;
	memcpy(ret_hardware_version, response->hardware_version, 3 * sizeof(uint8_t));
	memcpy(ret_firmware_version, response->firmware_version, 3 * sizeof(uint8_t));
	*ret_device_identifier = leconvert_uint16_from(response->device_identifier);


	mutex_unlock(&ambient_light->request_mutex);

	return ret;
}
