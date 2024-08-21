#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class flagDisplay : public CCNode {
    protected:
        bool init(int accountID, std::string countryCode);
    public:
        static flagDisplay* create(int accountID);

        static flagDisplay* create(std::string countryCode);

        CCMenuItemSpriteExtra* flagButton;
        LoadingCircle* IconLoadingC;
        CCLayer* loadingCLayer;

        std::string cCode;

        Team myTeam;

        int accID;

        void flagClicked(CCObject*);

        CCSize getContentSize();

        float getContentWidth();
        float getContentHeight();

        CCSize getScaledContentSize();

        float getScaledContentWidth();
        float getScaledContentHeight();

        EventListener<TeamsTask> listener;

        void onTeamsInfo(TeamsTask::Event* event);
};