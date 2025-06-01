#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include "EngineTypes.h"
#include "Vector2D.h"

enum EMouseButtons {
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

struct SInputObject;

class CInputHandler
{
public:
    ~CInputHandler();

    static CInputHandler* Instance()
    {
        if (s_pInstance == 0) {
            s_pInstance = new CInputHandler();
        }

        return s_pInstance;
    }

    void OnThink();

    void InitialiseJoysticks();
    bool JoysticksInitialised()
    {
        return m_bJoysticksInitialised;
    }

    float GetXAxis(int joy, int stick);
    float GetYAxis(int joy, int stick);

    bool GetButtonState(int joy, int buttonNumber)
    {
        return m_buttonStates[joy][buttonNumber];
    }
    bool GetMouseButtonState(int buttonNumber)
    {
        return m_mouseButtonStates[buttonNumber];
    }

    void AddActionKeyDown(SDL_Scancode key, KeyCallback callBack);
    void AddActionKeyUp(SDL_Scancode key, KeyCallback callBack);

    bool IsKeyDown(SDL_Scancode key);
    void SetReleaseState(SDL_Scancode key, bool state);

    void SetAxisValue(std::string name, SDL_Scancode key, float value);
    float GetAxisValue(std::string name);

    CVector2D* GetMousePosition()
    {
        return m_mousePosition;
    }

private:
    CInputHandler();

    // private functions to handle the different event types

    // handle keyboard events
    void onKeyDown();
    void onKeyUp();

    void setAxisValues();

    // handle mouse events
    void onMouseMove(SDL_Event& event);
    void onMouseButtonDown(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);

    // handle joystick events
    void onJoystickAxisMove(SDL_Event& event);
    void onJoystickButtonDown(SDL_Event& event);
    void onJoystickButtonUp(SDL_Event& event);

    std::vector<SDL_Joystick*> m_joysticks;
    bool m_bJoysticksInitialised;

    std::vector<std::pair<CVector2D*, CVector2D*>> m_joystickValues;
    std::vector<std::vector<bool>> m_buttonStates;

    std::map<SDL_Scancode, KeyCallback> m_keyDownCallbacks;
    std::map<SDL_Scancode, KeyCallback> m_keyUpCallbacks;

    std::map<SDL_Scancode, bool> m_keyReleased;

    std::map<SDL_Scancode, std::string> m_keyVirtualAxis;
    std::map<std::string, std::map<SDL_Scancode, float>> m_keyAxisValue;
    std::map<std::string, float> m_currentKeyAxisValue;

    // true down, false up
    std::map<SDL_Scancode, bool> m_keyAxisStates;

    std::vector<bool> m_mouseButtonStates;

    const int m_joystickDeadZone = 10000;

    CVector2D* m_mousePosition = new CVector2D(0,0);

    Uint8* m_keystates = 0;

    static CInputHandler* s_pInstance;
};
