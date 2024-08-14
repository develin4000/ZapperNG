/*
->===============================================<-
->= ZapperNG - (c) Copyright 2020-2024 OnyxSoft =<-
->===============================================<-
->= Version  : 1.3                              =<-
->= File     : ZapperNG.c                       =<-
->= Author   : Stefan Blixth                    =<-
->= Compiled : 2024-08-14                       =<-
->===============================================<-
*/

#include "ZapperNG.h"
#include "debug.h"

const char Version[] = VERSTAG;

// Our magic function that arranges the widow as we want it =)
int ArrangeActiveWindow(BYTE hotkey, struct Window *win)
{
   struct Screen *tsp = NULL;
   struct Window *twp = NULL;
   struct Window *awp = NULL;
   ULONG lock;
   BYTE BarHeight = 0;
   UBYTE wpos = 0;
   UBYTE wcntr = 0;
   UBYTE wcnt = 0;
   UBYTE nextwin = 1;
   WORD LeftEdge, BestLeft = 0;
   WORD TopEdge, BestTop = 0;
   WORD Width, BestWidth = 0;
   WORD Height, BestHeight = 0;
   WORD MaxWidth, MaxHeight;

   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   wcntr = 0;

   lock = LockIBase(0);
   tsp = (struct Screen *)IntuitionBase->FirstScreen; // Fetch the active screen pointer...
   BarHeight = tsp->BarHeight + 1;                    // Fetch the height of the ScreenBar...
   MaxWidth = tsp->Width;
   MaxHeight = tsp->Height;

   awp = win ? win : IntuitionBase->ActiveWindow;

   for(twp = (struct Window *)tsp->FirstWindow; twp; twp = twp->NextWindow)
   {
      if(!(twp->Flags & (WFLG_BACKDROP)))
      {
         if (awp == twp)  wpos = wcntr;

         wlist[wcntr].winptr   = twp;
         wlist[wcntr].LeftEdge = twp->LeftEdge;
         wlist[wcntr].TopEdge  = twp->TopEdge;
         wlist[wcntr].Width    = twp->Width;
         wlist[wcntr].Height   = twp->Height;
         wcntr++;
      }
   }

   if (hotkey == EVT_TAB)  //  Alt+Tab hotkey pressed...
   {
      if ((xwp == NULL) || (xcnt != wcntr))
      {
         xcnt = wcntr;
         xwp = (struct Window *)tsp->FirstWindow;

         if (!(xwp->Flags & (WFLG_BACKDROP | WFLG_BORDERLESS | WFLG_GIMMEZEROZERO)))
         {
            WindowToFront(xwp);
            ActivateWindow(xwp);
         }
      }
      else
      {
         do
         {
            if ((xwp = xwp->NextWindow) != NULL)
            {
               if (!(xwp->Flags & (WFLG_BACKDROP | WFLG_BORDERLESS | WFLG_GIMMEZEROZERO)))
               {
                  WindowToFront(xwp);
                  ActivateWindow(xwp);
                  nextwin = 0;
               }
               else
                  nextwin = 1;
            }
            else
            {
               nextwin = 0;
            }
         }
         while (nextwin != 0);
      }

      UnlockIBase(lock);
      return 0;
   }
   
   debug_print("ZapperNG : %s (%d) - Max resolution = %d x %d\n", __func__, __LINE__, MaxWidth, MaxHeight)
   debug_print("ZapperNG : %s (%d) - Number of windows found : %d\n", __func__, __LINE__, wcntr);
   wcnt = wcntr;

   if (hotkey == EVT_ZAPP)  // Zapper hotkey pressed or ZipWindow-function called...
   {
      for (wcntr = 0; wcntr <= wcnt; wcntr++)
      {
         LeftEdge = wlist[wcntr].LeftEdge;
         TopEdge  = wlist[wcntr].TopEdge;
         Width    = wlist[wcntr].Width;
         Height   = wlist[wcntr].Height;

         if ((LeftEdge+TopEdge+Width+Height) > 0)
         {
            if ((LeftEdge + Width) <= awp->LeftEdge)  // Windows to the left of the active window... OK
            {
               if ((TopEdge < (awp->TopEdge+awp->Height)) && ((TopEdge+Height) > awp->TopEdge))
               {
                  if (BestLeft)
                     BestLeft = (LeftEdge + Width) > BestLeft ? (LeftEdge + Width) : BestLeft;
                  else
                     BestLeft = (LeftEdge + Width);

                  debug_print("ZapperNG : %s (%d) - Left - Match! (%d/%d/%d/%d)\n", __func__, __LINE__, LeftEdge, TopEdge, Width, Height);
               }
            }

            if ((LeftEdge) >= (awp->LeftEdge+awp->Width))  // Windows to the right of the active window... OK
            {
               if ((TopEdge < (awp->TopEdge+awp->Height)) && ((TopEdge+Height) > awp->TopEdge))
               {
                  if (BestWidth)
                     BestWidth = LeftEdge < BestWidth ? LeftEdge : BestWidth;
                  else
                     BestWidth = LeftEdge;

                  debug_print("ZapperNG : %s (%d) - Right - Match! (%d/%d/%d/%d)\n", __func__, __LINE__, LeftEdge, TopEdge, Width, Height);
               }
            }

            if ((TopEdge + Height) <= awp->TopEdge)  // Windows above active window... OK
            {
               if (((LeftEdge + Width) > awp->LeftEdge)  && (LeftEdge < (awp->LeftEdge+awp->Width)))
               {
                  if (BestTop)
                     BestTop = (TopEdge + Height) > BestTop ? (TopEdge + Height) : BestTop ;
                  else 
                     BestTop = (TopEdge + Height);

                  debug_print("ZapperNG : %s (%d) - Above - Match! (%d/%d/%d/%d)\n", __func__, __LINE__, LeftEdge, TopEdge, Width, Height);
               }
            }

            if ((TopEdge) >= (awp->TopEdge + awp->Height))  // Windows under active window... OK
            {
               if (((LeftEdge + Width) > awp->LeftEdge)  && (LeftEdge < (awp->LeftEdge+awp->Width)))
               {
                  if (BestHeight)
                     BestHeight = TopEdge < BestHeight ? TopEdge : BestHeight;
                  else
                     BestHeight = TopEdge;

                  debug_print("ZapperNG : %s (%d) - Below - Match! (%d/%d/%d/%d)\n", __func__, __LINE__, LeftEdge, TopEdge, Width, Height);
               }
            }
         }
      }

      if (!BestWidth)
         BestWidth  = MaxWidth;
      if (!BestHeight)
         BestHeight = MaxHeight;
      
      debug_print("ZapperNG : %s (%d) - NewSize  = (%d/%d/%d/%d)\n", __func__, __LINE__, BestLeft, BestTop, BestWidth, BestHeight); 
   }
   else  //  BalanceWindow hotkey pressed...
   {
      if (awp == NULL)
         awp = IntuitionBase->ActiveWindow;

      BestLeft = 0;
      BestTop = 0;
      BestWidth = MaxWidth;
      BestHeight = MaxHeight;

      switch (hotkey)
      {
         case EVT_UP:
            BestHeight = MaxHeight/2;
            break;

         case EVT_DOWN:
            BestTop = MaxHeight/2;
            break;

         case EVT_LEFT:
            BestWidth = MaxWidth/2;
            break;

         case EVT_RIGHT:
            BestLeft = MaxWidth/2;
           break;
      }

      debug_print("ZapperNG : %s (%d) - awp = %d ; twp = %d ; wlist[wpos].winptr = %d\n", __func__, __LINE__, awp, twp, wlist[wpos].winptr);

      for (wcntr = 0; wcntr <= wcnt; wcntr++)
      {
         if (awp == wlist[wcntr].winptr)
         {
            wpos = wcntr;
            debug_print("ZapperNG : %s (%d) - Found a match! - wcntr = %d ; wlist[wcntr].winptr = %d\n", __func__, __LINE__, wcntr, wlist[wcntr].winptr);
         }
      }
   }

   if (wlist[wpos].zapped)
   {
      wlist[wpos].zapped =  FALSE;
      debug_print("ZapperNG : %s (%d) - Window allready Zapped - restore old positions...!\n", __func__, __LINE__);
      ChangeWindowBox(awp, wlist[wpos].OldLeft, wlist[wpos].OldTop, wlist[wpos].OldWidth, wlist[wpos].OldHeight);
      if (!opt_nofront) WindowToFront(awp);
   }
   else
   {
      wlist[wpos].zapped    = TRUE;
      wlist[wpos].OldLeft   = awp->LeftEdge;
      wlist[wpos].OldTop    = awp->TopEdge;
      wlist[wpos].OldWidth  = awp->Width;
      wlist[wpos].OldHeight = awp->Height;

      // Make this optional... IGNORESCREENTITLE
      if (opt_scrtitle == FALSE)
      {
         if (BestTop == 0)
            BestTop = BarHeight;
      }

      debug_print("ZapperNG : %s (%d) - Window Zapped - %d! OL : %d, OT : %d, OW : %d, OH : %d\n", __func__, __LINE__, BarHeight, wlist[wpos].OldLeft, wlist[wpos].OldTop, wlist[wpos].OldWidth, wlist[wpos].OldHeight); 
      ChangeWindowBox(awp, BestLeft, BestTop, BestWidth-BestLeft, BestHeight-BestTop);
      if (!opt_nofront) WindowToFront(awp);
   }

   UnlockIBase (lock);
}


// This is new ZipWindow function functionality...
void NewZipWindow(void)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);
   ArrangeActiveWindow(EVT_ZAPP, (APTR)REG_A0);//__reg("a0"));
}


// Some patching is needed to trap the function call under MorphOS... 
#ifdef __MORPHOS__
 struct EmulLibEntry GATE_NewZipWindow = {TRAP_LIB, 0, (void (*)(void)) NewZipWindow};
#endif


// Here we patch the ZipWindow function to our own function
void PatchZipWindow(void)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   Forbid();
#ifdef __MORPHOS__
   OldFunctionPtr = SetFunction(IntuitionBase, OFFSET_ZIPWINDOW, &GATE_NewZipWindow);
#else
   OldFunctionPtr = SetFunction(IntuitionBase, OFFSET_ZIPWINDOW, NewZipWindow);
#endif
   CacheClearU();
   Permit();
}


// Restore the ZipWindow function to its orginal functionality
void RestoreZipWindow(void)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   SetFunction(IntuitionBase, OFFSET_ZIPWINDOW, (APTR)OldFunctionPtr);
   OldFunctionPtr = NULL;
}


// Bind hotkeys to Commdodity Broker
CxObj *BindHotkey(STRPTR Code, LONG ID)
{
   CxObj *Filter;
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   if(Filter = CxFilter(Code))
   {
      CxObj *Sender;

      if(Sender = CxSender(zapperbroker.nb_Port, ID))
      {
         CxObj *Translator;

         AttachCxObj(Filter, Sender);

         if(Translator = CxTranslate(NULL))
         {
            AttachCxObj(Filter, Translator);

            if(!CxObjError(Filter))
               return(Filter);
         }
      }

      DeleteCxObjAll(Filter);
   }

   return(NULL);
}


// Clean up the Commodity Broker
void CleanupBroker(void)
{
   CxMsg *msg;
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   if (broker) DeleteCxObjAll(broker);   

   if (zapperbroker.nb_Port)
   {
      while ((msg = (CxMsg *)GetMsg(zapperbroker.nb_Port)))
      {
         ReplyMsg((struct Message *)msg);
      }
      DeleteMsgPort(zapperbroker.nb_Port);
   }

   broker = NULL;
   zapperbroker.nb_Port = NULL;
}


// Create Commdodity Broker
int CreateBroker(void)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   if ((zapperbroker.nb_Port = CreateMsgPort()))
   {
      cxsig = 1 << zapperbroker.nb_Port->mp_SigBit;
      zapperbroker.nb_Pri = 0;

      if ((broker = CxBroker(&zapperbroker, NULL)))
      {
         ActivateCxObj(broker, 1);
         return (TRUE);
      }
   }

   CleanupBroker();
   return (FALSE);
}


// Handler for the Commodity Broker
int HandleBroker(void)
{
   CxMsg *msg;
   LONG id, type;

   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   while ((msg = (CxMsg *)GetMsg(zapperbroker.nb_Port)))
   {
      type = CxMsgType(msg);
      id = CxMsgID(msg);

      ReplyMsg((struct Message *)msg);

      switch (type)
      {
         case CXM_IEVENT:
            ArrangeActiveWindow(id, NULL);
            break;

         case CXM_COMMAND:
            switch (id)
            {
               case CXCMD_DISABLE:
                  ActivateCxObj(broker, 0);
                  break;

               case CXCMD_ENABLE:
                  ActivateCxObj(broker, 1);
                  break;

               case CXCMD_KILL:
               case CXCMD_UNIQUE:
                  return 0;
                  break;
            }
            break;
      }
   }
   return 1;
}


// Cleanup function
void Cleanup(void)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);
   CleanupBroker();

#ifdef __amigaos4__
   if (IIntuition)   DropInterface((struct Interface*)IIntuition);
   if (ICommodities) DropInterface((struct Interface*)ICommodities);
   if (IIcon)        DropInterface((struct Interface*)IIcon);
#endif

   if (IntuitionBase) CloseLibrary((struct Library *) IntuitionBase);
   if (CxBase) CloseLibrary((struct Library *) CxBase);
   if (IconBase) CloseLibrary((struct Library *) IconBase);
}


// Init function
BOOL Init(void)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   if (!(IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 37L))) return FALSE;
   if (!(CxBase = OpenLibrary("commodities.library", 37))) return FALSE;
   if (!(IconBase = (struct Library *)OpenLibrary("icon.library", 37L))) return FALSE;

#ifdef __amigaos4__
   if (!(IIntuition = (struct IntuitionIFace*)GetInterface((struct Library*)IntuitionBase,"main",1,NULL))) return FALSE;
   if (!(ICommodities = (struct CommoditiesIFace*)GetInterface((struct Library *) CxBase,"main",1,NULL))) return FALSE;
   if (!(IIcon = (struct IconIFace*)GetInterface(IconBase,"main",1,NULL))) return FALSE;
#endif

   struct NewBroker newbroker = 
   {
      NB_VERSION,
      NAME,
      MUISTRING,
      DESCRIPTION,
      NBU_UNIQUE | NBU_NOTIFY,
      COF_SHOW_HIDE,
      0,
      0,
      0
   };

   zapperbroker = newbroker;
   if (!CreateBroker()) return FALSE;

   return TRUE;
}


// Handler function
void Handler(void)
{
   ULONG sigs = 0;
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   while (going)
   { 
      sigs = Wait(cxsig | SIGBREAKF_CTRL_C);

      if (sigs & cxsig)
      {
        if (!HandleBroker())
           going = FALSE;
      }
      else if (sigs & SIGBREAKF_CTRL_C)
      {
         going = FALSE;
      }
   }
}


// Parse the type of tool...
BOOL FetchType(struct WBArg *wbarg)
{
   struct DiskObject *dobj;
   char **toolarray;
   char *temp;
   BOOL success = FALSE;

   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   if ((*wbarg->wa_Name) && (dobj=GetDiskObject(wbarg->wa_Name)))
   {
      toolarray = (char **)dobj->do_ToolTypes;

      if ((temp = FindToolType((STRPTR *)toolarray,"BALANCEWINDOW")))
         use_hk = TRUE;

      if ((temp = FindToolType((STRPTR *)toolarray,"ZAPPERKEY")))
      {
         use_zk = TRUE;
         strcpy(txt_zapkey, temp);
      }

      if ((temp = FindToolType((STRPTR *)toolarray, "IGNORESCREENTITLE")))
         opt_scrtitle = TRUE;

      if ((temp = FindToolType((STRPTR *)toolarray, "NOPUSHTOFRONT")))
         opt_nofront = TRUE;

      if ((temp = FindToolType((STRPTR *)toolarray, "ALTTAB")))
         opt_alttab = TRUE;

      FreeDiskObject(dobj);
      success = TRUE;
   }

   return(success);
}


// Check ToolTypes settings on icon... 
BOOL FetchTools(int argc, char **argv)
{
   struct WBStartup *WBenchMsg;
   struct WBArg *wbarg;
   LONG olddir = -1;
   LONG i;

   WBenchMsg = (struct WBStartup *)argv;

   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   for (i=0, wbarg = WBenchMsg->sm_ArgList ; i < WBenchMsg->sm_NumArgs; i++, wbarg++)
   {
      olddir = -1;

      if ((wbarg->wa_Lock)&&(*wbarg->wa_Name))
         olddir = CurrentDir(wbarg->wa_Lock);

      FetchType(wbarg);

      if(olddir != -1)
         CurrentDir(olddir);
   }
}


// Setup the hotkeys for our BalancedWindow patch...
void SetupHotkeys(void)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   if (use_hk)
   {
      hk_keyup = BindHotkey("lamiga up", EVT_UP);
      if (hk_keyup) AttachCxObj(broker, hk_keyup);

      hk_keydown = BindHotkey("lamiga down", EVT_DOWN);
      if (hk_keydown) AttachCxObj(broker, hk_keydown);

      hk_keyleft = BindHotkey("lamiga left", EVT_LEFT);
      if (hk_keydown) AttachCxObj(broker, hk_keyleft);

      hk_keyright = BindHotkey("lamiga right", EVT_RIGHT);
      if (hk_keyright) AttachCxObj(broker, hk_keyright);
   }

   if (opt_alttab)
   {
      hk_alttab = BindHotkey("alt tab", EVT_TAB);
      if (hk_alttab) AttachCxObj(broker, hk_alttab);
   }

   if (use_zk)
   {
      hk_zapper = BindHotkey(txt_zapkey, EVT_ZAPP);
      if (hk_zapper) AttachCxObj(broker, hk_zapper);
   }
}


// Our starting point =)
int main(int argc, char **argv)
{
   debug_print("ZapperNG : %s (%d)\n", __func__, __LINE__);

   if (Init())                   // Open a required libraries etc.
   {
      if (argc == 0)             // Started from WB ?
         FetchTools(argc, argv); // ...Then check if we have some ToolTypes set 

      SetupHotkeys();            // Setup hotkeys for BalancedWindows patch
      PatchZipWindow();          // Patch the ZipWindow behaviour.
      Handler();                 // Wait for stuff to happen
      RestoreZipWindow();        // Restore the patch to normal.
   }

   Cleanup();                    // Clean up after us.
   return 0;
}
