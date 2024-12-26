#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

//the CCScaleTo action but edited to change content size :)

class ScaleContentTo : public CCActionInterval
{
public:
    bool initWithDuration(float duration, float s);

    bool initWithDuration(float duration, float w, float h);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

public:

    static ScaleContentTo* create(float duration, float s);

    static ScaleContentTo* create(float duration, float w, float h);
protected:
    float m_fWidth;
    float m_fHeight;
    float m_fStartWidth;
    float m_fStartHeight;
    float m_fEndWidth;
    float m_fEndHeight;
    float m_fDeltaWidth;
    float m_fDeltaHeight;
};