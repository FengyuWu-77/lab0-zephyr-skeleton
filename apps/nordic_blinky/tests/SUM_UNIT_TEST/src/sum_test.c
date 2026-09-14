#include <zephyr/ztest.h>

#include "sum_log.h"

ZTEST(sum_log_test_suite, test_sum_log_basic)
{
	zassert_equal(sum_log(2, 3), 5, "2 + 3 should equal 5");
}

ZTEST(sum_log_test_suite, test_sum_log_negative)
{
	zassert_equal(sum_log(-2, -3), -5,
		      "-2 + -3 should equal -5");
}

ZTEST(sum_log_test_suite, test_sum_log_zero)
{
	zassert_equal(sum_log(0, 0), 0, "0 + 0 should equal 0");
}

ZTEST_SUITE(sum_log_test_suite, NULL, NULL, NULL, NULL, NULL);
