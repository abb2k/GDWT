#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"
#include <Geode/utils/web.hpp>
#include "../layers/GDWTMatchLevelCell.hpp"

using namespace geode::prelude;

class GDWTMatchLayer : public Popup<const Match&>, public LevelManagerDelegate {
    protected:
        bool setup(const Match& _matchs) override;

        void onClose(cocos2d::CCObject*);
    public:
        static GDWTMatchLayer* create(const Match& _match);

        void show() override;

        void update(float delta);

        Match match;

        void ytClicked(CCObject*);
        void twitchClicked(CCObject*);

        ScrollLayer* hostsScrollLayer;
        ScrollLayer* coHostsScrollLayer;
        ScrollLayer* levelsListLayer;
        int scrollSwitch;

        void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1) override;
        void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2) override;

        void loadLevelsFailed(char const* p0) override;
        void loadLevelsFailed(char const* p0, int p1) override;

        std::vector<GDWTMatchLevelCell*> levelCells;

        EventListener<scoreCalcTask> overallScoresListener;
};