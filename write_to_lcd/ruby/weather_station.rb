#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_lcd_20x4'
require 'tinkerforge/bricklet_ambient_light'
require 'tinkerforge/bricklet_ambient_light_v2'
require 'tinkerforge/bricklet_ambient_light_v3'
require 'tinkerforge/bricklet_humidity'
require 'tinkerforge/bricklet_humidity_v2'
require 'tinkerforge/bricklet_barometer'
require 'tinkerforge/bricklet_barometer_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223

lcd = nil
ambient_light = nil
ambient_light_v2 = nil
ambient_light_v3 = nil
humidity = nil
humidity_v2 = nil
barometer = nil
barometer_v2 = nil

ipcon = IPConnection.new
while true
  begin
    ipcon.connect HOST, PORT
    break
  rescue Exception => e
    puts 'Connection Error: ' + e
    sleep 1
  end
end

ipcon.register_callback(IPConnection::CALLBACK_ENUMERATE) do |uid, connected_uid, position,
                                                              hardware_version, firmware_version,
                                                              device_identifier, enumeration_type|
  if enumeration_type == IPConnection::ENUMERATION_TYPE_CONNECTED or
     enumeration_type == IPConnection::ENUMERATION_TYPE_AVAILABLE
    if device_identifier == BrickletLCD20x4::DEVICE_IDENTIFIER
      begin
        lcd = BrickletLCD20x4.new uid, ipcon
        lcd.clear_display
        lcd.backlight_on
        puts 'LCD 20x4 initialized'
      rescue Exception => e
        lcd = nil
        puts 'LCD 20x4 init failed: ' + e
      end
    elsif device_identifier == BrickletAmbientLight::DEVICE_IDENTIFIER
      begin
        ambient_light = BrickletAmbientLight.new uid, ipcon
        ambient_light.set_illuminance_callback_period 1000
        ambient_light.register_callback(BrickletAmbientLight::CALLBACK_ILLUMINANCE) do |illuminance|
          if lcd != nil
            text = 'Illuminanc %6.2f lx' % (illuminance/10.0)
            lcd.write_line 0, 0, text
            puts "Write to line 0: #{text}"
          end
        end
        puts 'Ambient Light initialized'
      rescue Exception => e
        ambient_light = nil
        puts 'Ambient Light init failed: ' + e
      end
    elsif device_identifier == BrickletAmbientLightV2::DEVICE_IDENTIFIER
      begin
        ambient_light_v2 = BrickletAmbientLightV2.new uid, ipcon
        ambient_light_v2.set_configuration(BrickletAmbientLightV2::ILLUMINANCE_RANGE_64000LUX,
                                           BrickletAmbientLightV2::INTEGRATION_TIME_200MS)
        ambient_light_v2.set_illuminance_callback_period 1000
        ambient_light_v2.register_callback(BrickletAmbientLightV2::CALLBACK_ILLUMINANCE) do |illuminance|
          if lcd != nil
            text = 'Illumina %8.2f lx' % (illuminance/100.0)
            lcd.write_line 0, 0, text
            puts "Write to line 0: #{text}"
          end
        end
        puts 'Ambient Light 2.0 initialized'
      rescue Exception => e
        ambient_light = nil
        puts 'Ambient Light 2.0 init failed: ' + e
      end
    elsif device_identifier == BrickletAmbientLightV3::DEVICE_IDENTIFIER
      begin
        ambient_light_v3 = BrickletAmbientLightV3.new uid, ipcon
        ambient_light_v3.set_configuration(BrickletAmbientLightV3::ILLUMINANCE_RANGE_64000LUX,
                                           BrickletAmbientLightV3::INTEGRATION_TIME_200MS)
        ambient_light_v3.set_illuminance_callback_configuration 1000, false, 'x', 0, 0
        ambient_light_v3.register_callback(BrickletAmbientLightV3::CALLBACK_ILLUMINANCE) do |illuminance|
          if lcd != nil
            text = 'Illumina %8.2f lx' % (illuminance/100.0)
            lcd.write_line 0, 0, text
            puts "Write to line 0: #{text}"
          end
        end
        puts 'Ambient Light 3.0 initialized'
      rescue Exception => e
        ambient_light = nil
        puts 'Ambient Light 3.0 init failed: ' + e
      end
    elsif device_identifier == BrickletHumidity::DEVICE_IDENTIFIER
      begin
        humidity = BrickletHumidity.new uid, ipcon
        humidity.set_humidity_callback_period 1000
        humidity.register_callback(BrickletHumidity::CALLBACK_HUMIDITY) do |relative_humidity|
          if lcd != nil
            text = 'Humidity   %6.2f %%' % (relative_humidity/10.0)
            lcd.write_line 1, 0, text
            puts "Write to line 1: #{text}"
          end
        end
        puts 'Humidity initialized'
      rescue Exception => e
        humidity = nil
        puts 'Humidity init failed: ' + e
      end
    elsif device_identifier == BrickletHumidityV2::DEVICE_IDENTIFIER
      begin
        humidity_v2 = BrickletHumidityV2.new uid, ipcon
        humidity_v2.set_humidity_callback_configuration 1000, true, 'x', 0, 0
        humidity_v2.register_callback(BrickletHumidityV2::CALLBACK_HUMIDITY) do |relative_humidity|
          if lcd != nil
            text = 'Humidity   %6.2f %%' % (relative_humidity/100.0)
            lcd.write_line 1, 0, text
            puts "Write to line 1: #{text}"
          end
        end
        puts 'Humidity 2.0 initialized'
      rescue Exception => e
        humidity_v2 = nil
        puts 'Humidity 2.0 init failed: ' + e
      end
    elsif device_identifier == BrickletBarometer::DEVICE_IDENTIFIER
      begin
        barometer = BrickletBarometer.new uid, ipcon
        barometer.set_air_pressure_callback_period 1000
        barometer.register_callback(BrickletBarometer::CALLBACK_AIR_PRESSURE) do |air_pressure|
          if lcd != nil
            text = 'Air Press %7.2f mb' % (air_pressure/1000.0)
            lcd.write_line 2, 0, text
            puts "Write to line 2: #{text}"

            begin
              temperature = barometer.get_chip_temperature
            rescue Exception => e
              puts 'Could not get temperature: ' + e
              return
            end

            # 0xDF == ° on LCD 20x4 charset
            text = 'Temperature %5.2f %sC' % [(temperature/100.0), 0xDF.chr]
            lcd.write_line 3, 0, text
            puts "Write to line 3: #{text.sub(0xDF.chr, '°')}"
          end
        end
        puts 'Barometer initialized'
      rescue Exception => e
        barometer = nil
        puts 'Barometer init failed: ' + e
      end
    elsif device_identifier == BrickletBarometerV2::DEVICE_IDENTIFIER
      begin
        barometer_v2 = BrickletBarometerV2.new uid, ipcon
        barometer_v2.set_air_pressure_callback_configuration 1000, false, 'x', 0, 0
        barometer_v2.register_callback(BrickletBarometerV2::CALLBACK_AIR_PRESSURE) do |air_pressure|
          if lcd != nil
            text = 'Air Press %7.2f mb' % (air_pressure/1000.0)
            lcd.write_line 2, 0, text
            puts "Write to line 2: #{text}"

            begin
              temperature = barometer_v2.get_temperature
            rescue Exception => e
              puts 'Could not get temperature: ' + e
              return
            end

            # 0xDF == ° on LCD 20x4 charset
            text = 'Temperature %5.2f %sC' % [(temperature/100.0), 0xDF.chr]
            lcd.write_line 3, 0, text
            puts "Write to line 3: #{text.sub(0xDF.chr, '°')}"
          end
        end
        puts 'Barometer 2.0 initialized'
      rescue Exception => e
        barometer_v2 = nil
        puts 'Barometer 2.0 init failed: ' + e
      end
    end
  end
end

ipcon.register_callback(IPConnection::CALLBACK_CONNECTED) do |connected_reason|
  if connected_reason == IPConnection::CONNECT_REASON_AUTO_RECONNECT
    puts 'Auto Reconnect'
    while true
      begin
        ipcon.enumerate
        break
      rescue Exception => e
        puts 'Enumerate Error: ' + e
        sleep 1
      end
    end
  end
end

while true
  begin
    ipcon.enumerate
    break
  rescue Exception => e
    puts 'Enumerate Error: ' + e
    sleep 1
  end
end

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
