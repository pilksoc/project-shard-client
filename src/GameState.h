#pragma once

#include <string>
#include <vector>

class IGameObject;
class CTextureManager;

class CGameState
{
public:
	virtual void OnThink() = 0;
	virtual void Draw(CTextureManager* pManager) = 0;
	
	virtual bool OnPlay() = 0;
	virtual bool OnEnd() = 0;

    virtual bool OnPlay() = 0;
    virtual bool OnEnd() = 0;

    virtual std::string GetStateID() const = 0;

    virtual std::vector<IGameObject*> GetGameObjects() const = 0;
    virtual std::vector<std::string> GetLoadedTextures() const = 0;
    virtual std::vector<std::string> GetLoadedScripts() const = 0;

    virtual bool ShouldBeTicking() const = 0;
    virtual bool ShouldBeDrawing() const = 0;
};
