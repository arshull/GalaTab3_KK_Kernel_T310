#ifndef	PS5101_FUNC_H
#define PS5101_FUNC_H

#include "ps5101_Common_Def.h"
/*#include "../msm_fb.h"
#include "../hdmi_msm.h"
#include "../external_common.h"

extern struct external_common_state_type *external_common_state;
*/
typedef int (*INTFUNCTION)(struct ps5101_data *);


typedef struct {
	unsigned char ucPage;
	unsigned char ucOffset;
	unsigned char ucBit;
	unsigned char ucMaskOffset;
	unsigned char ucMaskBit;
	unsigned char ucIndex;
	unsigned char *ucInfo;
	INTFUNCTION   pfunc;
} t_INT;

extern int I2C_WriteByte(struct ps5101_data *ps5101,
		byte page_no, byte offset, byte value);
extern int I2C_ReadByte(struct ps5101_data *ps5101,
		byte page_no, byte offset, byte *value);

int initps5101(struct ps5101_data *ps5101);
int en_wakeup(struct ps5101_data *ps5101);

int stand_by(struct ps5101_data *ps5101);

int set_usbmode(struct ps5101_data *ps5101);

int set_mhlmode(struct ps5101_data *ps5101);

int start_discovery(struct ps5101_data *ps5101);

int handle_WRITE_STAT_INT(struct ps5101_data *ps5101);

int unmask_INT(struct ps5101_data *ps5101);

int mask_INT(struct ps5101_data *ps5101);

int request_3D(struct ps5101_data *ps5101);

int grant_write(struct ps5101_data *ps5101);

int request_write(struct ps5101_data *ps5101);

int scrachpad_change(struct ps5101_data *ps5101);

int capability_change(struct ps5101_data *ps5101);

int edid_change(struct ps5101_data *ps5101);

int handle_mscmsg(struct ps5101_data *ps5101);

int requestMSCMSGcmd(struct ps5101_data *ps5101,
	enum mhlcmd cmd, u8 data1, u8 data2);

int requestMSCREADCAPcmd(struct ps5101_data *ps5101,
	enum mhlcmd cmd, u8 data1, u8 data2);

int requestMSCWRITESTATcmd(struct ps5101_data *ps5101,
	enum mhlcmd cmd, u8 data1, u8 data2);

int finish_mscmsg(struct ps5101_data *ps5101);

int finish_readdcap_pass(struct ps5101_data *ps5101);

int finish_readdcap_fail(struct ps5101_data *ps5101);

int finish_writestat(struct ps5101_data *ps5101);


/*#ifdef PS5101B0*/
int clear_status(struct ps5101_data *ps5101);
int zbus_fail(struct ps5101_data *ps5101);
int clear_hpd(struct ps5101_data *ps5101);
/*#endif*/
#ifdef SOFTHPD
int softset_hpd(struct ps5101_data *ps5101);
#endif

int softset_hpd_des(struct ps5101_data *ps5101);

#ifdef PATCH4CTS
int hwhpd(struct ps5101_data *ps5101);
#endif

int discovery_fail(struct ps5101_data *ps5101);

int ps5101_enqueue_msc_work(struct ps5101_data *ps5101,
	u8 command, u8 data1, u8 data2);

extern void mhl_hpd_handler(bool state);

extern u8 mhl_onoff_ex(bool onoff);
#ifdef CONFIG_MHL_PS5101_VE
extern u8 sii9234_checkActive(void);
#endif
int read_dcap_action(struct ps5101_data *ps5101);

int set_cbus_timer(struct ps5101_data *ps5101, u8 value);

#endif
