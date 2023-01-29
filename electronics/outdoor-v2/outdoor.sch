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
L MCU_Microchip_ATtiny:ATtiny85V-10PU U2
U 1 1 63201EA0
P 4950 2400
F 0 "U2" H 4550 1850 50  0000 R CNN
F 1 "ATtiny45V-10PU" V 4400 2850 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 4950 2400 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 4950 2400 50  0001 C CNN
	1    4950 2400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 632038BA
P 6900 1900
F 0 "J1" H 6850 2100 50  0000 L CNN
F 1 "TX433MHz" V 7000 1700 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6900 1900 50  0001 C CNN
F 3 "~" H 6900 1900 50  0001 C CNN
	1    6900 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1600 4950 1800
Wire Wire Line
	4950 3150 4950 3000
Wire Wire Line
	3700 1600 3850 1600
NoConn ~ 5550 2100
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 6338E5DB
P 1850 3150
F 0 "#FLG0102" H 1850 3225 50  0001 C CNN
F 1 "PWR_FLAG" H 1850 3323 50  0000 C CNN
F 2 "" H 1850 3150 50  0001 C CNN
F 3 "~" H 1850 3150 50  0001 C CNN
	1    1850 3150
	1    0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 6338D996
P 1850 1300
F 0 "#FLG0101" H 1850 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 1850 1473 50  0000 C CNN
F 2 "" H 1850 1300 50  0001 C CNN
F 3 "~" H 1850 1300 50  0001 C CNN
	1    1850 1300
	1    0    0    -1  
$EndComp
Text Notes 6200 1850 0    50   ~ 0
VCC
$Comp
L Device:Battery BT1
U 1 1 63D2EE70
P 1850 2250
F 0 "BT1" H 1958 2296 50  0000 L CNN
F 1 "4.5V" H 1958 2205 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" V 1850 2310 50  0001 C CNN
F 3 "~" V 1850 2310 50  0001 C CNN
	1    1850 2250
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:MCP1700-3302E_TO92 U1
U 1 1 63D31815
P 3400 1600
F 0 "U1" H 3300 1450 50  0000 R CNN
F 1 "MCP1700-3302E" H 4100 1450 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 3400 1400 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001826D.pdf" H 3400 1600 50  0001 C CNN
	1    3400 1600
	1    0    0    1   
$EndComp
$Comp
L Device:CP C2
U 1 1 63D33E7B
P 3850 1750
F 0 "C2" H 3968 1796 50  0000 L CNN
F 1 "1u" H 3968 1705 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 3888 1600 50  0001 C CNN
F 3 "~" H 3850 1750 50  0001 C CNN
	1    3850 1750
	1    0    0    -1  
$EndComp
Connection ~ 3850 1600
Wire Wire Line
	3850 1600 4950 1600
$Comp
L Device:CP C1
U 1 1 63D348FC
P 2900 1750
F 0 "C1" H 3018 1796 50  0000 L CNN
F 1 "1u" H 3018 1705 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.5mm_P5.00mm" H 2938 1600 50  0001 C CNN
F 3 "~" H 2900 1750 50  0001 C CNN
	1    2900 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3150 4950 3150
Wire Wire Line
	2900 1600 3100 1600
Wire Wire Line
	2550 2400 4200 2400
Wire Wire Line
	4200 2400 4200 1750
Wire Wire Line
	4200 1750 5650 1750
Wire Wire Line
	5650 1750 5650 2400
Wire Wire Line
	5650 2400 5550 2400
$Comp
L Device:R R4
U 1 1 63A07D65
P 7300 1850
F 0 "R4" H 7400 1900 50  0000 C CNN
F 1 "4.7k" H 7350 1800 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7230 1850 50  0001 C CNN
F 3 "~" H 7300 1850 50  0001 C CNN
	1    7300 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 63D3A40B
P 2550 2000
F 0 "R1" H 2650 2050 50  0000 C CNN
F 1 "1M" H 2600 1950 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2480 2000 50  0001 C CNN
F 3 "~" H 2550 2000 50  0001 C CNN
	1    2550 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 63D3AC16
P 2550 2800
F 0 "R2" H 2650 2850 50  0000 C CNN
F 1 "100k" H 2600 2750 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2480 2800 50  0001 C CNN
F 3 "~" H 2550 2800 50  0001 C CNN
	1    2550 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2150 2550 2400
Connection ~ 2550 2400
Wire Wire Line
	2550 2400 2550 2650
Text Notes 6550 1750 0    50   ~ 0
DATA
$Comp
L Sensor_Temperature:DS18B20 U3
U 1 1 631FB4D2
P 7600 2400
F 0 "U3" H 7550 2150 50  0000 R CNN
F 1 "DS18B20" V 7350 2550 50  0000 R CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 6600 2150 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 7450 2650 50  0001 C CNN
	1    7600 2400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1850 2050 1850 1600
Wire Wire Line
	1850 1600 2550 1600
Connection ~ 2900 1600
Wire Wire Line
	2550 1850 2550 1600
Connection ~ 2550 1600
Wire Wire Line
	2550 1600 2900 1600
Wire Wire Line
	1850 1300 1850 1600
Connection ~ 1850 1600
Wire Wire Line
	7600 1600 7600 2100
Wire Wire Line
	7300 1700 7300 1600
Connection ~ 7300 1600
Wire Wire Line
	7300 1600 7600 1600
Wire Wire Line
	7300 2000 7300 2200
Connection ~ 7300 2200
Wire Wire Line
	7300 2200 7300 2400
Wire Wire Line
	7600 3150 7600 2700
Wire Wire Line
	2550 2950 2550 3150
Wire Wire Line
	2550 3150 3400 3150
Connection ~ 3400 3150
Wire Wire Line
	1850 2450 1850 3150
Wire Wire Line
	1850 3150 2550 3150
Connection ~ 2550 3150
Connection ~ 1850 3150
Connection ~ 1850 1300
Wire Wire Line
	5550 2300 6450 2300
Wire Wire Line
	5550 2200 7300 2200
Wire Wire Line
	1850 1300 6150 1300
Wire Wire Line
	4950 1600 5850 1600
Connection ~ 4950 1600
Connection ~ 4950 3150
$Comp
L Switch:SW_Push SW1
U 1 1 63D4C54F
P 5850 1900
F 0 "SW1" V 5750 2050 50  0000 C CNN
F 1 "SW_Push" H 5850 2094 50  0001 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 5850 2100 50  0001 C CNN
F 3 "~" H 5850 2100 50  0001 C CNN
	1    5850 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	5850 1700 5850 1600
Connection ~ 5850 1600
Wire Wire Line
	5850 1600 7300 1600
Wire Wire Line
	5550 2600 5850 2600
Wire Wire Line
	5850 2600 5850 2100
Wire Wire Line
	3400 1900 3400 2100
Wire Wire Line
	2900 1900 2900 2100
Wire Wire Line
	2900 2100 3400 2100
Connection ~ 3400 2100
Wire Wire Line
	3400 2100 3400 3150
Wire Wire Line
	3850 1900 3850 2100
Wire Wire Line
	3850 2100 3400 2100
Wire Wire Line
	4950 3150 6450 3150
Wire Wire Line
	6150 1900 6150 1300
Wire Wire Line
	6150 1900 6700 1900
Wire Wire Line
	6450 1800 6450 2300
Wire Wire Line
	6450 1800 6700 1800
$Comp
L Device:R R3
U 1 1 63D75132
P 6450 2750
F 0 "R3" H 6550 2800 50  0000 C CNN
F 1 "10k" H 6500 2700 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6380 2750 50  0001 C CNN
F 3 "~" H 6450 2750 50  0001 C CNN
	1    6450 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2300 6450 2600
Connection ~ 6450 2300
Wire Wire Line
	6450 2900 6450 3150
Connection ~ 6450 3150
Wire Wire Line
	6450 3150 6700 3150
Text Notes 6500 2050 0    50   ~ 0
GND
Wire Wire Line
	6700 2000 6700 3150
Connection ~ 6700 3150
Wire Wire Line
	6700 3150 7600 3150
NoConn ~ 5550 2500
$EndSCHEMATC
