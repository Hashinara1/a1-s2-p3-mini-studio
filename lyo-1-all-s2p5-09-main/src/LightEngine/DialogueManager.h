#pragma once

#include "AssetsManager.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>
#include <functional>


class DialogueManager
{

private:

	std::string m_text;
	bool m_isFinish = true;

	int m_index = 0;

	float m_inter = 0.01f;
	float m_timer = 0;

	sf::Sprite m_back;
	sf::Text m_display;

	std::function<void()> m_callback = nullptr;

	DialogueManager();

	~DialogueManager() = default;

	DialogueManager(const DialogueManager&) = delete;
	DialogueManager& operator=(const DialogueManager&) = delete;


public:

	static DialogueManager& getInstance() {

		static DialogueManager instance = DialogueManager();
		return instance;
	}

	void update();
	void Draw(sf::RenderWindow& window);

	bool say(const char* _text, std::function<void()> _callback = nullptr);

};
