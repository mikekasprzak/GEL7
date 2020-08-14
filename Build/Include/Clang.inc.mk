DEFINE						+=	USES_CLANG
USES_CLANG					:=	1

# To support Clang's sanitizers we need to do our linking with clang itself, not LD
CC							:=	clang
CXX							:=	clang++
LD							:=	clang
LINK_WITH_COMPILER			:=	1

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

# Enable Clang's sanitizers (memory and address checking)
ifneq ($(MSYSTEM),)
else  # !MSYSTEM
# https://clang.llvm.org/docs/AddressSanitizer.html
LD_FLAGS					+=	-fsanitize=address -fno-omit-frame-pointer
# https://clang.llvm.org/docs/LeakSanitizer.html
#LD_FLAGS					+=	-fsanitize=leak
endif # MSYSTEM
endif # DEBUG

# Enable optimizations
ifdef   RELEASE
COMMON_FLAGS				+=	-O2
endif # RELEASE
