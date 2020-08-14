include src/Build/Include/Clang.inc.mk

# Angle has to come first, so we know to omit linking
include src/Build/Include/Angle.inc.mk
include src/Build/Include/GLES2.inc.mk
include src/Build/Include/SDL2.inc.mk

include src/Build/Include/Windows.inc.mk

SRC_DIRS		+=	src/Platform/SDL2-GL
