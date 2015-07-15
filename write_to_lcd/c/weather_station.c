#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_lcd_20x4.h"
#include "bricklet_humidity.h"
#include "bricklet_barometer.h"
#include "bricklet_ambient_light.h"
#include "bricklet_ambient_light_v2.h"

#define HOST "localhost"
#define PORT 4223

typedef struct {
	IPConnection ipcon;
	LCD20x4 lcd;
	bool lcd_created;
	AmbientLight ambient_light;
	AmbientLightV2 ambient_light_v2;
	Humidity humidity;
	Barometer barometer;
} WeatherStation;

void cb_illuminance(uint16_t illuminance, void *user_data) {
	WeatherStation *ws = (WeatherStation *)user_data;

	if(ws->lcd_created) {
		char text[30] = {'\0'};
		sprintf(text, "Illuminanc %6.2f lx", illuminance/10.0);
		lcd_20x4_write_line(&ws->lcd, 0, 0, text);
		printf("Write to line 0: %s\n", text);
	}
}

void cb_illuminance_v2(uint32_t illuminance, void *user_data) {
	WeatherStation *ws = (WeatherStation *)user_data;

	if(ws->lcd_created) {
		char text[30] = {'\0'};
		sprintf(text, "Illuminanc %6.2f lx", illuminance/100.0);
		lcd_20x4_write_line(&ws->lcd, 0, 0, text);
		printf("Write to line 0: %s\n", text);
	}
}

void cb_humidity(uint16_t humidity, void *user_data) {
	WeatherStation *ws = (WeatherStation *)user_data;

	if(ws->lcd_created) {
		char text[30] = {'\0'};
		sprintf(text, "Humidity   %6.2f %%", humidity/10.0);
		lcd_20x4_write_line(&ws->lcd, 1, 0, text);
		printf("Write to line 1: %s\n", text);
	}
}

void cb_air_pressure(int32_t air_pressure, void *user_data) {
	WeatherStation *ws = (WeatherStation *)user_data;

	if(ws->lcd_created) {
		char text[30] = {'\0'};
		sprintf(text, "Air Press %7.2f mb", air_pressure/1000.0);
		lcd_20x4_write_line(&ws->lcd, 2, 0, text);
		printf("Write to line 2: %s\n", text);

		int16_t temperature;
		int rc = barometer_get_chip_temperature(&ws->barometer, &temperature);
		if(rc < 0) {
			fprintf(stderr, "Could not get temperature: %d\n", rc);
			return;
		}

		memset(text, '\0', sizeof(text));
		// 0xDF == ° on LCD 20x4 charset
		sprintf(text, "Temperature %5.2f %cC", temperature/100.0, 0xDF);
		lcd_20x4_write_line(&ws->lcd, 3, 0, text);
		sprintf(text, "Temperature %5.2f °C", temperature/100.0);
		printf("Write to line 3: %s\n", text);
	}
}

void cb_connected(uint8_t connected_reason, void *user_data) {
	WeatherStation *ws = (WeatherStation *)user_data;

	if(connected_reason == IPCON_CONNECT_REASON_AUTO_RECONNECT) {
		printf("Auto Reconnect\n");

		while(true) {
			int rc = ipcon_enumerate(&ws->ipcon);
			if(rc < 0) {
				fprintf(stderr, "Could not enumerate: %d\n", rc);
				// TODO: sleep 1s
				continue;
			}
			break;
		}
	}
}

void cb_enumerate(const char *uid, const char *connected_uid,
                  char position, uint8_t hardware_version[3],
                  uint8_t firmware_version[3], uint16_t device_identifier,
                  uint8_t enumeration_type, void *user_data) {
	WeatherStation *ws = (WeatherStation *)user_data;
	int rc;

	// avoid unused parameter warning
	(void)connected_uid; (void)position; (void)hardware_version; (void)firmware_version;

	if(enumeration_type == IPCON_ENUMERATION_TYPE_CONNECTED ||
	   enumeration_type == IPCON_ENUMERATION_TYPE_AVAILABLE) {
		if(device_identifier == LCD_20X4_DEVICE_IDENTIFIER) {
			lcd_20x4_create(&ws->lcd, uid, &ws->ipcon);
			lcd_20x4_clear_display(&ws->lcd);
			lcd_20x4_backlight_on(&ws->lcd);
			ws->lcd_created = true;
			printf("LCD 20x4 initialized\n");
		} else if(device_identifier == AMBIENT_LIGHT_DEVICE_IDENTIFIER) {
			ambient_light_create(&ws->ambient_light, uid, &ws->ipcon);
			ambient_light_register_callback(&ws->ambient_light,
			                                AMBIENT_LIGHT_CALLBACK_ILLUMINANCE,
			                                (void *)cb_illuminance,
			                                (void *)ws);
			rc = ambient_light_set_illuminance_callback_period(&ws->ambient_light, 1000);

			if(rc < 0) {
				fprintf(stderr, "Ambient Light init failed: %d\n", rc);
			} else {
				printf("Ambient Light initialized\n");
			}
		} else if(device_identifier == AMBIENT_LIGHT_V2_DEVICE_IDENTIFIER) {
			ambient_light_v2_create(&ws->ambient_light_v2, uid, &ws->ipcon);
			ambient_light_v2_register_callback(&ws->ambient_light_v2,
			                                   AMBIENT_LIGHT_V2_CALLBACK_ILLUMINANCE,
			                                   (void *)cb_illuminance_v2,
			                                   (void *)ws);
			rc = ambient_light_v2_set_illuminance_callback_period(&ws->ambient_light_v2, 1000);

			if(rc < 0) {
				fprintf(stderr, "Ambient Light 2.0 init failed: %d\n", rc);
			} else {
				printf("Ambient Light 2.0 initialized\n");
			}
		} else if(device_identifier == HUMIDITY_DEVICE_IDENTIFIER) {
			humidity_create(&ws->humidity, uid, &ws->ipcon);
			humidity_register_callback(&ws->humidity,
			                           HUMIDITY_CALLBACK_HUMIDITY,
			                           (void *)cb_humidity,
			                           (void *)ws);
			rc = humidity_set_humidity_callback_period(&ws->humidity, 1000);

			if(rc < 0) {
				fprintf(stderr, "Humidity init failed: %d\n", rc);
			} else {
				printf("Humidity initialized\n");
			}
		} else if(device_identifier == BAROMETER_DEVICE_IDENTIFIER) {
			barometer_create(&ws->barometer, uid, &ws->ipcon);
			barometer_register_callback(&ws->barometer,
			                            BAROMETER_CALLBACK_AIR_PRESSURE,
			                            (void *)cb_air_pressure,
			                            (void *)ws);
			rc = barometer_set_air_pressure_callback_period(&ws->barometer, 1000);

			if(rc < 0) {
				fprintf(stderr, "Barometer init failed: %d\n", rc);
			} else {
				printf("Barometer initialized\n");
			}
		}
	}
}

int main() {
	WeatherStation ws;

	ws.lcd_created = false;
	ipcon_create(&ws.ipcon);

	while(true) {
		int rc = ipcon_connect(&ws.ipcon, HOST, PORT);
		if(rc < 0) {
			fprintf(stderr, "Could not connect to brickd: %d\n", rc);
			// TODO: sleep 1s
			continue;
		}
		break;
	}

	ipcon_register_callback(&ws.ipcon,
	                        IPCON_CALLBACK_ENUMERATE,
	                        (void *)cb_enumerate,
	                        (void *)&ws);

	ipcon_register_callback(&ws.ipcon,
	                        IPCON_CALLBACK_CONNECTED,
	                        (void *)cb_connected,
	                        (void *)&ws);

	while(true) {
		int rc = ipcon_enumerate(&ws.ipcon);
		if(rc < 0) {
			fprintf(stderr, "Could not enumerate: %d\n", rc);
			// TODO: sleep 1s
			continue;
		}
		break;
	}

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ws.ipcon);
	return 0;
}
