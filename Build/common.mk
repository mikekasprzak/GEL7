# Functions (must use '=', and not ':='). Recursive wildcard borrowed from: https://stackoverflow.com/a/12959694
REMOVE_UNDERSCORE				=	$(foreach v,$(1),$(if $(findstring /_,$(v)),,$(v)))
RAW_FIND_FILE					=	$(foreach cleanup,$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call RAW_FIND_FILE,$d/,$2)),$(cleanup))
UNIX_FIND_FILE					=	$(shell find $(1) -name '$(2)')
#DOS_FIND_FILE					=	$(call REMOVE_UNDERSCORE,$(shell dir /s /b $(subst /,\\,$(1))\\$(2)))  # buggy due to / vs \ differences
FIND_FILE						=	$(call REMOVE_UNDERSCORE,$(call RAW_FIND_FILE,$(1),$(2)))
# NOTE: Mike's standard build tree rule is to ignore any file/folder prefixed with an underscore

COL_OFF							=	\e[0m
COL_RED							=	\e[91m
COL_GREEN						=	\e[92m
COL_YELLOW						=	\e[93m
COL_BLUE						=	\e[94m
COL_PURPLE						=	\e[95m
COL_CYAN						=	\e[96m
COL_WHITE						=	\e[97m
