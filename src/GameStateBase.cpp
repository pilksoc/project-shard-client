#include "GameStateBase.h"
#include "GameObject.h"
#include "TextureManager.h"

bool CGameStateBase::OnPlay()
{
    m_shouldTick = true;
    m_shouldDraw = true;

    return true;
}

bool CGameStateBase::OnEnd()
{
    // Stop accessing pointers that are about to be destroyed!
    m_shouldTick = false;
    m_shouldDraw = false;

    // Destroying everything!
    for (size_t i = 0; i < m_GameObjects.size(); i++) {
        m_GameObjects[i]->Destroy();
    }

    for (size_t i = 0; i < m_TextureIDList.size(); i++) {
        pTextureManager->Remove(m_TextureIDList[i]);
    }

    m_GameObjects.clear();
    m_TextureIDList.clear();

    return true;
}

void CGameStateBase::OnThink()
{
    if (!m_shouldTick) return;
    for (size_t i = 0; i < m_GameObjects.size(); i++) {
        if (!m_GameObjects[i]->OnThink()) return;
    }
}

void CGameStateBase::Draw(CTextureManager* pManager)
{
    if (!m_shouldDraw) return;

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->Draw(pManager);
	}
}