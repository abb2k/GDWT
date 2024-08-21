#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTMatchCell : public CCNode {
    protected:
        bool init(Match _match, CCSize size, bool s);
    public:
        static GDWTMatchCell* create(Match _match, CCSize size, bool s = false);

        Match match;

        void onLiveClicked(CCObject*);
        void onTeamClicked(CCObject*);
        void onMoreClicked(CCObject*);
};