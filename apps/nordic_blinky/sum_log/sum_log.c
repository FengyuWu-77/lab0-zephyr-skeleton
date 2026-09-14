#include <zephyr/logging/log.h>

#include "sum_log.h"

LOG_MODULE_REGISTER(sum_log, LOG_LEVEL_INF);

int sum_log(int a, int b)
{
	int inputs[2] = {a, b};
	int result = a + b;

	LOG_DBG("Starting sum calculation");
	LOG_INF("Logger: %d + %d = %d", a, b, result);
	LOG_HEXDUMP_INF(inputs, sizeof(inputs), "sum inputs");

	return result;
}