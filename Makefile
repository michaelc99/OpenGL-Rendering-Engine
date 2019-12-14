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

GLFW_INCLUDE_DIR :=
GLAD_INCLUDE_DIR :=
INCLUDE_DIR := include $(GLFW_INCLUDE_DIR) $(GLAD_INCLUDE_DIR)
IFLAGS := $(patsubst %,-I%,$(INCLUDE_DIR))

# STATICALLY LINKED LIBRARIES
GLFW_LIB_DIR :=
GLFW_LIBS := glfw3 gdi32
GLAD_LIB_DIR :=
GLAD_LIBS :=

LIBS_DIR := lib $(GLFW_LIB_DIR) $(GLAD_LIB_DIR)
LIBS := $(GLFW_LIBS) $(GLAD_LIBS) opengl32

LFLAGS := $(patsubst %,-L%,$(LIBS_DIR)) $(patsubst %,-l%,$(LIBS))

SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)

# ?????????????????????
GLAD_SRC_FILES := $(SRC_DIR)/glad.c

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

#$(info $$SRC_FILES is [${SRC_FILES}])
#$(info $$OBJ_FILES is [${OBJ_FILES}])

default_target: all

all : $(EXECUTABLE_FILE)

$(EXECUTABLE_FILE) : $(OBJ_FILES)
	g++ $(CXXFLAGS) $(IFLAGS) $^ -o $(OUTPUT_DIR)/$(EXECUTABLE_FILE) $(LFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
#	$(info compiling $@) 
	g++ -c $(CXXFLAGS) $(IFLAGS) $< -o $@ $(LFLAGS)

clean :
#   Need to do something to make this easier on Windows
	$(info windows path is [$(subst /,\,$(OBJ_FILES))])
	del /F $(subst /,\,$(OUTPUT_DIR)\$(EXECUTABLE_FILE).exe)
	del /F $(subst /,\,$(OBJ_FILES))
#   -rm -f $(OUTPUT_DIR)/$(EXECUTABLE_FILE)
#   -rm -f $(OBJ_FILES)
