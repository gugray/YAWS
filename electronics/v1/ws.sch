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
L ws-rescue:EA-DOGM128W-6-DOGM128-sandsim-rescue-ws-rescue U3
U 1 1 62D27F39
P 5500 3200
F 0 "U3" H 5500 4250 60  0000 L CNN
F 1 "EA-DOGM128W-6" V 6250 3200 60  0000 L CNN
F 2 "CustomParts:DOGM128" V 5900 3700 60  0001 C CNN
F 3 "" H 5850 3150 60  0001 C CNN
F 4 "ELECTRONIC ASSEMBLY" V 5800 3700 60  0001 C CNN "Manufacturer"
	1    5500 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 62D33855
P 6950 4100
F 0 "C4" H 6950 4250 50  0000 L CNN
F 1 "CP" H 7068 4055 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 6988 3950 50  0001 C CNN
F 3 "~" H 6950 4100 50  0001 C CNN
	1    6950 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 4250 7200 4250
Wire Wire Line
	6950 2900 6950 3950
$Comp
L Device:CP C5
U 1 1 62D386F2
P 7200 4100
F 0 "C5" H 7200 4250 50  0000 L CNN
F 1 "CP" H 7318 4055 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 7238 3950 50  0001 C CNN
F 3 "~" H 7200 4100 50  0001 C CNN
	1    7200 4100
	1    0    0    -1  
$EndComp
Connection ~ 7200 4250
$Comp
L Device:CP C6
U 1 1 62D38BBC
P 7450 4100
F 0 "C6" H 7450 4250 50  0000 L CNN
F 1 "CP" H 7568 4055 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 7488 3950 50  0001 C CNN
F 3 "~" H 7450 4100 50  0001 C CNN
	1    7450 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C7
U 1 1 62D3934D
P 7700 4100
F 0 "C7" H 7700 4250 50  0000 L CNN
F 1 "CP" H 7818 4055 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 7738 3950 50  0001 C CNN
F 3 "~" H 7700 4100 50  0001 C CNN
	1    7700 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C8
U 1 1 62D39837
P 7950 4100
F 0 "C8" H 7950 4250 50  0000 L CNN
F 1 "CP" H 8068 4055 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 7988 3950 50  0001 C CNN
F 3 "~" H 7950 4100 50  0001 C CNN
	1    7950 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C9
U 1 1 62D39F48
P 8200 4100
F 0 "C9" H 8200 4250 50  0000 L CNN
F 1 "CP" H 8318 4055 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 8238 3950 50  0001 C CNN
F 3 "~" H 8200 4100 50  0001 C CNN
	1    8200 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 2900 8550 2900
Wire Wire Line
	7200 3950 7200 3200
Wire Wire Line
	7200 3200 8550 3200
Wire Wire Line
	8550 3300 7450 3300
Wire Wire Line
	7450 3300 7450 3950
Wire Wire Line
	7700 3950 7700 3400
Wire Wire Line
	7700 3400 8550 3400
Wire Wire Line
	8550 3500 7950 3500
Wire Wire Line
	7950 3500 7950 3950
Wire Wire Line
	8200 3950 8200 3600
Wire Wire Line
	8200 3600 8550 3600
Wire Wire Line
	8550 3900 8450 3900
Wire Wire Line
	8450 3900 8450 4000
Wire Wire Line
	7200 4250 7450 4250
Connection ~ 7450 4250
Wire Wire Line
	7450 4250 7700 4250
Connection ~ 7700 4250
Wire Wire Line
	7700 4250 7950 4250
Connection ~ 7950 4250
Wire Wire Line
	7950 4250 8200 4250
Connection ~ 8200 4250
Wire Wire Line
	8200 4250 8450 4250
Wire Wire Line
	8450 4000 8550 4000
Connection ~ 8450 4000
Wire Wire Line
	8450 4000 8450 4250
$Comp
L Device:CP C12
U 1 1 62D6100B
P 9800 3800
F 0 "C12" H 9800 3950 50  0000 L CNN
F 1 "CP" H 9918 3755 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 9838 3650 50  0001 C CNN
F 3 "~" H 9800 3800 50  0001 C CNN
	1    9800 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 3600 9800 3600
Wire Wire Line
	9800 3600 9800 3650
Wire Wire Line
	9800 3950 9800 4000
Wire Wire Line
	9800 4000 9650 4000
$Comp
L Device:CP C10
U 1 1 62D64CF4
P 9800 2600
F 0 "C10" H 9800 2750 50  0000 L CNN
F 1 "CP" H 9918 2555 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 9838 2450 50  0001 C CNN
F 3 "~" H 9800 2600 50  0001 C CNN
	1    9800 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C11
U 1 1 62D65B3B
P 9800 3000
F 0 "C11" H 9800 3150 50  0000 L CNN
F 1 "CP" H 9918 2955 50  0001 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_L10mm_P2.50mm_Lying" H 9838 2850 50  0001 C CNN
F 3 "~" H 9800 3000 50  0001 C CNN
	1    9800 3000
	1    0    0    1   
$EndComp
Wire Wire Line
	9650 2400 9800 2400
Wire Wire Line
	9800 2400 9800 2450
Wire Wire Line
	9800 2750 9800 2800
Wire Wire Line
	9650 2800 9800 2800
Connection ~ 9800 2800
Wire Wire Line
	9800 2800 9800 2850
Wire Wire Line
	9650 3200 9800 3200
Wire Wire Line
	9800 3200 9800 3150
$Comp
L ws-rescue:EA-DOGM128W-6-DOGM128-sandsim-rescue-ws-rescue U3
U 3 1 62F13E03
P 8800 1850
F 0 "U3" H 8900 2800 60  0000 C CNN
F 1 "EA-DOGM128W-6" H 9200 2700 60  0000 C CNN
F 2 "CustomParts:DOGM128" V 9200 2350 60  0001 C CNN
F 3 "" H 9150 1800 60  0001 C CNN
F 4 "ELECTRONIC ASSEMBLY" V 9100 2350 60  0001 C CNN "Manufacturer"
	3    8800 1850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 62F1B5AF
P 5900 1700
F 0 "SW1" V 5854 1848 50  0000 L CNN
F 1 "SW_Push" V 5945 1848 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W6.5mm_P5.00mm" H 5900 1900 50  0001 C CNN
F 3 "~" H 5900 1900 50  0001 C CNN
	1    5900 1700
	0    1    -1   0   
$EndComp
$Comp
L Device:R R6
U 1 1 62F1DCAC
P 5900 1350
F 0 "R6" H 5970 1396 50  0000 L CNN
F 1 "10k" H 5970 1305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5830 1350 50  0001 C CNN
F 3 "~" H 5900 1350 50  0001 C CNN
	1    5900 1350
	1    0    0    1   
$EndComp
$Comp
L Device:C C1
U 1 1 62F21F0F
P 1150 1750
F 0 "C1" H 1265 1796 50  0000 L CNN
F 1 ".33uF" H 1265 1705 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 1188 1600 50  0001 C CNN
F 3 "~" H 1150 1750 50  0001 C CNN
	1    1150 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 -850 10050 -550
Wire Wire Line
	10050 -550 10050 -250
Connection ~ 10050 -550
$Comp
L Device:R R7
U 1 1 62F34145
P 7050 1550
F 0 "R7" V 7150 1600 50  0000 C CNN
F 1 "100R" V 7250 1550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6980 1550 50  0001 C CNN
F 3 "~" H 7050 1550 50  0001 C CNN
	1    7050 1550
	0    -1   1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 62F36408
P 7250 1300
F 0 "R8" H 7181 1254 50  0000 R CNN
F 1 "10K" H 7181 1345 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7180 1300 50  0001 C CNN
F 3 "~" H 7250 1300 50  0001 C CNN
	1    7250 1300
	1    0    0    1   
$EndComp
Wire Wire Line
	9600 -550 10050 -550
Text GLabel 6900 1050 0    50   Input ~ 0
3V3
Text GLabel 9850 1450 2    50   Input ~ 0
GND
Text GLabel 8450 2400 0    50   Input ~ 0
3V3
Wire Wire Line
	8450 2400 8550 2400
Wire Wire Line
	8550 2400 8550 2600
Text GLabel 7700 4400 3    50   Input ~ 0
GND
Wire Wire Line
	7700 4250 7700 4400
$Comp
L Device:C C3
U 1 1 62FC7756
P 2650 1750
F 0 "C3" H 2765 1796 50  0000 L CNN
F 1 ".1uF" H 2765 1705 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 2688 1600 50  0001 C CNN
F 3 "~" H 2650 1750 50  0001 C CNN
	1    2650 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 1600 1150 1600
Wire Wire Line
	2500 1600 2650 1600
Wire Wire Line
	2650 1600 3100 1600
Connection ~ 2650 1600
Wire Wire Line
	1000 1700 1000 1900
Wire Wire Line
	1000 1900 1150 1900
Connection ~ 2650 1900
Wire Wire Line
	2650 1900 3100 1900
Connection ~ 1150 1600
Text GLabel 3100 1900 2    50   Input ~ 0
GND
Text GLabel 3100 1600 2    50   Input ~ 0
5V0
Text GLabel 1100 3050 1    50   Input ~ 0
5V0
Text GLabel 1300 3050 1    50   Input ~ 0
3V3
Text GLabel 1200 4800 3    50   Input ~ 0
GND
Wire Wire Line
	1000 1900 1000 2100
Connection ~ 1000 1900
$Comp
L ws-rescue:EA-DOGM128W-6-DOGM128-sandsim-rescue U3
U 2 1 62FF7B8C
P 8750 4100
F 0 "U3" H 9100 6087 60  0000 C CNN
F 1 "EA-DOGM128W-6" H 9100 5981 60  0000 C CNN
F 2 "CustomParts:DOGM128" V 9150 4600 60  0001 C CNN
F 3 "" H 9100 4050 60  0001 C CNN
F 4 "ELECTRONIC ASSEMBLY" V 9050 4600 60  0001 C CNN "Manufacturer"
	2    8750 4100
	1    0    0    -1  
$EndComp
Connection ~ 8550 2400
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 62F22B45
P 800 1600
F 0 "J1" H 750 1750 50  0000 L CNN
F 1 "PWR" V 880 1501 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 800 1600 50  0001 C CNN
F 3 "~" H 800 1600 50  0001 C CNN
	1    800  1600
	-1   0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 63010160
P 1000 1350
F 0 "#FLG01" H 1000 1425 50  0001 C CNN
F 1 "PWR_FLAG" H 1000 1523 50  0000 C CNN
F 2 "" H 1000 1350 50  0001 C CNN
F 3 "~" H 1000 1350 50  0001 C CNN
	1    1000 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 1350 1000 1600
Connection ~ 1000 1600
Wire Wire Line
	1200 4650 1200 4800
$Comp
L power:PWR_FLAG #FLG02
U 1 1 630192AD
P 1000 2100
F 0 "#FLG02" H 1000 2175 50  0001 C CNN
F 1 "PWR_FLAG" H 1000 2273 50  0000 C CNN
F 2 "" H 1000 2100 50  0001 C CNN
F 3 "~" H 1000 2100 50  0001 C CNN
	1    1000 2100
	1    0    0    1   
$EndComp
$Comp
L Device:CP C2
U 1 1 63023090
P 1600 1750
F 0 "C2" H 1718 1796 50  0000 L CNN
F 1 "1000uF" H 1718 1705 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 1638 1600 50  0001 C CNN
F 3 "~" H 1600 1750 50  0001 C CNN
	1    1600 1750
	1    0    0    -1  
$EndComp
Connection ~ 1150 1900
$Comp
L MCU_Module:WeMos_D1_mini U1
U 1 1 62F0C408
P 1200 3850
F 0 "U1" H 1450 4650 50  0000 C CNN
F 1 "WeMos_D1_mini" V 800 3400 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 1200 2700 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H -650 2700 50  0001 C CNN
	1    1200 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:Solar_Cell SC1
U 1 1 62F0A31C
P 4450 1550
F 0 "SC1" H 4282 1646 50  0000 R CNN
F 1 "Solar_Cell" H 4282 1555 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" V 4450 1610 50  0001 C CNN
F 3 "~" V 4450 1610 50  0001 C CNN
	1    4450 1550
	-1   0    0    -1  
$EndComp
Text GLabel 3900 1800 0    50   Input ~ 0
GND
Text GLabel 3900 1500 0    50   Input ~ 0
A0
$Comp
L Device:R R4
U 1 1 62F1F5D9
P 4150 1350
F 0 "R4" H 4220 1396 50  0000 L CNN
F 1 "10k" H 4220 1305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4080 1350 50  0001 C CNN
F 3 "~" H 4150 1350 50  0001 C CNN
	1    4150 1350
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 62F2A697
P 4150 1650
F 0 "R5" H 4220 1696 50  0000 L CNN
F 1 "10k" H 4220 1605 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4080 1650 50  0001 C CNN
F 3 "~" H 4150 1650 50  0001 C CNN
	1    4150 1650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3900 1500 4150 1500
Connection ~ 4150 1500
Wire Wire Line
	4150 1200 4450 1200
Wire Wire Line
	4450 1200 4450 1350
Wire Wire Line
	3900 1800 4150 1800
Wire Wire Line
	4150 1800 4450 1800
Wire Wire Line
	4450 1800 4450 1650
Connection ~ 4150 1800
Text GLabel 1600 3350 2    50   Input ~ 0
A0
Text GLabel 1600 3550 2    50   Input ~ 0
D1
Text GLabel 1600 3650 2    50   Input ~ 0
D2
Text GLabel 5300 4100 0    50   Input ~ 0
D1
Text GLabel 5300 4500 0    50   Input ~ 0
D2
Text GLabel 6100 4900 3    50   Input ~ 0
GND
Text GLabel 6100 3700 1    50   Input ~ 0
3V3
Wire Wire Line
	9700 1150 9700 1450
Wire Wire Line
	9700 1450 9700 1750
Text GLabel 1600 4150 2    50   Input ~ 0
D7
Text GLabel 5300 2300 0    50   Input ~ 0
D7
Text GLabel 1600 3950 2    50   Input ~ 0
D5
Text GLabel 5300 2400 0    50   Input ~ 0
D5
Text GLabel 5300 3100 0    50   Input ~ 0
D0
Text GLabel 1600 3450 2    50   Input ~ 0
D0
Text GLabel 1600 4250 2    50   Input ~ 0
D8
Text GLabel 5300 2800 0    50   Input ~ 0
D8
Text GLabel 5300 2700 0    50   Input ~ 0
D3
Text GLabel 1600 3750 2    50   Input ~ 0
D3
Text GLabel 5700 1900 0    50   Input ~ 0
GND
Text GLabel 5700 1500 0    50   Input ~ 0
RX
Text GLabel 800  3750 0    50   Input ~ 0
RX
Wire Wire Line
	5700 1500 5900 1500
Wire Wire Line
	5700 1900 5900 1900
Connection ~ 5900 1500
Text GLabel 5700 1200 0    50   Input ~ 0
3V3
Wire Wire Line
	5700 1200 5900 1200
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 62F68077
P 2650 3250
F 0 "J2" H 2700 3500 50  0000 C CNN
F 1 "Recevier" V 2750 3150 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2650 3250 50  0001 C CNN
F 3 "~" H 2650 3250 50  0001 C CNN
	1    2650 3250
	-1   0    0    -1  
$EndComp
$Comp
L Sensor:BME280_I2C_Board U4
U 1 1 62F0691B
P 5900 4300
F 0 "U4" H 6300 4800 50  0000 R CNN
F 1 "BME280_I2C_Board" H 6500 3850 50  0000 R CNN
F 2 "CustomParts:BME280_Board" H 7400 3850 50  0001 C CNN
F 3 "" H 5900 4100 50  0001 C CNN
	1    5900 4300
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 62F94143
P 3300 3100
F 0 "R1" V 3507 3100 50  0000 C CNN
F 1 "100R" V 3416 3100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 3100 50  0001 C CNN
F 3 "~" H 3300 3100 50  0001 C CNN
	1    3300 3100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3850 3450 3850 3300
Wire Wire Line
	2850 3250 2850 3350
Wire Wire Line
	3150 3250 3150 3100
Wire Wire Line
	2850 3250 3150 3250
Connection ~ 2850 3250
$Comp
L Device:R R3
U 1 1 62FA926F
P 3850 2750
F 0 "R3" H 3781 2704 50  0000 R CNN
F 1 "10K" H 3781 2795 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3780 2750 50  0001 C CNN
F 3 "~" H 3850 2750 50  0001 C CNN
	1    3850 2750
	1    0    0    1   
$EndComp
Wire Wire Line
	2850 3450 3500 3450
$Comp
L Device:R R2
U 1 1 62F9675D
P 3500 3300
F 0 "R2" H 3431 3254 50  0000 R CNN
F 1 "10K" H 3431 3345 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3430 3300 50  0001 C CNN
F 3 "~" H 3500 3300 50  0001 C CNN
	1    3500 3300
	1    0    0    1   
$EndComp
$Comp
L Transistor_FET:2N7000 Q1
U 1 1 62F8CF15
P 3750 3100
F 0 "Q1" H 3954 3146 50  0000 L CNN
F 1 "TN0604G" H 3900 2900 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3950 3025 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 3750 3100 50  0001 L CNN
	1    3750 3100
	1    0    0    -1  
$EndComp
Connection ~ 3500 3450
Wire Wire Line
	3500 3450 3850 3450
Connection ~ 3850 2900
Wire Wire Line
	3850 2900 4300 2900
Wire Wire Line
	3450 3100 3500 3100
Wire Wire Line
	3500 3150 3500 3100
Connection ~ 3500 3100
Wire Wire Line
	3500 3100 3550 3100
Wire Wire Line
	2850 3150 2850 2450
Wire Wire Line
	2850 2450 4300 2450
Wire Wire Line
	3850 3450 4300 3450
Connection ~ 3850 3450
Wire Wire Line
	3850 2600 4300 2600
Text GLabel 4300 2450 2    50   Input ~ 0
5V0
Text GLabel 4300 2600 2    50   Input ~ 0
3V3
Text GLabel 4300 3450 2    50   Input ~ 0
GND
Text GLabel 1600 3850 2    50   Input ~ 0
D4
Text GLabel 4300 2900 2    50   Input ~ 0
D4
Wire Wire Line
	9350 -850 9350 -250
Connection ~ 9700 1450
$Comp
L Transistor_FET:TP0610L Q2
U 1 1 62FDF5B9
P 7550 1550
F 0 "Q2" H 7700 1300 50  0000 L CNN
F 1 "TP0610L" H 7700 1400 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92L_Inline" H 7750 1475 50  0001 L CIN
F 3 "http://www.vishay.com/docs/70209/70209.pdf" H 7550 1550 50  0001 L CNN
	1    7550 1550
	1    0    0    1   
$EndComp
Wire Wire Line
	7650 1350 7650 1050
Text GLabel 6900 1550 0    50   Input ~ 0
TX
Wire Wire Line
	6900 1050 7250 1050
Wire Wire Line
	7200 1550 7250 1550
Wire Wire Line
	7250 1450 7250 1550
Connection ~ 7250 1550
Wire Wire Line
	7250 1550 7350 1550
Wire Wire Line
	7250 1150 7250 1050
Connection ~ 7250 1050
Wire Wire Line
	7250 1050 7650 1050
Wire Wire Line
	9700 1450 9850 1450
$Comp
L Device:R R11
U 1 1 62F291F0
P 8450 1750
F 0 "R11" V 8550 1800 50  0000 C CNN
F 1 "60R" V 8350 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8380 1750 50  0001 C CNN
F 3 "~" H 8450 1750 50  0001 C CNN
	1    8450 1750
	0    -1   -1   0   
$EndComp
Text GLabel 800  3850 0    50   Input ~ 0
TX
$Comp
L Regulator_Linear:L7805 U2
U 1 1 63044E73
P 2200 1600
F 0 "U2" H 2200 1842 50  0000 C CNN
F 1 "L7805" H 2200 1751 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 2225 1450 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 2200 1550 50  0001 C CNN
	1    2200 1600
	1    0    0    -1  
$EndComp
Connection ~ 2200 1900
Wire Wire Line
	2200 1900 2650 1900
Connection ~ 1600 1600
Wire Wire Line
	1600 1600 1900 1600
Connection ~ 1600 1900
Wire Wire Line
	1600 1900 2200 1900
Wire Wire Line
	1150 1900 1600 1900
Wire Wire Line
	1150 1600 1600 1600
NoConn ~ 800  3450
NoConn ~ 1600 4050
$Comp
L Device:R R10
U 1 1 63065B62
P 8450 1450
F 0 "R10" V 8550 1500 50  0000 C CNN
F 1 "60R" V 8350 1500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8380 1450 50  0001 C CNN
F 3 "~" H 8450 1450 50  0001 C CNN
	1    8450 1450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R9
U 1 1 630661A7
P 8450 1150
F 0 "R9" V 8550 1200 50  0000 C CNN
F 1 "60R" V 8350 1200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8380 1150 50  0001 C CNN
F 3 "~" H 8450 1150 50  0001 C CNN
	1    8450 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7650 1750 8100 1750
Wire Wire Line
	8100 1750 8100 1450
Wire Wire Line
	8100 1150 8300 1150
Connection ~ 8100 1750
Wire Wire Line
	8100 1750 8300 1750
Wire Wire Line
	8100 1450 8300 1450
Connection ~ 8100 1450
Wire Wire Line
	8100 1450 8100 1150
$EndSCHEMATC
