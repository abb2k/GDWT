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

        void MoveOptionBy(int amount = 1);
        void MoveOptionTo(int optionIndex);
        void MoveOptionTo(CCNode* const option);

        void SetAnimationSpeed(float speed);
        void SetOffset(const CCPoint& offset);

    private:
        void MoveMeTo(CCNode* const option);

        void startIdleAnim();

        int currentOption;

        float animSpeed = 1;

        std::vector<CCNode*> options{};

        CCPoint offset;

        float topOrigen;
        float bottomOrigen;

        CCScale9Sprite* bottomLeft;
        CCScale9Sprite* bottomRight;
        CCScale9Sprite* topLeft;
        CCScale9Sprite* topRight;
};