DEFINE						+=	USES_GLEW
USES_GLEW					:=	1

COMMON_FLAGS				+=

ifdef   STATIC
DEFINE						+=	GLEW_STATIC
endif # STATIC

# MinGW
ifneq ($(MSYSTEM),)
ifdef   STATIC
LD_FLAGS					+=	-lglew32
else  # STATIC
LD_FLAGS					+=	-lglew32.dll
endif # STATIC

# Other Linux
else ifneq ($(SHELL),)
LD_FLAGS					+=	-lGLEW
endif # SYSTEM
