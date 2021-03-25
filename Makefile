#->=============================================<-
#->= ZapperNG - © Copyright 2020-2021 OnyxSoft =<-
#->=============================================<-
#->= Version  : 1.2                            =<-
#->= File     : Makefile                       =<-
#->= Author   : Stefan Blixth                  =<-
#->= Compiled : 2021-03-24                     =<-
#->=============================================<-

#
# Application specific stuff
#
APPNAME				= ZapperNG

APP_MORPHOS			= $(APPNAME)_MorphOS
APP_MORPHOS_DB		= $(APP_MORPHOS)_db
APP_AMIGAOS4		= $(APPNAME)_AmigaOS4
APP_AMIGAOS4_DB	= $(APP_AMIGAOS4)_db
APP_AROS				= $(APPNAME)_AROS
APP_AMIGAOS			= $(APPNAME)_AmigaOS3

#
# Compiler tools assign
#
#CC_MORPHOS			= ppc-morphos-gcc
#STRIP_MORPHOS		= ppc-morphos-strip
CC_MORPHOS			= gcc
STRIP_MORPHOS		= strip
CC_AMIGAOS4			= ppc-amigaos-gcc
STRIP_AMIGAOS4		= ppc-amigaos-strip
CC_AROS				= i386-aros-gcc
STRIP_AROS			= 
CC_AMIGAOS3			= m68-amigaos-gcc
STRIP_AMIGAOS3		=

#
# Relative paths
#
SRCDIR				= source
INCDIR				= include
OBJDIR				= obj
LIBDIR				= libs
EXEDIR				= release


#
# Platform specific compiler and linker flags 
#
CFLG_MOS			= -noixemul -D__AMIGADATE__=\"$(shell date "+%d.%m.%y")\"
LFLG_MOS			=
OPTS_MOS			= -O2
OBJS_MOS				= $(APPNAME)_morphos.o

CFLG_MOS_DB			= -noixemul -Wall -D__AMIGADATE__=\"$(shell date "+%d.%m.%y")\" -DUSEDEBUG -g
LFLG_MOS_DB			= -ldebug
OPTS_MOS_DB			= 
OBJS_MOS_DB			= $(APPNAME)_morphos_db.o

CFLG_OS4			= -D__AMIGADATE__=\"$(shell date "+%d.%m.%y")\" -D__USE_BASETYPE__ -D__USE_INLINE__ -D__USE_OLD_TIMEVAL__ -mcrt=clib2
LFLG_OS4			= 
OPTS_OS4			= -O2
OBJS_OS4			= $(APPNAME)_amigaos4.o

CFLG_OS4_DB			= -D__AMIGADATE__=\"$(shell date "+%d.%m.%y")\" -D__USE_BASETYPE__ -D__USE_INLINE__ -D__USE_OLD_TIMEVAL__ -DUSEDEBUG -g -gstabs -mcrt=clib2
LFLG_OS4_DB			= 
OPTS_OS4_DB			= 
OBJS_OS4_DB			= $(APPNAME)_amigaos4_db.o

CFLG_AROS			= -D__AMIGADATE__=\"$(shell date "+%d.%m.%y")\"
LFLG_AROS			=
OPTS_AROS			= -O2
OBJS_AROS				= $(APPNAME)_aros.o

CFLG_OS3			= -noixemul -D__AMIGADATE__=\"$(shell date "+%d.%m.%y")\"
LFLG_OS3			=
OPTS_OS3			= -O2
OBJS_OS3				= $(APPNAME)_amigaos3.o

#
# Source & include files
#
SOURCE_APP			= $(APPNAME).c

.PHONY:	clean usage


usage:
	@echo ""
	@echo "  Application - $(APPNAME)"
	@echo " +-------------------------------------------------------------------+"
	@echo " | clean       - Deletes all files in the obj & release-directories. |"
	@echo " | all         - Make all of the options below                       |"
	@echo " +-------------------------------------------------------------------+"
	@echo " | morphos     - Compiles a binary for MorphOS.                      |"
	@echo " | morphos_db  - Compiles a debug-enabled binary for MorphOS.        |"
	@echo " | amigaos4    - Compiles a binary for AmigaOS 4.x                   |"
	@echo " | amigaos4_db - Compiles a debug-enabled binary for AmigaOS 4.x     |"
	@echo " | amigaos3    - Compiles a binary for AmigaOS3                      |"
	@echo " | aros        - Compiles a binary for AROS x86                      |"
	@echo " +-------------------------------------------------------------------+"
	@echo ""

all:	morphos morphos_db

clean:
	@echo "Cleaning up..."
	@echo ""
	rm -f $(OBJDIR)/*
	rm -f $(EXEDIR)/*
	@echo ""
	@echo "Done."


morphos_db: $(APP_MORPHOS_DB)
	@echo ""
	@echo "Debug-enabled MorphOS binary sucessfully built..."
	@echo ""

$(APP_MORPHOS_DB):	$(SRCDIR)/$(SOURCE_APP)
	$(CC_MORPHOS) $(SRCDIR)/$(SOURCE_APP) $(CFLG_MOS_DB) $(LFLG_MOS_DB) -o $(EXEDIR)/$(APP_MORPHOS_DB)


morphos: $(APP_MORPHOS)
	@echo ""
	@echo "MorphOS binary sucessfully built..."
	@echo ""

$(APP_MORPHOS):	$(SRCDIR)/$(SOURCE_APP)
	$(CC_MORPHOS) $(SRCDIR)/$(SOURCE_APP) $(OPTS_MOS) $(CFLG_MOS) $(LFLG_MOS) -o $(EXEDIR)/$(APP_MORPHOS)
#	$(STRIP_MORPHOS) $(EXEDIR)/$(APP_MORPHOS)


amigaos4: $(APP_AMIGAOS4)
	@echo ""
	@echo "AmigaOS4 binary sucessfully built..."
	@echo ""

$(APP_AMIGAOS4):	$(SRCDIR)/$(SOURCE_APP)
	$(CC_AMIGAOS4)  $(SRCDIR)/$(SOURCE_APP) $(OPTS_OS4) $(CFLG_OS4) $(LFLG_OS4) -o $(EXEDIR)/$(APP_AMIGAOS4)
#	$(STRIP_AMIGAOS4) $(EXEDIR)/$(APP_AMIGAOS4)


amigaos4_db: $(APP_AMIGAOS4_DB)
	@echo ""
	@echo "Debug-enabled AmigaOS4 binary sucessfully built..."
	@echo ""

$(APP_AMIGAOS4_DB):	$(SRCDIR)/$(SOURCE_APP)
	$(CC_AMIGAOS4) $(SRCDIR)/$(SOURCE_APP) $(OPTS_OS4_DB) $(CFLG_OS4_DB) $(LFLG_OS4_DB) -o $(EXEDIR)/$(APP_AMIGAOS4_DB)


aros: $(APP_AROS)
	@echo ""
	@echo "AROS binary sucessfully built..."
	@echo ""

$(APP_AROS):	$(SRCDIR)/$(SOURCE_APP)
	$(CC_AROS)  $(SRCDIR)/$(SOURCE_APP) $(OPTS_AROS) $(CFLG_AROS) $(LFLG_AROS) -o $(EXEDIR)/$(APP_AROS)
#	$(STRIP_AROS) $(EXEDIR)/$(APP_AROS)


amigaos3: $(APP_AMIGAOS3)
	@echo ""
	@echo "AmigaOS3 binary sucessfully built..."
	@echo ""

$(APP_AMIGAOS3):	$(SRCDIR)/$(SOURCE_APP)
	$(CC_AMIGAOS3)  $(SRCDIR)/$(SOURCE_APP) $(OPTS_OS3) $(CFLG_OS3) $(LFLG_OS3) -o $(EXEDIR)/$(APP_AMIGAOS3)
#	$(STRIP_AMIGAOS3) $(EXEDIR)/$(APP_AMIGAOS3)
