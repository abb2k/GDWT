#pragma once
#include <Geode/Geode.hpp>
#include <matjson/stl_serialize.hpp>

using namespace geode::prelude;

typedef struct {
    int levelID;
    int playTime;
    std::vector<std::pair<int, int>> accountIDs;
    std::vector<std::tuple<std::string, std::string, int>> displayNames;
} Level;

typedef struct {
    int accountID;
    bool active;
    std::string displayName;
} Player;

typedef struct {
    int accountID;
    std::string displayName;
} Host;

typedef struct {
    std::string countryCode;
    std::vector<Player> accounts;
} Team;

enum class ScoreSystemType {
    Addition,
    WinDraw,
    AdditionT2,
    WinDrawT2,
    WinDrawTimes2
};

typedef struct {
    std::vector<Host> hosts;
    std::vector<Host> coHosts;
    std::string date;
    long long dateUnix;
    std::vector<std::string> teams;
    std::vector<Level> levels;
    std::string liveLink;
    std::string vodLink;
    std::string matchName;
    ScoreSystemType scoreType;
    int groupID;
} Match;

typedef struct {
    std::string userName; // :1:
    int userID; // :2:
    int stars; // :3:
    int demons; // :4:
    int creatorPoints; // :8:
    int playerColor; // :10:
    int playerColor2; // :11:
    int secretCoins; // :13:
    int accountID; // :16:
    int userCoins; // :17:
    int messageState; // :18:
    int friendState; // :19:
    std::string youtubeLink; // :20:
    int accIcon; // :21:
    int accShip; // :22:
    int accBall; // :23:
    int accUfo; // :24:
    int accWave; // :25:
    int accRobot; // :26:
    int accGlow; // :28:
    int isRegistered; // :29:
    int globalRank; // :30:
    int friendstate; // :31:
    int accSpider; // :43:
    std::string twitterLink; // :44:
    std::string twitch; // :45:
    int diamonds; // :46:
    int accExplosion; // :48:
    int modLevel; // :49:
    int commentHistoryState; // :50:
    int glowColor; // :51:
    int moons; // :52:
    std::string demonCategories; // :55:
    std::string normalLevelCategories; // :56:
    std::string platformerLevelCategories; // :57:
} UserInfo;

typedef struct{
    std::string range;
    std::string majorDimension;
    std::vector<std::vector<std::string>> values;
} sheetValues;

typedef struct {
    int accountID;
    int iconID;
    int color1ID;
    int color2ID;
    int glowColorID;
    bool glowEnabled;
    std::string ingameUserName;
} PlayerData;

template <>
struct matjson::Serialize<sheetValues> {
    static sheetValues from_json(const matjson::Value& value) {
        return sheetValues {
            .range = value["range"].as_string(),
            .majorDimension = value["majorDimension"].as_string(),
            .values = value["values"].as<std::vector<std::vector<std::string>>>(),
        };
    }

    static matjson::Value to_json(const sheetValues& value) {
        auto obj = matjson::Object();
        obj["range"] = value.range;
        obj["majorDimension"] = value.majorDimension;
        obj["values"] = value.values;
        return obj;
    }
};

typedef struct{
    int code;
    std::string message;
    std::string status;
} sheetError;

template <>
struct matjson::Serialize<sheetError> {
    static sheetError from_json(const matjson::Value& value) {
        return sheetError {
            .code = value["code"].as_int(),
            .message = value["message"].as_string(),
            .status = value["status"].as_string(),
        };
    }

    static matjson::Value to_json(const sheetError& value) {
        auto obj = matjson::Object();
        obj["code"] = value.code;
        obj["message"] = value.message;
        obj["status"] = value.status;
        return obj;
    }
};

typedef struct{
    int groupID;
    std::string groupName;
    int amountPass;
    ScoreSystemType scoreType;
} MatchGroup;

using MatchesTask = Task<std::vector<Match>>;
using TeamsTask = Task<std::vector<Team>>;
using UserInfoTask = Task<std::vector<UserInfo*>>;
using PlayerDataTask = Task<std::vector<PlayerData>>;
using scoreCalcTask = Task<std::vector<std::tuple<std::string, int, int>>>;
using MatchGroupsDataTask = Task<std::vector<MatchGroup>>;

class data {
    public:
        static MatchesTask getMatchesData();
        static TeamsTask getTeamsData();
        static UserInfoTask getUsersInfo(std::vector<int> userIDs);
        static PlayerDataTask getPlayersData();
        static MatchGroupsDataTask getMatchGroupsData();

        static UserInfo parseUserInfo(std::string infoRaw);

        static std::vector<std::string> splitStr(std::string str, std::string delim);
        static std::vector<int> splitStrInt(std::string str, std::string delim);
        static std::vector<std::string> eraseEmptys(std::vector<std::string> array);
        static std::vector<std::vector<std::string>> convertRawData(std::string data, bool rows);

        static void loadMatches(std::vector<Match> match);
        static void loadUserInfo(UserInfo user);
        static void loadTeams(std::vector<Team> teams);
        static GJGameLevel* getLoadedLevelByID(int level);
        static void loadLevel(GJGameLevel* level);
        static CCImage* getImage(std::string ID);
        static void addImage(CCImage* image, std::string ID);

        static scoreCalcTask calculateScores(std::vector<Level> levels, std::vector<std::string> expectedTeams, ScoreSystemType type);

        static CCNode* createCircleGlow(ccColor3B color, float opacity);

        static std::string ScoreSystemTypeToString(ScoreSystemType type);

        static Result<std::tuple<int, int, int>, int> splitDate(std::string date);
    
    private:

        static std::string SheetID;
        static std::string matchesPageID;
        static std::string teamsPageID;
        static std::string playersPageID;
        static std::string matchGroupsPageID;

        static std::vector<Match> loadedMatches;

        static std::vector<UserInfo> loadedUsersInfo;

        static std::vector<Team> loadedTeams;

        static std::vector<PlayerData> LoadedPlayers;

        static std::vector<MatchGroup> LoadedMatchGroups;
        
        static std::vector<GJGameLevel*> loadedLevels;

        static Ref<CCDictionary> loadedImages;
};