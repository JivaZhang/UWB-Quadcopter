

#ifndef QUAD_MOTOR_H
#define QUAD_MOTOR_H

#include "quad_common.h"
#include "driverlib/pwm.h"

// Preprocessor Definitions

// us == microseconds
// ms == milliseconds
// Hz == Hertz (Cycles per second)
#define MIN_PULSE_WIDTH_us	700		// 0.7 ms
#define MAX_PULSE_WIDTH_us	2000	// 2.0 ms
#define PWM_PERIOD_us		20000	// 20  ms
#define PULSE_TIME_RES_Hz	1000000	// 1000000 Hz = 1 us / cycle

#define MAX_VALUE			700

// Enums and Structs
enum Motors {
	MOTOR_1,
	MOTOR_2,
	MOTOR_3,
	MOTOR_4
};

// Global Variables:
volatile uint32_t min_pulse_ticks;
volatile uint32_t max_pulse_ticks;
volatile uint32_t period_num_ticks;

volatile uint32_t pwm_clock_freq_hz;

int prev_val_motor_1;
int prev_val_motor_2;
int prev_val_motor_3;
int prev_val_motor_4;


// Functions:
void quad_motors_init() ;
void quad_motors_calibrate();

// 'value' in quad_motors_set_value() should be a value between 0 and 1000.  
// If a value greater than 1000 is passed, it will be truncated to 1000.  Based
// on PWM clock of 500000 Hz, and the min pulse width of 0.5 ms, and a max pulse
// width of 2.5 ms, then value equates exactly to the number of ticks.  This is
// quite convenient and means that we do not lose any precision while converting
// to and from nd as such, should only be modified if absolutely
// necessary.  
void quad_motors_set_value(int motor, int value);
int quad_motors_get_last_val(int motor);

// helper function.  Calculates the number of ticks based on the min_pulse_ticks,
// max_pulse_ticks, and the value passed in to quad_motors_set_value().  The 
// number returned will be a rounded value, and not the result of integer
// division.  
uint32_t calculate_num_ticks();


#endif // QUAD_MOTOR_H
