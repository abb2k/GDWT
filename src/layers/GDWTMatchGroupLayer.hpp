#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTMatchGroupLayer : public Popup<MatchGroup> {
    protected:
        bool setup(MatchGroup _group);
    public:
        static GDWTMatchGroupLayer* create(MatchGroup _group);

        void show();

        MatchGroup group;

        EventListener<MatchesTask> l;

        EventListener<Task<std::vector<std::vector<std::tuple<std::string, int, int>> *>>> scoresL;
};