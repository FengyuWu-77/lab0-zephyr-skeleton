/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 2000 msec = 2 sec */
#define SLEEP_TIME_MS   2000

/* The devicetree node identifier for the "led5180" alias. */
#define LED5180_NODE DT_ALIAS(led5180)
#define BUTTON5180_NODE DT_ALIAS(button5180)
/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED5180_NODE, gpios);
static const struct gpio_dt_spec button =
	GPIO_DT_SPEC_GET(BUTTON5180_NODE, gpios);

int main(void)
{
	int ret;
	bool led_state = false;
	bool last_button_state = false;

	if (!gpio_is_ready_dt(&led) ||
	    !gpio_is_ready_dt(&button)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		int button_state = gpio_pin_get_dt(&button);

		if (button_state < 0) {
			return 0;
		}

		if (button_state && !last_button_state) {
			led_state = !led_state;

			ret = gpio_pin_set_dt(&led, led_state);
			if (ret < 0) {
				return 0;
			}

			printf("LED state: %s\n",
			       led_state ? "ON" : "OFF");
		}

		last_button_state = button_state;
		k_msleep(20);
	}

	return 0;
}