# Traffic-Light-PIC24J64GB002-
Simple traffic management system that simulates 4 traffic lights at a crossroad using a PIC24J64GB002 microcontroller. Timer 2 interrupts and change notifications were used to initiate state changes. Change notifications employed in simulating changes in traffic light timer delay to exhibit a shorter delay during office hours and a longer delay during non-office hours. (hours of heavy traffic). 
Pictorial illustration of scenario shown in Conceptualization.png.

Traffic light system electrical connection pinout is shown below

TRAFFIC LIGHT-COLOR-I/O PORT PIN
Main1 - Green - RB0	
Main1 - Yellow -RB1	
Main1 - Red -	RB2	
Main2 - Green -	RB3	
Main2 - Yellow - RB4	
Main2 - Red -	RB5	
Small1 - Green - RB7	
Small1 - Yellow -	RB8	
Small1 - Red - RB9	
Small2 - Green - RB13	
Small2 - Yellow -	RB14	
Small2 - Red - RB15
Change Notification Pin - N/A - CN0(RA4)

Flow chart for implementation is show in FlowChart.png.
Coding was done in MPLAB IDE in C. File containing code is found in Traffic_Light.c
Code can be used to used to simulate any integer value wait time with only one timer even beyond the 16-bit limit.

