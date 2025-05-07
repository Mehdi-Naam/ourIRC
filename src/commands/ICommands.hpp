#pragma once
#include <string>

class ICommands
{
public:
    virtual ~ICommands() {}
    virtual void execute() = 0;
};