#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_lcd_20x4'
require 'tinkerforge/bricklet_ambient_light'
require 'tinkerforge/bricklet_humidity'
require 'tinkerforge/bricklet_barometer'

include Tinkerforge

HOST          = 'localhost'
PORT          = 4223

lcd           = nil
ambient_light = nil
humidity      = nil
barometer     = nil

ipcon = IPConnection.new
while true
  begin
    ipcon.connect HOST, PORT
  rescue Errno::ECONNREFUSED
    sleep 1
    redo
  end
  break
end

ipcon.register_callback(IPConnection::CALLBACK_ENUMERATE) do |uid, connected_uid, position,
                                                              hardware_version, firmware_version,
                                                              device_identifier, enumeration_type|
  if enumeration_type == IPConnection::ENUMERATION_TYPE_CONNECTED or
     enumeration_type == IPConnection::ENUMERATION_TYPE_AVAILABLE
    if device_identifier == BrickletLCD20x4::DEVICE_IDENTIFIER
	  lcd = BrickletLCD20x4.new uid, ipcon
	  lcd.clear_display
	  lcd.backlight_on
	end
    if device_identifier == BrickletAmbientLight::DEVICE_IDENTIFIER
	  ambient_light = BrickletAmbientLight.new uid, ipcon
	  ambient_light.set_illuminance_callback_period 1000
	  ambient_light.register_callback(BrickletAmbientLight::CALLBACK_ILLUMINANCE) do |illuminance|
	    text = "Illuminanc %6.2f lx" % (illuminance/10.0)
		if lcd != nil
          lcd.write_line 0, 0, text
		end
		puts "Write to line 0: #{text}"
      end
	end
    if device_identifier == BrickletHumidity::DEVICE_IDENTIFIER
	  humidity = BrickletHumidity.new uid, ipcon
	  humidity.set_humidity_callback_period 1000
	  humidity.register_callback(BrickletHumidity::CALLBACK_HUMIDITY) do |humidity|
        text = "Humidity   %6.2f %%" % (humidity/10.0)
		if lcd != nil
          lcd.write_line 1, 0, text
		end
		puts "Write to line 1: #{text}"
      end
	end
    if device_identifier == BrickletBarometer::DEVICE_IDENTIFIER
	  barometer = BrickletBarometer.new uid, ipcon
	  barometer.set_air_pressure_callback_period 1000
	  barometer.register_callback(BrickletBarometer::CALLBACK_AIR_PRESSURE) do |air_pressure|
        text = "Air Press %7.2f mg" % (air_pressure/1000.0)
		if lcd != nil
          lcd.write_line 2, 0, text
		end
		puts "Write to line 2: #{text}"
	    temperature = barometer.get_chip_temperature

        text = "Temperature %2.2f %sC" % [(temperature/100.0), 0xDF.chr]
		if lcd != nil
          lcd.write_line 3, 0, text
		end
		puts "Write to line 3: #{text}"
      end
	end
  end
end

ipcon.register_callback(IPConnection::CALLBACK_CONNECTED) do |connected_reason|
  if connected_reason == IPConnection::CONNECT_REASON_AUTO_RECONNECT
    ipcon.enumerate
  end
end

ipcon.enumerate

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
