#pragma once
#include "Actor.h"

namespace DX
{
    class StageNotify :
        public Actor
    {
    public:
        StageNotify();

        ~StageNotify();

        virtual void Update(double delta) override;


    
        double* deltaTime;
    };

}

