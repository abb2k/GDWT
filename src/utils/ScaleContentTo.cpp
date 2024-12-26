#include "../utils/ScaleContentTo.hpp"

ScaleContentTo* ScaleContentTo::create(float duration, float s)
{
    ScaleContentTo *pScaleTo = new ScaleContentTo();
    pScaleTo->initWithDuration(duration, s);
    pScaleTo->autorelease();

    return pScaleTo;
}

bool ScaleContentTo::initWithDuration(float duration, float s)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_fEndWidth = s;
        m_fEndHeight = s;

        return true;
    }

    return false;
}

ScaleContentTo* ScaleContentTo::create(float duration, float w, float h)
{
    ScaleContentTo *pScaleTo = new ScaleContentTo();
    pScaleTo->initWithDuration(duration, w, h);
    pScaleTo->autorelease();

    return pScaleTo;
}

bool ScaleContentTo::initWithDuration(float duration, float w, float h)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_fEndWidth = w;
        m_fEndHeight = h;

        return true;
    }

    return false;
}

CCObject* ScaleContentTo::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    ScaleContentTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        pCopy = static_cast<ScaleContentTo*>(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new ScaleContentTo();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);


    pCopy->initWithDuration(m_fDuration, m_fEndWidth, m_fEndHeight);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void ScaleContentTo::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_fStartWidth = pTarget->getContentWidth();
    m_fStartHeight = pTarget->getContentHeight();
    m_fDeltaWidth = m_fEndWidth - m_fStartWidth;
    m_fDeltaHeight = m_fEndHeight - m_fStartHeight;
}

void ScaleContentTo::update(float time)
{
    if (m_pTarget)
    {
        m_pTarget->setContentWidth(m_fStartWidth + m_fDeltaWidth * time);
        m_pTarget->setContentHeight(m_fStartHeight + m_fDeltaHeight * time);
    }
}