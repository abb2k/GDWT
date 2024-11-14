#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"

using namespace geode::prelude;

class GDWTMatchGroupLayer : public Popup<const MatchGroup&> {
    protected:
        bool setup(const MatchGroup& _group);
    public:
        static GDWTMatchGroupLayer* create(const MatchGroup& _group);

        void show();

        MatchGroup group;

        EventListener<MatchesTask> l;

        EventListener<Task<std::vector<geode::Result<std::vector<std::tuple<std::string, int, int>>> *>>> scoresL;
};