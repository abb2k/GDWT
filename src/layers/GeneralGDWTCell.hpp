#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GeneralGDWTCell : public CCMenuItemSpriteExtra {
    protected:
        bool init(const std::string& title, int enterIndex);
    public:
        static GeneralGDWTCell* create(const std::string& title, int enterIndex = 0);

        CCSize size;

        CCNode* mainContainer;

        CCNode* content;

        geode::SimpleTextArea* titleLabel;
        CCScale9Sprite* BG;

        void setCentralContent(const Match& match);

        void setCentralContent(const Team& team);

        void setCentralContent(const MatchGroup& MG);

        void clearContent(bool nothingMessage = false);

        void playEnterTransition(float baseDuration, int enterIndex = 0);

        void setTitle(const std::string& title);
};