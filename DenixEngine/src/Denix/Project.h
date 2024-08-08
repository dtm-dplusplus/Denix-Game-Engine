//
// Created by Denis on 06/08/2024.
//

#ifndef PROJECT_H
#define PROJECT_H

#include "Engine.h"

using namespace Denix;

class Project: public Engine
{
public:

    Project()
    {
        m_ProjectName = "Playground";
    }

    ~Project() override
    {
    }

};
#endif //PROJECT_H
