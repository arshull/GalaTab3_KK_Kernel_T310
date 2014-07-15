/***************************************************************************

*

*   PS5101 - MHL Transmitter Driver

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
#ifndef Common_Def_H
#define Common_Def_H

#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/time.h>

#include <linux/wakelock.h>

#define			PS5101B0

#ifndef PS5101B0
#define			LCO_CLOCK18M
/*#define			LCO_CLOCK19M*/
#endif

#define		SWREAD_DCAP
/*#define		SOFTHPD*/

/*should not enabled with SOFTHPD*/
#define		PATCH4CTS

typedef u8 byte;
typedef u32 word;
typedef bool Bool;


/*struct i2c_client* get_ps5101_client(u8 device_id);*/
int ps5101_i2c_read(struct i2c_client *client, u8 reg, u8* value);
int ps5101_i2c_write(struct i2c_client *client, u8 reg, u8 data);


/*Device Power State*/
#define MHL_DEV_UNPOWERED		0x00
#define MHL_DEV_INACTIVE		0x01
#define MHL_DEV_QUIET			0x03
#define MHL_DEV_ACTIVE			0x04

/*Version that this chip supports*/
#define	MHL_VER_MAJOR		(0x01 << 4)	/*bits 4..7*/
#define	MHL_VER_MINOR		0x00	/*bits 0..3*/
#define MHL_VERSION		(MHL_VER_MAJOR | MHL_VER_MINOR)


/*Video Link Mode*/
#define	MHL_DEV_VID_LINK_SUPPRGB444			0x01
#define	MHL_DEV_VID_LINK_SUPPYCBCR444		0x02
#define	MHL_DEV_VID_LINK_SUPPYCBCR422		0x04
#define	MHL_DEV_VID_LINK_PPIXEL				0x08
#define	MHL_DEV_VID_LINK_SUPP_ISLANDS		0x10

/*Audio Link Mode Support*/
#define	MHL_DEV_AUD_LINK_2CH				0x01
#define	MHL_DEV_AUD_LINK_8CH				0x02


/*Feature Flag in the devcap*/
#define	MHL_DEV_FEATURE_FLAG_OFFSET			0x0A
/*Dongles have freedom to not support RCP*/
#define	MHL_FEATURE_RCP_SUPPORT				BIT_0
/*Dongles have freedom to not support RAP*/
#define	MHL_FEATURE_RAP_SUPPORT				BIT_1
/*Dongles have freedom to not support SCRATCHPAD*/
#define	MHL_FEATURE_SP_SUPPORT				BIT_2

/*
#define	MHL_POWER_SUPPLY_CAPACITY	16	// 160 mA current
#define	MHL_POWER_SUPPLY_PROVIDED	16	// 160mA 0r 0 for Wolverine.
#define	MHL_HDCP_STATUS				0	// Bits set dynamically
*/

/*VIDEO TYPES*/
#define		MHL_VT_GRAPHICS					0x00
#define		MHL_VT_PHOTO					0x02
#define		MHL_VT_CINEMA					0x04
#define		MHL_VT_GAMES					0x08
#define		MHL_SUPP_VT						0x80

/*Logical Dev Map*/
#define	MHL_DEV_LD_DISPLAY					(0x01 << 0)
#define	MHL_DEV_LD_VIDEO					(0x01 << 1)
#define	MHL_DEV_LD_AUDIO					(0x01 << 2)
#define	MHL_DEV_LD_MEDIA					(0x01 << 3)
#define	MHL_DEV_LD_TUNER					(0x01 << 4)
#define	MHL_DEV_LD_RECORD					(0x01 << 5)
#define	MHL_DEV_LD_SPEAKER					(0x01 << 6)
#define	MHL_DEV_LD_GUI						(0x01 << 7)

/*Bandwidth*/
/*225 MHz*/
#define	MHL_BANDWIDTH_LIMIT					22


#define	MHL_STATUS_DCAP_RDY					BIT_0
#define	MHL_INT_DCAP_CHG					BIT_0

/*On INTR_1 the EDID_CHG is located at BIT 0*/
#define	MHL_INT_EDID_CHG					BIT_1
/*This contains one nibble each - max offset*/
#define		MHL_INT_AND_STATUS_SIZE			0x303
#define		MHL_SCRATCHPAD_SIZE				16
/*manually define highest number*/
#define		MHL_MAX_BUFFER_SIZE		MHL_SCRATCHPAD_SIZE

#define		STARTUP_DELAY					10	/*ms*/
#define		HPD_TOGGLE_WIDTH				500
#define		SOFTHPD_DELAY					600
#define		ZCBUS_DELAY						1200
#define		MSC_DELAY						200
#define		CBUSFLOAT_DELAY					500
#define		MSC_CHECKIDLE_INTERVAL			10
#define		MSC_CHECKIDLE_REPEAT			300/*times*/
#define		MSC_WRITESTAT_DELAY				500
#define		RSEN_DELAY						400
#define		DISCOVERYFAIL_DELAY				500
#define		INIT_DELAY						100

enum {
	/*RCP sub-command*/
	MHL_MSC_MSG_RCP             = 0x10,
	/*RCP Acknowledge sub-command*/
	MHL_MSC_MSG_RCPK            = 0x11,
	/*RCP Error sub-command*/
	MHL_MSC_MSG_RCPE            = 0x12,
	/*Mode Change Warning sub-command*/
	MHL_MSC_MSG_RAP             = 0x20,
	/*MCW Acknowledge sub-command*/
	MHL_MSC_MSG_RAPK            = 0x21
};

enum {
	MHL_MSC_MSG_RAP_POLL		= 0x00,
	MHL_MSC_MSG_RAP_CONTENTON	= 0x10,
	MHL_MSC_MSG_RAP_CONTENTOFF	= 0x11

};

enum {
	MHL_MSC_MSG_RAPK_NOERROR		= 0x00,
	MHL_MSC_MSG_RAPK_UNRECOGNIZED	= 0x01,
	MHL_MSC_MSG_RAPK_UNSUPPORTED	= 0x02,
	MHL_MSC_MSG_RAPK_BUSY			= 0x03,
};

enum {
	MHL_MSC_MSG_RCPE_NOERROR		= 0x00,
	MHL_MSC_MSG_RCPE_INEFFECTIVE	= 0x01,
	MHL_MSC_MSG_RCPE_BUSY			= 0x02
};

enum {
	MHL_MSC_WRITEINT_DCAP_CHG			= 0x01,
};

enum {
	MHL_MSC_WRITESTATE_NO_PATHEN		= 0x00,
	MHL_MSC_WRITESTATE_DCAP_RDY			= 0x01,
	MHL_MSC_WRITESTATE_PATHEN			= 0x08,

};

enum {
	MHL_MSC_WRITEINT_RCHANGE_INT		= 0x20,
	MHL_MSC_WRITEINT_DCHANGE_INT		= 0x21,
	MHL_MSC_WRITESTATE_CONNECTED_RDY	= 0x30,
	MHL_MSC_WRITESTATE_LINK_MODE		= 0x31,
};

/*
 MHL spec related defines
*/
enum mhlcmd {
	/*Command or Data byte acknowledge*/
	MHL_ACK						= 0x33,
	/*Command or Data byte not acknowledge*/
	MHL_NACK					= 0x34,
	/*Transaction abort*/
	MHL_ABORT					= 0x35,
	/*Write one status register strip top bit*/
	MHL_WRITE_STAT				= 0x60,
	/*Write one interrupt register*/
	MHL_SET_INT					= 0x60,
	/*Read one register*/
	MHL_READ_DEVCAP				= 0x61,
	/*Read CBUS revision level from follower*/
	MHL_GET_STATE				= 0x62,
	/*Read vendor ID value from follower.*/
	MHL_GET_VENDOR_ID			= 0x63,
	/*Set Hot Plug Detect in follower*/
	MHL_SET_HPD					= 0x64,
	/*Clear Hot Plug Detect in follower*/
	MHL_CLR_HPD					= 0x65,
	/*Set Capture ID for downstream device.*/
	MHL_SET_CAP_ID				= 0x66,
	/*Get Capture ID from downstream device.*/
	MHL_GET_CAP_ID				= 0x67,
	/*VS command to send RCP sub-commands*/
	MHL_MSC_MSG					= 0x68,
	/*Get Vendor-Specific command error code.*/
	MHL_GET_SC1_ERRORCODE		= 0x69,
	/*Get DDC channel command error code.*/
	MHL_GET_DDC_ERRORCODE		= 0x6A,
	/*Get MSC command error code.*/
	MHL_GET_MSC_ERRORCODE		= 0x6B,
	/*Write 1-16 bytes to responder’s scratchpad.*/
	MHL_WRITE_BURST				= 0x6C,
	/*Get channel 3 command error code.*/
	MHL_GET_SC3_ERRORCODE		= 0x6D};

#define	MHL_RAP_CONTENT_ON		0x10	/*Turn content streaming ON.*/
#define	MHL_RAP_CONTENT_OFF		0x11	/*Turn content streaming OFF.*/

struct ps5101_regset {
	u8 page_no;
	u8 offset;
	u8 value;
};


struct ps5101_data {
	struct ps5101_platform_data	*pdata;
	struct input_dev		*input_dev;
	struct work_struct		startup_work;
	struct work_struct		waitmeasurezcbus_work;
	struct work_struct		msc_work;
	struct work_struct		writestatus_work;
	struct work_struct		softhpd_work;
	struct work_struct		rsen_timer;
	struct completion		zcbus_complete;
	struct completion		mscmsg_complete;
	struct completion		mscwstat_complete;
	struct completion		mscrcap_complete;
	struct completion		rsen_complete;
	struct completion		rddcap_complete;
	struct mutex			i2c_lock;
	struct mutex			list_lock;
	bool					cleared;
	bool					standby;
	u8						status_connected_rdy;
	u8						status_link_mode;
	atomic_t			is_irq_enabled;
	struct wake_lock		wake_lock;
	u8						chipver;
	u8						plim;
	int						vbus_owner;
	int						vbus_isplugd;
};

enum mhl_vbus_type {
	MHL_VBUS_TA_500mA = 0x00,
	MHL_VBUS_TA_900mA = 0x01,
	MHL_VBUS_TA_1500mA = 0x02,
	MHL_VBUS_USB = 0x03,
};
struct msc_packet {
	enum mhlcmd	command;
	u8	data_1;
	u8	data_2;
	struct list_head p_msc_packet_list;
};

#endif
