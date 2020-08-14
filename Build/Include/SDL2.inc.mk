DEFINE						+=	USES_SDL2
USES_SDL2					:=	1

SRC_DIRS					+=	src/Main/SDL2 \
								src/Event/SDL2 \
								src/Input/SDL2

# Currently only ignored by Android
ifdef USES_UNIX_TOOLS

# Setup config tool
ifneq ($(MSYSTEM),)
SDL2_CONFIG					=	$(shell $(MINGW_PREFIX)/bin/sdl2-config $1)
else  # MSYSTEM
SDL2_CONFIG					=	$(shell sdl2-config $1)
endif # MSYSTEM

# Populate flags
COMMON_FLAGS				+=	$(call SDL2_CONFIG, --cflags)

# Populate linker flags
ifdef   STATIC
LD_FLAGS					+=	$(call SDL2_CONFIG, --static-libs)
else  # STATIC
LD_FLAGS					+=	$(call SDL2_CONFIG, --libs)
endif # STATIC

endif # USES_UNIX_TOOLS
