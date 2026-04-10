#include "DialogueManager.h"
#include "GameManager.h"
#include "InputManager.h"

#include <SFML/Graphics.hpp>

#define Lentgh 54

bool DialogueManager::say(const char* _text, std::function<void()> _callback)
{
	if (m_isFinish == false)
		return false;

	m_text = _text;
	m_isFinish = false;
	m_timer = 0;
	m_index = 0;
	m_display.setString("");
	m_callback = _callback;
	return true;
}

DialogueManager::DialogueManager()
{
	sf::Texture* tex = AssetsManager::getInstance().getTexture("UIDialogue", "res/Ui/DialogBox.png")->getTexture();
	m_back.setTexture(*tex);
	m_back.setScale(2.4, 4);
	m_back.setPosition((1280 - (float)tex->getSize().x) / 2.f,0);

	m_display.setFont(GameManager::Get()->GetFont());
	m_display.setCharacterSize(18);
	m_display.setFillColor(sf::Color::Black);
	m_display.setPosition(m_back.getPosition() + sf::Vector2f(40.f, 60.f));
}

void DialogueManager::update()
{
	if (m_isFinish)
		return;

	if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Sud)) {
		if (m_index == (int)m_text.size()) {
			m_isFinish = true;
			m_callback();
			return;
		}
		m_index = (int)m_text.size();
		for (size_t i = 1; i < m_text.size() / Lentgh + 1; i++) {
			int i2 = i * Lentgh;
			while (m_text[i2] != ' ') {
				i2--;
				if (i2 == 0) {
					i2 = i * Lentgh;
					m_text.insert(i2, "-");
					break;
				}
			}
			
			m_text.insert(i2 + 1, "\n");
		}
	}

	m_timer += GameManager::Get()->GetDeltaTime();

	if (m_timer >= m_inter && m_index < (int)m_text.size()) {
		m_index++;
		m_timer -= m_inter;
	}

	if (m_index < (int)m_text.size() && m_index % Lentgh == 0 && m_index != 0) {
		int i = m_index;
		while (m_text[i] != ' ') {
			i--;
			if (i == 0) {
				i = m_index;
				m_text.insert(i, "-");
				i++;
				break;
			}
		}
		m_text.insert(i, "\n");
	}

	m_display.setString(m_text.substr(0, m_index));
}

void DialogueManager::Draw(sf::RenderWindow& window)
{
	if (m_isFinish)
		return;

	window.draw(m_back);
	window.draw(m_display);

	if (m_index >= (int)m_text.size()) {
		sf::Text hint;
		hint.setFont(GameManager::Get()->GetFont());
		hint.setCharacterSize(14);
		hint.setFillColor(sf::Color::Black);
		hint.setString("Appuyez sur Espace");
		hint.setPosition(m_back.getPosition() + sf::Vector2f(400.f, 800.f));
		window.draw(hint);
	}
}
