EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Solar_Cell SC1
U 1 1 631FA2BE
P 1050 1950
F 0 "SC1" H 1158 2046 50  0000 L CNN
F 1 "5V" H 1158 1955 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" V 1050 2010 50  0001 C CNN
F 3 "~" V 1050 2010 50  0001 C CNN
	1    1050 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 631FAB94
P 1350 1250
F 0 "R1" V 1557 1250 50  0000 C CNN
F 1 "59" V 1466 1250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 1280 1250 50  0001 C CNN
F 3 "~" H 1350 1250 50  0001 C CNN
	1    1350 1250
	0    -1   -1   0   
$EndComp
$Comp
L Device:D D1
U 1 1 631FBA3D
P 1650 1250
F 0 "D1" H 1650 1033 50  0000 C CNN
F 1 "1N60" H 1650 1124 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P10.16mm_Horizontal" H 1650 1250 50  0001 C CNN
F 3 "~" H 1650 1250 50  0001 C CNN
	1    1650 1250
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Zener D2
U 1 1 631FF125
P 1950 1900
F 0 "D2" V 1904 1980 50  0000 L CNN
F 1 "4.3V" V 1995 1980 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P10.16mm_Horizontal" H 1950 1900 50  0001 C CNN
F 3 "~" H 1950 1900 50  0001 C CNN
	1    1950 1900
	0    1    1    0   
$EndComp
$Comp
L Device:Battery BT1
U 1 1 631F93E3
P 2400 1900
F 0 "BT1" H 2508 1946 50  0000 L CNN
F 1 "3xNiCd" H 2508 1855 50  0000 L CNN
F 2 "CustomParts:NiCd-Small" V 2400 1960 50  0001 C CNN
F 3 "~" V 2400 1960 50  0001 C CNN
	1    2400 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1250 1950 1250
Wire Wire Line
	2400 1250 2400 1700
Wire Wire Line
	1950 1750 1950 1250
Connection ~ 1950 1250
Wire Wire Line
	1950 1250 2400 1250
$Comp
L MCU_Microchip_ATtiny:ATtiny85V-10PU U1
U 1 1 63201EA0
P 3650 2050
F 0 "U1" H 3250 2650 50  0000 R CNN
F 1 "ATtiny85V-10PU" V 3100 2500 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 3650 2050 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 3650 2050 50  0001 C CNN
	1    3650 2050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 632038BA
P 4850 2300
F 0 "J1" H 4800 2500 50  0000 L CNN
F 1 "TX433MHz" V 4950 2100 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4850 2300 50  0001 C CNN
F 3 "~" H 4850 2300 50  0001 C CNN
	1    4850 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 1250 3650 1450
Connection ~ 2400 1250
Wire Wire Line
	3650 2800 3650 2650
Wire Wire Line
	1050 2050 1050 2800
Connection ~ 2400 2800
Wire Wire Line
	1950 2050 1950 2800
Connection ~ 1950 2800
Wire Wire Line
	1950 2800 2400 2800
Wire Wire Line
	2400 2100 2400 2800
Wire Wire Line
	5050 1250 5050 1300
Wire Wire Line
	5050 1900 5050 2800
Wire Wire Line
	5050 2800 4650 2800
Wire Wire Line
	4650 2400 4650 2800
Wire Wire Line
	4650 1950 4650 2200
Wire Wire Line
	2400 2800 3650 2800
Wire Wire Line
	2400 1250 3650 1250
Wire Wire Line
	4250 1950 4650 1950
Wire Wire Line
	3650 2800 4650 2800
Connection ~ 3650 2800
Connection ~ 4650 2800
Connection ~ 3650 1250
$Comp
L Sensor_Temperature:DS18B20 U2
U 1 1 631FB4D2
P 5050 1600
F 0 "U2" H 5000 1350 50  0000 R CNN
F 1 "DS18B20" V 4800 1750 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 4050 1350 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 4900 1850 50  0001 C CNN
	1    5050 1600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4250 1850 4650 1850
Wire Wire Line
	4650 1850 4650 1600
Wire Wire Line
	4650 1600 4750 1600
NoConn ~ 4250 1750
NoConn ~ 4250 2150
NoConn ~ 4250 2250
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 6338E5DB
P 2400 2800
F 0 "#FLG0102" H 2400 2875 50  0001 C CNN
F 1 "PWR_FLAG" H 2400 2973 50  0000 C CNN
F 2 "" H 2400 2800 50  0001 C CNN
F 3 "~" H 2400 2800 50  0001 C CNN
	1    2400 2800
	1    0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 6338D996
P 2400 1250
F 0 "#FLG0101" H 2400 1325 50  0001 C CNN
F 1 "PWR_FLAG" H 2400 1423 50  0000 C CNN
F 2 "" H 2400 1250 50  0001 C CNN
F 3 "~" H 2400 1250 50  0001 C CNN
	1    2400 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 1250 1200 1250
Wire Wire Line
	1050 1250 1050 1750
Wire Wire Line
	1050 2800 1950 2800
Wire Wire Line
	3650 1250 5050 1250
Wire Wire Line
	4250 2050 4350 2050
Wire Wire Line
	4350 2050 4350 2300
Wire Wire Line
	4350 2300 4650 2300
Text Notes 4400 2300 0    50   ~ 0
VCC
Text Notes 4650 2000 0    50   ~ 0
DATA
$EndSCHEMATC
