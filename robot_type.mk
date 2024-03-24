###############################################################
###                Robot Type Configuration                 ###
###############################################################
# Author; Anissh G

# This variable is the only thing to modify in this file!!!
# Comp 15in Robot = 0
# Comp 18in Robot = 1
# Skills 15in Robot = 2
# Skills 18in Robot = 3
# NOTE: Once you change the robot type perform a clean build!
ROBOT_MODE := 1


###############################################################
####         Do not edit anything below this line          ####

# Location of each robot's working code
DIR_COMP15 := $(SRCDIR)/comp_15
DIR_COMP18 := $(SRCDIR)/comp_18
DIR_SKILLS15 := $(SRCDIR)/skills_thrower
DIR_SKILLS18 := $(SRCDIR)/skills_catcher

BIN_COMP15 := $(BINDIR)/comp_15
BIN_COMP18 := $(BINDIR)/comp_18
BIN_SKILLS15 := $(BINDIR)/skills_thrower
BIN_SKILLS18 := $(BINDIR)/skills_catcher

# Check if the directory names are same as the binary names
ifneq ($(findstring /comp_15,$(DIR_COMP15)),$(findstring /comp_15,$(BIN_COMP15)))
    $(error Directory names are not same for DIR_COMP15 and BIN_COMP15)
else ifneq ($(findstring /comp_18,$(DIR_COMP18)),$(findstring /comp_18,$(BIN_COMP18)))
    $(error Directory names are not same for DIR_COMP18 and BIN_COMP18)
else ifneq ($(findstring /skills_thrower,$(DIR_SKILLS15)),$(findstring /skills_thrower,$(BIN_SKILLS15)))
    $(error Directory names are not same for DIR_SKILLS15 and BIN_SKILLS15)
else ifneq ($(findstring /skills_catcher,$(DIR_SKILLS18)),$(findstring /skills_catcher,$(BIN_SKILLS18)))
    $(error Directory names are not same for DIR_SKILLS18 and BIN_SKILLS18)
endif

# Default all dir as excluded instead of all included
EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP18) $(DIR_SKILLS15) $(DIR_SKILLS18)
EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP18) $(BIN_SKILLS15) $(BIN_SKILLS18)

# Using C++ Flags to allow program to know what robot is being 
#  compiled and run via preprocessor macos. Can be used for other
#  program specific operations.
ROBOTFLAGS := -DNO_ROBOT_SELECTED # This mode, forces build error

ifeq ($(ROBOT_MODE),0)
    ROBOTFLAGS := -D_COMP_15
    EXCLUDED_DIR := $(DIR_COMP18) $(DIR_SKILLS15) $(DIR_SKILLS18)
	EXCLUDED_BIN := $(BIN_COMP18) $(BIN_SKILLS15) $(BIN_SKILLS18)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'comp_15')
else ifeq ($(ROBOT_MODE),1)
    ROBOTFLAGS := -D_COMP_18
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_SKILLS15) $(DIR_SKILLS18)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_SKILLS15) $(BIN_SKILLS18)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'comp_18')
else ifeq ($(ROBOT_MODE),2)
    ROBOTFLAGS := -D_SKILLS_THROWER
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP18) $(DIR_SKILLS18)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP18) $(BIN_SKILLS18)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'skills_thrower')
else ifeq ($(ROBOT_MODE),3)
    ROBOTFLAGS := -D_SKILLS_CATCHER
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP18) $(DIR_SKILLS15)
    EXCLUDED_BIN := $(BIN_COMP15) $(BIN_COMP18) $(BIN_SKILLS15)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'skills_catcher')
else
    $(error Robot_Mode is not specified! Double check ./robot_type.mk)
endif

$(info Excluding $(EXCLUDED_DIR) directories)