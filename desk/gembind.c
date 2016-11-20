/*      GEMBIND.C       9/11/85                         Lee Lorenzen    */
/*      for 3.0         3/11/86 - 8/26/86               MDF             */
/*      merge source    5/28/87                         mdf             */
/*      menu_click      9/25/87                         mdf             */

/*
*       Copyright 1999, Caldera Thin Clients, Inc.
*                 2002-2016 The EmuTOS development team
*
*       This software is licenced under the GNU Public License.
*       Please see LICENSE.TXT for further information.
*
*                  Historical Copyright
*       -------------------------------------------------------------
*       GEM Desktop                                       Version 2.3
*       Serial No.  XXXX-0000-654321              All Rights Reserved
*       Copyright (C) 1985 - 1987               Digital Research Inc.
*       -------------------------------------------------------------
*/

/* #define ENABLE_KDEBUG */

#include "config.h"
#include "portab.h"
#include "obdefs.h"
#include "gembind.h"
#include "kprint.h"
#include <stdarg.h>

#define CTRL_CNT        3


static const BYTE ctrl_cnts[] =
{
/* Application Manager          */
        0, 1, 0,                        /* func 010             */
        2, 1, 1,                        /* func 011             */
        2, 1, 1,                        /* func 012             */
        0, 1, 1,                        /* func 013             */
        2, 1, 1,                        /* func 014             */
        1, 1, 1,                        /* func 015             */
        2, 1, 0,                        /* func 016             */
        0, 0, 0,                        /* func 017             */
        0, 0, 0,                        /* func 018             */
        0, 1, 0,                        /* func 019             */
/* Event Manager                */
        0, 1, 0,                        /* func 020             */
        3, 5, 0,                        /* func 021             */
        5, 5, 0,                        /* func 022             */
        0, 1, 1,                        /* func 023             */
        2, 1, 0,                        /* func 024             */
        16, 7, 1,                       /* func 025             */
        2, 1, 0,                        /* func 026             */
        0, 0, 0,                        /* func 027             */
        0, 0, 0,                        /* func 028             */
        0, 0, 0,                        /* func 029             */
/* Menu Manager                 */
        1, 1, 1,                        /* func 030             */
        2, 1, 1,                        /* func 031             */
        2, 1, 1,                        /* func 032             */
        2, 1, 1,                        /* func 033             */
        1, 1, 2,                        /* func 034             */
        1, 1, 1,                        /* func 035             */
        1, 1, 0,                        /* func 036             */
        2, 1, 0,                        /* func 037             */
        0, 0, 0,                        /* func 038             */
        0, 0, 0,                        /* func 039             */
/* Object Manager               */
        2, 1, 1,                        /* func 040             */
        1, 1, 1,                        /* func 041             */
        6, 1, 1,                        /* func 042             */
        4, 1, 1,                        /* func 043             */
        1, 3, 1,                        /* func 044             */
        2, 1, 1,                        /* func 045             */
        4, 2, 1,                        /* func 046             */
        8, 1, 1,                        /* func 047             */
        0, 0, 0,                        /* func 048             */
        0, 0, 0,                        /* func 049             */
/* Form Manager                 */
        1, 1, 1,                        /* func 050             */
        9, 1, 1,                        /* func 051             */
        1, 1, 1,                        /* func 052             */
        1, 1, 0,                        /* func 053             */
        0, 5, 1,                        /* func 054             */
        3, 3, 1,                        /* func 055             */
        2, 2, 1,                        /* func 056             */
        0, 0, 0,                        /* func 057             */
        0, 0, 0,                        /* func 058             */
        0, 0, 0,                        /* func 059             */
/* Process Manager              */
        2, 2, 2,                        /* func 060             */
        3, 1, 2,                        /* func 061             */
        1, 1, 0,                        /* func 062             */
        1, 3, 0,                        /* func 063             */
        0, 0, 1,                        /* func 064             */
        0, 0, 0,                        /* func 065             */
        1, 1, 0,                        /* func 066             */
        1, 1, 0,                        /* func 067             */
        0, 0, 0,                        /* func 068             */
        0, 0, 0,                        /* func 069             */
/* Graphics Manager             */
        4, 3, 0,                        /* func 070             */
        8, 3, 0,                        /* func 071             */
        6, 1, 0,                        /* func 072             */
        8, 1, 0,                        /* func 073             */
        8, 1, 0,                        /* func 074             */
        4, 1, 1,                        /* func 075             */
        3, 1, 1,                        /* func 076             */
        0, 5, 0,                        /* func 077             */
        1, 1, 1,                        /* func 078             */
        0, 5, 0,                        /* func 079             */
/* Scrap Manager                */
        0, 1, 1,                        /* func 080             */
        0, 1, 1,                        /* func 081             */
        0, 0, 0,                        /* func 082             */
        0, 0, 0,                        /* func 083             */
        0, 0, 0,                        /* func 084             */
        0, 0, 0,                        /* func 085             */
        0, 0, 0,                        /* func 086             */
        0, 0, 0,                        /* func 087             */
        0, 0, 0,                        /* func 088             */
        0, 0, 0,                        /* func 089             */
/* File Selector Manager        */
        0, 2, 2,                        /* func 090             */
        0, 0, 0,                        /* func 091             */
        0, 0, 0,                        /* func 092             */
        0, 0, 0,                        /* func 093             */
        0, 0, 0,                        /* func 094             */
        0, 0, 0,                        /* func 095             */
        0, 0, 0,                        /* func 096             */
        0, 0, 0,                        /* func 097             */
        0, 0, 0,                        /* func 098             */
        0, 0, 0,                        /* func 099             */
/* Window Manager               */
        5, 1, 0,                        /* func 100             */
        5, 1, 0,                        /* func 101             */
        1, 1, 0,                        /* func 102             */
        1, 1, 0,                        /* func 103             */
        2, 5, 0,                        /* func 104             */
        6, 1, 0,                        /* func 105             */
        2, 1, 0,                        /* func 106             */
        1, 1, 0,                        /* func 107             */
        6, 5, 0,                        /* func 108             */
        0, 0, 0,                        /* func 109             */
/* Resource Manager             */
        0, 1, 1,                        /* func 110             */
        0, 1, 0,                        /* func 111             */
        2, 1, 0,                        /* func 112             */
        2, 1, 1,                        /* func 113             */
        1, 1, 1,                        /* func 114             */
        0, 0, 0,                        /* func 115             */
        0, 0, 0,                        /* func 116             */
        0, 0, 0,                        /* func 117             */
        0, 0, 0,                        /* func 118             */
        0, 0, 0,                        /* func 119             */
/* Shell Manager                */
        0, 1, 2,                        /* func 120             */
        3, 1, 2,                        /* func 121             */
        1, 1, 1,                        /* func 122             */
        1, 1, 1,                        /* func 123             */
        0, 1, 1,                        /* func 124             */
        0, 1, 2,                        /* func 125             */
        0, 1, 2,                        /* func 126             */
        0, 1, 2                         /* func 127             */
};


typedef struct gemblkstr
{
        UWORD           *gb_pcontrol;
        UWORD           *gb_pglobal;
        UWORD           *gb_pintin;
        UWORD           *gb_pintout;
        LONG            *gb_padrin;
        LONG            *gb_padrout;
} GEMBLK;


extern WORD             gem(GEMBLK *gb);    /* in deskstart.S */


static GEMBLK           gb;

static UWORD            control[C_SIZE];
GLOBAL UWORD            global[G_SIZE];
static UWORD            int_in[I_SIZE];
static UWORD            int_out[O_SIZE];
static LONG             addr_in[AI_SIZE];
static LONG             addr_out[AO_SIZE];





static WORD gem_if(WORD opcode)
{
    WORD i;
    const BYTE *pctrl;

    control[0] = opcode;

    pctrl = &ctrl_cnts[(opcode - 10) * 3];
    for (i = 1; i <= CTRL_CNT; i++)
        control[i] = *pctrl++;

    gem(&gb);

    return (WORD)RET_CODE;
}


/*
 *  Application Manager
 */
WORD appl_init(void)
{
    gb.gb_pcontrol = control;
    gb.gb_pglobal = global;
    gb.gb_pintin = int_in;
    gb.gb_pintout = int_out;
    gb.gb_padrin = addr_in;
    gb.gb_padrout = addr_out;
    return gem_if(APPL_INIT);
}


WORD appl_exit(void)
{
    gem_if(APPL_EXIT);
    return TRUE;
}


/* unused
WORD appl_write(WORD rwid, WORD length, const void *pbuff)
{
    AP_RWID = rwid;
    AP_LENGTH = length;
    AP_PBUFF = (LONG)pbuff;
    return gem_if(APPL_WRITE);
}
*/

/* unused
WORD appl_read(WORD rwid, WORD length, void *pbuff)
{
    AP_RWID = rwid;
    AP_LENGTH = length;
    AP_PBUFF = (LONG)pbuff;
    return gem_if(APPL_READ);
}
*/

/* unused
WORD appl_find(const BYTE *pname)
{
    AP_PNAME = (LONG)pname;
    return gem_if(APPL_FIND);
}
*/

/* unused
WORD appl_tplay(const EVNTREC *tbuffer, WORD tlength, WORD tscale)
{
    AP_TBUFFER = (LONG)tbuffer;
    AP_TLENGTH = tlength;
    AP_TSCALE = tscale;
    return gem_if(APPL_TPLAY);
}
*/

/* unused
WORD appl_trecord(EVNTREC *tbuffer, WORD tlength)
{
    AP_TBUFFER = (LONG)tbuffer;
    AP_TLENGTH = tlength;
    return gem_if(APPL_TRECORD);
}
*/


/*
 *  Event Manager
 */
/* unused
UWORD evnt_keybd(void)
{
    return (UWORD)gem_if(EVNT_KEYBD);
}
*/


WORD evnt_button(WORD clicks, UWORD mask, UWORD state,
                 WORD *pmx, WORD *pmy, WORD *pmb, WORD *pks)
{
    B_CLICKS = clicks;
    B_MASK = mask;
    B_STATE = state;
    gem_if(EVNT_BUTTON);
    *pmx = EV_MX;
    *pmy = EV_MY;
    *pmb = EV_MB;
    *pks = EV_KS;
    return (WORD)RET_CODE;
}


/* unused
WORD evnt_mouse(WORD flags, WORD x, WORD y, WORD width, WORD height,
                WORD *pmx,WORD *pmy,WORD *pmb,WORD *pks)
{
    MO_FLAGS = flags;
    MO_X = x;
    MO_Y = y;
    MO_WIDTH = width;
    MO_HEIGHT = height;
    gem_if(EVNT_MOUSE);
    *pmx = EV_MX;
    *pmy = EV_MY;
    *pmb = EV_MB;
    *pks = EV_KS;
    return (WORD)RET_CODE;
}
*/


/* unused
WORD evnt_mesag(WORD *pbuff)
{
    ME_PBUFF = (LONG)pbuff;
    return gem_if(EVNT_MESAG);
}
*/


WORD evnt_timer(UWORD locnt, UWORD hicnt)
{
    T_LOCOUNT = locnt;
    T_HICOUNT = hicnt;
    return gem_if(EVNT_TIMER);
}


WORD evnt_multi(UWORD flags, UWORD bclk, UWORD bmsk, UWORD bst,
                UWORD m1flags, UWORD m1x, UWORD m1y, UWORD m1w, UWORD m1h,
                UWORD m2flags, UWORD m2x, UWORD m2y, UWORD m2w, UWORD m2h,
                WORD *mepbuff, UWORD tlc, UWORD thc, UWORD *pmx, UWORD *pmy,
                UWORD *pmb, UWORD *pks, UWORD *pkr, UWORD *pbr )
{
    MU_FLAGS = flags;

    MB_CLICKS = bclk;
    MB_MASK = bmsk;
    MB_STATE = bst;

    MMO1_FLAGS = m1flags;
    MMO1_X = m1x;
    MMO1_Y = m1y;
    MMO1_WIDTH = m1w;
    MMO1_HEIGHT = m1h;

    MMO2_FLAGS = m2flags;
    MMO2_X = m2x;
    MMO2_Y = m2y;
    MMO2_WIDTH = m2w;
    MMO2_HEIGHT = m2h;

    MME_PBUFF = (LONG)mepbuff;

    MT_LOCOUNT = tlc;
    MT_HICOUNT = thc;

    gem_if(EVNT_MULTI);

    *pmx = EV_MX;
    *pmy = EV_MY;
    *pmb = EV_MB;
    *pks = EV_KS;
    *pkr = EV_KRET;
    *pbr = EV_BRET;
    return (WORD)RET_CODE;
}


WORD evnt_dclick(WORD rate, WORD setit)
{
    EV_DCRATE = rate;
    EV_DCSETIT = setit;
    return gem_if(EVNT_DCLICK);
}


/*
 *  Menu Manager
 */
WORD menu_bar(OBJECT *tree, WORD showit)
{
    MM_ITREE = (LONG)tree;
    SHOW_IT = showit;
    return gem_if(MENU_BAR);
}


WORD menu_icheck(OBJECT *tree, WORD itemnum, WORD checkit)
{
    MM_ITREE = (LONG)tree;
    ITEM_NUM = itemnum;
    CHECK_IT = checkit;
    return gem_if(MENU_ICHECK);
}


WORD menu_ienable(OBJECT *tree, WORD itemnum, WORD enableit)
{
    MM_ITREE = (LONG)tree;
    ITEM_NUM = itemnum;
    ENABLE_IT = enableit;
    return gem_if(MENU_IENABLE);
}


WORD menu_tnormal(OBJECT *tree, WORD titlenum, WORD normalit)
{
    MM_ITREE = (LONG)tree;
    TITLE_NUM = titlenum;
    NORMAL_IT = normalit;
    return gem_if(MENU_TNORMAL);
}


/* unused
WORD menu_text(OBJECT *tree, WORD inum, const BYTE *ptext)
{
    MM_ITREE = (LONG)tree;
    ITEM_NUM = inum;
    MM_PTEXT = (LONG) ptext;
    return gem_if(MENU_TEXT);
}
*/


/* unused
WORD menu_register(WORD pid, const BYTE *pstr)
{
    MM_PID = pid;
    MM_PSTR = (LONG)pstr;
    return gem_if(MENU_REGISTER);
}
*/


/* unused
WORD menu_unregister(WORD mid)
{
    MM_MID = mid;
    return gem_if(MENU_UNREGISTER);
}
*/


WORD menu_click(WORD click, WORD setit)
{
    MN_CLICK = click;
    MN_SETIT = setit;
    return gem_if(MENU_CLICK);
}


/*
 *  Object Manager
 */
WORD objc_add(OBJECT *tree, WORD parent, WORD child)
{
    OB_TREE = (LONG)tree;
    OB_PARENT = parent;
    OB_CHILD = child;
    return gem_if(OBJC_ADD);
}


/* unused
WORD objc_delete(OBJECT *tree, WORD delob)
{
    OB_TREE = (LONG)tree;
    OB_DELOB = delob;
    return gem_if(OBJC_DELETE);
}
*/


WORD objc_draw(OBJECT *tree, WORD drawob, WORD depth, WORD xc, WORD yc,
               WORD wc, WORD hc)
{
    OB_TREE = (LONG)tree;
    OB_DRAWOB = drawob;
    OB_DEPTH = depth;
    OB_XCLIP = xc;
    OB_YCLIP = yc;
    OB_WCLIP = wc;
    OB_HCLIP = hc;
    return gem_if(OBJC_DRAW);
}


WORD objc_find(OBJECT *tree, WORD startob, WORD depth, WORD mx, WORD my)
{
    OB_TREE = (LONG)tree;
    OB_STARTOB = startob;
    OB_DEPTH = depth;
    OB_MX = mx;
    OB_MY = my;
    return gem_if(OBJC_FIND);
}


WORD objc_order(OBJECT *tree, WORD mov_obj, WORD newpos)
{
    OB_TREE = (LONG)tree;
    OB_OBJ = mov_obj;
    OB_NEWPOS = newpos;
    return gem_if(OBJC_ORDER);
}


WORD objc_offset(OBJECT *tree, WORD obj, WORD *poffx, WORD *poffy)
{
    OB_TREE = (LONG)tree;
    OB_OBJ = obj;
    gem_if(OBJC_OFFSET);
    *poffx = OB_XOFF;
    *poffy = OB_YOFF;
    return (WORD)RET_CODE;
}


/* unused
WORD objc_edit(OBJECT *tree, WORD obj, WORD inchar, WORD *idx, WORD kind)
{
    OB_TREE = (LONG)tree;
    OB_OBJ = obj;
    OB_CHAR = inchar;
    OB_IDX = *idx;
    OB_KIND = kind;
    gem_if(OBJC_EDIT);
    *idx = OB_ODX;
    return (WORD)RET_CODE;
}
*/


WORD objc_change(OBJECT *tree, WORD drawob, WORD depth, WORD xc, WORD yc,
                 WORD wc, WORD hc, WORD newstate, WORD redraw)
{
    OB_TREE = (LONG)tree;
    OB_DRAWOB = drawob;
    OB_DEPTH = depth;
    OB_XCLIP = xc;
    OB_YCLIP = yc;
    OB_WCLIP = wc;
    OB_HCLIP = hc;
    OB_NEWSTATE = newstate;
    OB_REDRAW = redraw;
    return gem_if(OBJC_CHANGE);
}


/*
 *  Form Manager
 */
WORD form_do(OBJECT *form, WORD start)
{
    FM_FORM = (LONG)form;
    FM_START = start;
    return gem_if(FORM_DO);
}


WORD form_dial(WORD dtype, WORD ix, WORD iy, WORD iw, WORD ih,
               WORD x, WORD y, WORD w, WORD h)
{
    FM_TYPE = dtype;
    FM_IX = ix;
    FM_IY = iy;
    FM_IW = iw;
    FM_IH = ih;
    FM_X = x;
    FM_Y = y;
    FM_W = w;
    FM_H = h;
    return gem_if(FORM_DIAL);
}


WORD form_alert(WORD defbut, const BYTE *astring)
{
    FM_DEFBUT = defbut;
    FM_ASTRING = (LONG)astring;
    return gem_if(FORM_ALERT);
}


WORD form_error(WORD errnum)
{
    FM_ERRNUM = errnum;
    return gem_if(FORM_ERROR);
}


WORD form_center(OBJECT *tree, WORD *pcx, WORD *pcy, WORD *pcw, WORD *pch)
{
    FM_FORM = (LONG)tree;
    gem_if(FORM_CENTER);
    *pcx = FM_XC;
    *pcy = FM_YC;
    *pcw = FM_WC;
    *pch = FM_HC;
    return (WORD)RET_CODE;
}


/* unused
WORD form_keybd(OBJECT *form, WORD obj, WORD nxt_obj, WORD thechar,
                WORD *pnxt_obj, WORD *pchar)
{
    FM_FORM = (LONG)form;
    FM_OBJ = obj;
    FM_INXTOB = nxt_obj;
    FM_ICHAR = thechar;
    gem_if(FORM_KEYBD);
    *pnxt_obj = FM_ONXTOB;
    *pchar = FM_OCHAR;
    return (WORD)RET_CODE;
}
*/


/* unused
WORD form_button(OBJECT *form, WORD obj, WORD clks, WORD *pnxt_obj)
{
    FM_FORM = (LONG)form;
    FM_OBJ = obj;
    FM_CLKS = clks;
    gem_if(FORM_BUTTON);
    *pnxt_obj = FM_ONXTOB;
    return (WORD)RET_CODE;
}
*/


/*
 *  Graphics Manager
 */
WORD graf_rubbox(WORD xorigin, WORD yorigin, WORD wmin, WORD hmin,
                 WORD *pwend, WORD *phend)
{
    GR_I1 = xorigin;
    GR_I2 = yorigin;
    GR_I3 = wmin;
    GR_I4 = hmin;
    gem_if(GRAF_RUBBOX);
    *pwend = GR_O1;
    *phend = GR_O2;
    return (WORD)RET_CODE;
}


/* unused
WORD graf_dragbox(WORD w, WORD h, WORD sx, WORD sy, WORD xc, WORD yc,
                  WORD wc, WORD hc, WORD *pdx, WORD *pdy)
{
    GR_I1 = w;
    GR_I2 = h;
    GR_I3 = sx;
    GR_I4 = sy;
    GR_I5 = xc;
    GR_I6 = yc;
    GR_I7 = wc;
    GR_I8 = hc;
    gem_if(GRAF_DRAGBOX);
    *pdx = GR_O1;
    *pdy = GR_O2;
    return (WORD)RET_CODE;
}
*/


/* unused
WORD graf_mbox(WORD w, WORD h, WORD srcx, WORD srcy, WORD dstx, WORD dsty)
{
    GR_I1 = w;
    GR_I2 = h;
    GR_I3 = srcx;
    GR_I4 = srcy;
    GR_I5 = dstx;
    GR_I6 = dsty;
    return gem_if(GRAF_MBOX);
}
*/


WORD graf_growbox(WORD orgx, WORD orgy, WORD orgw, WORD orgh,
                  WORD x, WORD y, WORD w, WORD h)
{
    GR_I1 = orgx;
    GR_I2 = orgy;
    GR_I3 = orgw;
    GR_I4 = orgh;
    GR_I5 = x;
    GR_I6 = y;
    GR_I7 = w;
    GR_I8 = h;
    return gem_if(GRAF_GROWBOX);
}


WORD graf_shrinkbox(WORD orgx, WORD orgy, WORD orgw, WORD orgh,
                    WORD x, WORD y, WORD w, WORD h)
{
    GR_I1 = orgx;
    GR_I2 = orgy;
    GR_I3 = orgw;
    GR_I4 = orgh;
    GR_I5 = x;
    GR_I6 = y;
    GR_I7 = w;
    GR_I8 = h;
    return gem_if(GRAF_SHRINKBOX);
}


/* unused
WORD graf_watchbox(OBJECT *tree, WORD obj, UWORD instate, UWORD outstate)
{
    GR_TREE = (LONG)tree;
    GR_OBJ = obj;
    GR_INSTATE = instate;
    GR_OUTSTATE = outstate;
    return gem_if(GRAF_WATCHBOX);
}


WORD graf_slidebox(OBJECT *tree, WORD parent, WORD obj, WORD isvert)
{
    GR_TREE = (LONG)tree;
    GR_PARENT = parent;
    GR_OBJ = obj;
    GR_ISVERT = isvert;
    return gem_if(GRAF_SLIDEBOX);
}
*/


WORD graf_handle(WORD *pwchar, WORD *phchar, WORD *pwbox, WORD *phbox)
{
    gem_if(GRAF_HANDLE);
    *pwchar = GR_WCHAR ;
    *phchar = GR_HCHAR;
    *pwbox = GR_WBOX;
    *phbox = GR_HBOX;
    return (WORD)RET_CODE;
}


WORD graf_mouse(WORD m_number, void *m_addr)
{
    GR_MNUMBER = m_number;
    GR_MADDR = (LONG)m_addr;
    return gem_if(GRAF_MOUSE);
}


void graf_mkstate(WORD *pmx, WORD *pmy, WORD *pmstate, WORD *pkstate)
{
    gem_if(GRAF_MKSTATE);
    *pmx = GR_MX;
    *pmy = GR_MY;
    *pmstate = GR_MSTATE;
    *pkstate = GR_KSTATE;
}


/*
 *  Scrap Manager
 */
/* unused
WORD scrp_read(BYTE *pscrap)
{
    SC_PATH = (LONG)pscrap;
    return gem_if(SCRP_READ);
}

WORD scrp_write(const BYTE *pscrap)
{
    SC_PATH = (LONG)pscrap;
    return gem_if(SCRP_WRITE);
}
*/


/*
 *  File Selector Manager
 */
/* unused
WORD fsel_input(BYTE *pipath, BYTE *pisel, WORD *pbutton)
{
    FS_IPATH = (LONG)pipath;
    FS_ISEL = (LONG)pisel;
    gem_if(FSEL_INPUT);
    *pbutton = FS_BUTTON;
    return (WORD)RET_CODE;
}
*/


/*
 *  Window Manager
 */
WORD wind_create(UWORD kind, WORD wx, WORD wy, WORD ww, WORD wh)
{
    WM_KIND = kind;
    WM_WX = wx;
    WM_WY = wy;
    WM_WW = ww;
    WM_WH = wh;
    return gem_if(WIND_CREATE);
}


WORD wind_open(WORD handle, WORD wx, WORD wy, WORD ww, WORD wh)
{
    WM_HANDLE = handle;
    WM_WX = wx;
    WM_WY = wy;
    WM_WW = ww;
    WM_WH = wh;
    return gem_if(WIND_OPEN);
}


WORD wind_close(WORD handle)
{
    WM_HANDLE = handle;
    return gem_if(WIND_CLOSE);
}


WORD wind_delete(WORD handle)
{
    WM_HANDLE = handle;
    return gem_if(WIND_DELETE);
}


WORD wind_get(WORD w_handle, WORD w_field, WORD *pw1, WORD *pw2, WORD *pw3, WORD *pw4)
{
    WM_HANDLE = w_handle;
    WM_WFIELD = w_field;
    gem_if(WIND_GET);
    *pw1 = WM_OX;
    *pw2 = WM_OY;
    *pw3 = WM_OW;
    *pw4 = WM_OH;
    return (WORD)RET_CODE;
}


WORD wind_get_grect(WORD w_handle, WORD w_field, GRECT *gr)
{
    return wind_get(w_handle, w_field, &gr->g_x, &gr->g_y, &gr->g_w, &gr->g_h);
}


WORD wind_set(WORD w_handle, WORD w_field, ...) /* WORD w2, WORD w3, WORD w4, WORD w5) */
{
    va_list ap;

    va_start(ap, w_field);

    WM_HANDLE = w_handle;
    WM_WFIELD = w_field;
    WM_IX = va_arg(ap, int);
    WM_IY = va_arg(ap, int);
    WM_IW = va_arg(ap, int);
    WM_IH = va_arg(ap, int);
    va_end(ap);
    return gem_if(WIND_SET);
}


WORD wind_set_grect(WORD w_handle, WORD w_field, const GRECT *gr)
{
    return wind_set(w_handle, w_field, gr->g_x, gr->g_y, gr->g_w, gr->g_h);
}


WORD wind_find(WORD mx, WORD my)
{
    WM_MX = mx;
    WM_MY = my;
    return gem_if(WIND_FIND);
}


WORD wind_update(WORD beg_update)
{
    WM_BEGUP = beg_update;
    return gem_if(WIND_UPDATE);
}


WORD wind_calc(WORD wctype, UWORD kind, WORD x, WORD y, WORD w, WORD h,
               WORD *px, WORD *py, WORD *pw, WORD *ph)
{
    WM_WCTYPE = wctype;
    WM_WCKIND = kind;
    WM_WCIX = x;
    WM_WCIY = y;
    WM_WCIW = w;
    WM_WCIH = h;
    gem_if(WIND_CALC);
    *px = WM_WCOX;
    *py = WM_WCOY;
    *pw = WM_WCOW;
    *ph = WM_WCOH;
    return (WORD)RET_CODE;
}


/*
 *  Resource Manager
 */
/* unused
WORD rsrc_load(const BYTE *rsname)
{
    RS_PFNAME = (LONG)rsname;
    return gem_if(RSRC_LOAD);
}
*/


/* unused
WORD rsrc_free(void)
{
    return gem_if(RSRC_FREE);
}
*/


/* unused
 *
 * Note: We fake this call in deskmain.c when desktop is in ROM
 *
WORD rsrc_gaddr(WORD rstype, WORD rsid, void **paddr)
{
    RS_TYPE = rstype;
    RS_INDEX = rsid;
    gem_if(RSRC_GADDR);
    *paddr = (void *)RS_OUTADDR;
    return (WORD)RET_CODE;
}
*/


/* unused
WORD rsrc_saddr(WORD rstype, WORD rsid, void *lngval)
{
    RS_TYPE = rstype;
    RS_INDEX = rsid;
    RS_INADDR = (LONG)lngval;
    return gem_if(RSRC_SADDR);
}
*/


WORD rsrc_obfix(OBJECT *tree, WORD obj)
{
    RS_TREE = (LONG)tree;
    RS_OBJ = obj;
    return gem_if(RSRC_OBFIX);
}


/*
 *  Shell Manager
 */
/* unused
WORD shel_read(BYTE *pcmd, BYTE *ptail)
{
    SH_PCMD = (LONG)pcmd;
    SH_PTAIL = (LONG)ptail;
    return gem_if(SHEL_READ);
}
*/


WORD shel_write(WORD doex, WORD isgr, WORD iscr, BYTE *pcmd, BYTE *ptail)
{
    SH_DOEX = doex;
    SH_ISGR = isgr;
    SH_ISCR = iscr;
    SH_PCMD = (LONG)pcmd;
    SH_PTAIL = (LONG)ptail;
    return gem_if(SHEL_WRITE);
}


WORD shel_get(void *pbuffer, WORD len)
{
    SH_PBUFFER = (LONG)pbuffer;
    SH_LEN = len;
    return gem_if(SHEL_GET);
}


WORD shel_put(void *pdata, WORD len)
{
    SH_PDATA = (LONG)pdata;
    SH_LEN = len;
    return gem_if(SHEL_PUT);
}


WORD shel_find(BYTE *ppath)
{
    SH_PATH = (LONG)ppath;
    return gem_if(SHEL_FIND);
}


/* unused
WORD shel_envrn(BYTE *ppath, const BYTE *psrch)
{
    SH_PATH = (LONG)ppath;
    SH_SRCH = (LONG)psrch;
    return gem_if(SHEL_ENVRN);
}
*/


/* unused
WORD shel_rdef(BYTE *lpcmd, BYTE *lpdir)
{
    SH_LPCMD = (LONG)lpcmd;
    SH_LPDIR = (LONG)lpdir;
    return gem_if(SHEL_RDEF);
}
*/

/* unused
WORD shel_wdef(BYTE *lpcmd, BYTE *lpdir)
{
    SH_LPCMD = (LONG)lpcmd;
    SH_LPDIR = (LONG)lpdir;
    return gem_if(SHEL_WDEF);
}
*/
