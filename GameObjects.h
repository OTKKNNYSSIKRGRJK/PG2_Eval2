#pragma once

#include<cmath>
#include<LinkedList.h>

struct VEC2 {
	float x;
	float y;

	VEC2& operator+=(const VEC2& v_) { x += v_.x; y += v_.y; return *this; }
	friend VEC2 operator*(float s_, const VEC2& v_);

	VEC2 Rotate(float theta_) const {
		return {
			x * std::cos(theta_) - y * std::sin(theta_),
			x * std::sin(theta_) + y * std::cos(theta_)
		};
	}
};

inline VEC2 operator*(float s_, const VEC2& v_) { return VEC2{ s_ * v_.x, s_ * v_.y }; }

class GAMEOBJ {
protected:
	VEC2 Pos_{ 0.0f, 0.0f };
	VEC2 Vel_{ 0.0f, 0.0f };
	float Size_{ 10.0f };
	float Angle_{ 0.0f };
	float AngularVel_{ 0.0f };
	unsigned int Color_{ 0xFFFFFFFF };

	int Cnt{ 0 };

	void Move() { Pos_ += Vel_; }

public:
	int BorderCheck() {
		int flag{ 0 };
		if (Pos_.x - Size_ < 0.0f) { Pos_.x = Size_; flag |= 1; }
		else if (Pos_.x + Size_ > 1280.0f) { Pos_.x = 1280.0f - Size_; flag |= 1; }
		if (Pos_.y - Size_ < 0.0f) { Pos_.y = Size_; flag |= 2; }
		else if (Pos_.y + Size_ > 720.0f) { Pos_.y = 720.0f - Size_; flag |= 2; }
		return flag;
	}

public:
	void SetPos(const VEC2& pos_) { Pos_ = pos_; }
	void SetVel(const VEC2& vel_) { Vel_ = vel_; }
	void SetSize(float size_) { Size_ = size_ * (size_ > 0.0f) + Size_ * (size_ <= 0.0f); }
	void SetAngle(float angle_) { Angle_ = angle_; }
	void SetAngularVel(float angularVel_) { AngularVel_ = angularVel_; }
	void SetColor(unsigned int color_) { Color_ = color_; }
	const VEC2& GetPos() const { return Pos_; }
	const VEC2& GetVel() const { return Vel_; }
	float GetAngle() const { return Angle_; }
	float GetSize() const { return Size_; }
	unsigned int GetColor() const { return Color_; }

public:
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~GAMEOBJ() = default;
};