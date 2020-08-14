include src/GEL7/Build/Include/Clang.inc.mk

# Angle has to come first, so we know to omit linking
include src/GEL7/Build/Include/Angle.inc.mk
include src/GEL7/Build/Include/GLES2.inc.mk
include src/GEL7/Build/Include/SDL2.inc.mk

include src/GEL7/Build/Include/Windows.inc.mk

SRC_DIRS		+=	src/GEL7/Platform/SDL2-GL
