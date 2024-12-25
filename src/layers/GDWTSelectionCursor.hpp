#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GDWTSelectionCursor : public CCNode {
    protected:
        bool init();
    public:
        static GDWTSelectionCursor* create();

        void AddOption(CCNode* const option);
        void RemoveOption(CCNode* const option);
        void RemoveOption(const int optionIndex);
        void EraseAllOptions();

        void MoveOption(int amount = 1);

        void SetAnimationSpeed(float speed);

    private:
        void MoveMeTo(CCNode* const option);

        int currentOption;

        float animSpeed = 1;

        std::vector<CCNode*> options{};
};