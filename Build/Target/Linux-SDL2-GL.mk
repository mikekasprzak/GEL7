include src/Build/Include/Clang.inc.mk
include src/Build/Include/Linux.inc.mk

include src/Build/Include/GL2.inc.mk
include src/Build/Include/GLEW.inc.mk
include src/Build/Include/SDL2.inc.mk


SRC_DIRS		+=	src/Platform/SDL2-GL


.C_FLAGS		+=	$(SDL2_C_FLAGS) $(GFX_C_FLAGS) $(PLATFORM_C_FLAGS) $(COMPILER_C_FLAGS)
.CPP_FLAGS		+=
LD_FLAGS		+=	$(SDL2_FLAGS) $(GFX_FLAGS) $(PLATFORM_FLAGS) $(COMPILER_FLAGS)
#LD_FLAGS		+=	$(SDL2_STATIC_FLAGS) $(GFX_STATIC_FLAGS) $(PLATFORM_STATIC_FLAGS) $(STATIC_LD_FLAGS) $(COMPILER_STATIC_FLAGS)
