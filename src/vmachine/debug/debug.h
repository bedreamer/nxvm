/* This file is a part of NXVM project. */

#ifndef NXVM_DEBUG_H
#define NXVM_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	t_bool    flagbreak;                    /* breakpoint set (1) or not (0) */
	t_bool    flagbreakx;
	t_nubitcc breakcnt;
	t_nubit16 breakcs, breakip;
	t_nubit32 breaklinear;
	t_nubitcc tracecnt;
} t_debug;

extern t_debug vdebug;

void debugInit();
void debugRefresh();
void debugFinal();

void debug();

#ifdef __cplusplus
}/*_EOCD_*/
#endif

#endif
