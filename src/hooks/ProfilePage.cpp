#include "../hooks/ProfilePage.hpp"

void GDWTProfilePage::loadPageFromUserInfo(GJUserScore* user) {
    ProfilePage::loadPageFromUserInfo(user);

    m_fields->playerDataListener.bind(this, &GDWTProfilePage::playersDataRecieved);

    m_fields->playerDataListener.setFilter(data::getPlayersData());
    
    // auto layer = m_mainLayer;
        
    // CCMenu* username_menu = typeinfo_cast<CCMenu*>(layer->getChildByIDRecursive("username-menu"));

    // std::array<int, 3> managmentIDs{
    //     11684904,
    //     9074168,
    //     1380770
    // };

    // for (int i = 0; i < managmentIDs.size(); i++)
    // {
    //     if (managmentIDs[i] == user->m_accountID){
    //         addManagmentBadge(username_menu);
    //         break;
    //     }
    // }

    // std::array<int, 8> tourneyWinners2024IDs{
    //     8303773,
    //     6801276,
    //     11860237,
    //     5682434,
    //     7664649,
    //     4955259,
    //     26556060,
    //     2932785
    // };

    // for (int i = 0; i < tourneyWinners2024IDs.size(); i++)
    // {
    //     if (tourneyWinners2024IDs[i] == user->m_accountID){
    //         addTourneyWinners2024Badge(username_menu);
    //         break;
    //     }
    // }

    // std::array<int, 4> organizerIDs{
    //     11582651,
    //     9074168,
    //     1380770,
    //     19729991
    // };

    // for (int i = 0; i < organizerIDs.size(); i++)
    // {
    //     if (organizerIDs[i] == user->m_accountID){
    //         addOrganizerBadge(username_menu);
    //         break;
    //     }
    // }

    // username_menu->updateLayout();
}

void GDWTProfilePage::playersDataRecieved(PlayerDataTask::Event* e){
    if (auto* playerDataRes = e->getValue()){
        if (!playerDataRes) return;
        if (playerDataRes->isErr()) return;

        auto playerData = playerDataRes->unwrap();

        PlayerData myPlayer;
        bool didFindPlayer;

        for (const auto& player : playerData)
        {
            if (player.accountID == m_score->m_accountID){
                myPlayer = player;
                didFindPlayer = true;
                break;
            }
        }

        if (!didFindPlayer) return;

        std::set<std::string> allBadgeIDs{};

        std::set_union(myPlayer.staffIDs.begin(), myPlayer.staffIDs.end(), myPlayer.achievementIDs.begin(), myPlayer.achievementIDs.end(), inserter(allBadgeIDs, allBadgeIDs.begin()));

        for (const auto& badgeID : allBadgeIDs)
        {
            if (badgeID == "WT-MGT"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDWT Managment",
                    "This user is a member of the <cy>GD World Tournament management team</c>.\n\nThey are responsible for <cg>overseeing and creating</c> various GD World Tournament events, along with <co>developing systems</c> to further help the project as a whole.",
                    2
                );
            }

            if (badgeID == "WT-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2024 GD World Champion",
                    "This player was a member of the South Korea team for the <cl>2024 GD World Tournament</c>. After battling to get out of their group in the group stage, they defeated <cr>Canada</c>, <cb>France</c>, and finally <cg>Mexico</c> to become champions of the inaugural <cl>GD World Tournament</c>.\n\nThe team consisted of <cy>Baeru</c>, <cy>Clutter</c>, <cy>Laniakea</c>, <cy>PoCle</c>, <cy>Guraud</c>, <cy>hanni</c>, <cy>IcaroX</c>, and <cy>Knicide</c>",
                    0
                );
            }

            if (badgeID == "WT-EO"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDWT Event Organizer",
                    "This user is an <cy>Event Organizer</c> for the <cj>GD Wolrd Tournament</c>.\n\nThey are responsible for <cg>preparing and setting up</c> various GD World Tournament events.",
                    1
                );
            }

            if (badgeID == "PT-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "NA",
                    "No description available for this badge yet...",
                    0
                );
            }

            if (badgeID == "US-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "NA",
                    "No description available for this badge yet...",
                    0
                );
            }

            if (badgeID == "KR-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "NA",
                    "No description available for this badge yet...",
                    0
                );
            }

            if (badgeID == "KR-S2"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "NA",
                    "No description available for this badge yet...",
                    0
                );
            }

            if (badgeID == "KR-S3"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "NA",
                    "No description available for this badge yet...",
                    0
                );
            }
        }
    }
}

void GDWTProfilePage::createBadge(const std::string& ID, const std::string& name, const std::string& description, const int& orderPrio){
    auto username_menu = typeinfo_cast<CCMenu*>(m_mainLayer->getChildByIDRecursive("username-menu"));
    log::info("{}", fmt::format("{}"_spr, ID));
    auto yourBadge = CCSprite::create(fmt::format("{}.png"_spr, ID).c_str());
    auto bButton = CCMenuItemSpriteExtra::create(
        yourBadge,
        nullptr,
        this,
        menu_selector(GDWTProfilePage::onBadgeClicked)
    );
    bButton->setID(fmt::format("{}-badge", ID));
    bButton->setZOrder(-orderPrio);
    username_menu->addChild(bButton);

    m_fields->allBadges.insert(std::make_pair(bButton, std::make_pair(name, description)));
    username_menu->updateLayout();
}

void GDWTProfilePage::onBadgeClicked(CCObject* sender){
    if (m_fields->allBadges.contains(sender)){
        auto alert = FLAlertLayer::create(m_fields->allBadges[sender].first.c_str(), m_fields->allBadges[sender].second.c_str(), "OK");
        alert->show();
    }   
}