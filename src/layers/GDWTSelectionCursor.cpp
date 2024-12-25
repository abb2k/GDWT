#include "../layers/GDWTSelectionCursor.hpp"

GDWTSelectionCursor* GDWTSelectionCursor::create() {
    auto ret = new GDWTSelectionCursor();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GDWTSelectionCursor::init(){

    return true;
};

void GDWTSelectionCursor::AddOption(CCNode* const option){
    if (std::find(options.begin(), options.end(), option) == options.end()) return;

    options.push_back(option);
}

void GDWTSelectionCursor::RemoveOption(CCNode* const option){
    if (std::find(options.begin(), options.end(), option) != options.end()) return;

    options.erase(std::remove(options.begin(), options.end(), option), options.end());
}

void GDWTSelectionCursor::RemoveOption(const int optionIndex){
    if (options.size() - 1 > optionIndex) return;

    options.erase(options.begin() + optionIndex);
}

void GDWTSelectionCursor::EraseAllOptions(){
    options.clear();
}


void GDWTSelectionCursor::MoveOption(int amount){
    if (!options.size()) return;

    currentOption = abs(currentOption + amount) % options.size();

    GDWTSelectionCursor::MoveMeTo(options[currentOption]);
}


void GDWTSelectionCursor::SetAnimationSpeed(float speed){

}

void GDWTSelectionCursor::MoveMeTo(CCNode* const option){

}