#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTTeamCell : public CCNode {
    protected:
        bool init(Team _team, CCSize size, bool s);
    public:
        static GDWTTeamCell* create(Team _team, CCSize size, bool s = false);

        Team team;

        void onMoreClicked(CCObject*);
};