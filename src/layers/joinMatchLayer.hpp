#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class joinMatchLayer : public Popup<>, public FLAlertLayerProtocol {
    protected:
        bool setup();

        CCMenuItemSpriteExtra* joinButton;
        CCMenuItemSpriteExtra* leaveButton;
        ButtonSprite* joinSprite;
        ButtonSprite* joinSpriteDisabled;
        ButtonSprite* leaveSprite;
        ButtonSprite* leaveSpriteDisabled;

        FLAlertLayer* leaveMatchAlert;

        EventListener<Task<Result<>>> discordMessageListener;
        bool connecting;

        void onJoinBtnClicked(CCObject*);
        void onLeaveBtnClicked(CCObject*);

        void updateButtonsState();

        void leaveMatch();
        void joinMatch();

        void FLAlert_Clicked(FLAlertLayer* alert, bool confirm) override;

    public:
        static joinMatchLayer* create();

        void show();
};