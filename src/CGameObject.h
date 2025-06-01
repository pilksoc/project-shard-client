#pragma once
#include "GameObject.h"
#include "ObjectParams.h"
#include "Vector2D.h"
#include <memory>

class CTextureManager;

class CGameObject :
    public IGameObject
{
public:
    CGameObject();
    ~CGameObject();

    void OnPlay() override;
    void Draw(CTextureManager* pManager) override;
    bool OnThink() override;

    void Load(const CObjectParams* pParams) override;

    CVector2D GetPosition() override
    {
        return m_position;
    }
    void SetPosition(CVector2D& nPos) override
    {
        m_position = nPos;
    }

    virtual CVector2D GetVelocity()
    {
        return m_velocity;
    }
    virtual void SetVelocity(CVector2D nV)
    {
        m_velocity = nV;
    }
    virtual void AddVelocity(CVector2D aV)
    {
        m_velocity += aV;
    }

    virtual CVector2D GetAcceleration()
    {
        return m_acceleration;
    }
    virtual void SetAcceleration(CVector2D nA)
    {
        m_acceleration = nA;
    }
    virtual void AddAcceleration(CVector2D nA)
    {
        m_acceleration += nA;
    }

    virtual CVector2D GetSize()
    {
        return CVector2D((float)m_width, (float)m_height);
    }

    virtual int GetCurrentAnimRow()
    {
        return m_currentRow;
    }
    virtual void SetAnimRow(int nR)
    {
        m_currentRow = nR;
    }

    virtual int GetCurrentAnimFrame()
    {
        return m_currentFrame;
    }
    virtual void SetAnimFrame(int aF)
    {
        m_currentFrame = aF;
    }

    virtual int GetTotalAnimFrames()
    {
        return m_numFrames;
    }

    const char* GetName() override
    {
        return m_objectName.c_str();
    }
    virtual std::string GetTextureID()
    {
        return m_textureID;
    }

    bool ShouldCollide() override
    {
        return m_collidable;
    }
    void SetCollision(bool nC)
    {
        m_collidable = nC;
    }

    bool ShouldOverlap() override
    {
        return m_overlap;
    }
    void SetOverlap(bool nO)
    {
        m_overlap = nO;
    }

protected:
    CVector2D m_position;
    CVector2D m_velocity;
    CVector2D m_acceleration;

    int m_width = 0;
    int m_height = 0;

    int m_currentRow = 0;
    int m_currentFrame = 0;
    int m_numFrames = 1;

    std::string m_objectName = "";
    std::string m_textureID = "";

    bool m_collidable = false;
    bool m_overlap = false;
};
