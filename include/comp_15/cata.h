#pragma once
#include "main.h"
#include "common_code/subsystem_parent.h"

class CataClass : public SubsystemParent
{
    private:

    public:
        CataClass(const CataClass& other) = delete;
        ~CataClass();
};