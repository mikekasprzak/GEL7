DEFINE						+=	USES_GCC
USES_GCC					:=	1

# GCC does not support Clang's sanitizers
CC							:=	gcc
CXX							:=	g++
LD							:=	ld
LINK_WITH_LINKER			:=	1

ifdef   STATIC
LD_FLAGS					+=	--static
endif # STATIC

COMMON_FLAGS				+=	-Wall -Wno-unknown-pragmas -Wno-comment

ifeq ($(MSYSTEM),MINGW64)
COMMON_FLAGS				+=	-msse4.1
else ifeq ($(MSYSTEM),MINGW32)
COMMON_FLAGS				+=	-msse
endif # MSYSTEM

# Enable debug symbols
ifdef   DEBUG
COMMON_FLAGS				+=	-g -D_DEBUG
endif # DEBUG

# Enable optimizations
ifdef   RELEASE
COMMON_FLAGS				+=	-O2
endif # RELEASE
