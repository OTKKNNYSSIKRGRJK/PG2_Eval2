#pragma once

#include<cmath>
#include<random>
#include"GameObjects.h"

inline const float Inv_Sqrt2 = std::sqrt(0.5f);
inline constexpr float Pi = 3.14159265f;

inline std::mt19937 rndEngine{ std::random_device{}() };

inline float Rnd0To1() {
	return (rndEngine() & 65535) * 0.0000152587890625f;
}

inline void HSV2RGB(float rgb_[3], float h_, float s_, float v_) {
	static constexpr float Inv_60{ 1.0f / 60.0f };
	static float CX0[3]{ 0.0f, 0.0f, 0.0f, };
	static constexpr int CX0Indices[6][3]{
		{ 0, 1, 2, }, { 1, 0, 2, }, { 2, 0, 1, }, { 2, 1, 0, }, { 1, 2, 0, }, { 0, 2, 1, },
	};

	while (h_ >= 360.0f) { h_ -= 360.0f; }
	while (h_ < 0.0f) { h_ += 360.0f; }

	// Find the chroma, which would be the largest RGB component.
	CX0[0] = s_ * v_;
	float tmp{ h_ * Inv_60 };
	while (tmp > 2.0f) { tmp -= 2.0f; }
	// Find the intermediate value for the second-largest RGB component.
	CX0[1] = CX0[0] * (1.0f - std::abs(tmp - 1.0f));
	float m{ v_ - CX0[0] };

	int hueSection{ static_cast<int>(std::floor(h_ * Inv_60)) };
	hueSection = (hueSection + 6) % 6;
	rgb_[0] = CX0[CX0Indices[hueSection][0]] + m;
	rgb_[1] = CX0[CX0Indices[hueSection][1]] + m;
	rgb_[2] = CX0[CX0Indices[hueSection][2]] + m;
}

inline unsigned int RGB2UINT(const float rgb_[3]) {
	return
		(static_cast<unsigned int>(rgb_[0] * 255.0f) << 16) +
		(static_cast<unsigned int>(rgb_[1] * 255.0f) << 8) +
		static_cast<unsigned int>(rgb_[2] * 255.0f);
}

inline unsigned int GetRGBA(unsigned int uintRGB_, unsigned int uintA_) {
	return (uintRGB_ << 8) + uintA_;
}

void DrawStar(const VEC2& pos_, float size_, float angle_, unsigned int rgba_);