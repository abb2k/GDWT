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
    std::string displayName;
} Host;

typedef struct {
    int accountID;
    int iconID;
    int color1ID;
    int color2ID;
    int glowColorID;
    bool glowEnabled;
    std::string displayName;
    std::string countryCode;
    bool isActive;
} PlayerData;

typedef struct {
    std::string countryCode;
    std::vector<PlayerData> accounts;
} Team;

enum class ScoreSystemType {
    Addition,
    WinDraw,
    AdditionT2,
    WinDrawT2,
    WinDrawTimes2
};

struct Match_s {
    std::vector<Host> hosts{};
    std::vector<Host> coHosts{};
    std::string date;
    long long dateUnix;
    std::vector<std::string> teams{};
    std::vector<Level> levels{};
    std::string liveLink;
    std::string vodLink;
    std::string matchName;
    ScoreSystemType scoreType;
    int groupID;
};
typedef struct Match_s Match;

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

template <>
struct matjson::Serialize<sheetValues> {
    static Result<sheetValues> fromJson(const matjson::Value& value) {
        sheetValues sheetVal;
        GEODE_UNWRAP_INTO(sheetVal.range, value["range"].asString());
        GEODE_UNWRAP_INTO(sheetVal.majorDimension, value["majorDimension"].asString());
        GEODE_UNWRAP_INTO(sheetVal.values, value["values"].as<std::vector<std::vector<std::string>>>());

        return Ok(sheetVal);
    }

    static matjson::Value toJson(const sheetValues& value) {
        auto obj = matjson::makeObject({
            { "range", value.range },
            { "majorDimension", value.majorDimension },
            { "values", value.values },
        });
        return obj;
    }
};

typedef struct{
    int groupID;
    std::string groupName;
    int amountPass;
    ScoreSystemType scoreType;
} MatchGroup;

using MatchesTask = Task<Result<std::vector<Match>>>;
using TeamsTask = Task<Result<std::vector<Team>>>;
using UserInfoTask = Task<std::vector<UserInfo*>>;
using PlayerDataTask = Task<Result<std::vector<PlayerData>>>;
using scoreCalcTask = Task<Result<std::vector<std::tuple<std::string, int, int>>>>;
using MatchGroupsDataTask = Task<Result<std::vector<MatchGroup>>>;
using CurrentMatchTask = Task<Result<std::map<std::string, std::pair<std::string, std::vector<std::string>>>>>;

typedef struct{
    int code;
    std::string message;
    std::string status;
} sheetError;

template <>
struct matjson::Serialize<sheetError> {
    static Result<sheetError> fromJson(const matjson::Value& value) {
        sheetError sheetErr;
        GEODE_UNWRAP_INTO(sheetErr.code, value["code"].asInt());
        GEODE_UNWRAP_INTO(sheetErr.message, value["message"].asString());
        GEODE_UNWRAP_INTO(sheetErr.status, value["status"].asString());

        return Ok(sheetErr);
    }

    static matjson::Value toJson(const sheetError& value) {
        auto obj = matjson::makeObject({
            { "code", value.code },
            { "message", value.message },
            { "status", value.status },
        });
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
    static Result<sheetRefreshedToken> fromJson(const matjson::Value& value) {
        sheetRefreshedToken refToken;
        GEODE_UNWRAP_INTO(refToken.access_token, value["access_token"].asString());
        GEODE_UNWRAP_INTO(refToken.expires_in, value["expires_in"].asInt());
        GEODE_UNWRAP_INTO(refToken.scope, value["scope"].asString());
        GEODE_UNWRAP_INTO(refToken.token_type, value["token_type"].asString());

        return Ok(refToken);
    }

    static matjson::Value toJson(const sheetRefreshedToken& value) {
        auto obj = matjson::makeObject({
            { "access_token", value.access_token },
            { "expires_in", value.expires_in },
            { "scope", value.scope },
            { "token_type", value.token_type },
        });
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
    static Result<sheetWriteReport> fromJson(const matjson::Value& value) {
        sheetWriteReport wreport;
        GEODE_UNWRAP_INTO(wreport.spreadsheetId, value["spreadsheetId"].asString());
        GEODE_UNWRAP_INTO(wreport.updatedRange, value["updatedRange"].asString());
        GEODE_UNWRAP_INTO(wreport.updatedRows, value["updatedRows"].asInt());
        GEODE_UNWRAP_INTO(wreport.updatedColumns, value["updatedColumns"].asInt());
        GEODE_UNWRAP_INTO(wreport.updatedCells, value["updatedCells"].asInt());

        return Ok(wreport);
    }

    static matjson::Value toJson(const sheetWriteReport& value) {
        auto obj = matjson::makeObject({
            { "spreadsheetId", value.spreadsheetId },
            { "updatedRange", value.updatedRange },
            { "updatedRows", value.updatedRows },
            { "updatedColumns", value.updatedColumns },
            { "updatedCells", value.updatedCells },
        });
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
    static Result<DiscordError> fromJson(const matjson::Value& value) {
        DiscordError derr;
        GEODE_UNWRAP_INTO(derr.message, value["message"].asString());
        GEODE_UNWRAP_INTO(derr.code, value["code"].asInt());

        return Ok(derr);
    }

    static matjson::Value toJson(const DiscordError& value) {
        auto obj = matjson::makeObject({
            { "message", value.message },
            { "code", value.code },
        });
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
    static Result<DiscordEmbedField> fromJson(const matjson::Value& value) {
        DiscordEmbedField dEmbedField;
        GEODE_UNWRAP_INTO(dEmbedField.name, value["name"].asString());
        GEODE_UNWRAP_INTO(dEmbedField.value, value["value"].asString());
        GEODE_UNWRAP_INTO(dEmbedField.inl, value["inline"].asBool());

        return Ok(dEmbedField);
    }

    static matjson::Value toJson(const DiscordEmbedField& value) {
        auto obj = matjson::makeObject({
            { "name", value.name },
            { "value", value.value },
            { "inline", value.inl },
        });
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
    static Result<DiscordEmbed> fromJson(const matjson::Value& value) {
        DiscordEmbed dEmbed;
        GEODE_UNWRAP_INTO(dEmbed.title, value["title"].asString());
        GEODE_UNWRAP_INTO(dEmbed.description, value["description"].asString());
        GEODE_UNWRAP_INTO(dEmbed.color, value["color"].asInt());
        GEODE_UNWRAP_INTO(dEmbed.fields, value["fields"].as<std::vector<DiscordEmbedField>>());

        return Ok(dEmbed);
    }

    static matjson::Value toJson(const DiscordEmbed& value) {
        auto obj = matjson::makeObject({
            { "title", value.title },
            { "description", value.description },
            { "color", value.color },
            { "fields", value.fields },
        });
        return obj;
    }
};

typedef struct {
    std::string content;
    std::vector<DiscordEmbed> embeds;
} DiscordMessage;

template <>
struct matjson::Serialize<DiscordMessage> {
    static Result<DiscordMessage> fromJson(const matjson::Value& value) {
        DiscordMessage dMess;
        GEODE_UNWRAP_INTO(dMess.content, value["content"].asString());
        GEODE_UNWRAP_INTO(dMess.embeds, value["embeds"].as<std::vector<DiscordEmbed>>());

        return Ok(dMess);
    }

    static matjson::Value toJson(const DiscordMessage& value) {
        auto obj = matjson::makeObject({
            { "content", value.content },
            { "embeds", value.embeds },
        });
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
    static Result<std::vector<ZSAToggleSaveData>> fromJson(matjson::Value const& value) {
        GEODE_UNWRAP_INTO(auto arr, value.asArray());
        
        auto vec = std::vector<ZSAToggleSaveData> {};
        for (auto const& item : arr) {
            ZSAToggleSaveData togSave;
            GEODE_UNWRAP_INTO(togSave.key, item["key"].asString());
            GEODE_UNWRAP_INTO(togSave.toggled, item["toggled"].asBool());
            GEODE_UNWRAP_INTO(togSave.saved_time, item["original_time"].asInt());

            vec.push_back(togSave);
        }
        return Ok(vec);
    }

    static matjson::Value toJson(std::vector<ZSAToggleSaveData> const& vec) {
        auto arr = matjson::Value {};
        for (auto const& item : vec) {
            auto obj = matjson::makeObject({
                { "key", item.key},
                { "toggled", item.toggled },
                { "original_time", item.saved_time },
            });

            arr.push(obj);
        }
        return arr;
    }

    static bool is_json(matjson::Value const& value) {
        return value.isArray();
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

        static Result<UserInfo> parseUserInfo(std::string infoRaw);

        static std::vector<std::string> splitStr(std::string str, std::string delim);
        static std::vector<std::string> eraseEmptys(std::vector<std::string> array);
        static std::vector<std::vector<std::string>> convertRawData(std::string data, bool rows);

        static void loadMatches(const std::vector<Match>& match);
        static void loadUserInfo(UserInfo& user);
        static void loadTeams(const std::vector<Team>& teams);
        static GJGameLevel* getLoadedLevelByID(int level);
        static void loadLevel(GJGameLevel* level);
        static CCImage* getImage(std::string ID);
        static void addImage(CCImage* image, std::string ID);

        static scoreCalcTask calculateScores(std::vector<Level> levels, std::vector<std::string> expectedTeams, ScoreSystemType type);

        static CCNode* createCircleGlow(ccColor3B color, float opacity);

        static std::string ScoreSystemTypeToString(ScoreSystemType type);

        static Result<std::tuple<int, int, int>> splitDate(std::string date);

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

        static void checkConnectionComplete(std::string errMessage = "OK");
        static void setConnectionCompleteCallback(SEL_CallFuncO callback, CCNode* target);

        static void sendError(std::string err);

        static bool discordConnectionCheck;
        static bool sheetsConnectionCheck;
        static bool connecting;
    private:

        static std::string SheetID;
        static std::string matchesPageID;
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
        static std::string matchSheetID;
        static std::string matchSheetName;
        static CCPoint columnsMinMax;
        static std::string connectCheckCell;

        static SEL_CallFuncO m_callback;
        static CCNode* m_target;

        static std::tuple<int, int, int> lastLevelProgress;

        static int getKey();
        static std::vector<unsigned char> base64Decode(const std::string& input);
        static std::string decryptString(const std::string& encryptedInput);
};