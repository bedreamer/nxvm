/* This file is a part of NXVM project. */

#include "../vapi.h"
#include "../vmachine.h"
#include "../debug/aasm32.h"

#include "qdcga.h"
#include "qdkeyb.h"
#include "qddisk.h"
#include "qdmisc.h"

#include "qdrtc.h"
#include "post.h"

#include "qdbios.h"

t_faddrcc qdbiosInt[0x100];
static t_nubit16 ics, iip;

static t_nubit32 vbiosAsm(const t_strptr stmt, t_nubit16 seg, t_nubit16 off)
{
	t_nubit32 len;
	t_nubit8 code[0x10000];
	len = aasm32x(stmt, (t_vaddrcc)code);
	memcpy((void *)vramGetRealAddr(seg, off), (void *)code, len);
	return len;
}

void qdbiosMakeInt(t_nubit8 intid, t_strptr stmt)
{
	t_nubit32 len;
	vramRealWord(0x0000, intid * 4 + 0) = iip;
	vramRealWord(0x0000, intid * 4 + 2) = ics;
	len = vbiosAsm(stmt, ics, iip);
	if (len) iip += len;
	else vapiPrint("qdbios: invalid asm instruction.\n");
}
void qdbiosExecInt(t_nubit8 intid)
{
	if (qdbiosInt[intid]) ExecFun(qdbiosInt[intid]);
}

static void vbiosLoadData()
{
	memset((void *)vramGetRealAddr(0x0040,0x0000), 0x00, 0x100);
	vramRealByte(0x0040, 0x0000) = 0xf8;
	vramRealByte(0x0040, 0x0001) = 0x03;
	vramRealByte(0x0040, 0x0008) = 0x78;
	vramRealByte(0x0040, 0x0008) = 0x03;
	vramRealByte(0x0040, 0x000e) = 0xc0;
	vramRealByte(0x0040, 0x000f) = 0x9f;
	vramRealByte(0x0040, 0x0010) = 0x21;
	vramRealByte(0x0040, 0x0013) = 0x7f;
	vramRealByte(0x0040, 0x0014) = 0x02;
	vramRealByte(0x0040, 0x0017) = 0x20;
	vramRealByte(0x0040, 0x001a) = 0x1e;
	vramRealByte(0x0040, 0x001b) = 0x04;
	vramRealByte(0x0040, 0x001c) = 0x1e;
	vramRealByte(0x0040, 0x001d) = 0x04;
	vramRealByte(0x0040, 0x001e) = 0x1c;
	vramRealByte(0x0040, 0x001f) = 0x0d;
	vramRealByte(0x0040, 0x0020) = 0x1c;
	vramRealByte(0x0040, 0x0021) = 0x0d;
	vramRealByte(0x0040, 0x0022) = 0x22;
	vramRealByte(0x0040, 0x0023) = 0x67;
	vramRealByte(0x0040, 0x003e) = 0x01;
	vramRealByte(0x0040, 0x0040) = 0x25;
	vramRealByte(0x0040, 0x0041) = 0x09;
	vramRealByte(0x0040, 0x0045) = 0x01;
	vramRealByte(0x0040, 0x0047) = 0x01;
	vramRealByte(0x0040, 0x0048) = 0x02;
	vramRealByte(0x0040, 0x0049) = 0x03;
	vramRealByte(0x0040, 0x004a) = 0x50;
	vramRealByte(0x0040, 0x004d) = 0x10;
	vramRealByte(0x0040, 0x0051) = 0x05;
	vramRealByte(0x0040, 0x0060) = 0x0e;
	vramRealByte(0x0040, 0x0061) = 0x0d;
	vramRealByte(0x0040, 0x0063) = 0xd4;
	vramRealByte(0x0040, 0x0064) = 0x03;
	vramRealByte(0x0040, 0x0065) = 0x29;
	vramRealByte(0x0040, 0x0066) = 0x30;
	vramRealByte(0x0040, 0x0074) = 0x01;
	vramRealByte(0x0040, 0x0075) = vhdd.flagexist ? 0x01 : 0x00; /* number of hard disks */
	vramRealByte(0x0040, 0x0076) = 0xc0;
	vramRealByte(0x0040, 0x0078) = 0x14;
	vramRealByte(0x0040, 0x007c) = 0x0a;
	vramRealByte(0x0040, 0x0080) = 0x1e;
	vramRealByte(0x0040, 0x0082) = 0x3e;
	vramRealByte(0x0040, 0x0084) = 0x18;
	vramRealByte(0x0040, 0x0085) = 0x10;
	vramRealByte(0x0040, 0x0087) = 0x60;
	vramRealByte(0x0040, 0x0088) = 0x09;
	vramRealByte(0x0040, 0x0089) = 0x11;
	vramRealByte(0x0040, 0x008a) = 0x0b;
	vramRealByte(0x0040, 0x008f) = 0x77;
	vramRealByte(0x0040, 0x0090) = 0x17;
	vramRealByte(0x0040, 0x0096) = 0x10;
	vramRealByte(0x0040, 0x0097) = 0x02;
	vramRealByte(0x0040, 0x00a8) = 0x3a;
	vramRealByte(0x0040, 0x00a9) = 0x5d;
	vramRealByte(0x0040, 0x00ab) = 0xc0;
	vramRealByte(0x0040, 0x0100) = vmachine.flagboot ? 0x80 : 0x00; /* boot disk */
}
static void vbiosLoadRomInfo()
{
	vramRealWord(0xf000, 0xe6f5) = 0x0008;
	vramRealByte(0xf000, 0xe6f7) = 0xfc;
	vramRealByte(0xf000, 0xe6f8) = 0x00;
	vramRealByte(0xf000, 0xe6f9) = 0x01;
	vramRealByte(0xf000, 0xe6fa) = 0xb4;
	vramRealByte(0xf000, 0xe6fb) = 0x40;
	vramRealByte(0xf000, 0xe6fc) = 0x00;
	vramRealByte(0xf000, 0xe6fd) = 0x00;
	vramRealByte(0xf000, 0xe6fe) = 0x00;
}
static void vbiosLoadInt()
{
	t_nubit16 i;
	for (i = 0x0000;i < 0x0100;++i) {
		qdbiosInt[i] = (t_faddrcc)NULL;
		vramRealWord(0x0000, i * 4 + 0) = iip;
		vramRealWord(0x0000, i * 4 + 2) = ics;
	}
	iip += (t_nubit16)vbiosAsm("iret", ics, iip);
	/* load rtc int */
	qdbiosMakeInt(0x08, HARD_RTC_INT_08);
	qdbiosMakeInt(0x1a, SOFT_RTC_INT_1A);
	/* load bios service int */
	qdbiosMakeInt(0x11, SOFT_MISC_INT_11);
	qdbiosMakeInt(0x12, SOFT_MISC_INT_12);
	qdbiosMakeInt(0x15, SOFT_MISC_INT_15);
	/* load disk int */
	qdbiosMakeInt(0x0e, HARD_FDD_INT_0E);
	qdbiosMakeInt(0x13, SOFT_DISK_INT_13);
	qdbiosMakeInt(0x40, SOFT_DISK_INT_40);
	/* qdkeyb init */
	qdkeybReset();
	/* qdcga init */
	qdcgaReset();
	/* qddisk init: remaining int for hdd */
	qddiskReset();
}
static void vbiosLoadPost()
{
	t_string stmt;
	SPRINTF(stmt, "jmp %04x:%04x", ics, iip, 1);
	vbiosAsm(stmt, 0xf000, 0xfff0);
	iip += (t_nubit16)vbiosAsm(VBIOS_POST_VPIC,  ics, iip);
	iip += (t_nubit16)vbiosAsm(VBIOS_POST_VDMA,  ics, iip);
	iip += (t_nubit16)vbiosAsm(VBIOS_POST_VFDC,  ics, iip);
	iip += (t_nubit16)vbiosAsm(VBIOS_POST_VPIT,  ics, iip);
	iip += (t_nubit16)vbiosAsm(VBIOS_POST_VCMOS, ics, iip);
	iip += (t_nubit16)vbiosAsm(VBIOS_POST_VRTC,  ics, iip);
	iip += (t_nubit16)vbiosAsm(VBIOS_POST_BOOT,  ics, iip);
	/* hard disk param table */
	vramRealWord(0x0000, 0x0104) = 0xe431;
	vramRealWord(0x0000, 0x0106) = 0xf000;
	vramRealWord(0xf000, 0xe431 +  0) = vhdd.ncyl;
	vramRealByte(0xf000, 0xe431 +  2) = (t_nubit8)vhdd.nhead;
	vramRealByte(0xf000, 0xe431 +  3) = 0xa0;
	vramRealByte(0xf000, 0xe431 +  4) = (t_nubit8)vhdd.nsector;
	vramRealWord(0xf000, 0xe431 +  5) = 0xffff;
	vramRealByte(0xf000, 0xe431 +  7) = 0x00;
	vramRealByte(0xf000, 0xe431 +  8) = 0x08;
	vramRealWord(0xf000, 0xe431 +  9) = vhdd.ncyl;
	vramRealByte(0xf000, 0xe431 + 11) = (t_nubit8)vhdd.nhead;
	vramRealWord(0xf000, 0xe431 + 12) = 0x0000;
	vramRealByte(0xf000, 0xe431 + 14) = (t_nubit8)vhdd.nsector;
	vramRealByte(0xf000, 0xe431 + 15) = 0x00;
}

void qdbiosReset()
{
	ics = 0xf000;
	iip = 0x0000;
/* bios data area */
	vbiosLoadData();
/* bios rom info area */
	vbiosLoadRomInfo();
/* bios interrupt services */
	vbiosLoadInt();
/* bios init/post program */
	vbiosLoadPost();
}
