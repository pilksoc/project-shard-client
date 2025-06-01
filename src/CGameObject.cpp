#include "CGameObject.h"
#include "TextureManager.h"

CGameObject::CGameObject() 
	: IGameObject(), m_height(0), m_width(0), 
	m_numFrames(1), m_currentFrame(0), m_currentRow(0)
{
}

void CGameObject::Load(const CObjectParams* pParams)
{
	m_objectName = pParams->GetName();
	
	m_position = CVector2D(pParams->GetX(), pParams->GetY());
	m_velocity = CVector2D(0, 0);
	m_acceleration = CVector2D(0, 0);

	m_width = pParams->GetWidth();
	m_height = pParams->GetHeight();
	m_textureID = pParams->GetTextureID();

	m_currentRow = 1;
	m_currentFrame = 0;
	m_numFrames = pParams->GetNumberOfFrames();
}

void CGameObject::OnPlay()
{
}

void CGameObject::Draw(CTextureManager* pManager)
{
	if (m_velocity.GetX() > 0)
	{
		pManager->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height, 
			m_currentRow, m_currentFrame, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		pManager->DrawFrame(m_textureID, (int)m_position.GetX(),
			(int)m_position.GetY(), m_width, m_height,
			m_currentRow, m_currentFrame);
	}
}

bool CGameObject::OnThink()
{
	m_velocity += m_acceleration;  
	m_position += m_velocity;

	return true;
}

void CGameObject::Destroy()
{
}