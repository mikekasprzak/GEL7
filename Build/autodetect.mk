ifndef $(TARGET)
# MSYS2
ifneq ($(MSYSTEM),)
TARGET				:=	AUTO-$(MSYSTEM)
# Other Unix
else ifneq ($(SHELL),)
TARGET				:=	AUTO-NIX-$(HOSTTYPE)
endif # AUTO
endif # $(TARGET)