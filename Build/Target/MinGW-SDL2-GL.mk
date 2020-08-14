include src/Build/Include/GCC.inc.mk

# GLEW must come first to statically link
include src/Build/Include/GLEW.inc.mk
include src/Build/Include/GL2.inc.mk
include src/Build/Include/SDL2.inc.mk

include src/Build/Include/Windows.inc.mk

SRC_DIRS		+=	src/Platform/SDL2-GL
