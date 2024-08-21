#include "../utils/data.hpp"
#include <Geode/utils/web.hpp>

// == static members ==

std::string data::apiKey = "AIzaSyAnUtlLdyZEXQILqY-ye75SVzAu_s8PXag";

std::string data::SheetID = "1D-x1ABxhvJHb6rQ1T0LC0TF3MOeaEXLSDxwRhuMk9nE";

std::string data::matchesPageID = "Match%20Raw%20Data";

std::string data::teamsPageID = "Team%20Raw%20Data";

std::string data::playersPageID = "Player%20Raw%20Data";

std::string data::matchGroupsPageID = "Match%20Groups%20Data";

std::vector<Match> data::loadedMatches{};

std::vector<UserInfo> data::loadedUsersInfo{};

std::vector<Team> data::loadedTeams{};

std::vector<PlayerData> data::LoadedPlayers{};

std::vector<GJGameLevel*> data::loadedLevels{};

std::vector<MatchGroup> data::LoadedMatchGroups{};

Ref<CCDictionary> data::loadedImages = CCDictionary::create();

// == functions ==

MatchesTask data::getMatchesData(){
    if (loadedMatches.size()){
        return MatchesTask::run(
        [] (auto progress, auto hasBeenCancelled) -> MatchesTask::Result{
            return data::loadedMatches;
        });
    }

    web::WebRequest req = web::WebRequest();

    req.param("key", apiKey);

    return req.get(fmt::format("https://sheets.googleapis.com/v4/spreadsheets/{}/values/{}!A2:DD", SheetID, matchesPageID)).map(
    [] (web::WebResponse* res) -> std::vector<Match> {

        auto jsonRes = res->json();


        if (jsonRes.isOk()){
            auto json = jsonRes.value();
            auto matches = std::vector<Match>{};

            if (json.contains("error")){
                auto err = json.get<sheetError>("error");
                log::warn("SHEET ERROR!\ncode: {}\nmessage: {}\nstatus: {}", err.code, err.message, err.status);
                return std::vector<Match>{};
            }

            auto values = json.get<std::vector<std::vector<std::string>>>("values");
            
            for (int r = 0; r < values.size(); r++)
            {
                if (values[r].size() - 1 != 9) continue;
                
                Match currMatch;
                
                auto hosts = splitStr(values[r][0], ";");
                
                for (int i = 0; i < hosts.size(); i++)
                {
                    if (hosts[i] == "NA")
                        break;
                    Host h;
                    auto splittedHost = splitStr(hosts[i], ",");
                    h.displayName = splittedHost[0];
                    h.accountID = std::stoi(splittedHost[1]);

                    currMatch.hosts.push_back(h);
                }
                
                auto coHosts = splitStr(values[r][1], ";");
                
                for (int i = 0; i < coHosts.size(); i++)
                {
                    if (coHosts[i] == "NA")
                        break;
                    Host coh;
                    auto splittedcoHost = splitStr(coHosts[i], ",");
                    coh.displayName = splittedcoHost[0];
                    coh.accountID = std::stoi(splittedcoHost[1]);

                    currMatch.coHosts.push_back(coh);
                }

                currMatch.dateUnix = std::stoll(values[r][2]);

                std::stringstream time;
                time << std::chrono::sys_seconds{std::chrono::seconds(currMatch.dateUnix)};
                auto dateStringArr = splitStr(splitStr(time.str(), " ")[0], "-");
                currMatch.date = fmt::format("{}/{}/{}", dateStringArr[1], dateStringArr[2], dateStringArr[0]);

                currMatch.teams = splitStr(values[r][3], ",");
                auto splittedLevels = splitStr(values[r][4], ";");
                
                if (values[r][4] == "NA" || values[r][4] == "")
                    splittedLevels.clear();

                for (int i = 0; i < splittedLevels.size(); i++)
                {
                    auto splittedLevel = splitStr(splittedLevels[i], ",");
                    Level level;
                    
                    auto splittedLevelStats = splitStr(splittedLevel[0], "-");

                    level.levelID = std::stoi(splittedLevelStats[0]);
                    if (splittedLevelStats.size() > 1)
                        level.playTime = std::stoi(splittedLevelStats[1]);
                    if (splittedLevel.size() > 1)
                        for (int d = 1; d < splittedLevel.size(); d++)
                        {
                            auto pdata = splitStr(splittedLevel[d], "-");
                            try{
                                auto toAdd = std::pair<int, int>{std::stoi(pdata[0]), 0};

                                if (pdata.size() > 1){
                                    toAdd.second = std::stoi(pdata[1]);
                                }
                                level.accountIDs.push_back(toAdd);
                            }
                            catch (...){
                                auto toAdd = std::tuple<std::string, std::string, int>{pdata[0], pdata[1], 0};

                                if (pdata.size() > 2){
                                    std::get<2>(toAdd) = std::stoi(pdata[2]);
                                }
                                
                                level.displayNames.push_back(toAdd);
                            }


                        }
                    currMatch.levels.push_back(level);
                }

                currMatch.liveLink = values[r][5];
                currMatch.vodLink = values[r][6];
                currMatch.matchName = values[r][7];

                currMatch.scoreType = static_cast<ScoreSystemType>(std::stoi(values[r][8]));
                currMatch.groupID = std::stoi(values[r][9]);

                matches.push_back(currMatch);
            }

            std::ranges::sort(matches, [](const Match a, const Match b) {

                return b.dateUnix < a.dateUnix;
            });
            
            loadMatches(matches);

            return matches;
        }
        else{
            
            return std::vector<Match>{};
        }
            
    },
    [](auto) -> std::monostate {
        return std::monostate();
    });
}

TeamsTask data::getTeamsData(){
    if (loadedTeams.size()){
        return TeamsTask::run(
        [] (auto progress, auto hasBeenCancelled) -> TeamsTask::Result{
            return data::loadedTeams;
        });
    }

    web::WebRequest req = web::WebRequest();

    req.param("key", apiKey);
    req.param("majorDimension", "COLUMNS");

    return req.get(fmt::format("https://sheets.googleapis.com/v4/spreadsheets/{}/values/{}!A1:DD", SheetID, teamsPageID)).map(
    [] (web::WebResponse* res) -> std::vector<Team> {

        auto jsonRes = res->json();


        if (jsonRes.isOk()){
            auto json = jsonRes.value();
            auto teams = std::vector<Team>{};

            if (json.contains("error")){
                auto err = json.get<sheetError>("error");
                log::warn("SHEET ERROR!\ncode: {}\nmessage: {}\nstatus: {}", err.code, err.message, err.status);
                return std::vector<Team>{};
            }

            auto values = json.get<std::vector<std::vector<std::string>>>("values");
            
            for (int r = 0; r < values.size(); r++)
            {
                Team currTeam;
                currTeam.countryCode = values[r][0];
                if (values[r].size() > 1)
                    for (int i = 1; i < values[r].size(); i++)
                    {
                        Player p;
                        auto pInfo = splitStr(values[r][i], "-");
                        try{
                            p.accountID = std::stoi(pInfo[0]);
                        }
                        catch (...){
                            p.displayName = pInfo[0];
                            p.accountID = -1;
                        }
                        if (pInfo.size() > 1)
                            if (pInfo[1] == "i")
                                p.active = false;
                            else
                                p.active = true;
                        else
                            p.active = true;
                        
                        if (pInfo.size() > 2)
                            p.displayName = pInfo[2];

                        currTeam.accounts.push_back(p);
                    }

                teams.push_back(currTeam);
            }

            loadTeams(teams);
            
            return teams;
        }
        else{
            
            return std::vector<Team>{};
        }
            
    },
    [](auto) -> std::monostate {
        return std::monostate();
    });
}

std::vector<std::string> data::splitStr(std::string str, std::string delim) {
    size_t posStart = 0;
    size_t posEnd;
    size_t delimLen = delim.length();

    std::vector<std::string> res{};

    while ((posEnd = str.find(delim, posStart)) != std::string::npos) {
        std::string token = str.substr(posStart, posEnd - posStart);
        posStart = posEnd + delimLen;
        if (token != "")
            res.push_back(token);
    }

    res.push_back(str.substr(posStart));
    return res;
}

std::vector<int> data::splitStrInt(std::string str, std::string delim) {
    size_t posStart = 0;
    size_t posEnd;
    size_t delimLen = delim.length();

    std::vector<int> res;

    while ((posEnd = str.find(delim, posStart)) != std::string::npos) {
        std::string token = str.substr(posStart, posEnd - posStart);
        posStart = posEnd + delimLen;
        if (token != "")
            res.push_back(std::stoi(token));
    }

    res.push_back(std::stoi(str.substr(posStart)));
    return res;
}

UserInfoTask data::getUsersInfo(std::vector<int> userIDs){

    std::vector<UserInfo> alreadyLoadedUsers{};

    for (int i = 0; i < userIDs.size(); i++)
    {
        UserInfo user;

        bool doesExist = false;
        for (int u = 0; u < loadedUsersInfo.size(); u++)
        {
            if (loadedUsersInfo[u].accountID == userIDs[i]){
                doesExist = true;
                user = loadedUsersInfo[u];
            }
        }
        
        if (doesExist){
            alreadyLoadedUsers.push_back(user);
            userIDs.erase(std::next(userIDs.begin(), i));
            i--;
        }
    }

    std::vector<Task<UserInfo>> tasks;

    for (int i = 0; i < userIDs.size(); i++)
    {
        if (userIDs[i] != -1){
            web::WebRequest req = web::WebRequest();

            req.header("Content-Type", "application/x-www-form-urlencoded");

            req.userAgent("");

            req.bodyString(fmt::format("secret=Wmfd2893gb7&targetAccountID={}", userIDs[i]));

            tasks.push_back(req.post("http://www.boomlings.com/database/getGJUserInfo20.php").map(
            [] (web::WebResponse* res) -> UserInfo{

                if (res->string().isOk()){

                    auto Info = data::parseUserInfo(res->string().unwrap());

                    data::loadUserInfo(Info);

                    return Info;
                }
                else{
                    UserInfo u{};
                    u.accountID = -1;
                    return u;
                }
            },
            [](auto) -> std::monostate {
                return std::monostate();
            }));
        }
        
    }

    for (int i = 0; i < alreadyLoadedUsers.size(); i++)
    {
        UserInfo info = alreadyLoadedUsers[i];

        tasks.push_back(Task<UserInfo>::run(
        [info] (auto progress, auto hasBeenCancelled) -> Task<UserInfo>::Result{
            return info;
        }));
    }
    

    return Task<UserInfo>::all(
        std::move(tasks)
    );
    
}

UserInfo data::parseUserInfo(std::string infoRaw){

    if (infoRaw.length() >= 5){
        if (infoRaw[0] == 'e' && infoRaw[1] == 'r' && infoRaw[2] == 'r' && infoRaw[3] == 'o' && infoRaw[4] == 'r'){
            UserInfo u{};
            u.accountID = -1;
            return u;
        }
    }

    std::map<int, std::string> infoMap{};

    bool isKey = true;

    std::pair<int, std::string> currentElement{};

    std::string currThing = "";

    //log::info("r {}", infoRaw);

    for (int i = 0; i < infoRaw.length(); i++)
    {
        if (infoRaw[i] == ':' || infoRaw.length() == i + 1){
            if (infoRaw.length() == i + 1 && infoRaw[i] != ':')
                currThing += infoRaw[i];
            //log::info("registering (key = {})", isKey);
            if (isKey){
                currentElement.first = std::stoi(currThing);
                //log::info("added '{}' into the key", currThing);
            }
            else{
                currentElement.second = currThing;
                //log::info("added '{}' into the value of key {}", currThing, currentElement.first);

                infoMap.insert(currentElement);
                //log::info("inserted {}-{}", currentElement.first, currentElement.second);
            }

            isKey = !isKey;
            currThing = "";
        }
        else{
            //log::info("added '{}'", infoRaw[i]);
            currThing += infoRaw[i];
        }
    }

    UserInfo info;
    
    info.userName = infoMap[1];
    info.userID = std::stoi(infoMap[2]);
    info.stars = std::stoi(infoMap[3]);
    info.demons = std::stoi(infoMap[4]);
    info.creatorPoints = std::stoi(infoMap[8]);
    info.playerColor = std::stoi(infoMap[10]);
    info.playerColor2 = std::stoi(infoMap[11]);
    info.secretCoins = std::stoi(infoMap[13]);
    info.accountID = std::stoi(infoMap[16]);
    info.userCoins = std::stoi(infoMap[17]);
    info.messageState = std::stoi(infoMap[18]);
    info.friendState = std::stoi(infoMap[19]);
    info.youtubeLink = infoMap[20];
    info.accIcon = std::stoi(infoMap[21]);
    info.accShip = std::stoi(infoMap[22]);
    info.accBall = std::stoi(infoMap[23]);
    info.accUfo = std::stoi(infoMap[24]);
    info.accWave = std::stoi(infoMap[25]);
    info.accRobot = std::stoi(infoMap[26]);
    info.accGlow = std::stoi(infoMap[28]);
    info.isRegistered = std::stoi(infoMap[29]);
    info.globalRank = std::stoi(infoMap[30]);
    info.friendstate = std::stoi(infoMap[31]);
    info.accSpider = std::stoi(infoMap[43]);
    info.twitterLink = infoMap[44];
    info.twitch = infoMap[45];
    info.diamonds = std::stoi(infoMap[46]);
    info.accExplosion = std::stoi(infoMap[48]);
    info.modLevel = std::stoi(infoMap[49]);
    info.commentHistoryState = std::stoi(infoMap[50]);
    info.glowColor = std::stoi(infoMap[51]);
    info.moons = std::stoi(infoMap[52]);
    info.demonCategories = infoMap[55];
    info.normalLevelCategories = infoMap[56];
    info.platformerLevelCategories = infoMap[57];

    return info;

}

void data::loadMatches(std::vector<Match> match){
    loadedMatches.clear();
    loadedMatches = match;
}

void data::loadUserInfo(UserInfo user){
    bool doesExist = false;
    for (int u = 0; u < loadedUsersInfo.size(); u++)
    {
        if (loadedUsersInfo[u].accountID == user.accountID)
            doesExist = true;
    }

    if (doesExist) return;
    
    loadedUsersInfo.insert(loadedUsersInfo.begin(), user);
}

void data::loadTeams(std::vector<Team> teams){
    loadedTeams.clear();
    loadedTeams = teams;
}

GJGameLevel* data::getLoadedLevelByID(int level){
    GJGameLevel* toReturn = nullptr;
    for (int l = 0; l < loadedLevels.size(); l++)
    {
        if (loadedLevels[l]->m_levelID.value() == level)
            toReturn = loadedLevels[l];
    }

    return toReturn;
}

void data::loadLevel(GJGameLevel* level){
    bool doesExist = false;
    for (int l = 0; l < loadedLevels.size(); l++)
    {
        if (loadedLevels[l]->m_levelID.value() == level->m_levelID.value())
            doesExist = true;
    }

    if (doesExist) return;
    
    loadedLevels.push_back(level);
}

PlayerDataTask data::getPlayersData(){
        if (LoadedPlayers.size()){
        return PlayerDataTask::run(
        [] (auto progress, auto hasBeenCancelled) -> PlayerDataTask::Result{
            return data::LoadedPlayers;
        });
    }

    web::WebRequest req = web::WebRequest();

    req.param("key", apiKey);

    return req.get(fmt::format("https://sheets.googleapis.com/v4/spreadsheets/{}/values/{}!A2:DD", SheetID, playersPageID)).map(
    [] (web::WebResponse* res) -> std::vector<PlayerData> {

        auto jsonRes = res->json();


        if (jsonRes.isOk()){
            auto json = jsonRes.value();
            auto players = std::vector<PlayerData>{};

            if (json.contains("error")){
                auto err = json.get<sheetError>("error");
                log::warn("SHEET ERROR!\ncode: {}\nmessage: {}\nstatus: {}", err.code, err.message, err.status);
                return std::vector<PlayerData>{};
            }

            auto values = json.get<std::vector<std::vector<std::string>>>("values");
            
            for (int r = 0; r < values.size(); r++)
            {
                PlayerData currPlayer;
                
                currPlayer.accountID = std::stoi(values[r][0]);

                auto iconInfo = splitStr(values[r][1], ",");
                currPlayer.iconID = std::stoi(iconInfo[0]);
                currPlayer.color1ID = std::stoi(iconInfo[1]);
                currPlayer.color2ID = std::stoi(iconInfo[2]);
                currPlayer.glowEnabled = iconInfo[3] == "1";
                currPlayer.glowColorID = std::stoi(iconInfo[4]);

                currPlayer.ingameUserName = values[r][2];

                players.push_back(currPlayer);
            }
            
            LoadedPlayers.clear();
            LoadedPlayers = players;

            return players;
        }
        else{
            
            return std::vector<PlayerData>{};
        }
            
    },
    [](auto) -> std::monostate {
        return std::monostate();
    });
}

void data::addImage(CCImage* image, std::string ID){
    loadedImages->setObject(image, ID);
}

CCImage* data::getImage(std::string ID){
    return dynamic_cast<CCImage*>(loadedImages->objectForKey(ID));
}

scoreCalcTask data::calculateScores(std::vector<Level> levels, std::vector<std::string> expectedTeams, ScoreSystemType type){
    return getTeamsData().map(
        [levels, expectedTeams, type] (std::vector<Team>* _teams) -> std::vector<std::tuple<std::string, int, int>> {
        auto teams = *_teams;

        std::vector<std::tuple<std::string, int, int>> toReturn{};

        std::map<std::string, std::pair<int, int>> overallScores{};

        for (int i = 0; i < levels.size(); i++)
        {
            //get level players teams
            std::vector<std::tuple<std::string, int>> players{};
            for (int p = 0; p < levels[i].accountIDs.size(); p++)
            {
                std::string cc = "NA";
                for (int t = 0; t < teams.size(); t++){
                    bool hasFound = false;
                    for (int ac = 0; ac < teams[t].accounts.size(); ac++){
                        if (teams[t].accounts[ac].accountID == levels[i].accountIDs[p].first){
                            cc = teams[t].countryCode;
                            std::transform(cc.begin(), cc.end(), cc.begin(), ::tolower);
                        }
                    }
                }

                int currScore = levels[i].accountIDs[p].second;

                players.push_back(std::tuple<std::string, int>{cc, currScore});
            }
            
            for (int p = 0; p < levels[i].displayNames.size(); p++)
            {
                std::string cc = std::get<1>(levels[i].displayNames[p]);
                std::transform(cc.begin(), cc.end(), cc.begin(), ::tolower);

                int currScore = std::get<2>(levels[i].displayNames[p]);

                for (int t = 0; t < teams.size(); t++){
                    bool hasFound = false;
                    for (int ac = 0; ac < teams[t].accounts.size(); ac++){
                        if (teams[t].accounts[ac].accountID == levels[i].accountIDs[p].first){
                            cc = teams[t].countryCode;
                            std::transform(cc.begin(), cc.end(), cc.begin(), ::tolower);
                        }
                    }
                }

                players.push_back(std::tuple<std::string, int>{cc, currScore});
            }

            //add missing teams
            for (int t = 0; t < expectedTeams.size(); t++)
            {
                std::string lowerTeamCCode = expectedTeams[t];
                std::transform(lowerTeamCCode.begin(), lowerTeamCCode.end(), lowerTeamCCode.begin(), ::tolower);

                bool doesTeamExist = false;
                for (int p = 0; p < players.size(); p++)
                {
                    if (std::get<0>(players[p]) == lowerTeamCCode)
                        doesTeamExist = true;
                }
                
                if (!doesTeamExist){
                    players.push_back(std::tuple<std::string, int>{lowerTeamCCode, 0});
                }
            }
            
            //sort by placement
            std::ranges::sort(players, [](const std::tuple<std::string, int> a, const std::tuple<std::string, int> b) {
                auto scoreA = std::get<1>(a);
                auto scoreB = std::get<1>(b);

                return scoreA < scoreB;
            });
            
            //calc windraw
            if (type == ScoreSystemType::WinDraw || type == ScoreSystemType::WinDrawT2 || type == ScoreSystemType::WinDrawTimes2){
                //get a list of all ties in the match
                int currHighest = 0;
                std::map<int, std::vector<std::pair<int, int>>> tiedScores;
                for (int p = 0; p < players.size(); p++)
                {
                    int score = p;
                    if (type == ScoreSystemType::WinDrawTimes2)
                        score *= 2;

                    if (currHighest == std::get<1>(players[p]) && currHighest != 0){
                        if (tiedScores.contains(currHighest))
                            tiedScores[currHighest].push_back(std::pair<int, int>{score, std::get<1>(players[p])});
                        else
                            tiedScores.insert(std::pair<int, std::vector<std::pair<int, int>>>{currHighest, std::vector<std::pair<int, int>>{{score, std::get<1>(players[p])}}});
                    }

                    currHighest = std::get<1>(players[p]);
                }

                //calc
                for (int placement = 0; placement < players.size(); placement++)
                {
                    std::string keyCC = std::get<0>(players[placement]);
                    int score = 0;

                    score = placement;
                    if (type == ScoreSystemType::WinDrawTimes2)
                        score *= 2;

                    int playerScore = std::get<1>(players[placement]);

                    if (tiedScores.contains(playerScore)){
                        int tiePoints = 0;
                        for (int p = 0; p < tiedScores[playerScore].size(); p++)
                        {
                            tiePoints += tiedScores[playerScore][p].first;
                        }
                        score = tiePoints / tiedScores[playerScore].size();
                    }

                    bool found = false;

                    for (int c = 0; c < toReturn.size(); c++)
                    {
                        if (std::get<0>(toReturn[c]) == keyCC){
                            std::get<1>(toReturn[c]) += score;
                            std::get<2>(toReturn[c]) += playerScore;
                            found = true;
                        }
                    }

                    if (!found)
                        toReturn.push_back(std::tuple<std::string, int, int>{keyCC, score, playerScore});
                }

            }
            //calc addition
            else if (type == ScoreSystemType::Addition || type == ScoreSystemType::AdditionT2){
                for (int placement = 0; placement < players.size(); placement++)
                {
                    std::string keyCC = std::get<0>(players[placement]);
                    int score = std::get<1>(players[placement]);

                    bool found = false;

                    for (int c = 0; c < toReturn.size(); c++)
                    {
                        if (std::get<0>(toReturn[c]) == keyCC){
                            std::get<1>(toReturn[c]) += score;
                            std::get<2>(toReturn[c]) += score;
                            found = true;
                        }
                    }

                    if (!found)
                        toReturn.push_back(std::tuple<std::string, int, int>{keyCC, score, score});
                }
            }
        }

        std::ranges::sort(toReturn, [](const std::tuple<std::string, int, int> a, const std::tuple<std::string, int, int> b) {
            auto A = std::get<1>(a);
            auto B = std::get<1>(b);

            if (A == B) return std::get<2>(a) < std::get<2>(b);
            return A < B;
        });

        return toReturn;
    },
    [](auto) -> std::monostate {
        return std::monostate();
    });
}

CCNode* data::createCircleGlow(ccColor3B color, float opacity){
    auto node = CCNode::create();

    auto glow1 = CCSprite::createWithSpriteFrameName("d_gradient_c_02_001.png");
    glow1->setAnchorPoint({1, 0});
    glow1->setColor(color);
    glow1->setOpacity(opacity);
    node->addChild(glow1);

    auto glow2 = CCSprite::createWithSpriteFrameName("d_gradient_c_02_001.png");
    glow2->setAnchorPoint({1, 0});
    glow2->setRotation(90);
    glow2->setColor(color);
    glow2->setOpacity(opacity);
    node->addChild(glow2);

    auto glow3 = CCSprite::createWithSpriteFrameName("d_gradient_c_02_001.png");
    glow3->setAnchorPoint({1, 0});
    glow3->setRotation(180);
    glow3->setColor(color);
    glow3->setOpacity(opacity);
    node->addChild(glow3);

    auto glow4 = CCSprite::createWithSpriteFrameName("d_gradient_c_02_001.png");
    glow4->setAnchorPoint({1, 0});
    glow4->setRotation(270);
    glow4->setColor(color);
    glow4->setOpacity(opacity);
    node->addChild(glow4);

    return node;
}

std::string data::ScoreSystemTypeToString(ScoreSystemType type){
    switch (type)
    {
        case ScoreSystemType::Addition:
            return "Addition";

        case ScoreSystemType::WinDraw:
            return "Win Draw";

        case ScoreSystemType::AdditionT2:
            return "Addition Top 2";

        case ScoreSystemType::WinDrawT2:
            return "Win Draw Top 2";

        case ScoreSystemType::WinDrawTimes2:
            return "Win Draw Times 2";

        default:
            return "";
    }
}

MatchGroupsDataTask data::getMatchGroupsData(){
        if (LoadedPlayers.size()){
        return MatchGroupsDataTask::run(
        [] (auto progress, auto hasBeenCancelled) -> MatchGroupsDataTask::Result{
            return data::LoadedMatchGroups;
        });
    }

    web::WebRequest req = web::WebRequest();

    req.param("key", apiKey);

    return req.get(fmt::format("https://sheets.googleapis.com/v4/spreadsheets/{}/values/{}!A2:DD", SheetID, matchGroupsPageID)).map(
    [] (web::WebResponse* res) -> std::vector<MatchGroup> {

        auto jsonRes = res->json();

        if (jsonRes.isOk()){
            auto json = jsonRes.value();
            auto groups = std::vector<MatchGroup>{};

            if (json.contains("error")){
                auto err = json.get<sheetError>("error");
                log::warn("SHEET ERROR!\ncode: {}\nmessage: {}\nstatus: {}", err.code, err.message, err.status);
                return std::vector<MatchGroup>{};
            }

            auto values = json.get<std::vector<std::vector<std::string>>>("values");
            
            for (int r = 0; r < values.size(); r++)
            {
                if (values[r].size() - 1 != 3) continue;

                MatchGroup currMGroups;
                
                currMGroups.groupID = std::stoi(values[r][0]);
                currMGroups.groupName = values[r][1];

                currMGroups.amountPass = std::stoi(values[r][2]);

                currMGroups.scoreType = static_cast<ScoreSystemType>(std::stoi(values[r][3]));

                groups.push_back(currMGroups);
            }
            
            LoadedMatchGroups.clear();
            LoadedMatchGroups = groups;

            return groups;
        }
        else{
            
            return std::vector<MatchGroup>{};
        }
            
    },
    [](auto) -> std::monostate {
        return std::monostate();
    });
}

Result<std::tuple<int, int, int>, int> data::splitDate(std::string date){
    auto splittedDate = splitStr(date, "/");

    try{
        return Ok(std::tuple<int, int, int>{std::stoi(splittedDate[0]), std::stoi(splittedDate[1]), std::stoi(splittedDate[2])});
    }
    catch (...){
        return Err(-1);
    }
}