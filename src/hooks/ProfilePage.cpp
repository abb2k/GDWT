#include "../hooks/ProfilePage.hpp"
#include "../utils/MDAlertLayer.hpp"

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
                    "This user is a member of the <cy>GD World Tournament Hub management team</c>.\n\nThey are responsible for <cg>overseeing and creating</c> various GD World Tournament Hub events, along with <co>developing systems</c> to further help the project as a whole.\n\nTo find out more about the GD World Tournament Hub, click [here](https://geometry-dash-fan.fandom.com/wiki/GD_World_Tournament_Hub).",
                    104
                );
            }

            if (badgeID == "WT-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2024 GD World Champion",
                    "This player was a member of the South Korea team for the <cl>2024 GD World Tournament</c>. After battling to get out of their group in the group stage, they defeated <cr>Canada</c>, <cb>France</c>, and finally <cg>Mexico</c> to become champions of the inaugural <cl>GD World Tournament</c>.\n\nThe team consisted of <cy>Baeru</c>, <cy>Clutter</c>, <cy>Laniakea</c>, <cy>PoCle</c>, <cy>Guraud</c>, <cy>hanni</c>, <cy>IcaroX</c>, and <cy>Knicide</c>\n\nA full wiki page on the tournament can be found [here](https://geometry-dash-fan.fandom.com/wiki/2024_GD_World_Tournament), which includes info on every match played.\n\nTo watch the final, you can do that [here](https://www.youtube.com/watch?v=XD0UqeRaYCE)",
                    100
                );
            }

            if (badgeID == "WT-EO"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDWT Event Organizer",
                    "This user is an <cj>Event Organizer</c> for the <cl>GD World Tournament</c>.\n\nThey are responsible for <cg>preparing and setting up</c> various <cl>GD World Tournament</c> events.\n\nTo find out more about the GD World Tournament Hub, click [here](https://geometry-dash-fan.fandom.com/wiki/GD_World_Tournament_Hub).",
                    103
                );
            }

            if (badgeID == "OC-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GD Open Cup Season 1 Champion",
                    "This user won the first <c-5f5f5f>GD Open Cup</c>, a 192 person single elimination tournament where the first to complete the level wins.\n\nThey defeated <cg>Remy202</c> in the final.\n\nA wiki page giving info on all matches of the tournament can be found [here](https://geometry-dash-fan.fandom.com/wiki/GD_Open_Cup_Season_1)\n\nLinked [here](https://gdworldtournament.challonge.com/gdopencupseason1) is the bracket for the tournament.",
                    -1
                );
            }


            
            if (badgeID == "LT-21"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GD Lucky Tournament 2021 Champion",
                    "<c-afafaf>LOS COSTARAMA</c> won the first <cr>GD Lucky Tournament</c>, a single elimination team tournament that involves getting as far as possible on randomly selected demons\n\nThey defeated <c-404040>PORRO</c> in the final\n\nThe wiki page that contains info on all <cr>GD Lucky Tournament</c> events can be found [here](https://geometry-dash-fan.fandom.com/wiki/GD_Lucky_Tournament)",
                    1
                );
            }

            if (badgeID == "LT-22"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GD Lucky Tournament 2022 Champion",
                    "<cy>INFALIBLES</c> won the 2022 <cr>GD Lucky Tournament</c>, a single elimination team tournament that involves getting as far as possible on randomly selected demons\n\nThey defeated <c-00ffff>LOS DANONINOS</c> in the final\n\nThe wiki page that contains info on all <cr>GD Lucky Tournament</c> events can be found [here](https://geometry-dash-fan.fandom.com/wiki/GD_Lucky_Tournament)",
                    2
                );
            }

            if (badgeID == "LT-23"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GD Lucky Tournament 2023 Champion",
                    "<c-007d7dc>LOS CULOS HELADOS</c> won the 2023 <cr>GD Lucky Tournament</c>, a single elimination team tournament that involves getting as far as possible on randomly selected demons\n\nThey defeated <cg>LOS SIGMAS</c> in the final\n\nThe wiki page that contains info on all <cr>GD Lucky Tournament</c> events can be found [here](https://geometry-dash-fan.fandom.com/wiki/GD_Lucky_Tournament)",
                    3
                );
            }

            if (badgeID == "LT-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GD Lucky Tournament 2024 Champion",
                    "<c-3d068c>LAS DONITAS</c> won the 2024 <cr>GD Lucky Tournament</c>, a single elimination team tournament that involves getting as far as possible on randomly selected demons\n\nThey defeated <cg>TORTUGA</c> in the final\n\nThe wiki page that contains info on all <cr>GD Lucky Tournament</c> events can be found [here](https://geometry-dash-fan.fandom.com/wiki/GD_Lucky_Tournament)",
                    4
                );
            }

            if (badgeID == "CO-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "Copa GD Colombia 2024",
                    "Santhor won the <c-FFCD00>Copa GD Colombia 2024</c>, defeating <cc>Leven</c> in the final\n\nThe tournament had <cr>40 participants</c>, and was determined by the players' ability to beat a level as fast as possible given practice\n\nThe bracket can be found [here](https://challonge.com/copagdcolombia2024)",
                    5
                );
            }

            if (badgeID == "US-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "Battle of the States Champion",
                    "No description available for this badge yet...",
                    5
                );
            }

            if (badgeID == "ES-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "LaLiga Dasher 2024 Champion",
                    "<cg>Cuatrocientos</c> won the 2024 edition of LaLiga Dasher.\n\nHe claimed the 1 seed in the league phase, where 18 players matched up against each other exactly once\n\nThe top eight performers would advance to the playoffs consisting of a best of three quarterfinal and semifinal, along with a best of five final\n\n<cg>Cuatrocientos</c> only lost a single set during their playoff, dropping one in the final against <c-af004b>Davine007</c>, but still winning the series 3 to 1 to win the first completed LaLiga Dasher\n\nTo read more, check out [here](https://laligadasher.fandom.com/es/wiki/LaLiga_Dasher_2024)",
                    5
                );
            }

            if (badgeID == "ES-25"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "LaLiga Dasher 2025 Champion",
                    "No description available for this badge yet...",
                    5
                );
            }

            if (badgeID == "NL-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDNL Tournament Champion",
                    "No description available for this badge yet...",
                    5
                );
            }

            if (badgeID == "PL-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2024 GDPL Champion",
                    "No description available for this badge yet...",
                    5
                );
            }

            if (badgeID == "PT-24"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2024 GDPT Champion",
                    "No description available for this badge yet...",
                    5
                );
            }

            if (badgeID == "KR-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDKF S1 Champion",
                    "<cg>Xylitol</c> won the first <c-CD2E3A>GMD Korea Forum championship</c>, with <cg>Kowoo</c> being the runner-up",
                    5
                );
            }

            if (badgeID == "KR-S2"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GDKF S2 Champion",
                    "<cj>TherZ</c> won the second <c-CD2E3A>GMD Korea Forum championship</c>, with <cg>Plasmalust</c> being the runner-up",
                    6
                );
            }

            if (badgeID == "KR-S3"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "GD Korea Forum S3 Champion",
                    "<cy>DNM121</c> won the third <c-CD2E3A>GMD Korea Forum championship</c>, with <cd>Baeru</c> being the runner-up",
                    7
                );
            }

            if (badgeID == "MY-S1"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "Inaugural GD Malaysia Tournament",
                    "<cg>Anglified</c> won the first <c-0032A0>GD Malaysia Tournament</c>, with <cb>Koba</c> being the runner-up\n\nThe tournament consisted of sixteen Malaysian players racing on various levels\n\nThe bracket can be found [here](https://challonge.com/edj2myy7)",
                    5
                );
            }

            if (badgeID == "MY-S2"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2022 GD Malaysia Tournament",
                    "<cr>MADxSG</c> won the <c-0032A0>2022 GD Malaysia Tournament</c>, with <cg>FarAwayLand</c> being the runner-up\n\nThe tournament consisted of sixteen Malaysian players racing on various levels\n\nThe bracket can be found [here](https://challonge.com/17fd9xn4)\n\nThe video of the final day of the tournament can be found [here](https://www.youtube.com/watch?v=yEokGB-wxqY)",
                    6
                );
            }

            if (badgeID == "MY-S3"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2023 Malaysia Independence Tournament",
                    "<cj>Fastboi</c> won the <c-0032A0>2023 GD Malaysian Independance Day Tournament</c>, with <cr>Zakuya</c> being the runner-up\n\nThe tournament consisted of fourteen Malaysian players racing on various levels\n\nThe VOD for the final day can be found [here](https://www.youtube.com/live/4JWSBAQ7BNk)",
                    7
                );
            }

            if (badgeID == "MY-S4"){
                GDWTProfilePage::createBadge(
                    badgeID,
                    "2023 GD Malaysia Tournament",
                    "<cl>Ryanrolanddd</c> won the <c-0032A0>2023 GD Malaysia Tournament</c>, with <cg>partyt</c> being the runner-up\n\nThe tournament consisted of fourteen Malaysian players racing on various levels\n\nThe VOD for the final can be found [here](https://www.youtube.com/live/IWp402jOjAw?si=c9Zna0dgn5bIweml)",
                    8
                );
            }
        }
    }
}

void GDWTProfilePage::createBadge(const std::string& ID, const std::string& name, const std::string& description, const int& orderPrio){
    auto username_menu = typeinfo_cast<CCMenu*>(m_mainLayer->getChildByIDRecursive("username-menu"));
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
        auto alert = MDAlertLayer::create(m_fields->allBadges[sender].first.c_str(), m_fields->allBadges[sender].second.c_str());
        alert->show();
    }   
}