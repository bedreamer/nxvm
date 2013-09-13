/* This file is a part of NXVM project. */

#include "../vapi.h"
#include "../vmachine.h"

#include "qdbios.h"
#include "qdkeyb.h"

#define bufptrHead (vramRealWord(0x0000,QDKEYB_VBIOS_ADDR_KEYB_BUF_HEAD))
#define bufptrTail (vramRealWord(0x0000,QDKEYB_VBIOS_ADDR_KEYB_BUF_TAIL))
#define bufGetSize (QDKEYB_VBIOS_ADDR_KEYB_BUFFER_END - \
                    QDKEYB_VBIOS_ADDR_KEYB_BUFFER_START + 1)
#define bufIsEmpty (bufptrHead == bufptrTail)
#define bufIsFull  ((bufptrHead - QDKEYB_VBIOS_ADDR_KEYB_BUFFER_START) == \
	(bufptrTail - QDKEYB_VBIOS_ADDR_KEYB_BUFFER_START + 2) % bufGetSize)
#define bufptrAdvance(ptr) ((ptr) = (QDKEYB_VBIOS_ADDR_KEYB_BUFFER_START + \
		((ptr) - QDKEYB_VBIOS_ADDR_KEYB_BUFFER_START + 2) % bufGetSize))

static t_bool bufPush(t_nubit16 code)
{
	if (bufIsFull) return 1;
	vramRealWord(0x0000, bufptrTail) = code;
	bufptrAdvance(bufptrTail);
	return 0;
}
static t_nubit16 bufPop()
{
	t_nubit16 res = 0;
	if (bufIsEmpty) return res;
	res = vramRealWord(0x0000, bufptrHead);
	bufptrAdvance(bufptrHead);
	return res;
}
static t_nubit16 bufPeek()
{
	return vramRealWord(0x0000, bufptrHead);
}

t_bool vapiCallBackKeyboardGetFlag0CapsLock() {return GetBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_CAPLCK);}
t_bool vapiCallBackKeyboardGetFlag0NumLock()  {return GetBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_NUMLCK);}
t_bool vapiCallBackKeyboardGetFlag0Shift()    {return GetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_LSHIFT) || GetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_RSHIFT);}
t_bool vapiCallBackKeyboardGetFlag0Alt()  {return GetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_ALT);}
t_bool vapiCallBackKeyboardGetFlag0Ctrl() {return GetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_CTRL);}
void vapiCallBackKeyboardClrFlag0() {qdkeybVarFlag0 = 0x00;}
void vapiCallBackKeyboardClrFlag1() {qdkeybVarFlag1 = 0x00;}

void vapiCallBackKeyboardSetFlag0Insert()     {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_INSERT);}
void vapiCallBackKeyboardSetFlag0CapLck()     {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_CAPLCK);}
void vapiCallBackKeyboardSetFlag0NumLck()     {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_NUMLCK);}
void vapiCallBackKeyboardSetFlag0ScrLck()     {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_SCRLCK);}
void vapiCallBackKeyboardSetFlag0Alt()        {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_ALT);}
void vapiCallBackKeyboardSetFlag0Ctrl()       {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_CTRL);}
void vapiCallBackKeyboardSetFlag0LeftShift()  {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_LSHIFT);}
void vapiCallBackKeyboardSetFlag0RightShift() {SetBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_RSHIFT);}

void vapiCallBackKeyboardClrFlag0Insert()     {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_INSERT);}
void vapiCallBackKeyboardClrFlag0CapLck()     {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_CAPLCK);}
void vapiCallBackKeyboardClrFlag0NumLck()     {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_NUMLCK);}
void vapiCallBackKeyboardClrFlag0ScrLck()     {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_A_SCRLCK);}
void vapiCallBackKeyboardClrFlag0Alt()        {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_ALT);}
void vapiCallBackKeyboardClrFlag0Ctrl()       {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_CTRL);}
void vapiCallBackKeyboardClrFlag0LeftShift()  {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_LSHIFT);}
void vapiCallBackKeyboardClrFlag0RightShift() {ClrBit(qdkeybVarFlag0, QDKEYB_FLAG0_D_RSHIFT);}

void vapiCallBackKeyboardSetFlag1Insert()   {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_INSERT);}
void vapiCallBackKeyboardSetFlag1CapLck()   {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_CAPLCK);}
void vapiCallBackKeyboardSetFlag1NumLck()   {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_NUMLCK);}
void vapiCallBackKeyboardSetFlag1ScrLck()   {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_SCRLCK);}
void vapiCallBackKeyboardSetFlag1Pause()    {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_PAUSE);}
void vapiCallBackKeyboardSetFlag1SysRq()    {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_SYSRQ);}
void vapiCallBackKeyboardSetFlag1LeftAlt()  {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_LALT);}
void vapiCallBackKeyboardSetFlag1LeftCtrl() {SetBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_LCTRL);}

void vapiCallBackKeyboardClrFlag1Insert()   {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_INSERT);}
void vapiCallBackKeyboardClrFlag1CapLck()   {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_CAPLCK);}
void vapiCallBackKeyboardClrFlag1NumLck()   {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_NUMLCK);}
void vapiCallBackKeyboardClrFlag1ScrLck()   {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_SCRLCK);}
void vapiCallBackKeyboardClrFlag1Pause()    {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_PAUSE);}
void vapiCallBackKeyboardClrFlag1SysRq()    {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_SYSRQ);}
void vapiCallBackKeyboardClrFlag1LeftAlt()  {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_LALT);}
void vapiCallBackKeyboardClrFlag1LeftCtrl() {ClrBit(qdkeybVarFlag1, QDKEYB_FLAG1_D_LCTRL);}

void vapiCallBackKeyboardRecvKeyPress(t_nubit16 code)
{
//	while(bufPush(code)) vapiSleep(1);
	bufPush(code);
	vpicSetIRQ(0x01);
}

void qdkeybReadInput()
{
	/* TODO: this should have been working with INT 15 */
	while (bufIsEmpty) vapiSleep(10);
	_ax = bufPop();
	vpicSetIRQ(0x01);
}
void qdkeybGetStatus()
{
	t_nubit16 x = bufPeek();
	if (bufIsEmpty) {
		_SetEFLAGS_ZF;
	} else {
		switch (x) {
		case 0x1d00:
		case 0x2a00:
		case 0x3800:
			_ax = 0x0000;
			break;
		default:
			_ax = x;
			break;
		}
		_ClrEFLAGS_ZF;
	}
}
void qdkeybGetShiftStatus()
{
	_al = qdkeybVarFlag0;
}
void qdkeybBufferKey()
{
	_al = bufPush((_ch << 8) | _cl);
}

void INT_09()
{
	vport.iobyte = 0x20;
	ExecFun(vport.out[0x20]);
}
void INT_16()
{
	switch (_ah) {
	case 0x00:
	case 0x10:
		qdkeybReadInput();
		break;
	case 0x01:
	case 0x11:
		qdkeybGetStatus();
		break;
	case 0x02:
		qdkeybGetShiftStatus();
		break;
	case 0x05:
		qdkeybBufferKey();
		break;
	default:
		break;
	}
}

void qdkeybReset()
{
	qdbiosInt[0x09] = (t_faddrcc)INT_09; /* hard keyb */
	qdbiosInt[0x16] = (t_faddrcc)INT_16; /* soft keyb */
/* special: INT 09 */
	qdbiosMakeInt(0x09, "qdx 09\niret");
/* special: INT 16 */
	qdbiosMakeInt(0x16, "qdx 16\niret");
}
