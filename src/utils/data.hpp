#pragma once
#include <Geode/Geode.hpp>
#include <matjson/stl_serialize.hpp>

using namespace geode::prelude;

typedef struct {
    int levelID;
    int playTime;
    std::vector<std::tuple<int, int, std::string>> accountIDs;
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
using CurrentMatchTask = Task<Result<std::map<std::string, std::pair<std::string, std::vector<std::string>>>>>;

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
    std::string access_token;
    int expires_in;
    std::string scope;
    std::string token_type;
} sheetRefreshedToken;

template <>
struct matjson::Serialize<sheetRefreshedToken> {
    static sheetRefreshedToken from_json(const matjson::Value& value) {
        return sheetRefreshedToken {
            .access_token = value["access_token"].as_string(),
            .expires_in = value["expires_in"].as_int(),
            .scope = value["scope"].as_string(),
            .token_type = value["token_type"].as_string()
        };
    }

    static matjson::Value to_json(const sheetRefreshedToken& value) {
        auto obj = matjson::Object();
        obj["access_token"] = value.access_token;
        obj["expires_in"] = value.expires_in;
        obj["scope"] = value.scope;
        obj["token_type"] = value.token_type;
        return obj;
    }
};

typedef struct{
    std::string spreadsheetId;
    std::string updatedRange;
    int updatedRows;
    int updatedColumns;
    int updatedCells;
} sheetWriteReport;

template <>
struct matjson::Serialize<sheetWriteReport> {
    static sheetWriteReport from_json(const matjson::Value& value) {
        return sheetWriteReport {
            .spreadsheetId = value["spreadsheetId"].as_string(),
            .updatedRange = value["updatedRange"].as_string(),
            .updatedRows = value["updatedRows"].as_int(),
            .updatedColumns = value["updatedColumns"].as_int(),
            .updatedCells = value["updatedCells"].as_int()
        };
    }

    static matjson::Value to_json(const sheetWriteReport& value) {
        auto obj = matjson::Object();
        obj["spreadsheetId"] = value.spreadsheetId;
        obj["updatedRange"] = value.updatedRange;
        obj["updatedRows"] = value.updatedRows;
        obj["updatedColumns"] = value.updatedColumns;
        obj["updatedCells"] = value.updatedCells;
        return obj;
    }
};

//discord stuff

typedef struct {
    std::string message;
    int code;
} DiscordError;

template <>
struct matjson::Serialize<DiscordError> {
    static DiscordError from_json(const matjson::Value& value) {
        return DiscordError {
            .message = value["message"].as_string(),
            .code = value["code"].as_int()
        };
    }

    static matjson::Value to_json(const DiscordError& value) {
        auto obj = matjson::Object();
        obj["message"] = value.message;
        obj["code"] = value.code;
        return obj;
    }
};

typedef struct {
    std::string name;
    std::string value;
    bool inl;
} DiscordEmbedField;

template <>
struct matjson::Serialize<DiscordEmbedField> {
    static DiscordEmbedField from_json(const matjson::Value& value) {
        return DiscordEmbedField {
            .name = value["name"].as_string(),
            .value = value["value"].as_string(),
            .inl = value["inline"].as_bool()
        };
    }

    static matjson::Value to_json(const DiscordEmbedField& value) {
        auto obj = matjson::Object();
        obj["name"] = value.name;
        obj["value"] = value.value;
        obj["inline"] = value.inl;
        return obj;
    }
};


typedef struct {
    std::string title;
    std::string description;
    int color;
    std::vector<DiscordEmbedField> fields;
} DiscordEmbed;

template <>
struct matjson::Serialize<DiscordEmbed> {
    static DiscordEmbed from_json(const matjson::Value& value) {
        return DiscordEmbed {
            .title = value["title"].as_string(),
            .description = value["description"].as_string(),
            .color = value["color"].as_int(),
            .fields = value["fields"].as<std::vector<DiscordEmbedField>>()
        };
    }

    static matjson::Value to_json(const DiscordEmbed& value) {
        auto obj = matjson::Object();
        obj["title"] = value.title;
        obj["description"] = value.description;
        obj["color"] = value.color;
        obj["fields"] = value.fields;
        return obj;
    }
};

typedef struct {
    std::string content;
    std::vector<DiscordEmbed> embeds;
} DiscordMessage;

template <>
struct matjson::Serialize<DiscordMessage> {
    static DiscordMessage from_json(const matjson::Value& value) {
        return DiscordMessage {
            .content = value["content"].as_string(),
            .embeds = value["embeds"].as<std::vector<DiscordEmbed>>()
        };
    }

    static matjson::Value to_json(const DiscordMessage& value) {
        auto obj = matjson::Object();
        obj["content"] = value.content;
        obj["embeds"] = value.embeds;
        return obj;
    }
};

// (percentage toggle save data struct)
struct ZSAToggleSaveData {
	std::string key;
	bool toggled;
    int saved_time;
};

template<>
struct matjson::Serialize<std::vector<ZSAToggleSaveData>> {
    static std::vector<ZSAToggleSaveData> from_json(matjson::Value const& value) {
        auto vec = std::vector<ZSAToggleSaveData> {};
        for (auto const& item : value.as_array()) {
            vec.push_back({
				.key = item["key"].as_string(),
                .toggled = item["toggled"].as_bool(),
                .saved_time = item["original_time"].as_int(),
            });
        }
        return vec;
    }

    static matjson::Value to_json(std::vector<ZSAToggleSaveData> const& vec) {
        auto arr = matjson::Array {};
        for (auto const& item : vec) {
            arr.push_back(matjson::Object {
				{"key", item.key},
                { "toggled", item.toggled },
                { "original_time", item.saved_time },
            });
        }
        return arr;
    }

    static bool is_json(matjson::Value const& value) {
        return value.is_array();
    }
};

class data {
    public:
        static MatchesTask getMatchesData();
        static TeamsTask getTeamsData();
        static UserInfoTask getUsersInfo(std::vector<int> userIDs);
        static PlayerDataTask getPlayersData();
        static MatchGroupsDataTask getMatchGroupsData();
        static CurrentMatchTask getCurrentMatchData(std::string accessToken);

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

        static void leaveMatch();
        static Result<> joinMatch(std::string joinCode);

        static bool getIsInMatch();

        static DiscordEmbed embedWithPlayerColor();

        static Task<Result<>> SendDiscordMessage(DiscordMessage message);
        static void SendSheetProgress(std::string message);

        static int getCombo(int levelID, int precent);
        
        static std::string columnNumberToLetter(int colNum);

        static Task<Result<std::string>> refreshAccessToken(std::string clientId, std::string clientSecret, std::string refreshToken);
        static Task<Result<>> writeToGoogleSheet(std::string spreadsheetId, std::string range, std::string value, std::string accessToken);

        static bool getCBF();
        static bool getCBFAllowed();

        static void disable2point1Percent(GJGameLevel* level);

        static void checkConnectionComplete(std::string errMessage = "ERR");
        static void setConnectionCompleteCallback(SEL_CallFuncO callback, CCNode* target);

        static bool discordConnectionCheck;
        static bool sheetsConnectionCheck;
        static bool connecting;
    private:

        static std::string SheetID;
        static std::string matchesPageID;
        static std::string teamsPageID;
        static std::string playersPageID;
        static std::string matchGroupsPageID;
        static std::string currentMatchPageID;

        static std::vector<Match> loadedMatches;

        static std::vector<UserInfo> loadedUsersInfo;

        static std::vector<Team> loadedTeams;

        static std::vector<PlayerData> LoadedPlayers;

        static std::vector<MatchGroup> LoadedMatchGroups;
        
        static std::vector<GJGameLevel*> loadedLevels;

        static Ref<CCDictionary> loadedImages;

        static bool isInMatch;
        static std::string discordWebhookLink;
        static std::string discordWebhookSecret;
        static std::string sheetsClientID;
        static std::string sheetsClientSecret;
        static std::string sheetsRefreshToken;
        static bool CBFAllowed;

        static SEL_CallFuncO m_callback;
        static CCNode* m_target;

        static std::tuple<int, int, int> lastLevelProgress;

        static int getKey();
        static std::vector<unsigned char> base64Decode(const std::string& input);
        static std::string decryptString(const std::string& encryptedInput);
};