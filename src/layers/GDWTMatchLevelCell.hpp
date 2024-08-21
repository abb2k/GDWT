#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class GDWTMatchLevelCell : public CCNode {
    protected:
        bool init(Level _level, CCSize size, bool s);
    public:
        static GDWTMatchLevelCell* create(Level _level, CCSize size, bool s = false);

        Level level;

        GJGameLevel* lvl = nullptr;

        LoadingCircle* levelLoadingC;
        CCLayer* loadingCLayer;

        CCClippingNode* c;

        CCLayerColor* bg;

        InputNode* levelName;

        void loadLevel(GJGameLevel* _lvl);

        void onLevelClicked(CCObject*);

        std::vector<std::pair<std::string, int>> scores;
        int highestScore;

        EventListener<web::WebTask> downloadListener;

        Ref<CCImage> image;

        void setImage(CCImage* image);
};