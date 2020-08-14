DEFINE						+=	USES_WINDOWS
USES_WINDOWS				:=	1

TARGET_EXT					:=	.exe

COMMON_FLAGS				+=	

ifdef   STATIC
LD_FLAGS					+=	-lwinmm -limm32 -lole32 -lversion -loleaut32 -lsetupapi 
else  # STATIC
LD_FLAGS					+=	
endif # STATIC

# Icons and Windows Metadata
OTHER_O_FILES				+=	win32/*.o

# Enable debug symbols
ifdef   DEBUG
ifneq ($(MSYSTEM),)
LD_FLAGS					+=	-mconsole
endif # MSYSTEM
endif # DEBUG
