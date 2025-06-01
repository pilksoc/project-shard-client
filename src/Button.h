#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <functional>
#include "CGameObject.h"

class CButton : public CGameObject
{
protected:
	typedef std::function<bool()> ButtonCallback;
public:
	CButton();

	virtual void Load(const CObjectParams* pParams);

	virtual void Draw(CTextureManager* pManager);
	virtual bool OnThink();
	virtual void Destroy();

	virtual void OnClick(ButtonCallback e);
	virtual void OnEnter(ButtonCallback e);
	virtual void OnLeave(ButtonCallback e);

	int GetOnClickID() const { return m_onClickID; }
	int GetOnEnterID() const { return m_onEnterID; }
	int GetOnLeaveID() const { return m_onLeaveID; }

private:
	// Holds the callback to trigger on events
	ButtonCallback m_OnClick = 0;
	ButtonCallback m_OnEnter = 0;
	ButtonCallback m_OnLeave = 0;

	enum button_state
	{
		NO_HOVER = 0,
		HOVER = 1,
		PRESSED = 2
	};

protected:
	// Initial values for callbacks
	virtual bool onClick();
	virtual bool onEnter();
	virtual bool onLeave();

private:
	// Whether the button has been released or not.
	bool m_bReleased = true;

	int m_onClickID = 0;
	int m_onEnterID = 0;
	int m_onLeaveID = 0;	
};

#endif