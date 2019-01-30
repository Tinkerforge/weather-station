Imports Tinkerforge

Module WeatherStation
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223

    Private ipcon As IPConnection = Nothing
    Private brickletLCD As BrickletLCD20x4 = Nothing
    Private brickletAmbientLight As BrickletAmbientLight = Nothing
    Private brickletAmbientLightV2 As BrickletAmbientLightV2 = Nothing
    Private brickletAmbientLightV3 As BrickletAmbientLightV3 = Nothing
    Private brickletHumidity As BrickletHumidity = Nothing
    Private brickletHumidityV2 As BrickletHumidityV2 = Nothing
    Private brickletBarometer As BrickletBarometer = Nothing
    Private brickletBarometerV2 As BrickletBarometerV2 = Nothing

    Sub IlluminanceCB(ByVal sender As BrickletAmbientLight, ByVal illuminance As Integer)
        If brickletLCD IsNot Nothing Then
            Dim text As String = String.Format("Illuminanc {0,6:###.00} lx", illuminance/10.0)
            brickletLCD.WriteLine(0, 0, text)
            System.Console.WriteLine("Write to line 0: " + text)
        End If
    End Sub

    Sub IlluminanceV2CB(ByVal sender As BrickletAmbientLightV2, ByVal illuminance As Long)
        If brickletLCD IsNot Nothing Then
            Dim text As String = String.Format("Illumina {0,8:###.00} lx", illuminance/100.0)
            brickletLCD.WriteLine(0, 0, text)
            System.Console.WriteLine("Write to line 0: " + text)
        End If
    End Sub

    Sub IlluminanceV3CB(ByVal sender As BrickletAmbientLightV3, ByVal illuminance As Long)
        If brickletLCD IsNot Nothing Then
            Dim text As String = String.Format("Illumina {0,8:###.00} lx", illuminance/100.0)
            brickletLCD.WriteLine(0, 0, text)
            System.Console.WriteLine("Write to line 0: " + text)
        End If
    End Sub

    Sub HumidityCB(ByVal sender As BrickletHumidity, ByVal humidity As Integer)
        If brickletLCD IsNot Nothing Then
            Dim text As String = String.Format("Humidity   {0,6:###.00} %", humidity/10.0)
            brickletLCD.WriteLine(1, 0, text)
            System.Console.WriteLine("Write to line 1: " + text)
        End If
    End Sub

    Sub HumidityV2CB(ByVal sender As BrickletHumidityV2, ByVal humidity As Integer)
        If brickletLCD IsNot Nothing Then
            Dim text As String = String.Format("Humidity   {0,6:###.00} %", humidity/100.0)
            brickletLCD.WriteLine(1, 0, text)
            System.Console.WriteLine("Write to line 1: " + text)
        End If
    End Sub

    Sub AirPressureCB(ByVal sender As BrickletBarometer, ByVal airPressure As Integer)
        If brickletLCD IsNot Nothing Then
            Dim text As String = String.Format("Air Press {0,7:####.00} mb", airPressure/1000.0)
            brickletLCD.WriteLine(2, 0, text)
            System.Console.WriteLine("Write to line 2: " + text)

            Dim temperature As Integer
            Try
                temperature = sender.GetChipTemperature()
            Catch e As TinkerforgeException
                System.Console.WriteLine("Could not get temperature" + e.Message)
                Return
            End Try

            ' &HDF == ° on LCD 20x4 charset
            text = String.Format("Temperature {0,5:##.00} {1}C", temperature/100.0, Chr(&HDF))
            brickletLCD.WriteLine(3, 0, text)
            System.Console.WriteLine("Write to line 3: " + text.Replace(Chr(&HDF), "°"C))
        End If
    End Sub

    Sub AirPressureV2CB(ByVal sender As BrickletBarometerV2, ByVal airPressure As Integer)
        If brickletLCD IsNot Nothing Then
            Dim text As String = String.Format("Air Press {0,7:####.00} mb", airPressure/1000.0)
            brickletLCD.WriteLine(2, 0, text)
            System.Console.WriteLine("Write to line 2: " + text)

            Dim temperature As Integer
            Try
                temperature = sender.GetTemperature()
            Catch e As TinkerforgeException
                System.Console.WriteLine("Could not get temperature" + e.Message)
                Return
            End Try

            ' &HDF == ° on LCD 20x4 charset
            text = String.Format("Temperature {0,5:##.00} {1}C", temperature/100.0, Chr(&HDF))
            brickletLCD.WriteLine(3, 0, text)
            System.Console.WriteLine("Write to line 3: " + text.Replace(Chr(&HDF), "°"C))
        End If
    End Sub

    Sub EnumerateCB(ByVal sender As IPConnection, ByVal uid As String, _
                    ByVal connectedUid As String, ByVal position As Char, _
                    ByVal hardwareVersion() As Short, ByVal firmwareVersion() As Short, _
                    ByVal deviceIdentifier As Integer, ByVal enumerationType As Short)
        If enumerationType = IPConnection.ENUMERATION_TYPE_CONNECTED Or _
           enumerationType = IPConnection.ENUMERATION_TYPE_AVAILABLE Then
            If deviceIdentifier = BrickletLCD20x4.DEVICE_IDENTIFIER Then
                Try
                    brickletLCD = New BrickletLCD20x4(UID, ipcon)
                    brickletLCD.ClearDisplay()
                    brickletLCD.BacklightOn()
                    System.Console.WriteLine("LCD 20x4 initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("LCD 20x4 init failed: " + e.Message)
                    brickletLCD = Nothing
                End Try
            Else If deviceIdentifier = BrickletAmbientLight.DEVICE_IDENTIFIER Then
                Try
                    brickletAmbientLight = New BrickletAmbientLight(UID, ipcon)
                    brickletAmbientLight.SetIlluminanceCallbackPeriod(1000)
                    AddHandler brickletAmbientLight.Illuminance, AddressOf IlluminanceCB
                    System.Console.WriteLine("Ambient Light initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("Ambient Light init failed: " + e.Message)
                    brickletAmbientLight = Nothing
                End Try
            Else If deviceIdentifier = BrickletAmbientLightV2.DEVICE_IDENTIFIER Then
                Try
                    brickletAmbientLightV2 = New BrickletAmbientLightV2(UID, ipcon)
                    brickletAmbientLightV2.SetConfiguration(BrickletAmbientLightV2.ILLUMINANCE_RANGE_64000LUX, _
                                                            BrickletAmbientLightV2.INTEGRATION_TIME_200MS)
                    brickletAmbientLightV2.SetIlluminanceCallbackPeriod(1000)
                    AddHandler brickletAmbientLightV2.Illuminance, AddressOf IlluminanceV2CB
                    System.Console.WriteLine("Ambient Light 2.0 initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("Ambient Light 2.0 init failed: " + e.Message)
                    brickletAmbientLightV2 = Nothing
                End Try
            Else If deviceIdentifier = BrickletAmbientLightV3.DEVICE_IDENTIFIER Then
                Try
                    brickletAmbientLightV3 = New BrickletAmbientLightV3(UID, ipcon)
                    brickletAmbientLightV3.SetConfiguration(BrickletAmbientLightV3.ILLUMINANCE_RANGE_64000LUX, _
                                                            BrickletAmbientLightV3.INTEGRATION_TIME_200MS)
                    brickletAmbientLightV3.SetIlluminanceCallbackConfiguration(1000, True, "x"C, 0, 0)
                    AddHandler brickletAmbientLightV3.IlluminanceCallback, AddressOf IlluminanceV3CB
                    System.Console.WriteLine("Ambient Light 3.0 initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("Ambient Light 3.0 init failed: " + e.Message)
                    brickletAmbientLightV3 = Nothing
                End Try
            Else If deviceIdentifier = BrickletHumidity.DEVICE_IDENTIFIER Then
                Try
                    brickletHumidity = New BrickletHumidity(UID, ipcon)
                    brickletHumidity.SetHumidityCallbackPeriod(1000)
                    AddHandler brickletHumidity.Humidity, AddressOf HumidityCB
                    System.Console.WriteLine("Humidity initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("Humidity init failed: " + e.Message)
                    brickletHumidity = Nothing
                End Try
            Else If deviceIdentifier = BrickletHumidityV2.DEVICE_IDENTIFIER Then
                Try
                    brickletHumidityV2 = New BrickletHumidityV2(UID, ipcon)
                    brickletHumidityV2.SetHumidityCallbackConfiguration(1000, True, "x"C, 0, 0)
                    AddHandler brickletHumidityV2.HumidityCallback, AddressOf HumidityV2CB
                    System.Console.WriteLine("Humidity 2.0 initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("Humidity 2.0 init failed: " + e.Message)
                    brickletHumidityV2 = Nothing
                End Try
            Else If deviceIdentifier = BrickletBarometer.DEVICE_IDENTIFIER Then
                Try
                    brickletBarometer = New BrickletBarometer(UID, ipcon)
                    brickletBarometer.SetAirPressureCallbackPeriod(1000)
                    AddHandler brickletBarometer.AirPressure, AddressOf AirPressureCB
                    System.Console.WriteLine("Barometer initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("Barometer init failed: " + e.Message)
                    brickletBarometer = Nothing
                End Try
            Else If deviceIdentifier = BrickletBarometerV2.DEVICE_IDENTIFIER Then
                Try
                    brickletBarometerV2 = New BrickletBarometerV2(UID, ipcon)
                    brickletBarometerV2.SetAirPressureCallbackConfiguration(1000, True, "x"C, 0, 0)
                    AddHandler brickletBarometerV2.AirPressureCallback, AddressOf AirPressureV2CB
                    System.Console.WriteLine("Barometer 2.0 initialized")
                Catch e As TinkerforgeException
                    System.Console.WriteLine("Barometer 2.0 init failed: " + e.Message)
                    brickletBarometerV2 = Nothing
                End Try
            End If
        End If
    End Sub

    Sub ConnectedCB(ByVal sender As IPConnection, ByVal connectedReason as Short)
        If connectedReason = IPConnection.CONNECT_REASON_AUTO_RECONNECT Then
            System.Console.WriteLine("Auto Reconnect")
            while True
                Try
                    ipcon.Enumerate()
                    Exit While
                Catch e As NotConnectedException
                    System.Console.WriteLine("Enumeration Error: " + e.Message)
                    System.Threading.Thread.Sleep(1000)
                End Try
            End While
        End If
    End Sub

    Sub Main()
        ipcon = New IPConnection()
        while True
            Try
                ipcon.Connect(HOST, PORT)
                Exit While
            Catch e As System.Net.Sockets.SocketException
                System.Console.WriteLine("Connection Error: " + e.Message)
                System.Threading.Thread.Sleep(1000)
            End Try
        End While

        AddHandler ipcon.EnumerateCallback, AddressOf EnumerateCB
        AddHandler ipcon.Connected, AddressOf ConnectedCB

        while True
            try
                ipcon.Enumerate()
                Exit While
            Catch e As NotConnectedException
                System.Console.WriteLine("Enumeration Error: " + e.Message)
                System.Threading.Thread.Sleep(1000)
            End Try
        End While

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
