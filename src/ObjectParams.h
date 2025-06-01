#pragma once
#include <string>

class CObjectParams
{
public:
    CObjectParams(float x, float y)
        : m_x(x), m_y(y) {}

    // Setters
    void SetName(std::string n)
    {
        m_objName = n;
    }

    void SetWidth(int w)
    {
        m_width = w;
    }

    void SetHeight(int h)
    {
        m_height = h;
    }

    void SetAnimSpeed(int s)
    {
        m_animSpeed = s;
    }

    void SetNumFrames(int n)
    {
        m_numFrames = n;
    }

    void SetOnClick(int c)
    {
        m_OnClick = c;
    }

    void SetOnEnter(int c)
    {
        m_OnEnter = c;
    }

    void SetOnLeave(int c)
    {
        m_OnLeave = c;
    }

    void SetTextureID(std::string id)
    {
        m_textureID = id;
    }

    // Getters
    float GetX() const
    {
        return m_x;
    }
    float GetY() const
    {
        return m_y;
    }

    std::string GetName() const
    {
        return m_objName;
    }
    std::string GetFactoryID() const
    {
        return m_factoryID;
    }

    int GetWidth() const
    {
        return m_width;
    }
    int GetHeight() const
    {
        return m_height;
    }

    int GetAnimSpeed() const
    {
        return m_animSpeed;
    }
    int GetNumberOfFrames() const
    {
        return m_numFrames;
    }

    int GetOnClickID() const
    {
        return m_OnClick;
    }
    int GetOnEnterID() const
    {
        return m_OnEnter;
    }
    int GetOnLeaveID() const
    {
        return m_OnLeave;
    }

    std::string GetSoundPath() const
    {
        return m_soundPath;
    }

    std::string GetTextureID() const
    {
        return m_textureID;
    }

private:
    // Metadata + Map Logic
    std::string m_textureID = "";
    std::string m_objName = "";
    std::string m_factoryID = "";
    std::string m_soundPath = "";

    float m_x = 0;
    float m_y = 0;

    int m_width = 0;
    int m_height = 0;

    int m_animSpeed = 1;
    int m_numFrames = 1;

    int m_OnClick = 0;
    int m_OnEnter = 0;
    int m_OnLeave = 0;
};
