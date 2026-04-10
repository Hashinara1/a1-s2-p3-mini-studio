#pragma once

#include <unordered_map>
#include <SFML/Window.hpp>


class GameManager;

//ids buttons SFML
enum class JoystickButton
{
    Sud = 0,
    Est = 1,
    Ouest = 2,
    Nord = 3,
    L = 4,
    R = 5,
    Moin = 6,
    Plus = 7,
    JLeft = 8,
    JRight = 9,
    LeftTrigger = 10,
    RightTrigger = 11,

    count,
};

//ids axes SFML
enum class JoystickAxis
{
    LeftJoystickX = 0,
    LeftJoystickY = 1,
    RTrigger = 2,
    LTrigger = 3,
    RightJoystickX = 4,
    RightJoystickY = 5
};

class InputManager
{
    struct States {
        bool isDown = false;
        bool isHeld = false;
        bool isRelease = false;
    };

private:

    //controller
    std::unordered_map<JoystickButton, States> m_joystickState;

    //keyboard
    std::unordered_map<sf::Keyboard::Key, States> m_keysStates;

    //mouse
    std::unordered_map<sf::Mouse::Button, States> m_mouseStates;
    sf::Vector2i m_mousePos = { 0, 0 };
    int m_controllerId = -1;


    //singleton
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;


    InputManager();

public:

    static InputManager& getInstance()
    {
        static InputManager instance = InputManager();
        return instance;
    }

    void update();

    //stats getter 
    bool isKeyDown(const sf::Keyboard::Key _key) const ;
    bool isKeyHeld(const sf::Keyboard::Key _key) const ;
    bool isKeyRelease(const sf::Keyboard::Key _key) const ;

    bool isMouseButtonDown(const sf::Mouse::Button _btn) const;
    bool isMouseButtonHeld(const sf::Mouse::Button _btn) const;
    bool isMouseButtonRelease(const sf::Mouse::Button _btn) const;

    bool isJoystickButtonDown(const JoystickButton _btn) const;
    bool isJoystickButtonHeld(const JoystickButton _btn) const;
    bool isJoystickButtonRelease(const JoystickButton _btn) const;



    //getter
    sf::Vector2i getMousePos() const { return m_mousePos; }
    float getAxisValue(JoystickAxis _axis) const;

    int getControllerId() const { return m_controllerId; }
};

