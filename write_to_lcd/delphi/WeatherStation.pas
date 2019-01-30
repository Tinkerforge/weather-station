program WeatherStation;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  Device,
  SysUtils,
  IPConnection,
  BrickletLCD20x4,
  BrickletHumidity,
  BrickletBarometer,
  BrickletHumidityV2,
  BrickletBarometerV2,
  BrickletAmbientLight,
  BrickletAmbientLightV2,
  BrickletAmbientLightV3;

const
  HOST = 'localhost';
  PORT = 4223;

type
  TWeatherStation = class

  private
    ipcon: TIPConnection;
    brickletLCD: TBrickletLCD20x4;
    brickletHumidity: TBrickletHumidity;
    brickletBarometer: TBrickletBarometer;
    brickletHumidityV2: TBrickletHumidityV2;
    brickletBarometerV2: TBrickletBarometerV2;
    brickletAmbientLight: TBrickletAmbientLight;
    brickletAmbientLightV2: TBrickletAmbientLightV2;
    brickletAmbientLightV3: TBrickletAmbientLightV3;

  public
    procedure Execute;
    constructor Create;
    destructor Destroy; override;
    procedure EnumerateCB(sender: TIPConnection;
                          const uid: string;
                          const connectedUid: string;
                          const position: char;
                          const hardwareVersion: TVersionNumber;
                          const firmwareVersion: TVersionNumber;
                          const deviceIdentifier: word;
                          const enumerationType: byte);
    procedure HumidityCB(sender: TBrickletHumidity; const humidity: word);
    procedure HumidityV2CB(sender: TBrickletHumidityV2; const humidity: word);
    procedure ConnectedCB(sender: TIPConnection; const connectedReason: byte);
    procedure AirPressureCB(sender: TBrickletBarometer; const airPressure: longint);
    procedure IlluminanceCB(sender: TBrickletAmbientLight; const illuminance: word);
    procedure AirPressureV2CB(sender: TBrickletBarometerV2; const airPressure: longint);
    procedure IlluminanceV2CB(sender: TBrickletAmbientLightV2; const illuminance: longword);
    procedure IlluminanceV3CB(sender: TBrickletAmbientLightV3; const illuminance: longword);
  end;

var
  ws: TWeatherStation;

constructor TWeatherStation.Create;
begin
  ipcon := nil;
  brickletLCD := nil;
  brickletHumidity := nil;
  brickletBarometer := nil;
  brickletHumidityV2 := nil;
  brickletBarometerV2 := nil;
  brickletAmbientLight := nil;
  brickletAmbientLightV2 := nil;
  brickletAmbientLightV3 := nil;
end;

destructor TWeatherStation.Destroy;
begin
  if (ipcon <> nil) then ipcon.Destroy;
  if (brickletLCD <> nil) then brickletLCD.Destroy;
  if (brickletHumidity <> nil) then brickletHumidity.Destroy;
  if (brickletBarometer <> nil) then brickletBarometer.Destroy;
  if (brickletHumidityV2 <> nil) then brickletHumidityV2.Destroy;
  if (brickletBarometerV2 <> nil) then brickletBarometerV2.Destroy;
  if (brickletAmbientLight <> nil) then brickletAmbientLight.Destroy;
  if (brickletAmbientLightV2 <> nil) then brickletAmbientLightV2.Destroy;
  if (brickletAmbientLightV3 <> nil) then brickletAmbientLightV3.Destroy;

  inherited Destroy;
end;

procedure TWeatherStation.ConnectedCB(sender: TIPConnection; const connectedReason: byte);
begin
  if (connectedReason = IPCON_CONNECT_REASON_AUTO_RECONNECT) then
  begin
    WriteLn('Auto Reconnect');

    while (true) do begin
      try
        ipcon.Enumerate;

        break;
      except
        on e: Exception do
        begin
          WriteLn('Enumeration Error: ' + e.Message);
          Sleep(1000);
        end;
      end;
    end;
  end;
end;

procedure TWeatherStation.EnumerateCB(sender: TIPConnection; const uid: string;
                                      const connectedUid: string; const position: char;
                                      const hardwareVersion: TVersionNumber;
                                      const firmwareVersion: TVersionNumber;
                                      const deviceIdentifier: word; const enumerationType: byte);
begin
  if ((enumerationType = IPCON_ENUMERATION_TYPE_CONNECTED) or
      (enumerationType = IPCON_ENUMERATION_TYPE_AVAILABLE)) then
    begin
      if (deviceIdentifier = BRICKLET_LCD_20X4_DEVICE_IDENTIFIER) then
      begin
        try
          brickletLCD := TBrickletLCD20x4.Create(UID, ipcon);
          brickletLCD.ClearDisplay;
          brickletLCD.BacklightOn;
          WriteLn('LCD 20x4 initialized');
        except
          on e: Exception do
          begin
            WriteLn('LCD 20x4 init failed: ' + e.Message);
            brickletLCD := nil;
          end;
        end;
      end
    else if (deviceIdentifier = BRICKLET_AMBIENT_LIGHT_DEVICE_IDENTIFIER) then
    begin
      try
        brickletAmbientLight := TBrickletAmbientLight.Create(uid, ipcon);
        brickletAmbientLight.SetIlluminanceCallbackPeriod(1000);
        brickletAmbientLight.OnIlluminance := {$ifdef FPC}@{$endif}IlluminanceCB;
        WriteLn('Ambient Light initialized');
      except
        on e: Exception do
        begin
          WriteLn('Ambient Light init failed: ' + e.Message);
          brickletAmbientLight := nil;
        end;
      end;
    end
    else if (deviceIdentifier = BRICKLET_AMBIENT_LIGHT_V2_DEVICE_IDENTIFIER) then
    begin
      try
        brickletAmbientLightV2 := TBrickletAmbientLightV2.Create(uid, ipcon);
        brickletAmbientLightV2.SetConfiguration(BRICKLET_AMBIENT_LIGHT_V2_ILLUMINANCE_RANGE_64000LUX,
                                                BRICKLET_AMBIENT_LIGHT_V2_INTEGRATION_TIME_200MS);
        brickletAmbientLightV2.SetIlluminanceCallbackPeriod(1000);
        brickletAmbientLightV2.OnIlluminance := {$ifdef FPC}@{$endif}IlluminanceV2CB;
        WriteLn('Ambient Light 2.0 initialized');
      except
        on e: Exception do
        begin
          WriteLn('Ambient Light 2.0 init failed: ' + e.Message);
          brickletAmbientLightV2 := nil;
        end;
      end;
    end
    else if (deviceIdentifier = BRICKLET_AMBIENT_LIGHT_V3_DEVICE_IDENTIFIER) then
    begin
      try
        brickletAmbientLightV3 := TBrickletAmbientLightV3.Create(uid, ipcon);
        brickletAmbientLightV3.SetConfiguration(BRICKLET_AMBIENT_LIGHT_V2_ILLUMINANCE_RANGE_64000LUX,
                                                BRICKLET_AMBIENT_LIGHT_V2_INTEGRATION_TIME_200MS);
        brickletAmbientLightV3.SetIlluminanceCallbackConfiguration(1000, False, 'x', 0, 0);
        brickletAmbientLightV3.OnIlluminance := {$ifdef FPC}@{$endif}IlluminanceV3CB;
        WriteLn('Ambient Light 3.0 initialized');
      except
        on e: Exception do
        begin
          WriteLn('Ambient Light 3.0 init failed: ' + e.Message);
          brickletAmbientLightV3 := nil;
        end;
      end;
    end
    else if (deviceIdentifier = BRICKLET_HUMIDITY_DEVICE_IDENTIFIER) then
    begin
      try
        brickletHumidity := TBrickletHumidity.Create(uid, ipcon);
        brickletHumidity.SetHumidityCallbackPeriod(1000);
        brickletHumidity.OnHumidity := {$ifdef FPC}@{$endif}HumidityCB;
        WriteLn('Humidity initialized');
      except
        on e: Exception do
        begin
          WriteLn('Humidity init failed: ' + e.Message);
          brickletHumidity := nil;
        end;
      end;
    end
    else if (deviceIdentifier = BRICKLET_HUMIDITY_V2_DEVICE_IDENTIFIER) then
    begin
      try
        brickletHumidityV2 := TBrickletHumidityV2.Create(uid, ipcon);
        brickletHumidityV2.SetHumidityCallbackConfiguration(1000, true, 'x', 0, 0);
        brickletHumidityV2.OnHumidity := {$ifdef FPC}@{$endif}HumidityV2CB;
        WriteLn('Humidity 2.0 initialized');
      except
        on e: Exception do
        begin
          WriteLn('Humidity 2.0 init failed: ' + e.Message);
          brickletHumidityV2 := nil;
        end;
      end;
    end
    else if (deviceIdentifier = BRICKLET_BAROMETER_DEVICE_IDENTIFIER) then
    begin
      try
        brickletBarometer := TBrickletBarometer.Create(uid, ipcon);
        brickletBarometer.SetAirPressureCallbackPeriod(1000);
        brickletBarometer.OnAirPressure := {$ifdef FPC}@{$endif}AirPressureCB;
        WriteLn('Barometer initialized');
      except
        on e: Exception do
        begin
          WriteLn('Barometer init failed: ' + e.Message);
          brickletBarometer := nil;
        end;
      end;
    end
    else if (deviceIdentifier = BRICKLET_BAROMETER_V2_DEVICE_IDENTIFIER) then
    begin
      try
        brickletBarometerV2 := TBrickletBarometerV2.Create(uid, ipcon);
        brickletBarometerV2.SetAirPressureCallbackConfiguration(1000, False, 'x', 0, 0);
        brickletBarometerV2.OnAirPressure := {$ifdef FPC}@{$endif}AirPressureV2CB;
        WriteLn('Barometer 2.0 initialized');
      except
        on e: Exception do
        begin
          WriteLn('Barometer 2.0 init failed: ' + e.Message);
          brickletBarometerV2 := nil;
        end;
      end;
    end;
  end;
end;

procedure TWeatherStation.IlluminanceCB(sender: TBrickletAmbientLight; const illuminance: word);

var text: string;

begin
  if (brickletLCD <> nil) then
  begin
    text := Format('Illuminanc %6.2f lx', [illuminance/10.0]);
    brickletLCD.WriteLine(0, 0, text);
    WriteLn('Write to line 0: ' + text);
  end;
end;

procedure TWeatherStation.IlluminanceV2CB(sender: TBrickletAmbientLightV2;
                                          const illuminance: longword);

var text: string;

begin
  if (brickletLCD <> nil) then
  begin
    text := Format('Illumina %8.2f lx', [illuminance/100.0]);
    brickletLCD.WriteLine(0, 0, text);
    WriteLn('Write to line 0: ' + text);
  end;
end;

procedure TWeatherStation.IlluminanceV3CB(sender: TBrickletAmbientLightV3;
                                          const illuminance: longword);
var text: string;
begin
  if (brickletLCD <> nil) then
  begin
    text := Format('Illumina %8.2f lx', [illuminance / 100.0]);
    brickletLCD.WriteLine(0, 0, text);
    WriteLn('Write to line 0: ' + text);
  end;
end;

procedure TWeatherStation.HumidityCB(sender: TBrickletHumidity;
                                     const humidity: word);
var text: string;
begin
  if (brickletLCD <> nil) then
  begin
    text := Format('Humidity   %6.2f %%', [humidity/10.0]);
    brickletLCD.WriteLine(1, 0, text);
    WriteLn('Write to line 1: ' + text);
  end;
end;

procedure TWeatherStation.HumidityV2CB(sender: TBrickletHumidityV2;
                                       const humidity: word);
var text: string;
begin
  if (brickletLCD <> nil) then
  begin
    text := Format('Humidity   %6.2f %%', [humidity/100.0]);
    brickletLCD.WriteLine(1, 0, text);
    WriteLn('Write to line 1: ' + text);
  end;
end;

procedure TWeatherStation.AirPressureCB(sender: TBrickletBarometer;
                                        const airPressure: longint);
var text: string; temperature: smallint;
begin
  if (brickletLCD <> nil) then
  begin
    text := Format('Air Press %7.2f mb', [airPressure/1000.0]);
    brickletLCD.WriteLine(2, 0, text);
    WriteLn('Write to line 2: ' + text);

    try
      temperature := brickletBarometer.GetChipTemperature;
    except
      on e: Exception do
      begin
        WriteLn('Could not get temperature: ' + e.Message);
        exit;
      end;
    end;

    { $DF == ° on LCD 20x4 charset. }
    text := Format('Temperature %5.2f %sC', [temperature/100.0, '' + char($DF)]);
    brickletLCD.WriteLine(3, 0, text);
    text := StringReplace(text, char($DF), '°', [rfReplaceAll]);
    WriteLn('Write to line 3: ' + text);
  end;
end;

procedure TWeatherStation.AirPressureV2CB(sender: TBrickletBarometerV2;
                                          const airPressure: longint);
var text: string;
temperature: smallint;
begin
  if (brickletLCD <> nil) then
  begin
    text := Format('Air Press %7.2f mb', [airPressure/1000.0]);
    brickletLCD.WriteLine(2, 0, text);
    WriteLn('Write to line 2: ' + text);

    try
      temperature := brickletBarometerV2.GetTemperature;
    except
      on e: Exception do
      begin
        WriteLn('Could not get temperature: ' + e.Message);
        exit;
      end;
    end;

    { $DF == ° on LCD 20x4 charset. }
    text := Format('Temperature %5.2f %sC', [temperature/100.0, '' + char($DF)]);
    brickletLCD.WriteLine(3, 0, text);
    text := StringReplace(text, char($DF), '°', [rfReplaceAll]);
    WriteLn('Write to line 3: ' + text);
  end;
end;

procedure TWeatherStation.Execute;
begin
  ipcon := TIPConnection.Create;
  while (true) do
  begin
    try
      ipcon.Connect(HOST, PORT);
      break;
    except
      on e: Exception do
      begin
        WriteLn('Connection Error: ' + e.Message);
        Sleep(1000);
      end;
    end;
  end;
  ipcon.OnEnumerate := {$ifdef FPC}@{$endif}EnumerateCB;
  ipcon.OnConnected := {$ifdef FPC}@{$endif}ConnectedCB;
  while (true) do
  begin
    try
      ipcon.Enumerate;
      break;
    except
      on e: Exception do
      begin
        WriteLn('Enumeration Error: ' + e.Message);
        Sleep(1000);
      end;
    end;
  end;
  WriteLn('Press key to exit');
  ReadLn;
end;

begin
  ws := TWeatherStation.Create;
  ws.Execute;
  ws.Destroy;
end.
