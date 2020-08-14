TARGET_BIN			:=	$(TARGET)$(TARGET_EXT)


BUILD_FILES			:=	$(call FIND_FILE,src,build.mk)
-include $(BUILD_FILES)

# Populated by build.mk files
#SRC_DIRS			+=	
#INCLUDE_DIRS		+=	


.C_FILES			:=	$(foreach v,$(SRC_DIRS),$(call FIND_FILE,$(v),*.c))
.CPP_FILES			:=	$(foreach v,$(SRC_DIRS),$(call FIND_FILE,$(v),*.cpp))


SRC_FILES			+=	$(.C_FILES) $(.CPP_FILES)

COMMON_FLAGS		+=	$(addprefix -I,$(INCLUDE_DIRS)) $(addprefix -D,$(DEFINE))
LD_FLAGS			+=	
						

$(TARGET_BIN): $(SRC_FILES)
	$(CXX) $(OTHER_O_FILES) -x c $(.C_FILES) -x c++ $(.CPP_FILES) $(COMMON_FLAGS) $(.C_FLAGS) -o $(TARGET_BIN) $(LD_FLAGS)

error:
	@echo "ERROR: I don't know what platform to build!"

info:
	@echo "TARGET: $(TARGET)"
	@echo "BUILD_FILES: $(BUILD_FILES)"
	@echo "SRC_FILES: $(SRC_FILES)"
	
run: $(TARGET_BIN)
	./$(TARGET_BIN)

clean:
	rm $(TARGET_BIN)

.PHONY: error info run clean
