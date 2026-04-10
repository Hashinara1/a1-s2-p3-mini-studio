#pragma once
#include "Action.h"
#include "Enemies.h"

class Enemie1ActionIdle : public Action<Enemie1>
{
	void OnStart(Enemie1* pOwner) override {};
	void OnUpdate(Enemie1* pOwner) override { };
	void OnEnd(Enemie1* pOwner) override {};
};

class Enemie1ActionAttack : public Action<Enemie1>
{
	void OnStart(Enemie1* pOwner) override;
	void OnUpdate(Enemie1* pOwner) override {};
	void OnEnd(Enemie1* pOwner) override {};
};

class Enemie2ActionIdle : public Action<Enemie2>
{
	void OnStart(Enemie2* pOwner) override {};
	void OnUpdate(Enemie2* pOwner) override {};
	void OnEnd(Enemie2* pOwner) override {};
};

class Enemie2ActionAttack : public Action<Enemie2>
{
	void OnStart(Enemie2* pOwner) override;
	void OnUpdate(Enemie2* pOwner) override {};
	void OnEnd(Enemie2* pOwner) override {};
};


class Enemie3ActionIdle : public Action<Enemie3>
{
	void OnStart(Enemie3* pOwner) override {};
	void OnUpdate(Enemie3* pOwner) override {};
	void OnEnd(Enemie3* pOwner) override {};
};

class Enemie3ActionAttack : public Action<Enemie3>
{
	void OnStart(Enemie3* pOwner) override;
	void OnUpdate(Enemie3* pOwner) override {};
	void OnEnd(Enemie3* pOwner) override {};
};
