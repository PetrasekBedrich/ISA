#include <string> 
#include "RuntimeConfiguration.h"
#pragma once
class RuntimeConfiguration
{
    public:
       SortByENUM SortBy;
       char* Interface;
       int NumberOfCommunications = 10;
};
