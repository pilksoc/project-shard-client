#include "CTexture.h"
#include <SDL_image.h>
#include "TextureManager.h"
#include "Vector2D.h"

CTexture::CTexture(SDL_Texture* nTex, double angle, SDL_Point* center)
{
	m_texture = nTex;
	m_angle = angle;
	m_center = center;
	if (m_texture != nullptr)
		SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
}

CTexture::~CTexture()
{
	Free();
}

void CTexture::Free()
{
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
		m_width = 0;
		m_height = 0;
	}
}

void CTexture::SetColour(Uint8 red, Uint8 green, Uint8 blue)
{
	if (m_texture == nullptr) return;
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void CTexture::SetBlendMode(SDL_BlendMode blending)
{
	if (m_texture == nullptr) return;
	SDL_SetTextureBlendMode(m_texture, blending);
}

void CTexture::SetAlpha(Uint8 alpha)
{
	if (m_texture == nullptr) return;
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

void CTexture::Draw(CTextureManager* pManager, CVector2D pos)
{
	if (!m_texture) return;
    if (!pManager) return;
	pManager->Draw(this, (int)pos.GetX(), (int)pos.GetY(), m_angle);
}

void CTexture::SetAngle(double pAngle)
{
	m_angle = pAngle;
}

void CTexture::SetCenter(SDL_Point* pNewPoint)
{
	m_center = pNewPoint;
}