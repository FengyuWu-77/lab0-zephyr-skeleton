#include <zephyr/devicetree.h>
#include <zephyr/ztest.h>

#define BME280_NODE DT_NODELABEL(bme280_lab)

ZTEST(bme280_dt_test_suite, test_bme280_node_exists)
{
	zassert_true(DT_NODE_EXISTS(BME280_NODE),
		     "The BME280 node must exist in the Device Tree");
}

ZTEST(bme280_dt_test_suite, test_bme280_node_is_enabled)
{
	zassert_true(DT_NODE_HAS_STATUS(BME280_NODE, okay),
		     "The BME280 node must have status okay");
}

ZTEST(bme280_dt_test_suite, test_bme280_i2c_address)
{
	zassert_equal(DT_REG_ADDR(BME280_NODE), 0x77,
		      "The BME280 default I2C address must be 0x77");
}

ZTEST_SUITE(bme280_dt_test_suite, NULL, NULL, NULL, NULL, NULL);
