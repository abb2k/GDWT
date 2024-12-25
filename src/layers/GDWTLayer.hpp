#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"
#include "../layers/GDWTSelectionCursor.hpp"

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

        GDWTSelectionCursor* myCursor;
        void test(CCObject* s);

        //

        void openJoinMatchMenu(CCObject*);
};