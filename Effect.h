#pragma once

#include"GameObjects.h"

class Effect : public GAMEOBJ {
protected:
	int Shape;

	float Alpha_;

	int Life_;
	int LifeLeft_;

public:
	Effect() : Shape(0), Alpha_(192.0), Life_(30), LifeLeft_(6) {
		Size_ = 5.0f;
		Angle_ = 0.0f;
		AngularVel_ = 0.0f;
	}
	Effect(float size_, int life_) : Shape(0), Alpha_(192.0), Life_(life_), LifeLeft_(life_ + 1) {
		Size_ = size_;
		Angle_ = 0.0f;
		AngularVel_ = 0.0f;
	}
	~Effect() {}

	inline void SetShape(int shape) {
		Shape = shape;
	}

	inline void SetLife(int life) {
		Life_ = life;
		LifeLeft_ = life;
	}

	inline int GetLifeLeft() const {
		return LifeLeft_;
	}

	virtual void Update() override;
	virtual void Draw() override;
};

class EffectEmitter {
protected:
	GAMEOBJ* Attached;

public:
	EffectEmitter(GAMEOBJ* attached) : Attached(attached) {}
	virtual ~EffectEmitter() = default;

	virtual void Update() = 0;
};

class TraceEffectEmitter : public EffectEmitter {
private:
	int PalatteHSVData[6];

public:
	TraceEffectEmitter(GAMEOBJ* attached, int hsvData[6]);
	virtual ~TraceEffectEmitter() = default;

	virtual void Update() override;
};

class CollisionEffectEmitter : public EffectEmitter {
private:
	int PalatteHSVData[6];

public:
	CollisionEffectEmitter(GAMEOBJ* attached, int hsvData[6]);
	virtual ~CollisionEffectEmitter() = default;

	void Update() override;
};

void UpdateEffects();
void DrawEffects();
void ClearEffects();