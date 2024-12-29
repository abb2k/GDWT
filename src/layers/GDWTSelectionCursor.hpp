#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GDWTSelectionCursor : public CCNode {
    protected:
        bool init();
    public:
        static GDWTSelectionCursor* create();

        void addOption(CCNode* const option);
        void removeOption(CCNode* const option);
        void removeOption(const int optionIndex);
        void eraseAllOptions();

        void moveOptionBy(int amount = 1);
        void moveOptionTo(int optionIndex);
        void moveOptionTo(CCNode* const optionIndex);

        void setAnimationSpeed(float speed);
        void setOffset(const CCPoint& offset);
        void setWrapOffset(float offset);
        void setIdleMoveDistance(float dictance);

        void setCallback(const std::function<void(CCNode* const)>& callback);

        void realign();

    private:
        void MoveMeTo(const int optionIndex, bool realign = false);

        void startIdleAnim();

        int currentOption = -1;

        float animSpeed = 1;

        std::vector<CCNode*> options{};

        CCPoint offset;
        float wrapOffset;
        float idleMoveDist = 5;

        float topOrigen;
        float bottomOrigen;

        CCScale9Sprite* bottomLeft;
        CCScale9Sprite* bottomRight;
        CCScale9Sprite* topLeft;
        CCScale9Sprite* topRight;

        std::function<void(CCNode* const)> callback = NULL;
};