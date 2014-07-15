/*
 * Copyright (C) 2011 Parade Tech Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _PS5101_H_
#define _PS5101_H_

#ifdef __KERNEL__

/*8bit I2C address*/
#define		PS5101I2CBASEADDR	0x10
#ifdef CONFIG_MHL_PS5101_VE
extern u8 ps5101_checkActive(void);
extern u8 sii9234_checkActive(void);
#endif
static ssize_t sysfs_check_mhl_command(struct class *class,
		struct class_attribute *attr, char *buf);
static ssize_t sysfs_check_factory_store(struct class *class,
		struct class_attribute *attr, const char *buf, size_t size);

extern u8 ps5101_mhl_onoff_ex(bool on);
struct ps5101_platform_data {
	struct i2c_client *mhl_tx_client_p0;
	struct i2c_client *mhl_tx_client_p1;
	struct i2c_client *mhl_tx_client_p2;
	struct i2c_client *mhl_tx_client_p3;

	u8 pageno;
	u8 offset;
	bool power_state;
	u8 factory_test;

	void (*gpio_cfg)(void);
	int (*get_irq)(void);
	void (*hw_onoff)(bool);
	void (*hw_reset)(void);
	void (*vbus_present)(bool on , int mhl_charge);
	int ddc_i2c_num;
	void (*ps5101_muic_cb)(bool otg_enable, int plim);

};



#endif

#endif

