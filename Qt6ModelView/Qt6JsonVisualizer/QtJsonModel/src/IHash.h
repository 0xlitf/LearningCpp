#pragma once

#include "string"
#include "qstring.h"

class IHash
{
public:
    virtual std::string GetHash(QString data) = 0;
};
