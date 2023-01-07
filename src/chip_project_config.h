/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 *    @file
 *          Example project configuration file for CHIP.
 *
 *          This is a place to put application or project-specific overrides
 *          to the default configuration values for general CHIP features.
 *
 */

#pragma once

/* Use a default pairing code if one hasn't been provisioned in flash. */
#define CHIP_DEVICE_CONFIG_USE_TEST_SETUP_PIN_CODE 20202021
#define CHIP_DEVICE_CONFIG_USE_TEST_SETUP_DISCRIMINATOR 0xF00

/*
 * Switching from Thread child to router may cause a few second packet stall.
 * Until this is improved in OpenThread we need to increase the retransmission
 * interval to survive the stall.
 */
#define CHIP_CONFIG_MRP_LOCAL_ACTIVE_RETRY_INTERVAL (1000_ms32)


// CONFIG_CHIP_DEVICE_VENDOR_ID sets the device manufacturer identifier that is assigned by the Connectivity Standards Alliance.

// CONFIG_CHIP_DEVICE_PRODUCT_ID sets the product identifier that is assigned by the product manufacturer.

// CONFIG_CHIP_DEVICE_TYPE sets the type of the device using the Matter Device Type Identifier,
//                         for example Door Lock (0x000A) or Dimmable Light Bulb (0x0101).

// CONFIG_CHIP_COMMISSIONABLE_DEVICE_TYPE enables including an optional device type subtype in the commissionable node discovery record.
//                                           This allows filtering of the discovery results to find the nodes that match the device type.

// CONFIG_CHIP_ROTATING_DEVICE_ID enables an optional rotating device identifier feature that provides an additional unique identifier for each device. This identifier is similar to the serial number, but it additionally changes at predefined times to protect against long-term tracking of the device.