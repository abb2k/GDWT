#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTTeamCell : public CCNode {
    protected:
        bool init(const Team& _team, CCSize size, bool s);
    public:
        static GDWTTeamCell* create(const Team& _team, CCSize size, bool s = false);

        Team team;

        void onMoreClicked(CCObject*);
};