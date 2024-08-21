#pragma once

#include <Geode/Geode.hpp>
#include "../utils/data.hpp"
#include "../layers/flagDisplay.hpp"

using namespace geode::prelude;

class CountyTextDisplay : public CCNode {
    protected:
        bool init(std::vector<std::string> _counrtyCodes, CCSize _size, bool autoScale);

        bool init(std::vector<std::tuple<std::string, int, bool, int>> _counrtyCodesWPlacement, CCSize _size, bool autoScale);
    public:
        static CountyTextDisplay* create(std::vector<std::string> _counrtyCodes, CCSize _size, bool autoScale);

        static CountyTextDisplay* create(std::vector<std::tuple<std::string, int, bool, int>> _counrtyCodesWPlacement, CCSize _size, bool autoScale);

        std::vector<std::string> counrtyCodes;
        std::vector<flagDisplay*> flags;
        CCNode* scalingPoint;
        CCSize size;
};