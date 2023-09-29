###############################################################
###                Robot Type Configuration                 ###
###############################################################

# In an effort to organize the robotics code repository into a single
# project folder, this file will be used to specify which robot program
# will compile and run allowing all four robots to share the same common
# algorithms while, remaining in one project.

# This variable is the only thing to modify in this file!!!
# Comp 15in Robot = 0
# Comp 18in Robot = 1
# Skills 15in Robot = 2
# Skills 18in Robot = 3
# NOTE: Once you change the robot type perform a clean build!
ROBOT_MODE := 2


###############################################################
####         Do not edit anything below this line          ####

# Location of each robot's working code
DIR_COMP15 := $(SRCDIR)/comp_15
DIR_COMP18 := $(SRCDIR)/comp_18
DIR_SKILLS15 := $(SRCDIR)/skills_15
DIR_SKILLS18 := $(SRCDIR)/skills_18

# Default all dir as excluded instead of all included
EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP18) $(DIR_SKILLS15) $(DIR_SKILLS18)

# Using C++ Flags to allow program to know what robot is being 
#  compiled and run via preprocessor macos. Can be used for other
#  program specific operations.
ROBOTFLAGS := -DNO_ROBOT_SELECTED # This mode, forces build error

ifeq ($(ROBOT_MODE),0)
    ROBOTFLAGS := -D_COMP_15
    EXCLUDED_DIR := $(DIR_COMP18) $(DIR_SKILLS15) $(DIR_SKILLS18)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'comp_15')
else ifeq ($(ROBOT_MODE),1)
    ROBOTFLAGS := -D_COMP_18
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_SKILLS15) $(DIR_SKILLS18)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'comp_18')
else ifeq ($(ROBOT_MODE),2)
    ROBOTFLAGS := -D_SKILLS_15
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP18) $(DIR_SKILLS18)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'skills_15')
else ifeq ($(ROBOT_MODE),3)
    ROBOTFLAGS := -D_SKILLS_18
    EXCLUDED_DIR := $(DIR_COMP15) $(DIR_COMP18) $(DIR_SKILLS15)
    $(info Robot_Mode is set to $(ROBOT_MODE) aka 'skills_18')
else
    $(error Robot_Mode is not specified! Double check ./robot_type.mk)
endif

$(info Excluding $(EXCLUDED_DIR) directories)