/*
->===============================================<-
->= ZapperNG - (c) Copyright 2020-2024 OnyxSoft =<-
->===============================================<-
->= Version  : 1.3                              =<-
->= File     : ZapperNG.h                       =<-
->= Author   : Stefan Blixth                    =<-
->= Compiled : 2024-08-14                       =<-
->===============================================<-
*/

#ifndef ZAPPERNG_H_
#define ZAPPERNG_H_


/* Standard... */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* System... */
#if !defined (__amigaos4__) || !defined(__AROS__)
 #include <clib/alib_protos.h>
#endif

#ifdef __MORPHOS__
 #include <emul/emulinterface.h>
#endif

#include <exec/types.h>
#include <libraries/dos.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>

#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/exec.h>
#include <proto/commodities.h>
#include <proto/icon.h>
#include <libraries/dos.h>
#include <libraries/commodities.h>
#include <intuition/intuitionbase.h>

#define USE_INLINE_STDARG

#ifdef __amigaos4__
struct IntuitionIFace   *IIntuition   = NULL;
struct CommoditiesIFace *ICommodities = NULL;
struct IconIFace        *IIcon        = NULL;
#endif

struct IntuitionBase *IntuitionBase = NULL;
struct Library       *CxBase        = NULL;
struct Library       *IconBase      = NULL;


/*=----------------------------- Patches and Macros()-------------------------*
 * Patching some incompatible functions and adds some useful macros           *
 *----------------------------------------------------------------------------*/
#ifndef __MORPHOS__

   #define ASM
  //#define REG(x) register __ ## x
  //#define REG(reg, arg)	arg __asm(#req)
  #define __REGA0(x) x __asm("a0")
#endif


BOOL going = TRUE;
BOOL use_hk = FALSE;
BOOL use_zk = FALSE;
CxObj *broker;
CxObj *hk_keyup, *hk_keydown, *hk_keyleft, *hk_keyright, *hk_zapper, *hk_alttab;
ULONG cxsig;
struct NewBroker zapperbroker;
APTR OldFunctionPtr = NULL;
struct Window *xwp = NULL;
UBYTE xcnt = 0;

BOOL opt_scrtitle = FALSE, opt_nofront = FALSE, opt_alttab = FALSE;

char txt_zapkey[100];

struct winlist
{
   BOOL zapped;
   struct Window *winptr;
   WORD LeftEdge;
   WORD TopEdge;
   WORD Width;
   WORD Height;
   WORD OldLeft;
   WORD OldTop;
   WORD OldWidth;
   WORD OldHeight;
};

struct winlist wlist[200];       // This is ugly, I know =)

#define OFFSET_ZIPWINDOW   -504  // -0x1f8 for function ZipWindow() triggered by the Zoom gadget


// Events for the hotkeys...
#define EVT_UP    1
#define EVT_DOWN  2
#define EVT_LEFT  3
#define EVT_RIGHT 4
#define EVT_ZAPP  5
#define EVT_TAB   6


#ifndef __AMIGADATE__
 #define __AMIGADATE__   "14.08.24"
#endif


/*----------------------------------------------------------------------------*
 *                                  Defines                                   *
 *----------------------------------------------------------------------------*/

#define VYEARS                "2020-2024"
#define AUTHOR                "Stefan Blixth"
#define EMAIL                 "stefan@onyxsoft.se"
#define URL                   "www.onyxsoft.se"
#define COMPANY               "OnyxSoft"
#define COPYRIGHT             "(c)" VYEARS " " AUTHOR ", " COMPANY
#define DATE                  __AMIGADATE__
#define VERSION               1
#define STRVERSION            "1"
#define REVISION              3
#define STRREVISION           "3"

#define NAME                  "ZapperNG"
#define DESCRIPTION           "Patches the intuition behaviour"
#define VERS                  NAME " "STRVERSION"."STRREVISION
#define VSTRING               VERS" ("DATE") (c) "VYEARS " " AUTHOR ", " COMPANY
#define MUISTRING             VERS" ("DATE") (c) "COMPANY
#define VERSTAG               "$VER:" VSTRING
#define VERSTAG_MUI           "$VER: "VERS " ("DATE")"

//static const char Version[] = VERSTAG;

#endif /* ZAPPERNG_H_ */
