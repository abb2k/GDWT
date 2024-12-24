#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTLayer : public CCLayer {
    protected:
        bool init();
    public:
        static GDWTLayer* create();

        void open();

        void keyBackClicked();

        void OnBackButton(CCObject*);

        ScrollLayer* mainScroll;

        //

        void openJoinMatchMenu(CCObject*);
};