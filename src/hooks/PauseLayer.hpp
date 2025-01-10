#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
class $modify(GDWTPauseLayer, PauseLayer) {
    public:
        void customSetup();
};