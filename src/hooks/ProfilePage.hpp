#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include "../utils/data.hpp"

#include <Geode/modify/ProfilePage.hpp>
class $modify(GDWTProfilePage, ProfilePage) {
    struct Fields{
        EventListener<PlayerDataTask> playerDataListener;
        std::unordered_map<CCObject*, std::pair<std::string, std::string>> allBadges;
    };

    public:
        void playersDataRecieved(PlayerDataTask::Event* e);

        void loadPageFromUserInfo(GJUserScore* a2);

        void createBadge(const std::string& ID, const std::string& name, const std::string& description, const int& orderPrio);

        void onBadgeClicked(CCObject* sender);
};