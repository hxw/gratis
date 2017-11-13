// Copyright 2013-2015 Pervasive Displays, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied.  See the License for the specific language
// governing permissions and limitations under the License.

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <libgpio.h>

#include "gpio.h"

// global handle to access gpio
gpio_handle_t handle = GPIO_INVALID_HANDLE;


// set up access to the GPIO and PWM
bool GPIO_setup() {

	handle = gpio_open(0);
	if (handle == GPIO_INVALID_HANDLE) {
		warn("gpio_open failed");
		return false;
	}

	return true;
}


// revoke access to GPIO and PWM
bool GPIO_teardown() {
	if (handle != GPIO_INVALID_HANDLE) {
		gpio_close(handle);
	}

	handle = GPIO_INVALID_HANDLE;

	return true;
}


void GPIO_mode(GPIO_pin_type pin, GPIO_mode_type mode) {
	switch (mode) {
	default:
	case GPIO_INPUT:
		gpio_pin_input(handle, pin);
		break;
	case GPIO_OUTPUT:
		gpio_pin_output(handle, pin);
		break;
	case GPIO_PWM:  // only certain pins allowed
		if (GPIO_P1_12 == pin) {
			// not actually available
			gpio_pin_output(handle, pin);
		}
		break;
	}
}


int GPIO_read(GPIO_pin_type pin) {
	if ((unsigned)(pin) > 63) {
		return 0;
	}
	gpio_value_t v = gpio_pin_get(handle, pin);
	if (GPIO_VALUE_LOW == v) {
		return 0;
	} else {
		return 1;
	}
}


void GPIO_write(GPIO_pin_type pin, int value) {
	if ((unsigned)(pin) > 63) {
		return;
	}
	gpio_pin_set(handle, pin, (value != 0) ? GPIO_VALUE_HIGH : GPIO_VALUE_LOW);
}


// only affetct PWM if correct pin is addressed
void GPIO_pwm_write(GPIO_pin_type pin, uint32_t value) {
	if (GPIO_P1_12 == pin) {
		// not available
	}
}
