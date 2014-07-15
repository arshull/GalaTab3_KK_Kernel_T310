#include <linux/input.h>
#include <linux/delay.h>
#include <linux/ps5101.h>

#include "ps5101_Common_Def.h"
#include "ps5101_func.h"


#ifdef print_err
	#undef	print_err
#endif

#ifdef print_info
	#undef	print_info
#endif

#ifdef print_dbg
	#undef	print_dbg
#endif



#define	PREFIX5101	"[MHL]ps5101 "
#define	print_err(format, ...) \
		printk(KERN_ERR PREFIX5101 format, ##__VA_ARGS__);
#define	print_info(format, ...) \
		printk(KERN_INFO PREFIX5101 format, ##__VA_ARGS__);
#define	print_dbg(format, ...) \
		printk(KERN_DEBUG PREFIX5101 format, ##__VA_ARGS__);


int I2C_WriteByte(struct ps5101_data *ps5101,
			byte page_no, byte offset, byte value)
{


	int ret;

	switch (page_no) {

	case 0:
		ret = ps5101_i2c_write(ps5101->pdata->mhl_tx_client_p0,
				offset, value);
		break;
	case 1:
		ret = ps5101_i2c_write(ps5101->pdata->mhl_tx_client_p1,
				offset, value);
		break;
	case 2:
		ret = ps5101_i2c_write(ps5101->pdata->mhl_tx_client_p2,
				offset, value);
		break;
	case 3:
		ret = ps5101_i2c_write(ps5101->pdata->mhl_tx_client_p3,
				offset, value);
		break;
	default:
		print_err("I2C_WriteByte page error %x\n", page_no);
		ret = -EINVAL;

	}

	if (ret < 0)
		print_err("i2c write p%d.0x%02x = 0x%02x error\n",
				page_no, offset, value);

	return ret;

}

int I2C_ReadByte(struct ps5101_data *ps5101,
		byte page_no, byte offset, byte *value)
{

	int ret;

	if (value == NULL)
		return -EINVAL;


	switch (page_no) {

	case 0:
		ret = ps5101_i2c_read(ps5101->pdata->mhl_tx_client_p0,
				offset, value);
		break;
	case 1:
		ret = ps5101_i2c_read(ps5101->pdata->mhl_tx_client_p1,
				offset, value);
		break;
	case 2:
		ret = ps5101_i2c_read(ps5101->pdata->mhl_tx_client_p2,
				offset, value);
		break;
	case 3:
		ret = ps5101_i2c_read(ps5101->pdata->mhl_tx_client_p3,
				offset, value);
		break;
	default:
		print_err("I2C_ReadByte page error %x\n", page_no);
		ret = -EINVAL;

	}

	if (ret < 0)
		print_err("i2c read p%d.0x%02x error\n", page_no, offset);


	return ret;

}


int requestMSCMSGcmd(struct ps5101_data *ps5101,
			enum mhlcmd cmd, u8 data1, u8 data2)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing */
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
	if (ret < 0)
		goto err_i2c;

	ret = I2C_WriteByte(ps5101, 3, 0x62, data1);
	if (ret < 0)
		goto err_i2c;

	ret = I2C_WriteByte(ps5101, 3, 0x63, data2);
	if (ret < 0)
		goto err_i2c;
	/*trigger sending*/
	ret = I2C_WriteByte(ps5101, 3, 0x4e, 0x20);
	if (ret < 0)
		goto err_i2c;


	mutex_unlock(&ps5101->i2c_lock);

	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;

}


int requestMSCREADCAPcmd(struct ps5101_data *ps5101,
			enum mhlcmd cmd, u8 data1, u8 data2)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing*/
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
	if (ret < 0)
		goto err_i2c;


	ret = I2C_WriteByte(ps5101, 3, 0x64, data1);
	if (ret < 0)
		goto err_i2c;

	/*trigger sending*/
	ret = I2C_WriteByte(ps5101, 3, 0x4e, 0x40);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);
	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;


}

int requestMSCWRITESTATcmd(struct ps5101_data *ps5101,
			enum mhlcmd cmd, u8 data1, u8 data2)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing*/
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
	if (ret < 0)
		goto err_i2c;

	ret = I2C_WriteByte(ps5101, 3, 0x67, data1);
	if (ret < 0)
		goto err_i2c;


	ret = I2C_WriteByte(ps5101, 2, data1, data2);
	if (ret < 0)
		goto err_i2c;

	/*send WRITE_STAT cmd*/
	ret = I2C_WriteByte(ps5101, 3, 0x4f, 0x01);
	if (ret < 0)
		goto err_i2c;


	mutex_unlock(&ps5101->i2c_lock);

	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;


}

int en_wakeup(struct ps5101_data *ps5101)
{
	int ret = 0;

	complete(&ps5101->zcbus_complete);

	print_info("zcbus complete sent\n");

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing */
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
	if (ret < 0)
		goto err_i2c;
	/*enable wakeup sequence*/
	ret = I2C_WriteByte(ps5101, 3, 0x28, 0x60);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);
	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;

}

/*#ifdef PS5101B0*/
int zbus_fail(struct ps5101_data *ps5101)
{
	complete(&ps5101->zcbus_complete);

	print_info("zcbus complete sent\n");

	return 0;
}
/*#endif*/


int stand_by(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);

	/*enable register writing*/
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
	if (ret < 0)
		goto err_i2c;

	/*set to standby*/
	ret = I2C_WriteByte(ps5101, 1, 0xf5, 0x04);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);

	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;

}

int set_mhlmode(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing*/
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
	if (ret < 0)
		goto err_i2c;

	/*set to MHL mode*/
	ret = I2C_WriteByte(ps5101, 1, 0xf5, 0x02);
	if (ret < 0)
		goto err_i2c;

	/*enable check for INTER_ALIGN/SRC_MHLPAIR_ACT*/
	/*disable SRC_MHLPAIR_ACT check*/
	ret = I2C_WriteByte(ps5101, 1, 0xeb, 0x03);

	if (ret < 0)
		goto err_i2c;

	/*disable discovery*/
	ret = I2C_WriteByte(ps5101, 3, 0x08, 0x00);

	if (ret < 0)
		goto err_i2c;

	/*enable all MSC command*/
	ret = I2C_WriteByte(ps5101, 3, 0x4d, 0x0c);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);

	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;

}

int start_discovery(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*disable wait some time to discovery process after switch to MHL*/
	/*enable wake and discovery sequence when VBUS sensed*/
	ret = I2C_WriteByte(ps5101, 3, 0x08, 0x60);

	mutex_unlock(&ps5101->i2c_lock);

	if (ret < 0)
		print_err("I2C write error\n");


	print_info("enable discovery\n");

	return ret;
}


int set_usbmode(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing*/
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);
	if (ret < 0)
		goto err_i2c;

	/*set to USB mode*/
	ret = I2C_WriteByte(ps5101, 1, 0xf5, 0x09);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);

	ps5101_mhl_onoff_ex(false);
	mhl_hpd_handler(false);
	msleep(100);
	ps5101_mhl_onoff_ex(true);

	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;



}

#ifdef SOFTHPD

int softset_hpd(struct ps5101_data *ps5101)
{
	int ret;
	u8 readvalue;
	u8 hpd = 0;

	print_dbg("soft set HPD\n");

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, 3, 0x02, &readvalue);
	mutex_unlock(&ps5101->i2c_lock);

	if (ret < 0) {
		print_err("i2c read err\n");
		return ret;
	}

	readvalue = readvalue & 0xef;

	print_dbg("soft set HPD deasserted\n");

	mutex_lock(&ps5101->i2c_lock);
	/*software control HPD*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, readvalue);
	mutex_unlock(&ps5101->i2c_lock);
	if (ret < 0) {
		print_err("i2c write err\n");
		return ret;
	}

	msleep(SOFTHPD_DELAY);

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_WriteByte(ps5101, 3, 0xae, 0x0b);
	mutex_unlock(&ps5101->i2c_lock);
	if (ret < 0) {
		print_err("i2c write err\n");
		return ret;
	}

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, 3, 0xaf, &readvalue);
	mutex_unlock(&ps5101->i2c_lock);
	if (ret < 0) {
		print_err("i2c read err\n");
		return ret;
	}

	hpd = (readvalue & 0x20) >> 1;

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, 3, 0x02, &readvalue);
	mutex_unlock(&ps5101->i2c_lock);

	if (ret < 0) {
		print_err("i2c read err\n");
		return ret;
	}



	readvalue = (readvalue & 0xef) | hpd;

	mutex_lock(&ps5101->i2c_lock);
	/*software control HPD*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, readvalue);
	mutex_unlock(&ps5101->i2c_lock);
	if (ret < 0) {
		print_err("i2c write err\n");
		return ret;
	}

	print_dbg("soft set HPD to %d\n", hpd >> 4);

	return 0;
}

#endif

int softset_hpd_des(struct ps5101_data *ps5101)
{
	int ret;
	u8 readvalue;

	print_dbg("soft set HPD deassserted\n");

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, 3, 0x02, &readvalue);
	mutex_unlock(&ps5101->i2c_lock);

	if (ret < 0) {
		print_err("i2c read err\n");
		return ret;
	}
	/*software deassert HPD*/
	readvalue = (readvalue & 0xef) | 0x08;

	mutex_lock(&ps5101->i2c_lock);
	/*software control HPD*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, readvalue);
	mutex_unlock(&ps5101->i2c_lock);
	if (ret < 0) {
		print_err("i2c write err\n");
		return ret;
	}

	return 0;
}




#ifdef PATCH4CTS

int hwhpd(struct ps5101_data *ps5101)
{
	int ret;
	u8 readvalue;
	u8 hpd = 0;


	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, 3, 0x02, &readvalue);
	mutex_unlock(&ps5101->i2c_lock);
	if (ret < 0) {
		print_err("i2c read err\n");
		return ret;
	}

	readvalue = readvalue & 0xf7;

	mutex_lock(&ps5101->i2c_lock);
	/*hardware control HPD*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, readvalue);
	mutex_unlock(&ps5101->i2c_lock);
	if (ret < 0) {
		print_err("i2c write err\n");
		return ret;
	}

	print_dbg("hardware set HPD\n");

	return 0;
}
#endif

int handle_WRITE_STAT_INT(struct ps5101_data *ps5101)
{
	int ret;
	u8  status_connected_rdy, status_link_mode;
#ifdef SWREAD_DCAP
	int i;
#endif

	mutex_lock(&ps5101->i2c_lock);

	ret = I2C_ReadByte(ps5101, 2, 0xb1, &status_link_mode);
	if (ret < 0) {
		print_err("read i2c error\n");
		mutex_unlock(&ps5101->i2c_lock);

		return ret;
	}

	ret = I2C_ReadByte(ps5101, 2, 0xb0, &status_connected_rdy);
	mutex_unlock(&ps5101->i2c_lock);

	if (ret < 0) {
		print_err("read i2c error\n");
		return ret;
	}


	if (((ps5101->status_link_mode ^ status_link_mode) &
		MHL_MSC_WRITESTATE_PATHEN)
		 || ((ps5101->status_connected_rdy == status_connected_rdy)
			&& (ps5101->status_link_mode == status_link_mode)
			&& ((status_link_mode & MHL_MSC_WRITESTATE_PATHEN) ==\
			MHL_MSC_WRITESTATE_NO_PATHEN)
		)) {
		print_info("getting PATH_EN = %02x\n", status_link_mode);

		ps5101_enqueue_msc_work(ps5101,
			MHL_WRITE_STAT,
			MHL_MSC_WRITESTATE_LINK_MODE,
			status_link_mode & MHL_MSC_WRITESTATE_PATHEN);

	}



	if ((status_connected_rdy & MHL_MSC_WRITESTATE_DCAP_RDY)\
			== MHL_MSC_WRITESTATE_DCAP_RDY) {
		print_info("capability register values are stable\n");

#ifdef SWREAD_DCAP
/*		for (i = 0; i < 2; i++)
			ps5101_enqueue_msc_work(ps5101, MHL_READ_DEVCAP, i, 0);		*/
	
		ps5101_enqueue_msc_work(ps5101, MHL_READ_DEVCAP, 0x02, 0);		
	
#endif
	}




	ps5101->status_connected_rdy = status_connected_rdy;
	ps5101->status_link_mode = status_link_mode;


	return 0;


}

struct ps5101_regset ps5101A0_maskreg[] = {

	{ 3, 0x39, 0x20 },	/* open all discovery irq*/
	{ 3, 0xa0, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa1, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa2, 0xc0 },	/* open all MSC irq*/
	{ 3, 0xa3, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa4, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa5, 0xe0 },	/* open all MSC irq*/
	{ 3, 0xa6, 0xe0 },	/* open all MSC irq*/
	{ 3, 0xa7, 0xfd },	/* open all MSC irq*/

	{ 0xff, 0xff, 0xff },

};

struct ps5101_regset ps5101B0_maskreg[] = {


	{ 3, 0x39, 0x00 },	/* open all discovery irq*/
	{ 3, 0xa0, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa1, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa2, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa3, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa4, 0x00 },	/* open all MSC irq*/
	{ 3, 0xa5, 0xc0 },	/* open all MSC irq*/
	{ 3, 0xa6, 0xe0 },	/* open all MSC irq*/
	{ 3, 0xa7, 0xfd },	/* open all MSC irq*/

	{ 0xff, 0xff, 0xff },

};


int unmask_INT(struct ps5101_data *ps5101)
{
	int i = 0;
	int ret = 0;

	print_info("unmask interrupt\n");

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing*/
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);

	if (ret < 0) {
		print_err("unmask interrupt mask error\n");
		mutex_unlock(&ps5101->i2c_lock);
		return ret;
	}


	if (ps5101->chipver == 0x0a) {

		while (ps5101A0_maskreg[i].page_no < 4) {
			ret = I2C_WriteByte(ps5101,
				ps5101A0_maskreg[i].page_no,
				ps5101A0_maskreg[i].offset,
				ps5101A0_maskreg[i].value);
			if (ret < 0) {
				print_err("unmask interrupt mask error\n");
				break;
			}

			i++;
		}
	} else if (ps5101->chipver == 0x0b) {
		while (ps5101B0_maskreg[i].page_no < 4) {
			ret = I2C_WriteByte(ps5101,
				ps5101B0_maskreg[i].page_no,
				ps5101B0_maskreg[i].offset,
				ps5101B0_maskreg[i].value);
			if (ret < 0) {
				print_err("unmask interrupt mask error\n");
				break;
			}

			i++;
		}
	}

	mutex_unlock(&ps5101->i2c_lock);

	return ret;

}

#if 0
int mask_INT(struct ps5101_data *ps5101)
{
	int i = 0;
	int ret = 0;

	print_info("mask interrupt\n");

	mutex_lock(&ps5101->i2c_lock);
	/*enable register writing*/
	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x00);

	if (ret < 0) {
		print_err("mask interrupt mask error\n");
		mutex_unlock(&ps5101->i2c_lock);
		return ret;
	}

	while (ps5101_maskreg[i].page_no < 4) {
		ret = I2C_WriteByte(ps5101,
			ps5101_maskreg[i].page_no,
			ps5101_maskreg[i].offset,
			~ps5101_maskreg[i].value);
		if (ret < 0) {
			print_err("mask interrupt mask error\n");
			break;
		}

		i++;
	}

	mutex_unlock(&ps5101->i2c_lock);

	return ret;

}
#endif

int request_3D(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);

	/*clear the int; p3.96[4] is mapping from this bit*/
	ret = I2C_WriteByte(ps5101, 2, 0xa0, 0x10);

	mutex_unlock(&ps5101->i2c_lock);

	return ret;
}

int grant_write(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*clear the int; p3.96[3] is mapping from this bit*/
	ret = I2C_WriteByte(ps5101, 2, 0xa0, 0x08);
	mutex_unlock(&ps5101->i2c_lock);

	return ret;
}

int request_write(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*clear the int; p3.96[2] is mapping from this bit*/
	ret = I2C_WriteByte(ps5101, 2, 0xa0, 0x04);
	mutex_unlock(&ps5101->i2c_lock);

	return ret;
}


int scrachpad_change(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*clear the int; p3.96[1] is mapping from this bit*/
	ret = I2C_WriteByte(ps5101, 2, 0xa0, 0x02);
	mutex_unlock(&ps5101->i2c_lock);

	return ret;
}

int capability_change(struct ps5101_data *ps5101)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);
	/*clear the int; p3.96[0] is mapping from this bit*/
	ret = I2C_WriteByte(ps5101, 2, 0xa0, 0x01);
	mutex_unlock(&ps5101->i2c_lock);

	return ret;
}

int edid_change(struct ps5101_data *ps5101)
{

	int ret = 0;
	u8  readvalue;


	mutex_lock(&ps5101->i2c_lock);
	/*clear the int; p3.97[1] is mapping from this bit*/
	ret = I2C_WriteByte(ps5101, 2, 0xa1, 0x02);
	if (ret < 0)
		goto err_i2c;

	ret = I2C_ReadByte(ps5101, 3, 0x02, &readvalue);

	if (ret < 0)
		goto err_i2c;

	readvalue = (readvalue | 0x08) & 0xef;
	/*software control deassert HPD*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, readvalue);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);
	print_dbg("deassert HPD\n");


	msleep(HPD_TOGGLE_WIDTH);

	mutex_lock(&ps5101->i2c_lock);

#ifdef SOFTHPD
	readvalue = readvalue | 0x10;
#else
	readvalue = readvalue & 0xf7;
#endif

	/*resume to hardware control HPD*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, readvalue);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);

	print_dbg("resume hardware HPD status\n");

	return 0;

err_i2c:
	mutex_unlock(&ps5101->i2c_lock);
	return ret;

}

static int is_rcp_key_code_valid(u8 key)
{
	switch (key+1) {
	/*should resemble /system/usr/keylayout/ps5101_rcp.kl*/
	case 1:		/* ENTER		WAKE_DROPPED*/
	case 2:		/* DPAD_UP		WAKE_DROPPED*/
	case 3:		/* DPAD_DOWN		WAKE_DROPPED*/
	case 4:		/* DPAD_LEFT		WAKE_DROPPED*/
	case 5:		/* DPAD_RIGHT		WAKE_DROPPED*/
	case 10:	/* MENU			WAKE_DROPPED*/
	case 14:	/* BACK			WAKE_DROPPED*/
	case 33:	/* 0	*/
	case 34:	/* 1	*/
	case 35:	/* 2	*/
	case 36:	/* 3	*/
	case 37:	/* 4	*/
	case 38:	/* 5	*/
	case 39:	/* 6	*/
	case 40:	/* 7	*/
	case 41:	/* 8	*/
	case 42:	/* 9	*/
	case 43:    /* ENTER*/
	case 45:	/* DEL	*/
	case 69:    /* MEDIA_PLAY_PAUSE		WAKE*/
	case 70:    /* MEDIA_STOP		WAKE*/
	case 71:    /* MEDIA_PAUSE		WAKE*/
	case 73:    /* MEDIA_REWIND		WAKE*/
	case 74:    /* MEDIA_FAST_FORWARD	WAKE*/
	case 76:    /* MEDIA_NEXT		WAKE*/
	case 77:    /* MEDIA_PREVIOUS		WAKE*/
		return 1;
	default:
		return 0;
	}

}

static void rcp_uevent_report(struct ps5101_data *ps5101, u8 key)
{
	if (!ps5101->input_dev) {
		print_err("input device not available\n");
		return;
	}


	print_info("rcp_uevent_report key: %d\n", key);
	input_report_key(ps5101->input_dev, (unsigned int)key+1, 1);
	input_report_key(ps5101->input_dev, (unsigned int)key+1, 0);
	input_sync(ps5101->input_dev);
}


int handle_mscmsg(struct ps5101_data *ps5101)
{
	int ret = 0;
	u8  data1 = 0;
	u8  data2 = 0;

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, 3, 0x41, &data1);
	mutex_unlock(&ps5101->i2c_lock);


	if (ret < 0) {
		print_err("read i2c error\n");
		return ret;
	}

	mutex_lock(&ps5101->i2c_lock);
	ret = I2C_ReadByte(ps5101, 3, 0x42, &data2);
	mutex_unlock(&ps5101->i2c_lock);


	if (ret < 0) {
		print_err("read i2c error\n");
		return ret;
	}


	if (data1 == MHL_MSC_MSG_RCP) {

		if (is_rcp_key_code_valid(data2)) {
			rcp_uevent_report(ps5101, data2);
			ps5101_enqueue_msc_work(ps5101,
				MHL_MSC_MSG, MHL_MSC_MSG_RCPK, data2);

		} else {
			print_info("rcp ineffective key code: %d\n", data2);
			ps5101_enqueue_msc_work(ps5101,
				MHL_MSC_MSG, MHL_MSC_MSG_RCPE,
				MHL_MSC_MSG_RCPE_INEFFECTIVE);

		}

	} else if (data1 == MHL_MSC_MSG_RAP) {
		if (data2 == MHL_MSC_MSG_RAP_POLL) {
			ps5101_enqueue_msc_work(ps5101,
				MHL_MSC_MSG, MHL_MSC_MSG_RAPK,
				MHL_MSC_MSG_RAPK_NOERROR);

			print_info("get rap poll\n");
		} else if (data2 == MHL_MSC_MSG_RAP_CONTENTON) {
			ps5101_enqueue_msc_work(ps5101,
				MHL_MSC_MSG, MHL_MSC_MSG_RAPK,
				MHL_MSC_MSG_RAPK_NOERROR);

			print_info("get rap content on\n");

			mutex_lock(&ps5101->i2c_lock);
			/*get HPD status*/
			ret = I2C_WriteByte(ps5101, 3, 0xae, 0x0b);
			if (ret < 0) {
				print_err("read i2c error\n");
				mutex_unlock(&ps5101->i2c_lock);
				return ret;
			}


			ret = I2C_ReadByte(ps5101, 3, 0xaf, &data1);
			mutex_unlock(&ps5101->i2c_lock);

			if (ret < 0) {
				print_err("read i2c error\n");
				return ret;
			}

			if ((data1 & 0x20) == 0x20) { /*HPD asserted*/
				mutex_lock(&ps5101->i2c_lock);
				ret = I2C_ReadByte(ps5101, 1, 0x32, &data1);
				if (ret < 0) {
					print_err("read i2c error\n");
					mutex_unlock(&ps5101->i2c_lock);
					return ret;
				}
				/*open TMDS*/
				ret = I2C_WriteByte(ps5101, 1, 0x32,
							data1 & 0xfd);
				mutex_unlock(&ps5101->i2c_lock);

				if (ret >= 0) {
					print_info("TMDS opened\n");
				} else {
					print_err("write i2c error to open TMDS\n");
				}
			}


		} else if (data2 == MHL_MSC_MSG_RAP_CONTENTOFF) {
			ps5101_enqueue_msc_work(ps5101, MHL_MSC_MSG,
					MHL_MSC_MSG_RAPK,
					MHL_MSC_MSG_RAPK_NOERROR);

			mutex_lock(&ps5101->i2c_lock);
			ret = I2C_ReadByte(ps5101, 1, 0x32, &data1);
			if (ret < 0) {
				print_err("read i2c error\n");
				mutex_unlock(&ps5101->i2c_lock);
				return ret;
			}
			/*close TMDS*/
			ret = I2C_WriteByte(ps5101, 1, 0x32, data1 | 0x02);
			mutex_unlock(&ps5101->i2c_lock);

			if (ret >= 0) {
				print_info("get rap content off, close TMDS\n");
			} else {
				print_err("write i2c error to close TMDS\n");
			}

		} else {
			ps5101_enqueue_msc_work(ps5101,
				MHL_MSC_MSG, MHL_MSC_MSG_RAPK,
				MHL_MSC_MSG_RAPK_UNRECOGNIZED);

			print_info("get rap code not recognized\n");
		}

	}

	return ret;
}


struct ps5101_regset ps5101A0_initvalue[] = {
#if (!defined(LCO_CLOCK19M))
	{0, 0x10, 0x92 },
#endif
	{0, 0x11, 0x94 },
	{1, 0xf4, 0x00 },
	{1, 0xeb, 0x03 },
	{1, 0x2b, 0x00 },
/*/	{1, 0x2b, 0x00 },*/
	{1, 0x2c, 0x0d },		/*for debug; CTS*/
	{1, 0x2d, 0x03 },
	{1, 0x00, 0x40 },		/*enable external switch*/

	{1, 0x32, 0x10 },


	{1, 0x32, 0x04 },		/*force drive on*/



	{1, 0x01, 0x42 },
/*//	{1, 0x2f, 0x40 },		// close internal termination*/
/*//	{1, 0x30, 0x80 },		// close internal termination*/

	{2, 0x00, 0x00 },
	{2, 0x01, 0x12 }, /*MHL_VERSION*/
	{2, 0x02, 0x02 },
	{2, 0x03, 0x83 }, /*ADOPTER_ID_H*/
	{2, 0x04, 0x84 }, /*ADOPTER_ID_L*/
	{2, 0x05, 0x03 }, /*VID_LINK_MODE*/
	{2, 0x06, 0x03 }, /*AID_LINK_MODE*/
	{2, 0x07, 0x00 },
	{2, 0x08, 0x80 },
	{2, 0x09, 0x0f },
	{2, 0x0a, 0x07 },
	{2, 0x0b, 0x00 },
	{2, 0x0c, 0x00 },
	{2, 0x0d, 0x10 },
	{2, 0x0e, 0x33 },

#ifdef SOFTHPD
	{3, 0x02, 0x08 },
#endif
#ifdef PATCH4CTS
	{3, 0x02, 0x28 },
#endif
	{3, 0x08, 0x60 },
	{3, 0x4d, 0x0c },
	{3, 0x03, 0x0d },
	{3, 0x31, 0x70 },

#ifndef SWREAD_DCAP
	{3, 0x5d, 0x12 },
	{3, 0x5e, 0x09 },
#else
	{3, 0x5d, 0x12 },
	{3, 0x5e, 0x01 },
#endif


	{3, 0x74, 0x18 },
	{3, 0x71, 0x18 },
	{3, 0x72, 0x18 },
	{3, 0x73, 0x18 },
	{3, 0x55, 0x18 },
	{3, 0x56, 0x18 },
	{3, 0x57, 0x18 },
	{3, 0x58, 0x18 },



#if (!defined(LCO_CLOCK19M))
	{3, 0x5a, 0x40 },
	{3, 0x5b, 0x43 },
	{3, 0x1f, 0x46 },
	{3, 0x1e, 0x50 },
	{3, 0x1d, 0xb4 },
#endif


	{3, 0x5a, 0x32 },
	{3, 0x5b, 0x6e },



	{1, 0xf5, 0x09 },

	{ 0xff, 0xff, 0xff },

};

struct ps5101_regset ps5101B0_initvalue[] = {
	{0, 0x11, 0x94 },
	{1, 0xf4, 0x00 },
	{1, 0xeb, 0x03 },
	{1, 0x2b, 0x00 },
/*	{1, 0x2b, 0x00 },*/
	{1, 0x2c, 0x0d },		/*for debug; CTS*/
	{1, 0x2d, 0x03 },
	{1, 0x00, 0x40 },		/*enable external switch*/

	{1, 0x32, 0x10 },



	{1, 0x01, 0x42 },
/*//	{1, 0x2f, 0x40 },		close internal termination*/
/*//	{1, 0x30, 0x80 },		close internal termination*/


	{2, 0x00, 0x00 },
	{2, 0x01, 0x12 }, /*MHL_VERSION*/
	{2, 0x02, 0x02 },
	{2, 0x03, 0x01 }, /*ADOPTER_ID_H*/
	{2, 0x04, 0x41 }, /*ADOPTER_ID_L*/
	{2, 0x05, 0x03 }, /*VID_LINK_MODE*/
	{2, 0x06, 0x03 }, /*AID_LINK_MODE*/
	{2, 0x07, 0x00 },
	{2, 0x08, 0x80 },
	{2, 0x09, 0x0f },
	{2, 0x0a, 0x07 },
	{2, 0x0b, 0x00 },
	{2, 0x0c, 0x00 },
	{2, 0x0d, 0x10 },
	{2, 0x0e, 0x33 },

#ifdef SOFTHPD
	{3, 0x02, 0x08 },
#endif
#ifdef PATCH4CTS
	{3, 0x02, 0x28 },
#endif
	{3, 0x08, 0x60 },
	{3, 0x4d, 0x0c },
	{3, 0x03, 0x0d },
	{3, 0x31, 0x70 },

#ifndef SWREAD_DCAP
	{3, 0x5d, 0x12 },
	{3, 0x5e, 0x09 },
#else
	{3, 0x5d, 0x12 },
	{3, 0x5e, 0x01 },
#endif

	{3, 0x5a, 0x32 },
	{3, 0x5b, 0x6e },

	{1, 0xf5, 0x09 },

	{ 0xff, 0xff, 0xff },

};

int initps5101(struct ps5101_data *ps5101)
{

	int i = 0;
	int ret = 0;
	u8  chipver = 0;

	mutex_lock(&ps5101->i2c_lock);
	msleep(200);

	ret = I2C_WriteByte(ps5101, 1, 0xf4, 0x80);
	if (ret < 0)
		goto err_i2c;

#ifdef	LCO_CLOCK18M
	ret = I2C_WriteByte(ps5101, 1, 0x1e, 0xe0);
	if (ret < 0)
		goto err_i2c;

	ret = I2C_WriteByte(ps5101, 1, 0x1f, 0x07);
	if (ret < 0)
		goto err_i2c;

#endif



#ifdef	LCO_CLOCK19M
	ret = I2C_WriteByte(ps5101, 1, 0x1e, 0x06);
	if (ret < 0)
		goto err_i2c;

	ret = I2C_WriteByte(ps5101, 1, 0x1f, 0x04);
	if (ret < 0)
		goto err_i2c;
#endif

	ret = I2C_ReadByte(ps5101, 1, 0xf1, &chipver);
	if (ret < 0) {
		print_err("read i2c error\n");
		goto err_i2c;
	}


	/*wait 10ms*/
	usleep_range(STARTUP_DELAY * USEC_PER_MSEC,
			STARTUP_DELAY * USEC_PER_MSEC);

	if (chipver == 0x0a) {

		while (ps5101A0_initvalue[i].page_no < 4) {
			ret = I2C_WriteByte(ps5101,
				ps5101A0_initvalue[i].page_no,
				ps5101A0_initvalue[i].offset,
				ps5101A0_initvalue[i].value);
			i++;

			if (ret < 0)
				break;
		}
	} else if (chipver == 0x0b) {
		while (ps5101B0_initvalue[i].page_no < 4) {
			ret = I2C_WriteByte(ps5101,
				ps5101B0_initvalue[i].page_no,
				ps5101B0_initvalue[i].offset,
				ps5101B0_initvalue[i].value);
			i++;

			if (ret < 0)
				break;
		}
	} else {
		print_err("chip version error\n");
		goto err_i2c;
	}

	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);


	ps5101->cleared = true;
	ps5101->standby = false;
	ps5101->status_connected_rdy = 0;
	ps5101->status_link_mode = 0;
	ps5101->chipver = chipver;


	ret = unmask_INT(ps5101);

	if (ret >= 0)
		print_info("initialized\n");

	return ret;

err_i2c:
	mutex_unlock(&ps5101->i2c_lock);
	print_info("err in initialization\n");
	return ret;

}


int finish_mscmsg(struct ps5101_data *ps5101)
{

	complete(&ps5101->mscmsg_complete);

	print_info("MSC_MSG complete sent\n");

	return 0;

}

int finish_readdcap_fail(struct ps5101_data *ps5101)
{

	int ret = 0;
	u8 data1 = 0;

#ifdef SWREAD_DCAP
	complete(&ps5101->mscrcap_complete);

	print_info("MSC_READ_DEVCAP complete sent\n");
#endif


	mutex_lock(&ps5101->i2c_lock);

	ret = I2C_ReadByte(ps5101, 3, 0x5d, &data1);
	if (ret < 0) {
		print_err("read i2c error\n");
		goto err_i2c;
	}
	/*disable HW read DCAP*/
	ret = I2C_WriteByte(ps5101, 3, 0x5d, data1 & 0xbf);
	if (ret < 0) {
		print_err("write i2c error\n");
		goto err_i2c;
	}
	/*enable HW read DCAP*/
	ret = I2C_WriteByte(ps5101, 3, 0x5d, data1 | 0x40);
	if (ret < 0) {
		print_err("write i2c error\n");
		goto err_i2c;
	}

	mutex_unlock(&ps5101->i2c_lock);

	return 0;

err_i2c:
	mutex_unlock(&ps5101->i2c_lock);
	return ret;



}


int finish_readdcap_pass(struct ps5101_data *ps5101)
{


#ifdef SWREAD_DCAP
	complete(&ps5101->mscrcap_complete);

	print_info("MSC_READ_DEVCAP complete sent\n");
#endif

	return 0;

}


int finish_writestat(struct ps5101_data *ps5101)
{
	complete(&ps5101->mscwstat_complete);

	print_info("MSC_WRITE_STAT complete sent\n");

	return 0;
}

int discovery_fail(struct ps5101_data *ps5101)
{
	set_usbmode(ps5101);
	/*mhl_onoff_ex(false);*/
	msleep(DISCOVERYFAIL_DELAY);

	return 0;

}

/*#ifdef PS5101B0*/
int clear_status(struct ps5101_data *ps5101)
{
	int ret = 0;
	u8 data1 = 0;

	mutex_lock(&ps5101->i2c_lock);

	ret = I2C_ReadByte(ps5101, 3, 0x5d, &data1);
	if (ret < 0) {
		print_err("read i2c error\n");
		goto err_i2c;
	}

	data1 = data1 | 0x04;
	/*reset RX device related registers*/
	ret = I2C_WriteByte(ps5101, 3, 0x5d, data1);
	if (ret < 0)
		goto err_i2c;

	data1 = data1 & 0xfd;
	/*release reset RX device related registers*/
	ret = I2C_WriteByte(ps5101, 3, 0x5d, data1);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);

	return 0;

err_i2c:

	mutex_unlock(&ps5101->i2c_lock);

	return ret;


}

int clear_hpd(struct ps5101_data *ps5101)
{
	int ret = 0;
	u8  readvalue;
	u8  savedvalue;


	mutex_lock(&ps5101->i2c_lock);


	ret = I2C_ReadByte(ps5101, 3, 0x02, &readvalue);

	if (ret < 0)
		goto err_i2c;


	savedvalue = readvalue;

	readvalue = (readvalue | 0x08) & 0xef;
	/* software control deassert HPD*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, readvalue);
	if (ret < 0)
		goto err_i2c;


	mutex_unlock(&ps5101->i2c_lock);
	print_dbg("soft deassert HPD\n");


	msleep(50);

	mutex_lock(&ps5101->i2c_lock);
	/*resume to original HPD setting*/
	ret = I2C_WriteByte(ps5101, 3, 0x02, savedvalue);
	if (ret < 0)
		goto err_i2c;

	mutex_unlock(&ps5101->i2c_lock);

	print_dbg("resume original HPD status\n");

	return 0;

err_i2c:
	mutex_unlock(&ps5101->i2c_lock);
	return ret;

}


int read_dcap_action(struct ps5101_data *ps5101)
{



	complete(&ps5101->rddcap_complete);

	print_info("READ_DEVCAP complete sent\n");


	return 0;

}

int set_cbus_timer(struct ps5101_data *ps5101, u8 value)
{
	int ret = 0;

	mutex_lock(&ps5101->i2c_lock);

	/*software control deassert HPD*/
	ret = I2C_WriteByte(ps5101, 0, 0x12, value);

	mutex_unlock(&ps5101->i2c_lock);

	if (ret < 0)
		print_err("i2c error\n");

	return ret;

}

/*#endif*/

