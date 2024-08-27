/*
 * @Author: Ruben Salas Ramirez
 * @Author: Joseph David Jimenez Zuniga
 * CLOCK ALARM on CPU NIOS
 */
#include <stdio.h>
#include <system.h>
#include <sys/alt_irq.h>
#include <altera_avalon_timer_regs.h>
#include <altera_avalon_pio_regs.h>
//Function statements
void init_timer_interrupt(void);
static void timer_isr(void *context, alt_u32 id);
void check_buttons(void);
void setup(void);
void toggle_leds(void);
void display_seven_segments(int hours, int mins, int secs);
void alarm(void);
/*
 *  Alarm global variables
 *  0 : CLOCK
 *  1 : SET CLOCK TIME
 *  2 : CLOCK ALARM
 *  3 : SET CLOCK ALARM
 *  4 : ALARM
 */
static int watch_state = 0;
static int last_watch_state = 0;
static int actual_hours = 12;
static int actual_minutes = 34;
static int actual_seconds = 0;
static int temp_hours = 0;
static int temp_minutes = 0;
static int temp_seconds = 0;
static int alarm_hours = 0;
static int alarm_minutes = 0;
static int alarm_seconds = 0;
static int setting_hours = 1;		// Variable for edit mode
static int leds_interval = 0;		// Variable handle LEDs toggle
static int alarm_time_left = 30;	// Time of alarm interaction
unsigned int actual_button_status = 0xf;
unsigned int actual_switch_status = 0x0;
int main(void) {
    init_timer_interrupt(); // Sets timer for interruptions
    while (1);
    return 0;
}
/*
 * @ Function: init_timer_interrupt()
 * @ Description: Allows timer interruption
 */
void init_timer_interrupt(void) {
	alt_ic_isr_register(TIMER_INTERRUPTION_IRQ_INTERRUPT_CONTROLLER_ID,TIMER_INTERRUPTION_IRQ,
						(void *)timer_isr,NULL,0x0);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_INTERRUPTION_BASE,
                                     ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
                                     ALTERA_AVALON_TIMER_CONTROL_START_MSK |
                                     ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);
}
/*
 * @ Function: timer_isr(context,id)
 * @ Description: Handles available interruptions
 */
static void timer_isr(void *context, alt_u32 id) {
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_INTERRUPTION_BASE, 0);//Deletes interruption
    actual_seconds++;
    // The range for seconds, minutes and hours
    if (actual_seconds == 60) {
    	actual_seconds = 0;
    	actual_minutes++;
    }
    if (actual_minutes == 60) {
    	actual_minutes = 0;
    	actual_hours++;
    }
    if (actual_hours == 24) {
    	actual_hours = 0;
    }
	check_buttons();
	toggle_leds();
	alarm();
}
/*
 * @ Function: check_buttons()
 * @ Description: Each second checks if the button is pressed
 */
void check_buttons(void) {
	actual_button_status = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);
	actual_switch_status = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
	if (watch_state == 0 || watch_state == 2) {
		display_seven_segments(actual_hours, actual_minutes, actual_seconds);// Display 7 segments output
		// Turn ON first switch ([0][0][0][0][0][0][0][0][0][1])
		if (actual_switch_status == 0x1) {
			last_watch_state = watch_state;
			watch_state = 1;// SET CLOCK TIME
			temp_hours = actual_hours;
			temp_minutes = actual_minutes;
			temp_seconds = 0;
		}
		// Turn ON switch ([0][0][0][0][0][0][0][0][1][0])
		else if (actual_switch_status == 0x2) {
			last_watch_state = watch_state;
			watch_state = 3;//  SET CLOCK ALARM
			temp_hours = actual_hours;
			temp_minutes = actual_minutes;
			temp_seconds = 0;
		}
	}
	// STATE: SET CLOCK TIME
	else if (watch_state == 1) {
		setup(); // Begins setup step
		// CONFIRMATION SET CLOCK TIME : First Button Pressed ([X][0][0][0])
		if (actual_button_status == 0x7) {
			// Back to last state
			watch_state = last_watch_state;
			last_watch_state = 1;
			// Updates new hour
			actual_hours = temp_hours;
			actual_minutes = temp_minutes;
			actual_seconds = 0;
		}
		// CANCEL SET CLOCK TIME : Two last Buttons Pressed ([0][0][X][X])
		else if (actual_button_status == 0xc) {
			// Back to last state
			watch_state = last_watch_state;
			last_watch_state = 1;
		}
	}
	// STATE: SET ALARM TIME
	else if (watch_state == 3) {
		setup(); // Begins setup step
		// CONFIRMATION SET ALARM CLOCK TIME : First Button Pressed ([X][0][0][0])
		if (actual_button_status == 0x7) {
			last_watch_state = 3;
			watch_state = 2;
			// SETS ALARM HOUR
			alarm_hours = temp_hours;
			alarm_minutes = temp_minutes;
			alarm_seconds = 0;
		}
		// CANCEL SET ALARM CLOCK TIME : Two last Buttons Pressed ([0][0][X][X])
		else if (actual_button_status == 0xc) {
			last_watch_state = 3;
			watch_state = 0;
		}
	}
	// STATE: ALARM
	else if (watch_state == 4) {
		if (actual_button_status == 0x0 || alarm_time_left == 0) {
			last_watch_state = watch_state;
			watch_state = 0;
			alarm_time_left = 30;
			IOWR_ALTERA_AVALON_PIO_DATA(BUZZER_BASE, 0x0);// TURNS OFF BUZZER
		}
	}
}
/*
 * @ Function: setup()
 * @ Description: Setup for clock time and alarm
 */
void setup(void){
	display_seven_segments(temp_hours, temp_minutes, temp_seconds);
	if (setting_hours) {
		// UP VALUE: PRESS SECOND BUTTON ([0][0][X][0])
		if (actual_button_status == 0xd) {
			temp_hours++;
			if (temp_hours == 24) {
				temp_hours = 0;
			}
		}
		// DOWN VALUE: PRESS FIRST BUTTON ([0][0][0][X])
		else if (actual_button_status == 0xe) {
			temp_hours--;
			if (temp_hours == -1) {
				temp_hours = 23;
			}
		}
		// CHANGE BETW VALUES: PRESS THIRD BUTTON ([0][X][0][0])
		else if (actual_button_status == 0xb) {
			setting_hours = !setting_hours;
		}
	}
	else if (!setting_hours) {
		if (actual_button_status == 0xd) {
			temp_minutes++;
			if (temp_minutes == 60) {
				temp_minutes = 0;
			}
		}
		else if (actual_button_status == 0xe) {
			temp_minutes--;
			if (temp_minutes == -1) {
				temp_minutes = 59;
			}
		}
		else if (actual_button_status == 0xb) {
			setting_hours = !setting_hours;
		}
	}
}
/*
 * @ Function: toggle_leds()
 * @ Description: Allows LED control
 */
void toggle_leds() {
	if (watch_state == 0) {
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x200);  // On state 0 LED: [(1)(0)(0)(0)(0)(0)(0)(0)(0)(0)]
    }
	else if (watch_state == 1) {
		if (setting_hours){
			if (leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x201); // On state 1 Clock Hour LEDs: [(1)(0)(0)(0)(0)(0)(0)(0)(0)(1)]
			} else if (!leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x101); // On state 1 Clock Hour LEDs: [(0)(1)(0)(0)(0)(0)(0)(0)(0)(1)]
			}
		} else if (!setting_hours){
			if (leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x41); // On state 1 Clock Minutes LEDs: [(0)(0)(0)(1)(0)(0)(0)(0)(0)(1)]
			} else if (!leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x21); // On state 1 Clock Minutes LEDs: [(0)(0)(0)(0)(1)(0)(0)(0)(0)(1)]
			}
		}
		leds_interval = !leds_interval;
    }
	else if (watch_state == 2) {
		IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x300);  // On state 2 Clock with Alarm LEDs: [(1)(1)(0)(0)(0)(0)(0)(0)(0)(0)]
    }
	else if (watch_state == 3) {
		if (setting_hours){
			if (leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x202); // On state 3 Alarm Clock Hour LEDs: [(1)(0)(0)(0)(0)(0)(0)(0)(1)(0)]
			} else if (!leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x102); // On state 3 Alarm Clock Hour LEDs: [(0)(1)(0)(0)(0)(0)(0)(0)(1)(0)]
			}
		} else if (!setting_hours){
			if (leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x42); // On state 3 Alarm Clock Minutes LEDs:[(0)(0)(0)(1)(0)(0)(0)(0)(1)(0)]
			} else if (!leds_interval){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x22); // On state 3 Alarm Clock Minutes LEDs: [(0)(0)(0)(0)(1)(0)(0)(0)(1)(0)]
			}
		}
		leds_interval = !leds_interval;
    }
	else if (watch_state == 4) {
		if (leds_interval){
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x2aa); // On state 4 Alarm LEDs: [(1)(0)(1)(0)(1)(0)(1)(0)(1)(0)]
		} else if (!leds_interval){
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x155); // On state 4 Alarm LEDs:[(0)(1)(0)(1)(0)(1)(0)(1)(0)(1)]
		}
    } else {
    	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, 0x3ff); // LED interval change
    }
}
/*
 * @ Function: alarm()
 * @ Description: Allows LED control
 */
void alarm(void){
	if (watch_state == 2) {
		if (actual_seconds == 0 && actual_minutes == alarm_minutes && actual_hours == alarm_hours){
			watch_state = 4;
		}
	} else if (watch_state == 4) {
		IOWR_ALTERA_AVALON_PIO_DATA(BUZZER_BASE, 0x1); //Buzzer sounds
		if (leds_interval){
			display_seven_segments(actual_hours, actual_minutes, 0); // TURN ON LEDs: [(1)(0)(1)(0)(1)(0)(1)(0)(1)(0)]
		} else if (!leds_interval){
			display_seven_segments(-1, -1, -1); // TURN ON LEDs: [(0)(1)(0)(1)(0)(1)(0)(1)(0)(1)]
		}
		leds_interval = !leds_interval;
		alarm_time_left--;
	}
}
/*
 * Lookup Table for digit mapping [0,9] to his binary value on 7 segments display
 */
const alt_u8 segment_map[10] = {
    0xC0, // 0 -> 0x3F inverted
    0xF9, // 1 -> 0x06 inverted
    0xA4, // 2 -> 0x5B inverted
    0xB0, // 3 -> 0x4F inverted
    0x99, // 4 -> 0x66 inverted
    0x92, // 5 -> 0x6D inverted
    0x82, // 6 -> 0x7D inverted
    0xF8, // 7 -> 0x07 inverted
    0x80, // 8 -> 0x7F inverted
    0x90  // 9 -> 0x6F inverted
};
/*
 * @ Function: display_seven_segments(hours, mins, secs)
 * @ Description: Sends values to Display
 */
void display_seven_segments(int hours, int mins, int secs) {
	if (hours != -1) {
		// Mapping
		alt_u8 sevseg_hour_tens  = segment_map[hours / 10];
		alt_u8 sevseg_hour_units = segment_map[hours % 10];
		alt_u8 sevseg_mins_tens  = segment_map[mins / 10];
		alt_u8 sevseg_mins_units = segment_map[mins % 10];
		alt_u8 sevseg_secs_tens  = segment_map[secs / 10];
		alt_u8 sevseg_secs_units = segment_map[secs % 10];
		// Write hardware register
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_HOURS_TENS_BASE, sevseg_hour_tens);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_HOURS_UNITS_BASE, sevseg_hour_units);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_MINS_TENS_BASE, sevseg_mins_tens);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_MINS_UNITS_BASE, sevseg_mins_units);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_SECS_TENS_BASE, sevseg_secs_tens);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_SECS_UNITS_BASE, sevseg_secs_units);
	} else {
		// Read hardware register
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_SECS_UNITS_BASE, 0x7F);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_SECS_TENS_BASE, 0x7F);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_MINS_UNITS_BASE, 0x7F);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_MINS_TENS_BASE, 0x7F);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_HOURS_UNITS_BASE, 0x7F);
		IOWR_ALTERA_AVALON_PIO_DATA(SEVSEG_HOURS_TENS_BASE, 0x7F);
	}
}
