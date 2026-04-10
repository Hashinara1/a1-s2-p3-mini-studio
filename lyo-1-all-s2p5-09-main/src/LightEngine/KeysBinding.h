#pragma once

#include <variant>
#include <vector>
//not window but sf::keybord sf::mouse
#include "SFML/Window.hpp"

enum class TriggerState { Down, Held, Released };

using Input = std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoystickButton>;

struct KeyBinding {
    std::vector<std::pair<Input, TriggerState>> inputs;
};