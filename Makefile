# DETERMINE IF ON WINDOWS
ifeq ($(OS),Windows_NT)
	OS_WINDOWS := 1
else
# ASSUME USING LINUX WITH GCC IF NOT WINDOWS
	OS_WINDOWS := 0
endif

# RELEASE MAKEFILE -D _DEBUG -U _DEBUG
CXXFLAGS := -std=c++17 -Wall -D _DEBUG
ifdef RELEASE
	CXXFLAGS += -O3 -U _DEBUG
else
# FOR GDB
	CXXFLAGS += -g
endif

# EXECUTABLES AND MAIN FILES SPECIFICATION
MAIN_SRC_FILES := main.cpp model_converter_utility.cpp
EXECUTABLE_FILES := main model_converter_utility
OUTPUT_DIR := bin/core

TEST_MAIN_SRC_FILES := math_tests/math_tests.cpp
TEST_EXECUTABLE_FILES := math_tests
TEST_OUTPUT_DIR := bin/test

ifneq ($(words $(MAIN_SRC_FILES)),$(words $(EXECUTABLE_FILES)))
	$(error ERROR: Core require matching number of executables as files with main() function)
endif
ifneq ($(words $(TEST_MAIN_SRC_FILES)),$(words $(TEST_EXECUTABLE_FILES)))
	$(error ERROR: Tests require matching number of executables as files with main() function)
endif

# SRC CODE SECTION
SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*/*.cpp)
HEADER_FILES := $(wildcard $(SRC_DIR)/*.h) $(wildcard $(SRC_DIR)/*/*.h) $(wildcard $(SRC_DIR)/*/*/*.h) $(wildcard $(SRC_DIR)/*/*/*/*.h)

TEST_SRC_DIR := test
TEST_SRC_FILES := $(wildcard $(TEST_SRC_DIR)/*.cpp) $(wildcard $(TEST_SRC_DIR)/*/*.cpp) $(wildcard $(TEST_SRC_DIR)/*/*/*.cpp) $(wildcard $(TEST_SRC_DIR)/*/*/*/*.cpp)
TEST_HEADER_FILES := $(wildcard $(TEST_SRC_DIR)/*.h) $(wildcard $(TEST_SRC_DIR)/*/*.h) $(wildcard $(TEST_SRC_DIR)/*/*/*.h) $(wildcard $(TEST_SRC_DIR)/*/*/*/*.h)

# COPY GLSL SHADERS TO OUTPUT DIR FOR EXECUTABLES TO USE
SHADER_FILES := $(wildcard $(SRC_DIR)/*.glsl) $(wildcard $(SRC_DIR)/*/*.glsl) $(wildcard $(SRC_DIR)/*/*/*.glsl) $(wildcard $(SRC_DIR)/*/*/*/*.glsl)
SHADER_FILES_WITHOUT_DIR := $(filter %.glsl,$(subst /, ,$(SHADER_FILES)))
SHADER_DEPS := $(patsubst %.glsl,%,$(SHADER_FILES))

# INCLUDE SECTION
ifeq ($(OS_WINDOWS),1) # WINDOWS
	GLFW_INCLUDE_DIR :=
	GLAD_INCLUDE_DIR :=
	INCLUDE_DIR := $(SRC_DIR) $(TEST_SRC_DIR) include $(GLFW_INCLUDE_DIR) $(GLAD_INCLUDE_DIR)
	IFLAGS := $(patsubst %,-I%,$(INCLUDE_DIR))
else # LINUX
	GLFW_INCLUDE_DIR := /usr/local/include 
	GLAD_INCLUDE_DIR :=
	INCLUDE_DIR := $(SRC_DIR) $(TEST_SRC_DIR) include $(GLFW_INCLUDE_DIR) $(GLAD_INCLUDE_DIR)
	IFLAGS := $(patsubst %,-I%,$(INCLUDE_DIR))
endif

# STATICALLY LINKED LIBRARIES SECTION
ifeq ($(OS_WINDOWS),1) # WINDOWS
    GLFW_LIB_DIR := lib/windows
    GLFW_LIBS := glfw3 gdi32
    GLAD_LIB_DIR :=
    GLAD_LIBS :=
    OPENGL_LIBS := opengl32
else # LINUX
    GLFW_LIB_DIR := lib/unix usr/local/lib
    GLFW_LIBS := glfw3 pthread rt m dl X11
    GLAD_LIB_DIR :=
    GLAD_LIBS := dl
    OPENGL_LIBS := GL
endif


LIBS_DIR := lib $(GLFW_LIB_DIR) $(GLAD_LIB_DIR)
LIBS := $(GLFW_LIBS) $(GLAD_LIBS)

LFLAGS := $(patsubst %,-L%,$(LIBS_DIR)) $(patsubst %,-l%,$(LIBS))

# LIBRARY CODE THAT NEEDS TO BE COMPILED WITH PROJECT
GLFW_LIB_SRC_FILES :=
GLAD_LIB_SRC_FILES := glad.c
STB_IMAGE_LIB_SRC_FILES := stb_image.cpp
LIB_SRC_DIR := lib

LIB_SRC_FILES_PART := $(GLFW_LIB_SRC_FILES) $(GLAD_LIB_SRC_FILES) $(STB_IMAGE_LIB_SRC_FILES)
LIB_SRC_FILES := $(patsubst %,$(LIB_SRC_DIR)/%,$(LIB_SRC_FILES_PART))

# OBJ BUILDING SECTION
BUILD_DIR := build

OBJ_DIR := $(BUILD_DIR)/$(SRC_DIR)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LIB_OBJ_DIR := $(BUILD_DIR)/$(LIB_SRC_DIR)
LIB_OBJ_FILES := $(subst .c,.o,$(subst .cc,.o,$(subst .cpp,.o,$(patsubst $(LIB_SRC_DIR)/%,$(LIB_OBJ_DIR)/%,$(LIB_SRC_FILES)))))

TEST_OBJ_DIR := $(BUILD_DIR)/$(TEST_SRC_DIR)
TEST_OBJ_FILES := $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRC_FILES))

# RECIPIES
default_target: all

.PHONY : all
all : core test

.PHONY : core
core : $(EXECUTABLE_FILES) $(SHADER_DEPS)

.PHONY : test
test : $(TEST_EXECUTABLE_FILES)

ifeq ($(OS_WINDOWS),1) # WINDOWS
$(SHADER_DEPS) :
	$(eval SHADER_FILE_WITHOUT_DIR := $(lastword $(subst /, ,$@)).glsl)
	
	@if NOT exist $(subst /,\,$(OUTPUT_DIR)) mkdir $(subst /,\,$(OUTPUT_DIR))
	@copy $(subst /,\,$@.glsl) $(subst /,\,$(OUTPUT_DIR)/$(SHADER_FILE_WITHOUT_DIR))
else # LINUX
$(SHADER_DEPS) :
	$(eval SHADER_FILE_WITHOUT_DIR := $(lastword $(subst /, ,$@)).glsl)
	
	@mkdir -p $(subst \,/,$(OUTPUT_DIR))
	@cp $(subst \,/,$@.glsl) $(subst \,/,$(OUTPUT_DIR)/$(SHADER_FILE_WITHOUT_DIR))
endif

ifeq ($(OS_WINDOWS),1) # WINDOWS
$(EXECUTABLE_FILES) : $(OBJ_FILES) $(LIB_OBJ_FILES)
	$(eval MAIN_OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRC_FILES)))
	$(eval TARGET_MAIN_OBJ_FILE := $(filter %/$(lastword $(subst /, ,$@)).o,$(MAIN_OBJ_FILES)))
	$(eval OBJ_FILES_WITHOUT_OTHER_MAINS := $(filter-out $(filter-out $(TARGET_MAIN_OBJ_FILE),$(MAIN_OBJ_FILES)),$^))
#	$(info recipe for $(OUTPUT_DIR)/$@ with prerequisite $(OBJ_FILES_WITHOUT_OTHER_MAINS))
	$(eval EXECUTABLE_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(OUTPUT_DIR)/$@)),%,$(OUTPUT_DIR)/$@)))
	
	@if NOT exist $(EXECUTABLE_DIR_WITHOUT_FILENAME) mkdir $(EXECUTABLE_DIR_WITHOUT_FILENAME)
	
	g++ $(CXXFLAGS) $(IFLAGS) $(OBJ_FILES_WITHOUT_OTHER_MAINS) -o $(OUTPUT_DIR)/$@ $(LFLAGS)
else # LINUX
$(EXECUTABLE_FILES) : $(OBJ_FILES) $(LIB_OBJ_FILES)
	$(eval MAIN_OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRC_FILES)))
	$(eval TARGET_MAIN_OBJ_FILE := $(filter %/$(lastword $(subst /, ,$@)).o,$(MAIN_OBJ_FILES)))
	$(eval OBJ_FILES_WITHOUT_OTHER_MAINS := $(filter-out $(filter-out $(TARGET_MAIN_OBJ_FILE),$(MAIN_OBJ_FILES)),$^))
#	$(info recipe for $(OUTPUT_DIR)/$@ with prerequisite $(OBJ_FILES_WITHOUT_OTHER_MAINS))
	$(eval EXECUTABLE_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(OUTPUT_DIR)/$@)),%,$(OUTPUT_DIR)/$@)))
	
	@mkdir -p $(subst \,/,$(EXECUTABLE_DIR_WITHOUT_FILENAME))
	
	g++ $(CXXFLAGS) $(IFLAGS) $(OBJ_FILES_WITHOUT_OTHER_MAINS) -o $(OUTPUT_DIR)/$@ $(LFLAGS)
endif

ifeq ($(OS_WINDOWS),1) # WINDOWS
$(TEST_EXECUTABLE_FILES) : $(OBJ_FILES) $(LIB_OBJ_FILES) $(TEST_OBJ_FILES)
	$(eval MAIN_OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRC_FILES)) $(patsubst %.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_MAIN_SRC_FILES)))
	$(eval TARGET_MAIN_OBJ_FILE := $(filter %/$(lastword $(subst /, ,$@)).o,$(MAIN_OBJ_FILES)))
	$(eval OBJ_FILES_WITHOUT_OTHER_MAINS := $(filter-out $(filter-out $(TARGET_MAIN_OBJ_FILE),$(MAIN_OBJ_FILES)),$^))
#	$(info recipe for $(TEST_OUTPUT_DIR)/$@ with prerequisite $(OBJ_FILES_WITHOUT_OTHER_MAINS))
	$(eval EXECUTABLE_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(TEST_OUTPUT_DIR)/$@)),%,$(TEST_OUTPUT_DIR)/$@)))
	
	@if NOT exist $(EXECUTABLE_DIR_WITHOUT_FILENAME) mkdir $(EXECUTABLE_DIR_WITHOUT_FILENAME)
	
	g++ $(CXXFLAGS) $(IFLAGS) $(OBJ_FILES_WITHOUT_OTHER_MAINS) -o $(TEST_OUTPUT_DIR)/$@ $(LFLAGS)
else # LINUX
$(TEST_EXECUTABLE_FILES) : $(OBJ_FILES) $(LIB_OBJ_FILES) $(TEST_OBJ_FILES)
	$(eval MAIN_OBJ_FILES := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRC_FILES)) $(patsubst %.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_MAIN_SRC_FILES)))
	$(eval TARGET_MAIN_OBJ_FILE := $(filter %/$(lastword $(subst /, ,$@)).o,$(MAIN_OBJ_FILES)))
	$(eval OBJ_FILES_WITHOUT_OTHER_MAINS := $(filter-out $(filter-out $(TARGET_MAIN_OBJ_FILE),$(MAIN_OBJ_FILES)),$^))
#	$(info recipe for $(TEST_OUTPUT_DIR)/$@ with prerequisite $(OBJ_FILES_WITHOUT_OTHER_MAINS))
	$(eval EXECUTABLE_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(TEST_OUTPUT_DIR)/$@)),%,$(TEST_OUTPUT_DIR)/$@)))
	
	@mkdir -p $(subst \,/,$(EXECUTABLE_DIR_WITHOUT_FILENAME))
	
	g++ $(CXXFLAGS) $(IFLAGS) $(OBJ_FILES_WITHOUT_OTHER_MAINS) -o $(TEST_OUTPUT_DIR)/$@ $(LFLAGS)
endif

ifeq ($(OS_WINDOWS),1) # WINDOWS
$(OBJ_FILES) : $(BUILD_DIR)/%.o : %.cpp $(HEADER_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ_SRC := $(filter-out %.h,$(filter $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%,$@).%,$^)))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	
	@if NOT exist $(OBJ_DIR_WITHOUT_FILENAME) mkdir $(OBJ_DIR_WITHOUT_FILENAME)
	
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ_SRC) -o $@ $(LFLAGS)
else # LINUX
$(OBJ_FILES) : $(BUILD_DIR)/%.o : %.cpp $(HEADER_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ_SRC := $(filter-out %.h,$(filter $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%,$@).%,$^)))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	
	@mkdir -p $(subst \,/,$(OBJ_DIR_WITHOUT_FILENAME))
	
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ_SRC) -o $@ $(LFLAGS)
endif

ifeq ($(OS_WINDOWS),1) # WINDOWS
$(LIB_OBJ_FILES) : $(LIB_SRC_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(LIB_OBJ_DIR)/%.o,$(LIB_SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ_SRC := $(filter-out %.h,$(filter $(patsubst $(LIB_OBJ_DIR)/%.o,$(LIB_SRC_DIR)/%,$@).%,$^)))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	
	@if NOT exist $(OBJ_DIR_WITHOUT_FILENAME) mkdir $(OBJ_DIR_WITHOUT_FILENAME)
	
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ_SRC) -o $@ $(LFLAGS)
else # LINUX
$(LIB_OBJ_FILES) : $(LIB_SRC_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(LIB_OBJ_DIR)/%.o,$(LIB_SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ_SRC := $(filter-out %.h,$(filter $(patsubst $(LIB_OBJ_DIR)/%.o,$(LIB_SRC_DIR)/%,$@).%,$^)))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	
	@mkdir -p $(subst \,/,$(OBJ_DIR_WITHOUT_FILENAME))
	
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ_SRC) -o $@ $(LFLAGS)
endif

ifeq ($(OS_WINDOWS),1) # WINDOWS
$(TEST_OBJ_FILES) : $(BUILD_DIR)/%.o : %.cpp $(TEST_HEADER_FILES) $(HEADER_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(TEST_OBJ_DIR)/%.o,$(TEST_SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ_SRC := $(filter-out %.h,$(filter $(patsubst $(TEST_OBJ_DIR)/%.o,$(TEST_SRC_DIR)/%,$@).%,$^)))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	
	@if NOT exist $(OBJ_DIR_WITHOUT_FILENAME) mkdir $(OBJ_DIR_WITHOUT_FILENAME)
	
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ_SRC) -o $@ $(LFLAGS)
else # LINUX
$(TEST_OBJ_FILES) : $(BUILD_DIR)/%.o : %.cpp $(TEST_HEADER_FILES) $(HEADER_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(TEST_OBJ_DIR)/%.o,$(TEST_SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ_SRC := $(filter-out %.h,$(filter $(patsubst $(TEST_OBJ_DIR)/%.o,$(TEST_SRC_DIR)/%,$@).%,$^)))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	
	@mkdir -p $(subst \,/,$(OBJ_DIR_WITHOUT_FILENAME))
	
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ_SRC) -o $@ $(LFLAGS)
endif

# Regular clean only removes compiled source obj files and excecutables and not compiled library obj files

ifeq ($(OS_WINDOWS),1) # WINDOWS
    EXCECUTABLE_FILES_TO_REMOVE := $(patsubst %,"$(OUTPUT_DIR)/%.exe",$(EXECUTABLE_FILES))
    TEST_EXCECUTABLE_FILES_TO_REMOVE := $(patsubst %,"$(TEST_OUTPUT_DIR)/%.exe",$(TEST_EXECUTABLE_FILES))
    SHADER_FILES_TO_REMOVE := $(patsubst %,"$(OUTPUT_DIR)/%",$(SHADER_FILES_WITHOUT_DIR))
    OBJ_FILES_TO_REMOVE := $(patsubst %,"%",$(OBJ_FILES))
    TEST_OBJ_FILES_TO_REMOVE := $(patsubst %,"%",$(TEST_OBJ_FILES))
    FILES_TO_REMOVE := $(subst /,\,$(EXCECUTABLE_FILES_TO_REMOVE) $(TEST_EXCECUTABLE_FILES_TO_REMOVE) $(SHADER_FILES_TO_REMOVE) $(OBJ_FILES_TO_REMOVE) $(TEST_OBJ_FILES_TO_REMOVE))
.PHONY : clean
clean :
#	@for %%f in ($(FILES_TO_REMOVE)) do (if exist %%~f (echo found file to be deleted: "%%~f"))
	@for %%f in ($(FILES_TO_REMOVE)) do (if exist %%~f (del /F %%f))
else # LINUX
    EXCECUTABLE_FILES_TO_REMOVE := $(patsubst %,"$(OUTPUT_DIR)/%",$(EXECUTABLE_FILES))
    TEST_EXCECUTABLE_FILES_TO_REMOVE := $(patsubst %,"$(TEST_OUTPUT_DIR)/%",$(TEST_EXECUTABLE_FILES))
    SHADER_FILES_TO_REMOVE := $(patsubst %,"$(OUTPUT_DIR)/%",$(SHADER_FILES_WITHOUT_DIR))
    OBJ_FILES_TO_REMOVE := $(patsubst %,"%",$(OBJ_FILES))
    TEST_OBJ_FILES_TO_REMOVE := $(patsubst %,"%",$(TEST_OBJ_FILES))
    FILES_TO_REMOVE := $(subst \,/,$(EXCECUTABLE_FILES_TO_REMOVE) $(TEST_EXCECUTABLE_FILES_TO_REMOVE) $(SHADER_FILES_TO_REMOVE) $(OBJ_FILES_TO_REMOVE) $(TEST_OBJ_FILES_TO_REMOVE))
.PHONY : clean
clean :
	@rm -f $(FILES_TO_REMOVE)
endif

# Super clean removes everything in build and all executables
ifeq ($(OS_WINDOWS),1) # WINDOWS
.PHONY : super_clean
super_clean : clean
	$(eval EXECUTABLES_IN_BIN := $(wildcard $(OUTPUT_DIR)/*.exe) $(wildcard $(OUTPUT_DIR)/*/*.exe) $(wildcard $(OUTPUT_DIR)/*/*/*.exe) $(wildcard $(OUTPUT_DIR)/*/*/*/*.exe))
	$(eval TEST_EXECUTABLES_IN_BIN := $(wildcard $(TEST_OUTPUT_DIR)/*.exe) $(wildcard $(TEST_OUTPUT_DIR)/*/*.exe) $(wildcard $(TEST_OUTPUT_DIR)/*/*/*.exe) $(wildcard $(TEST_OUTPUT_DIR)/*/*/*/*.exe))
	$(eval EXECUTABLES_TO_REMOVE := $(subst /,\,$(EXECUTABLES_IN_BIN) $(TEST_EXECUTABLES_IN_BIN)))
	
	@for %%f in ($(EXECUTABLES_TO_REMOVE)) do (del /F %%f)
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
else # LINUX
.PHONY : super_clean
super_clean : clean
# Can't use wildcard to get all executables on Linux system. Possible Solution?
	@if test -d $(BUILD_DIR); then rm -r -f $(BUILD_DIR); fi
endif
