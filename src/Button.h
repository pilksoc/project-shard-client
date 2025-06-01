#pragma once
#include <functional>
#include "CGameObject.h"

class CButton : public CGameObject
{
protected:
    typedef std::function<bool()> ButtonCallback;
public:
    CButton();

    void Load(const CObjectParams* pParams);

    void Draw(CTextureManager* pManager);
    bool OnThink();

    void OnClick(ButtonCallback e);
    void OnEnter(ButtonCallback e);
    void OnLeave(ButtonCallback e);

    int GetOnClickID() const
    {
        return m_onClickID;
    }
    int GetOnEnterID() const
    {
        return m_onEnterID;
    }
    int GetOnLeaveID() const
    {
        return m_onLeaveID;
    }

private:
    // Holds the callback to trigger on events
    ButtonCallback m_OnClick = 0;
    ButtonCallback m_OnEnter = 0;
    ButtonCallback m_OnLeave = 0;

    enum button_state {
        NO_HOVER = 0,
        HOVER = 1,
        PRESSED = 2
    };

protected:
    // Initial values for callbacks
    bool onClick();
    bool onEnter();
    bool onLeave();

private:
    // Whether the button has been released or not.
    bool m_bReleased = true;

    int m_onClickID = 0;
    int m_onEnterID = 0;
    int m_onLeaveID = 0;
};
