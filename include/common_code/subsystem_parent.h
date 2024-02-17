#pragma once
#include "main.h"

// want (this to be) abstract base class

class SubsystemParent {
    public:
        // abstract classes can have a constructor
        SubsystemParent(std::string subsystem_name);
        virtual ~SubsystemParent();

        /**
         * Returns the subsystem to a safe state and stops all motors.
         * 
         * This is a pure virtual function which means a few things:
         * 
         *  1. We can't make an object of type SubsystemParent because SubsystemParent is now considered
         *      an abstract (base) class
         *  2. All child classes must have their own implementation/definition of this method because
         *      there is no parent version to use instead.
        */
        virtual void stop() = 0;

        // Not virtual so not overriden by child classes, which is ok because it shouldn't need to be.
        std::string get_subsystem_name() const;

    protected:
        const std::string kSubsystemName;
};