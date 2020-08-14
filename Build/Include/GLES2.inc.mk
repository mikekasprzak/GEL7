DEFINE						+=	USES_OPENGL USES_GLES2 USES_GLSL
USES_OPENGL					:=	1
USES_GLES2					:=	1
USES_GLSL					:=	1

SRC_DIRS					+=	src/GEL7/GFX/GL

# Linux
ifdef USES_ANGLE
COMMON_FLAGS				+=	-Isrc/GEL7/External
LD_FLAGS					+=	
else ifneq ($(SHELL),)
COMMON_FLAGS				+=
LD_FLAGS					+=	-lGLESv2
# Android
else  # SYSTEM
COMMON_FLAGS				+=	-Isrc/GEL7/External
LD_FLAGS					+=	
endif # SYSTEM
