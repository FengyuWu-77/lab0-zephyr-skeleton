/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>

LOG_MODULE_REGISTER(lab0_bme280, LOG_LEVEL_INF);

#if defined(CONFIG_SUM_PRINT)
#include "sum_printk.h"
#elif defined(CONFIG_SUM_LOG)
#include "sum_log.h"
#endif

/* 2000 msec = 2 sec */
#define SLEEP_TIME_MS   2000

/* BME280 register addresses used by Lab 0. */
#define BME280_REG_DIG_T1_LSB 0x88
#define BME280_REG_CTRL_MEAS  0xF4
#define BME280_REG_TEMP_MSB   0xFA

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
static const struct i2c_dt_spec bme280 =
	I2C_DT_SPEC_GET(DT_NODELABEL(bme280_lab));

struct bme280_calibration {
	uint16_t dig_t1;
	int16_t dig_t2;
	int16_t dig_t3;
};

static int bme280_read_calibration(struct bme280_calibration *cal)
{
	uint8_t data[6];
	int ret;

	ret = i2c_burst_read_dt(&bme280, BME280_REG_DIG_T1_LSB,
				       data, sizeof(data));
	if (ret < 0) {
		return ret;
	}

	cal->dig_t1 = sys_get_le16(&data[0]);
	cal->dig_t2 = (int16_t)sys_get_le16(&data[2]);
	cal->dig_t3 = (int16_t)sys_get_le16(&data[4]);

	return 0;
}

static int bme280_configure(void)
{
	const uint8_t ctrl_meas[] = {BME280_REG_CTRL_MEAS, 0x27};

	/* Temperature x1, pressure x1, normal measurement mode. */
	return i2c_write_dt(&bme280, ctrl_meas, sizeof(ctrl_meas));
}

static int bme280_read_temperature(const struct bme280_calibration *cal,
				   int32_t *temperature_centi_c)
{
	uint8_t raw[3];
	int32_t adc_t;
	int32_t var1;
	int32_t var2;
	int32_t t_fine;
	int ret;

	ret = i2c_burst_read_dt(&bme280, BME280_REG_TEMP_MSB,
				       raw, sizeof(raw));
	if (ret < 0) {
		return ret;
	}

	adc_t = ((int32_t)raw[0] << 12) |
		((int32_t)raw[1] << 4) |
		(raw[2] >> 4);

	/* Bosch BME280 integer temperature compensation formula. */
	var1 = ((((adc_t >> 3) - ((int32_t)cal->dig_t1 << 1)) *
		 (int32_t)cal->dig_t2) >> 11);
	var2 = (((((adc_t >> 4) - (int32_t)cal->dig_t1) *
		  ((adc_t >> 4) - (int32_t)cal->dig_t1)) >> 12) *
		 (int32_t)cal->dig_t3) >> 14;
	t_fine = var1 + var2;
	*temperature_centi_c = (t_fine * 5 + 128) >> 8;

	return 0;
}

static int bme280_init(struct bme280_calibration *cal)
{
	int ret;

	if (!device_is_ready(bme280.bus)) {
		return -ENODEV;
	}

	ret = bme280_read_calibration(cal);
	if (ret < 0) {
		return ret;
	}

	ret = bme280_configure();
	if (ret < 0) {
		return ret;
	}

	/* Allow the first normal-mode measurement to complete. */
	k_msleep(100);
	return 0;
}

int main(void)
{
	int ret;
	int64_t next_temperature_ms = 0;
	struct bme280_calibration bme280_cal;
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

	ret = bme280_init(&bme280_cal);
	if (ret < 0) {
		LOG_ERR("BME280 initialization failed: %d", ret);
		return 0;
	}

	LOG_INF("BME280 initialized at I2C address 0x77");

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

#if defined(CONFIG_SUM_PRINT)
	(void)sum_printk(7, 5);
#elif defined(CONFIG_SUM_LOG)
	(void)sum_log(7, 5);
#endif

	while (1) {
		int64_t now_ms = k_uptime_get();
		int button_state = gpio_pin_get_dt(&button);
		int32_t temperature_centi_c;

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

		if (now_ms >= next_temperature_ms) {
			ret = bme280_read_temperature(&bme280_cal,
						      &temperature_centi_c);
			if (ret < 0) {
				LOG_ERR("BME280 temperature read failed: %d", ret);
			} else {
				int32_t whole = temperature_centi_c / 100;
				int32_t fraction = temperature_centi_c % 100;

				if (fraction < 0) {
					fraction = -fraction;
				}

				LOG_INF("BME280 temperature: %d.%02d C",
					whole, fraction);
			}

			next_temperature_ms = now_ms + SLEEP_TIME_MS;
		}

		k_msleep(20);
	}

	return 0;
}
