/*
 * Copyright (c) 2024
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	LOG_INF("Zephyr T3 Forest Template Application Started!");
	LOG_INF("Board: %s", CONFIG_BOARD);
	LOG_INF("Zephyr version: %s", KERNEL_VERSION_STRING);

	uint32_t count = 0;

	while (1) {
		LOG_INF("Hello World! Count: %u", count++);
		k_sleep(K_SECONDS(2));
	}

	return 0;
}
