#pragma once

class CObjectParams;
class CVector2D;
class CTextureManager;

class IGameObject
{
public:
    virtual void OnPlay() = 0;
    virtual void Draw(CTextureManager* pManager) = 0;
    virtual bool OnThink() = 0;

    virtual void Load(const CObjectParams* pParams) = 0;

    virtual CVector2D GetPosition() = 0;
    virtual void SetPosition(CVector2D& nPos) = 0;

    virtual const char* GetName() = 0;

    virtual bool ShouldCollide() = 0;
    virtual bool ShouldOverlap() = 0;
    virtual ~IGameObject() {}
protected:
    friend class IGameObjectDeleter;
    IGameObject() {}
};

class IGameObjectDeleter
{
public:
    void operator()(IGameObject* p)
    {
        delete p;
    }
};
