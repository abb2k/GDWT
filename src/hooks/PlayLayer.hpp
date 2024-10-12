#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
class $modify(GDWTPlayLayer, PlayLayer) {
    struct Fields {
        bool hasRespawned = false;
        EventListener<Task<Result<>>> listener;
        float currentRunStartprecent;
        geode::Notification* notifCBF = nullptr;
    };

    void destroyPlayer(PlayerObject* p0, GameObject* p1);

    void resetLevel();
    
    void levelComplete();

    void onDiscordMessageSent(Task<Result<>>::Event* e);

    void sendProgressMessage(int precent, GJGameLevel* level, int combo);
};