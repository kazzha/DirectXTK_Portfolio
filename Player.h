#pragma once

namespace DX
{
    class Player :
        public AnimatedActor
    {
    public:
        Player();
        ~Player();
        virtual void Update(double delta, WPARAM wParam);
        void Move(float X, float Y);
         

    public:
        std::vector<Actor*>* StageInfo;
        int PlayerIndex;
        Goal* hasGoal;
    };
}

