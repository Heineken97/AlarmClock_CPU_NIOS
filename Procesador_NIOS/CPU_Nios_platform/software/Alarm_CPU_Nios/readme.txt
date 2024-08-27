CLOCK ALARM on CPU NIOS
Project Overview
This project implements a digital clock with an alarm feature on a NIOS II soft processor synthesized onto an FPGA. The clock displays the current time and allows users to set and activate an alarm. The user can interact with the system via buttons and switches to set the time, configure the alarm, and control the LED indicators.

Features
Real-Time Clock: Displays the current time in hours, minutes, and seconds.
Alarm Functionality: Allows setting an alarm time. When the set time is reached, the alarm is triggered with a buzzer sound and LED indicators.
User Interaction: Configurable via buttons and switches:
Set the current time.
Set the alarm time.
Cancel or confirm settings.
LED Indicators: Visual feedback for different states (normal operation, setting time, alarm triggered).
Seven-Segment Display: Displays the current time and the alarm time during the setup.
Hardware Requirements
FPGA Board with Altera/Intel Quartus support.
NIOS II Soft Processor configured on the FPGA.
LEDs and Seven-Segment Displays for visual output.
Buttons and Switches for user interaction.
Buzzer for alarm sound.
Software Components
NIOS II IDE: C-based embedded programming environment.
Quartus Prime: FPGA development software for hardware synthesis and integration.
Code Structure
init_timer_interrupt(): Initializes the timer interrupt for the clock.
timer_isr(): Interrupt service routine to update time every second.
check_buttons(): Checks the status of buttons and switches to handle user input.
setup(): Manages the setup process for setting the clock and alarm time.
toggle_leds(): Controls the LED states based on the current watch state.
alarm(): Handles the alarm activation and deactivation process.
display_seven_segments(): Displays the current or set time on the seven-segment displays.
How to Use
Compile and Load: Compile the provided code and load it onto the FPGA board with the NIOS II processor.
Set the Clock: Use the switches to enter the clock setting mode and adjust the time using the buttons.
Set the Alarm: Similarly, use the switches to enter the alarm setting mode and set the desired alarm time.
Normal Operation: Once the time and alarm are set, the clock will run in normal mode. The alarm will trigger when the set time is reached.
Stop the Alarm: Press any button to stop the alarm when it sounds.
Authors
Ruben Salas Ramirez
Joseph David Jimenez Zuniga
License
This project is licensed under the MIT License - see the LICENSE file for details.

Acknowledgments
Altera/Intel for providing the Quartus Prime software and NIOS II development environment.
University/Institution (if applicable) for resources and support during the development of this project.
