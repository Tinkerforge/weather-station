Imports Tinkerforge

Module WeatherStation
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223

    Private ipcon As IPConnection
    Private brickletLCD As BrickletLCD20x4
    Private brickletHumidity As BrickletHumidity
    Private brickletBarometer As BrickletBarometer
    Private brickletAmbientLight As BrickletAmbientLight

    Sub IlluminanceCB(ByVal sender As BrickletAmbientLight, ByVal illuminance As Integer)
        Dim text As String = String.Format("Illuminanc {0,6:###.00} lx", illuminance/10.0)
        brickletLCD.WriteLine(0, 0, text)
        System.Console.WriteLine("Write to line 0: " + text)
    End Sub

    Sub HumidityCB(ByVal sender As BrickletHumidity, ByVal humidity As Integer)
        Dim text As String = String.Format("Humidity   {0,6:###.00} %", humidity/10.0)
        brickletLCD.WriteLine(1, 0, text)
        System.Console.WriteLine("Write to line 1: " + text)
    End Sub

    Sub AirPressureCB(ByVal sender As BrickletBarometer, ByVal airPressure As Integer)
        Dim text As String = String.Format("Air Press {0,7:####.00} mb", airPressure/1000.0)
        brickletLCD.WriteLine(2, 0, text)
        System.Console.WriteLine("Write to line 2: " + text)

        Dim temperature As Integer = sender.GetChipTemperature()
        text = String.Format("Temperature {0,5:##.00} {1}C", temperature/100.0, Chr(&HDF))
        brickletLCD.WriteLine(3, 0, text)
        System.Console.WriteLine("Write to line 3: " + text)
    End Sub

    Sub EnumerateCB(ByVal sender As IPConnection, ByVal uid As String, _
                    ByVal connectedUid As String, ByVal position As Char, _
                    ByVal hardwareVersion() As Short, ByVal firmwareVersion() As Short, _
                    ByVal deviceIdentifier As Integer, ByVal enumerationType As Short)
        If enumerationType = IPConnection.ENUMERATION_TYPE_CONNECTED Or _
           enumerationType = IPConnection.ENUMERATION_TYPE_AVAILABLE Then
            If deviceIdentifier = BrickletLCD20x4.DEVICE_IDENTIFIER Then
                brickletLCD = New BrickletLCD20x4(UID, ipcon)
                brickletLCD.ClearDisplay()
                brickletLCD.BacklightOn()
            Else If deviceIdentifier = BrickletAmbientLight.DEVICE_IDENTIFIER Then
                brickletAmbientLight = New BrickletAmbientLight(UID, ipcon)
                brickletAmbientLight.SetIlluminanceCallbackPeriod(1000)
                AddHandler brickletAmbientLight.Illuminance, AddressOf IlluminanceCB
            Else If deviceIdentifier = BrickletHumidity.DEVICE_IDENTIFIER Then
                brickletHumidity = New BrickletHumidity(UID, ipcon)
                brickletHumidity.SetHumidityCallbackPeriod(1000)
                AddHandler brickletHumidity.Humidity, AddressOf HumidityCB
            Else If deviceIdentifier = BrickletBarometer.DEVICE_IDENTIFIER Then
                brickletBarometer = New BrickletBarometer(UID, ipcon)
                brickletBarometer.SetAirPressureCallbackPeriod(1000)
                AddHandler brickletBarometer.AirPressure, AddressOf AirPressureCB
            End If
        End If
    End Sub

    Sub ConnectedCB(ByVal sender As IPConnection, ByVal connectedReason as Short)
        If connectedReason = IPConnection.CONNECT_REASON_AUTO_RECONNECT Then
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
        System.Console.ReadKey()
        ipcon.Disconnect()
    End Sub
End Module
