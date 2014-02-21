

#ifndef QUAD_BUTTONS_H
#define QUAD_BUTTONS_H

#include "quad_common.h"
#include "quad_motor.h"

// We want the buttons to be "OR"-able so we can detect multiple button presses
// simultaneously.
enum Buttons {
	LEFT_BUTTON = 1,
	RIGHT_BUTTON = 2,
	E_STOP_BUTTON = 4
};

// Globals to keep track of button presses and releases
volatile bool button_pressed_right;
volatile bool button_pressed_left;
volatile bool button_pressed_e_stop;
volatile bool button_pressed_left_and_right;

volatile bool button_released_right;
volatile bool button_released_left;
volatile bool button_released_left_and_right;
volatile bool button_released_e_stop;


// Functions
void quad_buttons_init();
void quad_buttons_handle_button_events(int button);
int quad_buttons_get_button_events();

#endif //QUAD_BUTTONS_H
