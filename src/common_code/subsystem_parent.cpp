#include "common_code/subsystem_parent.h"

SubsystemParent::SubsystemParent(std::string subsystem_name): kSubsystemName(subsystem_name) {}

std::string SubsystemParent::get_subsystem_name() const {
    return kSubsystemName;
}