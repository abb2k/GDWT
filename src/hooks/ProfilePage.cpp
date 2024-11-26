#include "../hooks/ProfilePage.hpp"

void GDWTProfilePage::loadPageFromUserInfo(GJUserScore* user) {
    ProfilePage::loadPageFromUserInfo(user);

    m_fields->playerDataListener.bind(this, &GDWTProfilePage::playersDataRecieved);

    m_fields->playerDataListener.setFilter(data::getPlayersData());
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
                    "2024 GDWT Champion",
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

            if (badgeID == "OC-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "Open Cup S1 Champion",
                    "This user won the first <cl>GD Open Cup</c>, a 192 person single elimination tournament where the first to complete the level wins.\n\nThey defeated <cg>Remy202</c> in the final.",
                    -1
                );
            }

            if (badgeID == "CO-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "Copa GD Colombia 2024",
                    "Santhor won the <cy>Copa GD Colombia 2024</c>, defeating <cc>Leven</c> in the final.\n\nThe tournament had <cr>40 participants</c>, and was determined by the players' ability to beat a level as fast as possible given practice.",
                    0
                );
            }

            if (badgeID == "US-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "Battle of the States S1",
                    "No description available for this badge yet...",
                    0
                );
            }

            if (badgeID == "NL-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDNL Tournament S1",
                    "No description available for this badge yet...",
                    0
                );
            }

            if (badgeID == "PT-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2024 GDPT Champion",
                    "No description available for this badge yet...",
                    0
                );
            }

            if (badgeID == "KR-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDKF S1 Champion",
                    "<cg>Xylitol</c> won the first <cr>GMD Korea Forum</c> championship, with <cg>Kowoo</c> being the runner-up",
                    0
                );
            }

            if (badgeID == "KR-S2"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDKF S2 Champion",
                    "<cj>TherZ</c> won the second <cr>GMD Korea Forum</c> championship, with <cg>Plasmalust</c> being the runner-up",
                    0
                );
            }

            if (badgeID == "KR-S3"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDKF S3 Champion",
                    "<cy>DNM121</c> won the third <cr>GMD Korea Forum</c> championship, with <cd>Baeru</c> being the runner-up",
                    0
                );
            }

            if (badgeID == "MY-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "1st GDMY Tournament",
                    "<cg>Anglified</c> won the first <cb>GD Malaysia Tournament</c>, with <cb>Koba</c> being the runner-up",
                    0
                );
            }

            if (badgeID == "MY-S2"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2022 GDMY Tournament",
                    "<cr>MADxSG</c> won the <cb>2022 GD Malaysia Tournament</c>, with <cg>FarAwayLand</c> being the runner-up",
                    0
                );
            }

            if (badgeID == "MY-S3"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "Malaysian Independance Tournament",
                    "<cj>Fastboi</c> won the <cb>2023 GD Malaysian Independance Day Tournament</c>, with <cr>Zakuya</c> being the runner-up",
                    0
                );
            }

            if (badgeID == "MY-S4"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2023 GDMY Tournament",
                    "<cl>Ryanrolanddd</c> won the <cb>2023 GD Malaysian Tournament</c>, with <cg>partyt</c> being the runner-up",
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