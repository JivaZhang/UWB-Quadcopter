

#include "quad_common.h"
#include "quad_pwm.h"
#include "quad_i2c.h"
#include "quad_uart.h"
#include "quad_buttons.h"
#include "quad_motor.h"
#include "quad_9_axis.h"
#include "quad_rgb_led.h"
#include "quad_ranger.h"
#include "quad_system_time.h"
#include "quad_serial.h"



void setup() {
	// We need to set the clock before anything else!
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	quad_pwm_init();
	quad_i2c_init();
	
	// Since we use the RGB Led for debugging, it should be initialized before
	// everything else.
	quad_rgb_led_init();
	quad_uart_init();
	quad_rgb_led_set_color(BLUE);
	
	quad_motors_init();
	quad_rgb_led_set_color(YELLOW);
	quad_buttons_init();
	quad_rgb_led_set_color(RED);
	quad_uart_init();
	quad_rgb_led_set_color(MAGENTA);
	quad_9_axis_init();
	quad_rgb_led_set_color(CYAN);
	quad_system_time_init();
	quad_rgb_led_set_color(WHITE);

	quad_ranger_init();
	
}




int main() {
	setup();
	quad_rgb_led_set_color(GREEN); // Green light indicates that we have
								   // successfully finished initializing the 
								   // quadcopter.



	uint32_t micros_prev = 0;
	uint32_t micros_cur = 0;
	uint32_t micros_dif = 0;

	uint32_t seconds_prev = 0;
	uint32_t seconds_cur = 0;
	uint32_t seconds_dif = 0;

	while(1) {
		// Handle button events
		int button_events = quad_buttons_get_button_events();
		quad_buttons_handle_button_events(button_events);

		// Get Sonar Data
		uint32_t dist = quad_ranger_get_last_distance();

		// Get 9-axis Data
		quad_9_axis_read_raw_data();

		// Get the new time
		micros_cur = get_system_time_micros();
		seconds_cur = get_system_time_seconds();
		seconds_dif = seconds_cur - seconds_prev;

		if (seconds_cur > seconds_prev) {
			micros_dif = (1000000 * seconds_dif) - (micros_prev - micros_cur);
		} else {
			micros_dif = micros_cur - micros_prev;
		}

		// Print out all of the data
		// 9-axis
		serial_put_short((int16_t)nar_cur.gyro_x);
		UARTCharPut(UART0_BASE, ',');
		UARTCharPut(UART0_BASE, ' ');
		serial_put_short((int16_t)nar_cur.gyro_y);
		UARTCharPut(UART0_BASE, ',');
		UARTCharPut(UART0_BASE, ' ');
		serial_put_short((int16_t)nar_cur.gyro_z);
		UARTCharPut(UART0_BASE, ',');
		UARTCharPut(UART0_BASE, ' ');
		serial_put_short((int16_t)nar_cur.accel_x);
		UARTCharPut(UART0_BASE, ',');
		UARTCharPut(UART0_BASE, ' ');
		serial_put_short((int16_t)nar_cur.accel_y);
		UARTCharPut(UART0_BASE, ',');
		UARTCharPut(UART0_BASE, ' ');
		serial_put_short((int16_t)nar_cur.accel_z);
		UARTCharPut(UART0_BASE, ',');
		UARTCharPut(UART0_BASE, ' ');
		// Distance
		serial_put_int(dist);
		UARTCharPut(UART0_BASE, ',');
		UARTCharPut(UART0_BASE, ' ');
		// Time
		serial_put_int(micros_dif);
		
		// New Line
		serial_put_string("\n\r");


		// Update the previous time.
		seconds_prev = seconds_cur;
		micros_prev = micros_cur;
	}

}
