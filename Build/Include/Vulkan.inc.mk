DEFINE						+=	USES_VULKAN
USES_VULKAN					:=	1

SRC_DIRS					+=	src/GFX/Vulkan

COMMON_FLAGS				+=	

ifdef   STATIC
LD_FLAGS					+=	-lvulkan.dll
else  # STATIC
LD_FLAGS					+=	-lvulkan
endif # STATIC