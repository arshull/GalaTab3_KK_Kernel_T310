
/***************************************************************************
*  PS5101 - MHL Transmitter Driver

*

* Copyright (C) (2012, Parade Tech Inc)

*

* This program is free software; you can redistribute it and/or modify

* it under the terms of the GNU General Public License as published by

* the Free Software Foundation version 2.

*

* This program is distributed ¡°as is¡± WITHOUT ANY WARRANTY of any

* kind, whether express or implied; without even the implied warranty

* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

* GNU General Public License for more details.

*

*****************************************************************************/

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <asm/irq.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/ctype.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/ps5101.h>

#include <linux/gpio.h>


#include "ps5101_Common_Def.h"
#include "ps5101_func.h"

#ifdef print_err
	#undef print_err
#endif

#ifdef print_info
	#undef print_info
#endif

#ifdef print_dbg
	#undef print_dbg
#endif



#define	PREFIX5101	"[MHL]ps5101 "
#define	print_err(format, ...) \
		printk(KERN_ERR PREFIX5101 format, ##__VA_ARGS__);
#define	print_info(format, ...) \
		printk(KERN_INFO PREFIX5101 format, ##__VA_ARGS__);
#define	print_dbg(format, ...) \
		printk(KERN_DEBUG PREFIX5101 format, ##__VA_ARGS__);

struct i2c_driver ps5101_i2c_driver;
struct i2c_client *ps5101_i2c_client;

static struct device *ps5101_mhldev;

int MHL_i2c_init;

#define	__CONFIG_SS_FACTORY__

#define __CONFIG_RSEN_TIMER__
#ifdef __CONFIG_RSEN_TIMER__
struct timer_list check_rsen_timer;

#define RSEN_CHECK_TIME 10

/*static void mhl_check_rsen_timer_callback(void);*/
static void mhl_check_rsen_timer_callback(struct work_struct *work);
static void mhl_check_rsen_timer(unsigned long data);
#endif
irqreturn_t mhl_int_irq_handler(int irq, void *dev_id);

static struct mutex ps5101_irq_lock;
#define	__PS5101_IRQ_DEBUG__
#ifdef __PS5101_IRQ_DEBUG__
int ps5101_en_irq;
#	define ps5101_enable_irq() \
	do { \
		mutex_lock(&ps5101_irq_lock); \
		if (atomic_read(&ps5101->is_irq_enabled) == false) { \
			atomic_set(&ps5101->is_irq_enabled, true); \
			enable_irq(ps5101->pdata->mhl_tx_client_p0->irq); \
			printk(KERN_INFO"%s() : enable_irq(%d)\n", __func__, \
					++ps5101_en_irq); \
		} else { \
			printk(KERN_INFO"%s() : irq is already enabled(%d)\n" \
					, __func__, ps5101_en_irq); \
		} \
		mutex_unlock(&ps5101_irq_lock); \
	} while (0)
#	define ps5101_disable_irq() \
	do { \
		mutex_lock(&ps5101_irq_lock); \
		if (atomic_read(&ps5101->is_irq_enabled) == true) { \
			atomic_set(&ps5101->is_irq_enabled, false); \
			disable_irq_nosync(ps5101->pdata->mhl_tx_client_p0->irq);\
			printk(KERN_INFO"%s() : disable_irq(%d)\n", \
					__func__, --ps5101_en_irq); \
		} else { \
			printk(KERN_INFO"%s() : irq is already disabled(%d)\n"\
					, __func__, ps5101_en_irq); \
		} \
		mutex_unlock(&ps5101_irq_lock); \
	} while (0)
#else
#	define ps5101_enable_irq() \
	do { \
		mutex_lock(&ps5101_irq_lock); \
		if (atomic_read(&ps5101->is_irq_enabled) == false) { \
			atomic_set(&ps5101->is_irq_enabled, true); \
			enable_irq(ps5101->pdata->mhl_tx_client_p0->irq); \
		} \
		mutex_unlock(&ps5101_irq_lock); \
	} while (0)
#	define ps5101_disable_irq() \
	do { \
		mutex_lock(&ps5101_irq_lock); \
		if (atomic_read(&ps5101->is_irq_enabled) == true) { \
			atomic_set(&ps5101->is_irq_enabled, false); \
			disable_irq_nosync(ps5101->pdata->mhl_tx_client_p0->irq);\
		} \
		mutex_unlock(&ps5101_irq_lock); \
	} while (0)
#endif /*__PS5101_IRQ_DEBUG__*/
LIST_HEAD(g_msc_packet_list_ps5101);
static int g_list_cnt_ps5101;
static struct workqueue_struct *ps5101_msc_wq;
static struct workqueue_struct *ps5101_misc_wq;

u8 ps5101_mhl_onoff_ex(bool onoff)
{
	struct ps5101_data *ps5101 = dev_get_drvdata(ps5101_mhldev);


	print_info("ps5101 onoff: %d\n", onoff);

	if (!ps5101 || !ps5101->pdata) {
		print_info("mhl_onoff_ex: getting resource is failed\n");
		return 2;
	}
	if (ps5101->pdata->power_state == onoff) {
		print_info("mhl_onoff_ex: mhl already %s\n", \
							 onoff ? "on" : "off");
		return 2;
	}

	msleep(20);

	ps5101->pdata->power_state = onoff; /*save power state*/
	if (onoff) {
		wake_lock(&ps5101->wake_lock);

#ifdef __CONFIG_RSEN_TIMER__
		check_rsen_timer.function = mhl_check_rsen_timer;
		check_rsen_timer.data = 0;
		check_rsen_timer.expires = jiffies + HZ*RSEN_CHECK_TIME;
		add_timer(&check_rsen_timer);
#endif
	/*send some data for VBUS SRC such a TA or USB or UNKNOWN */

		if (ps5101->pdata->hw_onoff)
			ps5101->pdata->hw_onoff(1);



		if (ps5101->pdata->hw_reset)
			ps5101->pdata->hw_reset();

		msleep(100);

		initps5101(ps5101);
		ps5101_enable_irq();
		return 0;
	} else {

#ifdef __CONFIG_RSEN_TIMER__
		del_timer(&check_rsen_timer);
#endif
		ps5101_disable_irq();

		if (ps5101->pdata->hw_onoff)
			ps5101->pdata->hw_onoff(0);


		if (ps5101->pdata->ps5101_muic_cb) {
			ps5101->pdata->ps5101_muic_cb(false, -1);
			ps5101->vbus_isplugd = false;
			print_info("ps5101_muic_cb: TA disconnect\n");
		}
		wake_unlock(&ps5101->wake_lock);

		return 0;

	}
	return 2;
}
EXPORT_SYMBOL(ps5101_mhl_onoff_ex);
u8 ps5101_checkActive(void)
{
	return MHL_i2c_init;
}
EXPORT_SYMBOL(ps5101_checkActive);


int ps5101_i2c_write(struct i2c_client *client, u8 reg, u8 data)
{
	if (!MHL_i2c_init) {
		print_err("I2C not ready\n");
		return -EIO;
	}

	return i2c_smbus_write_byte_data(client, reg, data);
}
EXPORT_SYMBOL(ps5101_i2c_write);



int ps5101_i2c_read(struct i2c_client *client, u8 reg, u8* value)
{
	int ret;

	if (!MHL_i2c_init) {
		print_err("I2C not ready\n");
		return -EIO;
	}

	ret = i2c_smbus_write_byte(client, reg);
	if (ret < 0) {
		print_err("Write reg 0x%02x error\n", reg);
		return ret;
	}


	ret = i2c_smbus_read_byte(client);


	if (ret < 0) {
		print_err("I2C read fail\n");
		return -EIO;
	}

	*value = ret & 0x000000ff;
	return ret;

}
EXPORT_SYMBOL(ps5101_i2c_read);




#define MHL_ACCESSREG_PS5101	1

#ifdef MHL_ACCESSREG_PS5101


static ssize_t read_register(struct class *class,
		struct class_attribute *attr, char *buf)
{
	int size0;
	u8  readvalue = 0;
	int ret;

	struct ps5101_data *ps5101 = dev_get_drvdata(ps5101_mhldev);

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, ps5101->pdata->pageno,
			ps5101->pdata->offset, &readvalue);
	mutex_unlock(&ps5101->i2c_lock);

	if (ret < 0) {
		print_err("read i2c error\n");
		return -EIO;
	}


	size0 = snprintf(buf, 8, "0x%02x\n", readvalue);

	print_info("read P%d.%02x: 0x%02x\n",\
		ps5101->pdata->pageno, ps5101->pdata->offset, readvalue);


	return size0;

}


static ssize_t write_register(struct class *class,
					struct class_attribute *attr,
					const char *buf, size_t size)
{
	struct ps5101_data *ps5101 = dev_get_drvdata(ps5101_mhldev);

	char temp[6] = { 0, };
	const char *p = buf;
	unsigned long value;
	u8 charcount = 0;
	u8  readvalue = 0;
	u8  writevalue = 0;
	int ret;

	while (*p != '\0')	{

		if (!isspace(*p)) {
			if (!isxdigit(*p)) {
				print_err("invalad parameter sent to file\n");
				return -EINVAL;
			}

			charcount++;
			if (charcount <= 6) {
				strncat(temp, p, 1);
			} else {
				print_err("too much char sent to file\n");
				return -EINVAL;
			}
		}
		p++;
	}


	ret = kstrtoul(temp, 16, &value);
	if (ret < 0) {
		print_err("file parameter error\n");
		return ret;
	}

	if (strlen(temp) == 4) {

		ps5101->pdata->pageno = value / 256;
		ps5101->pdata->offset = value & 0xff;

		mutex_lock(&ps5101->i2c_lock);
		ret = I2C_ReadByte(ps5101, ps5101->pdata->pageno,
				ps5101->pdata->offset, &readvalue);
		mutex_unlock(&ps5101->i2c_lock);

		if (ret < 0) {
			print_err("read i2c error\n");
			return -EIO;
		}

		print_info("read register value P%d.%02x = 0x%02x\n",\
		ps5101->pdata->pageno, ps5101->pdata->offset, readvalue);
	} else if (strlen(temp) == 6) {

		ps5101->pdata->pageno = value / 256 / 256;
		ps5101->pdata->offset = (value / 256) & 0xff;
		writevalue            = value & 0xff;

		print_info("write register value P%d.%02x = 0x%02x\n",\
		ps5101->pdata->pageno, ps5101->pdata->offset, writevalue);

		mutex_lock(&ps5101->i2c_lock);
		ret = I2C_WriteByte(ps5101, ps5101->pdata->pageno,
			ps5101->pdata->offset, writevalue);
		mutex_unlock(&ps5101->i2c_lock);

		if (ret < 0) {
			print_err("write i2c error\n");
			return -EIO;
		}

		mutex_lock(&ps5101->i2c_lock);
		ret = I2C_ReadByte(ps5101, ps5101->pdata->pageno,
				ps5101->pdata->offset, &readvalue);
		mutex_unlock(&ps5101->i2c_lock);

		if (ret < 0) {
			print_err("read i2c error\n");
			return -EIO;
		}

		print_info("read register value P%d.%02x = 0x%02x\n",\
		ps5101->pdata->pageno, ps5101->pdata->offset, readvalue);
	} else {
		print_err("file format error\n");
		return -EINVAL;
	}


	return size;
}

static CLASS_ATTR(mhl_ps5101, 0664, read_register, write_register);

#endif

void initps5101work(struct work_struct *work)
{
	struct ps5101_data *ps5101 = container_of(work,
						struct ps5101_data,
						startup_work);

	initps5101(ps5101);

}

void ps5101_request_msc_work(struct work_struct *work)
{
	struct msc_packet *p_msc_pkt, *temp;
	int ret;
	u8 readvalue = 0;
	u8 mode = 0;
	int loop = 0;

	struct ps5101_data *ps5101 = container_of(work,
						struct ps5101_data,
						msc_work);

	mutex_lock(&ps5101->list_lock);

	list_for_each_entry_safe(p_msc_pkt, temp,
		&g_msc_packet_list_ps5101, p_msc_packet_list)
	{
		print_dbg("get item command(0x%02x), data1(0x%02x), data2(0x%02x)\n",
		p_msc_pkt->command, p_msc_pkt->data_1, p_msc_pkt->data_2);
		do {
			mutex_lock(&ps5101->i2c_lock);
			ret = I2C_ReadByte(ps5101, 3, 0x40, &readvalue);
			ret = I2C_ReadByte(ps5101, 1, 0xf5, &mode);
			mutex_unlock(&ps5101->i2c_lock);
			msleep(MSC_CHECKIDLE_INTERVAL);
			loop++;

		} while ((ret >= 0) && ((readvalue & 0x08) != 0x08) &&
			(loop < MSC_CHECKIDLE_REPEAT) && (mode == 0x02));

		if (ret < 0) {
			print_err("read i2c error, empty list\n");
			break;
		} else if (loop >= MSC_CHECKIDLE_REPEAT) {
			print_err("wait MSC idle timeout, discard current msc command\n");
		} else if (mode != 0x02) {
			print_info("in usb mode, empty list\n");
			break;
		} else {

			switch (p_msc_pkt->command) {
			case MHL_MSC_MSG:

				init_completion(&ps5101->mscmsg_complete);

				requestMSCMSGcmd(ps5101, p_msc_pkt->command,
					p_msc_pkt->data_1, p_msc_pkt->data_2);

				ret = wait_for_completion_timeout(
						&ps5101->mscmsg_complete,
						msecs_to_jiffies(MSC_DELAY));
				if (ret == 0)
					print_err("wait sending MSC_MSG time out\n");

				break;


			case MHL_READ_DEVCAP:
#ifdef SWREAD_DCAP
				init_completion(&ps5101->mscrcap_complete);

				requestMSCREADCAPcmd(ps5101, p_msc_pkt->command,
					p_msc_pkt->data_1, p_msc_pkt->data_2);

				ret = wait_for_completion_timeout(
					&ps5101->mscrcap_complete,
					msecs_to_jiffies(MSC_DELAY));
				if (ret == 0){
					print_err("MSC_READ_DCAP time out\n");
				} else {

					mutex_lock(&ps5101->i2c_lock);
					ret = I2C_ReadByte(ps5101, 2, 0x82, &readvalue);
					mutex_unlock(&ps5101->i2c_lock);
				
					if (ret < 0) {
						print_err("read 2c error for capability\n");
					}else {
					
						ps5101->plim = (readvalue >> 5) & 0x03;
						print_info("ps5101->plim = %d \n", ps5101->plim);
						
						if (ps5101->plim != MHL_VBUS_TA_900mA ){
							ps5101->vbus_owner = MHL_VBUS_USB;
						}else{
							ps5101->vbus_owner = MHL_VBUS_TA_900mA;
							}
					}
				}
				

#endif
				break;

			case MHL_WRITE_STAT:

				init_completion(&ps5101->mscwstat_complete);

				requestMSCWRITESTATcmd(
					ps5101, p_msc_pkt->command,
					p_msc_pkt->data_1, p_msc_pkt->data_2);

				ret = wait_for_completion_timeout(
					&ps5101->mscwstat_complete,
					msecs_to_jiffies(MSC_DELAY));
				if (ret == 0)
					print_err("wait sending MSC_WRITE_STAT time out\n");

				break;

			default:
				print_err("not supported command\n");

			}
		}

		list_del(&p_msc_pkt->p_msc_packet_list);
		print_dbg("free item , addr = 0x%x, cnt=%d\n",
				(unsigned int)p_msc_pkt, --g_list_cnt_ps5101);

		kfree(p_msc_pkt);

	}

	/*empty list*/
	list_for_each_entry_safe(p_msc_pkt, temp,
			&g_msc_packet_list_ps5101, p_msc_packet_list)
	{
		list_del(&p_msc_pkt->p_msc_packet_list);
		print_dbg("free item , addr = 0x%x, cnt=%d\n",\
				(unsigned int)p_msc_pkt, --g_list_cnt_ps5101);
		kfree(p_msc_pkt);
	}

	mutex_unlock(&ps5101->list_lock);

}


int ps5101_enqueue_msc_work(struct ps5101_data *ps5101,
			u8 command, u8 data1, u8 data2) {
	struct msc_packet *packet_item;
	int ret;

	packet_item = kmalloc(sizeof(struct msc_packet), GFP_KERNEL);
	if (!packet_item) {
		print_err("%s() kmalloc error\n", __func__);
		return -ENOMEM;
	} else {
		print_dbg("add item, addr = 0x%x, cnt=%d\n",\
			(unsigned int)packet_item, ++g_list_cnt_ps5101);
	}

	packet_item->command = command;
	packet_item->data_1 = data1;
	packet_item->data_2 = data2;

	print_dbg("add item command(0x%02x),data1(0x%02x),data2(0x%02x)\n",\
		command, data1, data2);

	mutex_lock(&ps5101->list_lock);
	list_add_tail(&packet_item->p_msc_packet_list,
			&g_msc_packet_list_ps5101);
	mutex_unlock(&ps5101->list_lock);

	print_dbg("%s() msc work schedule\n", __func__);

	ret = queue_work(ps5101_msc_wq, &(ps5101->msc_work));

	return 0;
}


void cbuslowwork(struct work_struct *work)
{
	struct ps5101_data *ps5101 = container_of(work,
						struct ps5101_data,
						waitmeasurezcbus_work);

	int ret;


	print_info("status is clear, start measure 1K ohm\n");

	set_mhlmode(ps5101);
	start_discovery(ps5101);

	ps5101->cleared = false;

	init_completion(&ps5101->zcbus_complete);

	ret = wait_for_completion_timeout(&ps5101->zcbus_complete,
				  msecs_to_jiffies(ZCBUS_DELAY));
	if (ret == 0) {
		print_info("wait measure 1K ohm time out\n");

/*		set_usbmode(ps5101);	*/
	}


}

int cbuslow_action(struct ps5101_data *ps5101)
{

	struct msc_packet *p_msc_pkt, *temp;
	int ret = 0;

#ifdef SOFTHPD
	softset_hpd_des(ps5101);
#endif

	if (ps5101->cleared) {
		queue_work(ps5101_misc_wq, &(ps5101->waitmeasurezcbus_work));
	} else {
		print_info("status is not cleared, standby\n");
		/*empty list*/
		mutex_lock(&ps5101->list_lock);

		list_for_each_entry_safe(p_msc_pkt, temp,
				&g_msc_packet_list_ps5101,
				p_msc_packet_list)
		{
			list_del(&p_msc_pkt->p_msc_packet_list);
			print_dbg("free item , addr = 0x%x, cnt=%d\n",
			(unsigned int)p_msc_pkt, --g_list_cnt_ps5101);

			kfree(p_msc_pkt);
		}

		mutex_unlock(&ps5101->list_lock);

		if (ps5101->chipver == 0x0b) {
			ret = clear_status(ps5101);
		} else {
			ret = stand_by(ps5101);

			if (ret >= 0)
				ps5101->standby = true;
		}
	}

	return ret;

}

int rsen_deact(struct ps5101_data *ps5101)
{

	set_usbmode(ps5101);

	print_info("set to usb mode\n");

	mhl_hpd_handler(false);

	msleep(CBUSFLOAT_DELAY);

	return 0;

}

void writestatwork(struct work_struct *work)
{


	struct ps5101_data *ps5101 = container_of(work,
						struct ps5101_data,
						writestatus_work);

	int ret = 0;

	/*init_completion(&ps5101->rsen_complete);*/

	ret = wait_for_completion_timeout(&ps5101->rsen_complete,
				  msecs_to_jiffies(RSEN_DELAY));

	if (ret == 0) {
		print_info("wait RSEN_ACT time out, set to usb mode\n");

		set_usbmode(ps5101);
	} else {


		msleep(MSC_WRITESTAT_DELAY);


		ps5101_enqueue_msc_work(ps5101, MHL_WRITE_STAT,
					MHL_MSC_WRITESTATE_CONNECTED_RDY,
					MHL_MSC_WRITESTATE_DCAP_RDY);
#ifdef PATCH4CTS

		init_completion(&ps5101->rddcap_complete);

		ret = wait_for_completion_timeout(&ps5101->rddcap_complete,
			  msecs_to_jiffies(600));

		if (ret == 0) {
			print_info("no READ_DCAP int in 600ms\n");
		} else {
			print_info("get READ_DCAP int in 600ms\n");
			/*msleep(3200);*/
			set_cbus_timer(ps5101, 0x27);
			msleep(200);
		}

	if (ps5101->pdata->ps5101_muic_cb && ps5101->vbus_isplugd == true) {
		ps5101->pdata->ps5101_muic_cb(false, ps5101->vbus_owner);
		print_info("ps5101_muic_cb: TA connect\n");
	}
		hwhpd(ps5101);
#endif

	}

#ifdef __CONFIG_RSEN_TIMER__
	del_timer(&check_rsen_timer);
#endif
}


#ifdef __CONFIG_RSEN_TIMER__
static void mhl_check_rsen_timer_callback(struct work_struct *work)
{
/*	struct ps5101_data *ps5101 = dev_get_drvdata(ps5101_mhldev);*/


	ps5101_mhl_onoff_ex(0);

	msleep(100);
	ps5101_mhl_onoff_ex(1);


/*goto_d3(ps5101);*/
}

static void mhl_check_rsen_timer(unsigned long data)
{
	struct ps5101_data *ps5101 = dev_get_drvdata(ps5101_mhldev);

	/*mhl_log;*/

	INIT_WORK(&ps5101->rsen_timer, mhl_check_rsen_timer_callback);

	schedule_work(&(ps5101->rsen_timer));
}
#endif


int discover_action(struct ps5101_data *ps5101)
{
	ps5101->vbus_isplugd = true;
	
	softset_hpd_des(ps5101);

	set_cbus_timer(ps5101, 0x24);

	init_completion(&ps5101->rsen_complete);

	queue_work(ps5101_misc_wq, &(ps5101->writestatus_work));

	return 0;

}

int rsen_act(struct ps5101_data *ps5101)
{
	complete(&ps5101->rsen_complete);

	mhl_hpd_handler(true);

	print_info("rsen complete\n");

	return 0;
}

#ifdef SOFTHPD

void soft_hpd_work(struct work_struct *work)
{
	struct ps5101_data *ps5101 = container_of(work,
						struct ps5101_data,
						softhpd_work);

	softset_hpd(ps5101);

}

int softhpd_action(struct ps5101_data *ps5101)
{
	int ret;


	ret = queue_work(ps5101_misc_wq, &(ps5101->softhpd_work));

	print_info("queue soft hpd work return %x", ret);

	return 0;

}



#endif

u8 int_offset[9] = {
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x38,
};


u8 intmask_offset[9] = {
	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0x39,
};


t_INT s_Int[] = {
{ 0x03, 0x38, 1, 0x39, 1, 8,
	"p3.38[1] RSEN deact irq\n", rsen_deact},
{ 0x03, 0x38, 4, 0x39, 4, 8,
	"p3.38[4] CBUS low detect irq\n", cbuslow_action},
{ 0x03, 0x38, 7, 0x39, 7, 8,
	"p3.38[7] ZCBUS ok irq\n", en_wakeup},
{ 0x03, 0x38, 2, 0x39, 2, 8,
	"p3.38[2] discovery pass irq\n", discover_action},
{ 0x03, 0x90, 0, 0xa0, 0, 0,
	"p3.90[0] MSC_MSG data received ready irq\n", handle_mscmsg},
{ 0x03, 0x94, 4, 0xa4, 4, 4,
"p3.94[4] receiving WRITE_STA/WRITE_INT cmd pass irq\n", handle_WRITE_STAT_INT},
{ 0x03, 0x96, 4, 0xa6, 4, 6,
	"p3.96[4] RCHANGE_INT request for 3D irq\n", request_3D},
{ 0x03, 0x96, 3, 0xa6, 3, 6,
	"p3.96[3] RCHANGE_INT grant to write irq\n", grant_write},
{ 0x03, 0x96, 2, 0xa6, 2, 6,
	"p3.96[2] RCHANGE_INT request to write irq\n", request_write},
{ 0x03, 0x96, 1, 0xa6, 1, 6,
"p3.96[1] RCHAGNE_INT scratchpad value changed irq\n", scrachpad_change},
{ 0x03, 0x96, 0, 0xa6, 0, 6,
"p3.96[0] RCHANGE_INT capability value changed irq\n", capability_change},
{ 0x03, 0x97, 1, 0xa7, 1, 7,
	"p3.97[1] DCHANGE_INT EDID changed irq\n", edid_change},
{ 0x03, 0x92, 3, 0xa2, 3, 2,
"p3.92[3] sending WRITE_STA/WRITE_INT cmd fail irq\n", finish_writestat},
{ 0x03, 0x92, 2, 0xa2, 2, 2,
"p3.92[2] sending WRITE_STA/WRITE_INT cmd pass irq\n", finish_writestat},
{ 0x03, 0x91, 5, 0xa1, 5, 1,
	"p3.91[5] sending MSC_MSG cmd fail irq\n", finish_mscmsg },
{ 0x03, 0x91, 4, 0xa1, 4, 1,
	"p3.91[4] sending MSC_MSG pass irq\n", finish_mscmsg },
{ 0x03, 0x91, 7, 0xa1, 7, 1,
	"p3.91[7] sending READ_DEVCAP cmd fail irq\n", finish_readdcap_fail },
{ 0x03, 0x91, 6, 0xa1, 6, 1,
	"p3.91[6] sending READ_DEVCAP cmd pass irq\n", finish_readdcap_pass },
{ 0x03, 0x38, 0, 0x39, 0, 8,
	"p3.38[0] RSEN act irq\n", rsen_act },

#ifdef SOFTHPD
{ 0x03, 0x93, 0, 0xa3, 0, 3,
	"p3.93[0] receiving SET_HPD/CLR_HPD cmd pass irq\n", softhpd_action },
#else
{ 0x03, 0x93, 0, 0xa3, 0, 3,
	"p3.93[0] receiving SET_HPD/CLR_HPD cmd pass irq\n", NULL },
#endif

{ 0x03, 0x38, 3, 0x39, 3, 8,
	"p3.38[3] discovery fail irq\n", discovery_fail },
{ 0x03, 0x38, 5, 0x39, 5, 8,
	"p3.38[5] ZBUS measure fail irq\n", zbus_fail },


{ 0x03, 0x38, 6, 0x39, 6, 8,
	"p3.38[6] PWR12V_ON irq\n", NULL },

{ 0x03, 0x90, 7, 0xa0, 7, 0,
	"p3.90[7] sending GET_MSC_ERRORCODE cmd fail irq\n", NULL },
{ 0x03, 0x90, 6, 0xa0, 6, 0,
	"p3.90[6] sending GET_MSC_ERRORCODE cmd pass irq\n", NULL },
{ 0x03, 0x90, 5, 0xa0, 5, 0,
	"p3.90[5] sending GET_DDC_ERRORCODE cmd fail irq\n", NULL },
{ 0x03, 0x90, 4, 0xa0, 4, 0,
	"p3.90[4] sending GET_DDC_ERRORCODE cmd pass irq\n", NULL },
{ 0x03, 0x90, 3, 0xa0, 3, 0,
	"p3.90[3] sending RG_UNI_CMD_DAT cmd fail irq\n", NULL },
{ 0x03, 0x90, 2, 0xa0, 2, 0,
	"p3.90[2] sending RG_UNI_CMD_DAT cmd pass irq\n", NULL },
{ 0x03, 0x90, 1, 0xa0, 1, 0,
	"p3.90[1] MSC channel error irq\n", NULL },

{ 0x03, 0x91, 3, 0xa1, 3, 1,
	"p3.91[3] sending GET_VENDOR_ID cmd fail irq\n", NULL },
{ 0x03, 0x91, 2, 0xa1, 2, 1,
	"p3.91[2] sending GET_VENDOR_ID cmd pass irq\n", NULL },
{ 0x03, 0x91, 1, 0xa1, 1, 1,
	"p3.91[1] sending GET_STATE cmd fail irq\n", NULL },
{ 0x03, 0x91, 0, 0xa1, 0, 1,
	"p3.91[0] sending GET_STATE cmd pass irq\n", NULL },

{ 0x03, 0x92, 7, 0xa2, 7, 2,
	"p3.92[7] receiving CLR_HPD command irq\n", clear_hpd },
{ 0x03, 0x92, 6, 0xa2, 6, 2,
	"p3.92[6] PATH_EN done irq\n", NULL },
{ 0x03, 0x92, 5, 0xa2, 5, 2,
	"p3.92[5] HPD deassert irq\n", NULL },
{ 0x03, 0x92, 4, 0xa2, 4, 2,
	"p3.92[4] HPD assserted irq\n", NULL },
{ 0x03, 0x92, 1, 0xa2, 1, 2,
	"p3.92[1] sending WRITE_BURST cmd fail irq\n", NULL },
{ 0x03, 0x92, 0, 0xa2, 0, 2,
	"p3.92[0] sending WRITE_BURST cmd pass irq\n", NULL },


{ 0x03, 0x93, 7, 0xa3, 7, 3,
	"p3.93[7] receiving GET_DDC_ERRORCODE cmd fail irq\n", NULL },
{ 0x03, 0x93, 6, 0xa3, 6, 3,
	"p3.93[6] receiving GET_DDC_ERRORCODE cmd pass irq\n", NULL },
{ 0x03, 0x93, 5, 0xa3, 5, 3,
	"p3.93[5] receiving GET_VENDOR_ID cmd fail irq\n", NULL },
{ 0x03, 0x93, 4, 0xa3, 4, 3,
	"p3.93[4] receiving GET_VENDOR_ID cmd pass irq\n", NULL },
{ 0x03, 0x93, 3, 0xa3, 3, 3,
	"p3.93[3] receiving GET_STATE cmd fail irq\n", NULL },
{ 0x03, 0x93, 2, 0xa3, 2, 3,
	"p3.93[2] receiving GET_STATE cmd pass irq\n", NULL },
{ 0x03, 0x93, 1, 0xa3, 1, 3,
	"p3.93[1] receiving SET_HPD/CLR_HPD cmd fail irq\n", NULL },

{ 0x03, 0x94, 7, 0xa4, 7, 4,
	"p3.94[7] receiving WRITE_BURST cmd fail irq\n", NULL },
{ 0x03, 0x94, 6, 0xa4, 6, 4,
	"p3.94[6] receiving WRITE_BUTST cmd pass irq\n", NULL },
{ 0x03, 0x94, 5, 0xa4, 5, 4,
	"p3.94[5] receiving WRITE_STA/WRITE_INT cmd fail irq\n", NULL },
{ 0x03, 0x94, 3, 0xa4, 3, 4,
	"p3.94[3] receiving READ_DEVCAP cmd fail irq\n", NULL },
{ 0x03, 0x94, 2, 0xa4, 2, 4,
	"p3.94[2] receiving READ_DEVCAP cmd pass irq\n", read_dcap_action },
{ 0x03, 0x94, 1, 0xa4, 1, 4,
	"p3.94[1] receiving GET_MSC_ERRORCODE cmd fail irq\n", NULL },
{ 0x03, 0x94, 0, 0xa4, 0, 4,
	"p3.94[0] receiving GET_MSC_ERRORCODE cmd pass irq\n", NULL },

{ 0x03, 0x95, 5, 0xa5, 5, 5,
	"p3.95[5] RX_DCAP_RDY irq\n", NULL },
{ 0x03, 0x95, 4, 0xa5, 4, 5,
	"p3.95[4] MSC_VENDOR_RECEIVED irq\n", NULL },
{ 0x03, 0x95, 3, 0xa5, 3, 5,
	"p3.95[3] MSC_ABORT_RECEIVED irq\n", NULL },
{ 0x03, 0x95, 2, 0xa5, 2, 5,
	"p3.95[2] DDC_ABORT_RECEIVED irq\n", NULL },
{ 0x03, 0x95, 1, 0xa5, 1, 5,
	"p3.95[1] receiving MSC_MSG fail irq\n", NULL },
{ 0x03, 0x95, 0, 0xa5, 0, 5,
	"p3.95[0] receiving MSC_MSG pass irq\n", NULL },

	{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, "Dummy\n", NULL},
};

irqreturn_t mhl_int_irq_handler(int irq, void *dev_id)
{
	u8 readvalue = 0;
	u8 index = 0;
	int ret, i;
	int count = 0;
	struct ps5101_data *ps5101 = dev_id;
	u8 intbytes[9], intmaskbytes[9];
	bool normalINT = false;

	print_dbg("enter interrupt thread\n");

	ret = 0;
	count = 0;

	do {
		mutex_lock(&ps5101->i2c_lock);
		ret = I2C_ReadByte(ps5101, 1, 0xf4, &readvalue);
		mutex_unlock(&ps5101->i2c_lock);

		if (ret < 0) {
			msleep(50);
			count++;
			print_info("i2c error in isr: %d\n", count);
		}
	} while ((ret < 0) && (count < 5));

	if (ret < 0) {
		print_err("read reg error in irq\n");
		return IRQ_HANDLED;
	}

	if (readvalue != 0) {

		print_info("standby clear irq\n");

		mutex_lock(&ps5101->i2c_lock);
		/*enable register writing*/
		I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
		/*clear standby exit irq*/
		I2C_WriteByte(ps5101, 1, 0x01, 0x80);
		mutex_unlock(&ps5101->i2c_lock);


/*		queue_work(ps5101_misc_wq, &(ps5101->startup_work));*/
		initps5101(ps5101);


	} else {


		mutex_lock(&ps5101->i2c_lock);
		for (i = 0; i < 9; i++) {
			ret = I2C_ReadByte(ps5101, 3,
					int_offset[i], &intbytes[i]);
			if (ret < 0) {
				mutex_unlock(&ps5101->i2c_lock);
				print_err("read reg error in irq\n");
				return IRQ_HANDLED;
			}

			ret = I2C_ReadByte(ps5101, 3,
				intmask_offset[i], &intmaskbytes[i]);
			if (ret < 0) {
				mutex_unlock(&ps5101->i2c_lock);
				print_err("read reg error in irq\n");
				return IRQ_HANDLED;
			}

			print_dbg("0x%02x,0x%02x ",\
				intbytes[i], intmaskbytes[i]);


		}
		mutex_unlock(&ps5101->i2c_lock);

		while (s_Int[index].ucPage != 0xff) {
			if (((intbytes[s_Int[index].ucIndex] >>\
					s_Int[index].ucBit)
				& (~(intmaskbytes[s_Int[index].ucIndex] >>\
						s_Int[index].ucMaskBit))
				& 0x01) != 0) {

				print_info("%s", s_Int[index].ucInfo);
				readvalue = intbytes[s_Int[index].ucIndex] & \
						(1 << s_Int[index].ucBit);

				mutex_lock(&ps5101->i2c_lock);
				/*clear the interrupt*/
				I2C_WriteByte(ps5101, s_Int[index].ucPage,
					s_Int[index].ucOffset, readvalue);
				mutex_unlock(&ps5101->i2c_lock);

				if (s_Int[index].pfunc)
					s_Int[index].pfunc(ps5101);

				normalINT = true;

			}

			index++;
		}
#if 0
		if (normalINT == false) {
			print_info("not normal INT, standby clear irq\n");

			mutex_lock(&ps5101->i2c_lock);
			/*enable register writing*/
			I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
			/*clear standby exit irq*/
			I2C_WriteByte(ps5101, 1, 0x01, 0x80);
			mutex_unlock(&ps5101->i2c_lock);


			initps5101(ps5101);


		}
#endif
	}

	print_info("interrupt handled\n");
	return IRQ_HANDLED;

}

#ifdef __CONFIG_SS_FACTORY__

#define PS_ID 0x0B
static ssize_t sysfs_check_mhl_command(struct class *class,
			struct class_attribute *attr, char *buf) {

	int size;
	int ret;
	u8 ps_id = 0;
	struct ps5101_data *ps5101 = dev_get_drvdata(ps5101_mhldev);
	print_info("sysfs_check_mhl_command\n");

	if (ps5101->pdata->hw_onoff)
		ps5101->pdata->hw_onoff(1);

	if (ps5101->pdata->hw_reset)
		ps5101->pdata->hw_reset();


	ret = I2C_ReadByte(ps5101, 1, 0xf1, &ps_id);
	if (ret < 0) {
		print_err("read i2c error\n");
		return false;
	}


	print_info("ps5101 : sel_show ps_id: %X\n", ps_id);

	if (ps5101->pdata->hw_onoff)
		ps5101->pdata->hw_onoff(0);

	size = snprintf(buf, 10, "%d\n", ps_id == PS_ID ? 1 : 0);

	return size;

}

static ssize_t sysfs_check_factory_store(struct class *class,
		struct class_attribute *attr, const char *buf, size_t size) {
	struct ps5101_data *ps5101 = dev_get_drvdata(ps5101_mhldev);
	const char *p = buf;
	u8 value = 0;
	print_info("sysfs_check_factory_store\n");

	if (p[0] == '1')
		value = 1;
	else
		value = 0;

	ps5101->pdata->factory_test = value;

	if (ps5101->pdata->factory_test == 1)
		pr_info("ps5101: in factory mode\n");
	else
		pr_info("ps5101: not factory mode\n");

	return size;

}

static CLASS_ATTR(test_result, 0664, sysfs_check_mhl_command,
		sysfs_check_factory_store);

#endif /*__CONFIG_SS_FACTORY__*/

static int ps5101_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{

	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	struct ps5101_data *ps5101;
	struct input_dev *input;

#ifdef MHL_ACCESSREG_PS5101
	struct class *mhl_ps5101;
#endif
#ifdef __CONFIG_SS_FACTORY__
	struct class *sec_mhl;
#endif

	int ret;
#ifdef CONFIG_MHL_PS5101_VE
	if (sii9234_checkActive())
		return -EIO;
#endif

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EIO;




	ps5101 = kzalloc(sizeof(struct ps5101_data), GFP_KERNEL);
	if (ps5101 == NULL) {
		print_err("failed to allocate memory\n");
		return -ENOMEM;
	}


	ps5101->pdata = client->dev.platform_data;
	if (!ps5101->pdata) {
		print_err("failed to get platform data\n");
		ret = -EINVAL;
		goto err_exit0;
	}
	ps5101->pdata->gpio_cfg();

	ps5101->pdata->mhl_tx_client_p0 = client;
	ps5101->pdata->factory_test = 0;

	i2c_set_clientdata(client, ps5101);

	ps5101_mhldev = &client->dev;

	mutex_init(&ps5101->i2c_lock);
	mutex_init(&ps5101->list_lock);
	mutex_init(&ps5101_irq_lock);

	input = input_allocate_device();
	if (!input) {
		print_err("failed to allocate input device.\n");
		ret = -ENOMEM;
		goto err_exit0;
	}

	/* indicate that we generate key events */
	set_bit(EV_KEY, input->evbit);
	bitmap_fill(input->keybit, KEY_MAX);

	ps5101->input_dev = input;
	input_set_drvdata(input, ps5101);
	input->name = "ps5101_rcp";
	input->id.bustype = BUS_I2C;

	ret = input_register_device(input);
	if (ret < 0) {
		print_err("fail to register input device\n");
		goto err_exit1;
	}

	/* rest of the initialisation goes here. */

	print_info("attach success\n");

	ps5101_i2c_client = client;

	client->irq = ps5101->pdata->get_irq();

	ret = request_threaded_irq(client->irq, NULL, mhl_int_irq_handler,
				   IRQF_TRIGGER_HIGH | IRQF_ONESHOT,
				   "ps5101", ps5101);
	if (ret < 0)
		print_err("Failed to register ps5101 irq\n");

	atomic_set(&ps5101->is_irq_enabled, false);
	disable_irq(client->irq);

	ps5101_msc_wq = create_singlethread_workqueue("ps5101_msc_wq");
	if (!ps5101_msc_wq) {
		print_err("%s() msc workqueue create fail\n", __func__);
		ret = -ENOMEM;
	}

	ps5101_misc_wq = create_singlethread_workqueue("ps5101_misc_wq");
	if (!ps5101_misc_wq) {
		print_err("%s() misc workqueue create fail\n", __func__);
		ret = -ENOMEM;
	}



#ifdef MHL_ACCESSREG_PS5101
	mhl_ps5101 = class_create(THIS_MODULE, "mhl_ps5101");
	if (IS_ERR(mhl_ps5101)) {
		print_err("Failed to create class (mhl_ps5101)\n");
		goto err_exit2;
	}


	ret = class_create_file(mhl_ps5101 , &class_attr_mhl_ps5101);
	if (ret) {
		print_err("failed to create register access file\n");
		goto err_exit3;
	}

	print_info("class file %s created\n", class_attr_mhl_ps5101.attr.name);
#endif

#ifdef __CONFIG_RSEN_TIMER__
	init_timer(&check_rsen_timer);
#endif

	INIT_WORK(&ps5101->waitmeasurezcbus_work, cbuslowwork);

	INIT_WORK(&ps5101->msc_work, ps5101_request_msc_work);

	INIT_WORK(&ps5101->startup_work, initps5101work);

#ifdef SOFTHPD
	INIT_WORK(&ps5101->softhpd_work, soft_hpd_work);
#endif

	INIT_WORK(&ps5101->writestatus_work, writestatwork);


	init_completion(&ps5101->rsen_complete);

	init_completion(&ps5101->rddcap_complete);

	wake_lock_init(&ps5101->wake_lock, WAKE_LOCK_SUSPEND, "mhl");
/*
		{

				int i = 0;
				int ret = 0;
				u8  chipver = 0;

				mutex_lock(&ps5101->i2c_lock);
				ret = I2C_ReadByte( ps5101, 1, 0xf1, &chipver);
				if ( ret < 0 )
				{
					print_err( "read i2c error\n" );
					goto err_exit0;
				}
				mutex_unlock(&ps5101->i2c_lock);
				print_info("chipver=%d\n", chipver);

				if (chipver<1){
					print_info("goto err_exit0\n");
					goto err_exit0;
				}
		}

*/

#ifdef __CONFIG_SS_FACTORY__
	pr_info("ps5101 : create mhl sysfile\n");

	sec_mhl = class_create(THIS_MODULE, "mhl");
	if (IS_ERR(sec_mhl)) {
		pr_err("[ERROR] Failed to create class(sec_mhl)!\n");
		goto err_exit2;
	}

	ret = class_create_file(sec_mhl, &class_attr_test_result);
	if (ret) {
		pr_err("[ERROR] Failed to create device file in sysfs entries!\n");
		goto err_exit2a;
	}
#endif

	return 0;

err_exit3:
	class_destroy(mhl_ps5101);

err_exit2:
	free_irq(client->irq, ps5101);

err_exit1:
	input_free_device(input);

err_exit0:
	kfree(ps5101);

err_exit2a:
/*/#ifdef __CONFIG_SS_FACTORY__
//	class_destroy(sec_mhl);
//#endif
*/
 err_exit2b:
/*/#ifdef __CONFIG_SS_FACTORY__
//	class_remove_file(sec_mhl, &class_attr_test_result);
//#endif
*/
	return ret;
}

static int ps5101_p1_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ps5101_platform_data *pdata = client->dev.platform_data;

	if (!pdata)
		return -EINVAL;

	pdata->mhl_tx_client_p1 = client;

	return 0;
}


static int ps5101_p2_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ps5101_platform_data *pdata = client->dev.platform_data;

	if (!pdata)
		return -EINVAL;

	pdata->mhl_tx_client_p2 = client;

	return 0;
}
static int ps5101_p3_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ps5101_platform_data *pdata = client->dev.platform_data;

	if (!pdata)
		return -EINVAL;

	pdata->mhl_tx_client_p3 = client;

	return 0;
}

static int __devexit ps5101_remove(struct i2c_client *client)
{
	struct ps5101_data *ps5101 = i2c_get_clientdata(client);

	kfree(ps5101);

	return 0;
}

static int __devexit ps5101_p1_remove(struct i2c_client *client)
{

	return 0;
}

static int __devexit ps5101_p2_remove(struct i2c_client *client)
{

	return 0;
}

static int __devexit ps5101_p3_remove(struct i2c_client *client)
{

	return 0;
}


static struct i2c_device_id ps5101_p0_id[] = {
	{"ps5101_p0", 0},
	{}
};

static struct i2c_device_id ps5101_p1_id[] = {
	{"ps5101_p1", 0},
	{}
};

static struct i2c_device_id ps5101_p2_id[] = {
	{"ps5101_p2", 0},
	{}
};

static struct i2c_device_id ps5101_p3_id[] = {
	{"ps5101_p3", 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, ps5101_p0_id);
MODULE_DEVICE_TABLE(i2c, ps5101_p1_id);
MODULE_DEVICE_TABLE(i2c, ps5101_p2_id);
MODULE_DEVICE_TABLE(i2c, ps5101_p3_id);


struct i2c_driver ps5101_i2c_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "ps5101_p0",
	},
	.id_table	= ps5101_p0_id,
	.probe	= ps5101_i2c_probe,
	.remove	= __devexit_p(ps5101_remove),
	.command = NULL,
};

struct i2c_driver ps5101_p1_i2c_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "ps5101_p1",
	},
	.id_table	= ps5101_p1_id,
	.probe	= ps5101_p1_i2c_probe,
	.remove	= __devexit_p(ps5101_p1_remove),
	.command = NULL,
};

struct i2c_driver ps5101_p2_i2c_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "ps5101_p2",
	},
	.id_table	= ps5101_p2_id,
	.probe	= ps5101_p2_i2c_probe,
	.remove	= __devexit_p(ps5101_p2_remove),
	.command = NULL,
};

struct i2c_driver ps5101_p3_i2c_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "ps5101_p3",
	},
	.id_table	= ps5101_p3_id,
	.probe	= ps5101_p3_i2c_probe,
	.remove	= __devexit_p(ps5101_p3_remove),
	.command = NULL,
};


static int __init ps5101_module_init(void)
{
	int ret;
	struct ps5101_data *ps5101;

	print_info("start initialize ps5101\n");


	ret = i2c_add_driver(&ps5101_i2c_driver);
	if (ret < 0) {
		print_err("can't add i2c driver\n");
		return ret;
	}

	ret = i2c_add_driver(&ps5101_p1_i2c_driver);
	if (ret < 0)
		goto err_exit1;

	ret = i2c_add_driver(&ps5101_p2_i2c_driver);
	if (ret < 0)
		goto err_exit2;

	ret = i2c_add_driver(&ps5101_p3_i2c_driver);
	if (ret < 0)
		goto err_exit3;

	MHL_i2c_init = 1;
/*
	ps5101 = dev_get_drvdata(ps5101_mhldev);
	queue_work(ps5101_misc_wq, &(ps5101->startup_work));
*/
	print_info("add i2c driver ok\n");

	return 0;

err_exit3:
	i2c_del_driver(&ps5101_p2_i2c_driver);
err_exit2:
	i2c_del_driver(&ps5101_p1_i2c_driver);
err_exit1:
	i2c_del_driver(&ps5101_i2c_driver);
	print_err("i2c_add_driver fail\n");


	return ret;
}

module_init(ps5101_module_init);
/*sybsys_initcall(ps5101_module_init);*/

static void __exit ps5101_exit(void)
{
	i2c_del_driver(&ps5101_i2c_driver);
	i2c_del_driver(&ps5101_p1_i2c_driver);
	i2c_del_driver(&ps5101_p2_i2c_driver);
	i2c_del_driver(&ps5101_p3_i2c_driver);
};

module_exit(ps5101_exit);

MODULE_DESCRIPTION("ps5101 MHL driver");
MODULE_AUTHOR("Priscilla Zhou");
MODULE_LICENSE("GPL");
