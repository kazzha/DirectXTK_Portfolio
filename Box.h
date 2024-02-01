#pragma once

namespace DX
{
    class Box :
        public Actor
    {

    public:
        Box();
        ~Box();

        bool MoveForward();
        bool MoveBackward();
        bool MoveLeft();
        bool MoveRight();

    public:
        std::string color;
        Goal* currentGoal;
        Goal* alreadyGoal;
        int boxIndex;
        std::vector<Actor*>* StageInfo;
        int* remainBoxCount;
    };
}

