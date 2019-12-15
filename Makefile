# RELEASE MAKEFILE -D _DEBUG -U _DEBUG
CXXFLAGS := -std=c++17 -Wall -D _DEBUG
ifdef GDB
	CXXFLAGS += -g
endif
ifdef RELEASE
	CXXFLAGS += -U _DEBUG
endif

EXECUTABLE_FILE := main
OUTPUT_DIR := bin

# INCLUDE SECTION
GLFW_INCLUDE_DIR :=
GLAD_INCLUDE_DIR :=
INCLUDE_DIR := src include $(GLFW_INCLUDE_DIR) $(GLAD_INCLUDE_DIR)
IFLAGS := $(patsubst %,-I%,$(INCLUDE_DIR))

# STATICALLY LINKED LIBRARIE SECTION
GLFW_LIB_DIR :=
GLFW_LIBS := glfw3 gdi32

GLAD_LIB_DIR :=
GLAD_LIBS :=

LIBS_DIR := lib $(GLFW_LIB_DIR) $(GLAD_LIB_DIR)
LIBS := $(GLFW_LIBS) $(GLAD_LIBS) opengl32

LFLAGS := $(patsubst %,-L%,$(LIBS_DIR)) $(patsubst %,-l%,$(LIBS))

# LIBRARY CODE THAT NEEDS TO BE COMPILED WITH PROJECT
GLFW_LIB_SRC_FILES :=
GLAD_LIB_SRC_FILES := glad.c
LIB_SRC_DIR := lib

LIB_SRC_FILES_PART := $(GLFW_LIB_SRC_FILES) $(GLAD_LIB_SRC_FILES)
LIB_SRC_FILES := $(patsubst %,$(LIB_SRC_DIR)/%,$(LIB_SRC_FILES_PART))

# SRC CODE SECTION
SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)

# OBJ BUILDING SECTION
BUILD_DIR := build

OBJ_DIR := $(BUILD_DIR)/$(SRC_DIR)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LIB_OBJ_DIR := $(BUILD_DIR)/$(LIB_SRC_DIR)
LIB_OBJ_FILES := $(subst .c,.o,$(subst .cc,.o,$(subst .cpp,.o,$(patsubst $(LIB_SRC_DIR)/%,$(LIB_OBJ_DIR)/%,$(LIB_SRC_FILES)))))

# RECIPIES
default_target: all

.PHONY : all
all : $(EXECUTABLE_FILE)

$(EXECUTABLE_FILE) : $(OBJ_FILES) $(LIB_OBJ_FILES)
#	$(info recipe for $@ with prerequisite $^)
	@if NOT exist $(OUTPUT_DIR) mkdir $(OUTPUT_DIR)
	g++ $(CXXFLAGS) $(IFLAGS) $^ -o $(OUTPUT_DIR)/$@ $(LFLAGS)

$(OBJ_FILES) : $(SRC_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ := $(filter $(patsubst $(OBJ_DIR)/%.o,$(SRC_DIR)/%,$@).%,$^))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	@if NOT exist $(OBJ_DIR_WITHOUT_FILENAME) mkdir $(OBJ_DIR_WITHOUT_FILENAME)
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ) -o $@ $(LFLAGS)

$(LIB_OBJ_FILES) : $(LIB_SRC_FILES)
#	$(info recipe for $@ with prerequisite $(filter $(patsubst $(LIB_OBJ_DIR)/%.o,$(LIB_SRC_DIR)/%,$@).%,$^))
#	$(info directory without filename: $(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@)))
	$(eval PRE_REQ := $(filter $(patsubst $(LIB_OBJ_DIR)/%.o,$(LIB_SRC_DIR)/%,$@).%,$^))
	$(eval OBJ_DIR_WITHOUT_FILENAME := $(subst /,\,$(patsubst %/$(lastword $(subst /, ,$(patsubst %.o,%,$@))),%,$(patsubst %.o,%,$@))))
	@if NOT exist $(OBJ_DIR_WITHOUT_FILENAME) mkdir $(OBJ_DIR_WITHOUT_FILENAME)
	g++ -c $(CXXFLAGS) $(IFLAGS) $(PRE_REQ) -o $@ $(LFLAGS)

# Regular clean only removes src code obj files and not lib src obj files
FILES_TO_REMOVE := $(subst /,\,$(patsubst %,"%",$(OBJ_FILES)))
#FILES_TO_REMOVE := $(subst /,\,$(patsubst %,"%",$(OBJ_FILES) $(LIB_OBJ_FILES)))
.PHONY : clean
clean :
#   Need to do something to make this easier on Windows
	@if exist $(subst /,\,$(OUTPUT_DIR)\$(EXECUTABLE_FILE).exe) del /F $(subst /,\,$(OUTPUT_DIR)\$(EXECUTABLE_FILE).exe)
#	@for %%f in ($(FILES_TO_REMOVE)) do (if exist %%~f (echo found file to be deleted: "%%~f"))
	@for %%f in ($(FILES_TO_REMOVE)) do (if exist %%~f (del /F %%f))

.PHONY : super_clean
super_clean :
	@if exist $(subst /,\,$(OUTPUT_DIR)\$(EXECUTABLE_FILE).exe) del /F $(subst /,\,$(OUTPUT_DIR)\$(EXECUTABLE_FILE).exe)
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
# LINUX (NEEDS WORK)
#clean :
#   -rm -f $(OUTPUT_DIR)/$(EXECUTABLE_FILE)
#   -rm -f $(OBJ_FILES)
