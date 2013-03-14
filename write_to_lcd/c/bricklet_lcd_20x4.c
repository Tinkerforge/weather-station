/* ***********************************************************
 * This file was automatically generated on 2012-12-14.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/


#define IPCON_EXPOSE_INTERNALS

#include "bricklet_lcd_20x4.h"

#include <string.h>



typedef void (*ButtonPressedCallbackFunction)(uint8_t, void *);

typedef void (*ButtonReleasedCallbackFunction)(uint8_t, void *);

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
	uint8_t line;
	uint8_t position;
	char text[20];
} ATTRIBUTE_PACKED WriteLine_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED ClearDisplay_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED BacklightOn_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED BacklightOff_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED IsBacklightOn_;

typedef struct {
	PacketHeader header;
	bool backlight;
} ATTRIBUTE_PACKED IsBacklightOnResponse_;

typedef struct {
	PacketHeader header;
	bool cursor;
	bool blinking;
} ATTRIBUTE_PACKED SetConfig_;

typedef struct {
	PacketHeader header;
} ATTRIBUTE_PACKED GetConfig_;

typedef struct {
	PacketHeader header;
	bool cursor;
	bool blinking;
} ATTRIBUTE_PACKED GetConfigResponse_;

typedef struct {
	PacketHeader header;
	uint8_t button;
} ATTRIBUTE_PACKED IsButtonPressed_;

typedef struct {
	PacketHeader header;
	bool pressed;
} ATTRIBUTE_PACKED IsButtonPressedResponse_;

typedef struct {
	PacketHeader header;
	uint8_t button;
} ATTRIBUTE_PACKED ButtonPressedCallback_;

typedef struct {
	PacketHeader header;
	uint8_t button;
} ATTRIBUTE_PACKED ButtonReleasedCallback_;

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

static void lcd_20x4_callback_wrapper_button_pressed(LCD20x4 *lcd_20x4, Packet *packet) {
	ButtonPressedCallbackFunction callback_function = (ButtonPressedCallbackFunction)lcd_20x4->registered_callbacks[LCD_20X4_CALLBACK_BUTTON_PRESSED];
	void *user_data = lcd_20x4->registered_callback_user_data[LCD_20X4_CALLBACK_BUTTON_PRESSED];
	ButtonPressedCallback_ *callback = (ButtonPressedCallback_ *)packet;

	if (callback_function != NULL) {
		callback_function(callback->button, user_data);
	}
}

static void lcd_20x4_callback_wrapper_button_released(LCD20x4 *lcd_20x4, Packet *packet) {
	ButtonReleasedCallbackFunction callback_function = (ButtonReleasedCallbackFunction)lcd_20x4->registered_callbacks[LCD_20X4_CALLBACK_BUTTON_RELEASED];
	void *user_data = lcd_20x4->registered_callback_user_data[LCD_20X4_CALLBACK_BUTTON_RELEASED];
	ButtonReleasedCallback_ *callback = (ButtonReleasedCallback_ *)packet;

	if (callback_function != NULL) {
		callback_function(callback->button, user_data);
	}
}

void lcd_20x4_create(LCD20x4 *lcd_20x4, const char *uid, IPConnection *ipcon) {
	device_create(lcd_20x4, uid, ipcon);

	lcd_20x4->api_version[0] = 1;
	lcd_20x4->api_version[1] = 0;
	lcd_20x4->api_version[2] = 0;

	lcd_20x4->response_expected[LCD_20X4_FUNCTION_WRITE_LINE] = DEVICE_RESPONSE_EXPECTED_FALSE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_CLEAR_DISPLAY] = DEVICE_RESPONSE_EXPECTED_FALSE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_BACKLIGHT_ON] = DEVICE_RESPONSE_EXPECTED_FALSE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_BACKLIGHT_OFF] = DEVICE_RESPONSE_EXPECTED_FALSE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_IS_BACKLIGHT_ON] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_SET_CONFIG] = DEVICE_RESPONSE_EXPECTED_FALSE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_GET_CONFIG] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_IS_BUTTON_PRESSED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;
	lcd_20x4->response_expected[LCD_20X4_CALLBACK_BUTTON_PRESSED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	lcd_20x4->response_expected[LCD_20X4_CALLBACK_BUTTON_RELEASED] = DEVICE_RESPONSE_EXPECTED_ALWAYS_FALSE;
	lcd_20x4->response_expected[LCD_20X4_FUNCTION_GET_IDENTITY] = DEVICE_RESPONSE_EXPECTED_ALWAYS_TRUE;

	lcd_20x4->callback_wrappers[LCD_20X4_CALLBACK_BUTTON_PRESSED] = (void *)lcd_20x4_callback_wrapper_button_pressed;
	lcd_20x4->callback_wrappers[LCD_20X4_CALLBACK_BUTTON_RELEASED] = (void *)lcd_20x4_callback_wrapper_button_released;
}

void lcd_20x4_destroy(LCD20x4 *lcd_20x4) {
	device_destroy(lcd_20x4);
}


int lcd_20x4_get_response_expected(LCD20x4 *lcd_20x4, uint8_t function_id) {
	return device_get_response_expected(lcd_20x4, function_id);
}

void lcd_20x4_set_response_expected(LCD20x4 *lcd_20x4, uint8_t function_id, bool response_expected) {
	device_set_response_expected(lcd_20x4, function_id, response_expected);
}

void lcd_20x4_set_response_expected_all(LCD20x4 *lcd_20x4, bool response_expected) {
	device_set_response_expected_all(lcd_20x4, response_expected);
}

void lcd_20x4_register_callback(LCD20x4 *lcd_20x4, uint8_t id, void *callback, void *user_data) {
	lcd_20x4->registered_callbacks[id] = callback;
	lcd_20x4->registered_callback_user_data[id] = user_data;
}

int lcd_20x4_get_api_version(LCD20x4 *lcd_20x4, uint8_t ret_api_version[3]) {
	ret_api_version[0] = lcd_20x4->api_version[0];
	ret_api_version[1] = lcd_20x4->api_version[1];
	ret_api_version[2] = lcd_20x4->api_version[2];

	return E_OK;
}

int lcd_20x4_write_line(LCD20x4 *lcd_20x4, uint8_t line, uint8_t position, char text[20]) {
	WriteLine_ request;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_WRITE_LINE, lcd_20x4->ipcon, lcd_20x4);

	request.line = line;
	request.position = position;
	strncpy(request.text, text, 20);


	ret = device_send_request(lcd_20x4, (Packet *)&request);

	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_clear_display(LCD20x4 *lcd_20x4) {
	ClearDisplay_ request;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_CLEAR_DISPLAY, lcd_20x4->ipcon, lcd_20x4);


	ret = device_send_request(lcd_20x4, (Packet *)&request);

	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_backlight_on(LCD20x4 *lcd_20x4) {
	BacklightOn_ request;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_BACKLIGHT_ON, lcd_20x4->ipcon, lcd_20x4);


	ret = device_send_request(lcd_20x4, (Packet *)&request);

	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_backlight_off(LCD20x4 *lcd_20x4) {
	BacklightOff_ request;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_BACKLIGHT_OFF, lcd_20x4->ipcon, lcd_20x4);


	ret = device_send_request(lcd_20x4, (Packet *)&request);

	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_is_backlight_on(LCD20x4 *lcd_20x4, bool *ret_backlight) {
	IsBacklightOn_ request;
	IsBacklightOnResponse_ *response;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_IS_BACKLIGHT_ON, lcd_20x4->ipcon, lcd_20x4);


	ret = device_send_request(lcd_20x4, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&lcd_20x4->request_mutex);

		return ret;
	}

	response = (IsBacklightOnResponse_ *)&lcd_20x4->response_packet;
	*ret_backlight = response->backlight;


	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_set_config(LCD20x4 *lcd_20x4, bool cursor, bool blinking) {
	SetConfig_ request;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_SET_CONFIG, lcd_20x4->ipcon, lcd_20x4);

	request.cursor = cursor;
	request.blinking = blinking;

	ret = device_send_request(lcd_20x4, (Packet *)&request);

	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_get_config(LCD20x4 *lcd_20x4, bool *ret_cursor, bool *ret_blinking) {
	GetConfig_ request;
	GetConfigResponse_ *response;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_GET_CONFIG, lcd_20x4->ipcon, lcd_20x4);


	ret = device_send_request(lcd_20x4, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&lcd_20x4->request_mutex);

		return ret;
	}

	response = (GetConfigResponse_ *)&lcd_20x4->response_packet;
	*ret_cursor = response->cursor;
	*ret_blinking = response->blinking;


	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_is_button_pressed(LCD20x4 *lcd_20x4, uint8_t button, bool *ret_pressed) {
	IsButtonPressed_ request;
	IsButtonPressedResponse_ *response;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_IS_BUTTON_PRESSED, lcd_20x4->ipcon, lcd_20x4);

	request.button = button;

	ret = device_send_request(lcd_20x4, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&lcd_20x4->request_mutex);

		return ret;
	}

	response = (IsButtonPressedResponse_ *)&lcd_20x4->response_packet;
	*ret_pressed = response->pressed;


	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}

int lcd_20x4_get_identity(LCD20x4 *lcd_20x4, char ret_uid[8], char ret_connected_uid[8], char *ret_position, uint8_t ret_hardware_version[3], uint8_t ret_firmware_version[3], uint16_t *ret_device_identifier) {
	GetIdentity_ request;
	GetIdentityResponse_ *response;
	int ret;

	mutex_lock(&lcd_20x4->request_mutex);

	packet_header_create(&request.header, sizeof(request), LCD_20X4_FUNCTION_GET_IDENTITY, lcd_20x4->ipcon, lcd_20x4);


	ret = device_send_request(lcd_20x4, (Packet *)&request);

	if (ret < 0) {
		mutex_unlock(&lcd_20x4->request_mutex);

		return ret;
	}

	response = (GetIdentityResponse_ *)&lcd_20x4->response_packet;
	strncpy(ret_uid, response->uid, 8);
	strncpy(ret_connected_uid, response->connected_uid, 8);
	*ret_position = response->position;
	memcpy(ret_hardware_version, response->hardware_version, 3 * sizeof(uint8_t));
	memcpy(ret_firmware_version, response->firmware_version, 3 * sizeof(uint8_t));
	*ret_device_identifier = leconvert_uint16_from(response->device_identifier);


	mutex_unlock(&lcd_20x4->request_mutex);

	return ret;
}
