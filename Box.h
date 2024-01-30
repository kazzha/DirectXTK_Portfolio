#pragma once

namespace DX
{
    class Box :
        public Actor
    {

        void Move(float X, float Y);
        bool GetHasGoal() { return hasGoal; }
        void SetHasGOal(bool currentGoal) { hasGoal = currentGoal; }

    private:
        bool hasGoal;
    };
}

