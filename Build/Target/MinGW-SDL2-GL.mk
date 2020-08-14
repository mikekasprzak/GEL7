include src/GEL7/Build/Include/GCC.inc.mk

# GLEW must come first to statically link
include src/GEL7/Build/Include/GLEW.inc.mk
include src/GEL7/Build/Include/GL2.inc.mk
include src/GEL7/Build/Include/SDL2.inc.mk

include src/GEL7/Build/Include/Windows.inc.mk

SRC_DIRS		+=	src/GEL7/Platform/SDL2-GL
