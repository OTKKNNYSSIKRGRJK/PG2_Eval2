#pragma once

#include"GameObjects.h"
#include"Effect.h"

class PLAYERBULLET;
class PLAYER;
class ENEMY;

class PLAYERBULLET : public GAMEOBJ {
public:
	virtual void Update() override;
	virtual void Draw() override;
};

class PLAYER : public GAMEOBJ {
private:
	const char* Keys_{ nullptr };
	float Speed_{ 7.5f };
	float HP_{ 10.0f };

	LinkedList<PLAYERBULLET> Bullets_{ 256 };

	void UpdateVel();
	void Shoot();
	void UpdateBullets();

	unsigned int ColorList_[48]{};

	TraceEffectEmitter* TraceEmitter_{ nullptr };

public:
	void SetKeys(const char* keys_) { Keys_ = keys_; }
	void SetSpeed(float speed_) { Speed_ = speed_ * (speed_ >= 0.0f) + Speed_ * (speed_ < 0.0f); }
	float GetSpeed() const { return Speed_; }
	void ChangeHP(float hp_) { HP_ += hp_; }
	float GetHP() const { return HP_; }

	void CheckCollisionBetweenPlayerBulletsAndEnemy(ENEMY& enemy_);

	virtual void Update() override;
	virtual void Draw() override;

	PLAYER();
	virtual ~PLAYER() {
		if (TraceEmitter_ != nullptr) {
			delete TraceEmitter_;
			TraceEmitter_ = nullptr;
		}
	};
};

class ENEMY : public GAMEOBJ {
private:
	unsigned int ColorList_[24]{};

	float HP_{ 10.0f };
	int RespawnCountDown_{ 180 };

public:
	void ChangeHP(float hp_) { HP_ += hp_; }
	float GetHP() const { return HP_; }

	virtual void Update() override;
	virtual void Draw() override;

	ENEMY();
	virtual ~ENEMY() = default;
};