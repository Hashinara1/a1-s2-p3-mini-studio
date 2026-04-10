#include "AliveEntity.h"
#include "AssetsManager.h"

class Rock : public AliveEntity {
private:
	int m_width = 240;
	int m_height = 263;
	std::function<void()> m_callback = nullptr;
public:

	void OnInitialize() override {
		SetTexture(AssetsManager::getInstance().getTexture("Rock", "res/Items/StoneSprite.png"));
		mSprite.setTextureRect(sf::IntRect((m_maxHp - m_hp) * m_width, 0, m_width, m_height));
		mSprite.setScale(2, 2);
	}
	void OnDamage() override {
		mSprite.setTextureRect(sf::IntRect((m_maxHp - m_hp) * m_width, 0, m_width, m_height));
	};
	void OnDead() override {
		
		for (Entity* e : GameManager::Get()->getEntities()) {
			if (e->IsTag(9)) {
				e->setActive(true);
				break;
			}
		}
		if (m_callback)
			m_callback();
		Destroy();
	}

	void setCallBack(std::function<void()> _func) { m_callback = _func; }







};