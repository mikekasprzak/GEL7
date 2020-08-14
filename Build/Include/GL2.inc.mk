DEFINE						+=	USES_OPENGL USES_GL2 USES_GLSL USES_DESKTOP_GL
USES_OPENGL					:=	1
USES_GL2					:=	1
USES_GLSL					:=	1
USES_DESKTOP_GL				:=	1

SRC_DIRS					+=	src/GFX/GL

# MinGW
ifneq ($(MSYSTEM),)
COMMON_FLAGS				+=
LD_FLAGS					+=	-lopengl32
# Other Linux
else ifneq ($(SHELL),)
COMMON_FLAGS				+=
LD_FLAGS					+=	-lGL
endif # SYSTEM
