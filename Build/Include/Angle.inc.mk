DEFINE						+=	USES_ANGLE
USES_ANGLE					:=	1

COMMON_FLAGS				+=	

ifdef   STATIC
LD_FLAGS					+=	libEGL.dll libGLESv2.dll d3dcompiler_47.dll
else  # STATIC
LD_FLAGS					+=	libEGL.dll libGLESv2.dll d3dcompiler_47.dll
endif # STATIC