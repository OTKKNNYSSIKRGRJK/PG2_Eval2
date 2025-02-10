#include"Utilities.h"
#include"GameObjects.h"
#include<Novice.h>

void DrawStar(const VEC2& pos_, float size_, float angle_, unsigned int rgba_) {
	static VEC2 verts[10]{
		{ std::cos(0.0f * Pi), std::sin(0.0f * Pi) }, { std::cos(0.2f * Pi), std::sin(0.2f * Pi) },
		{ std::cos(0.4f * Pi), std::sin(0.4f * Pi) }, { std::cos(0.6f * Pi), std::sin(0.6f * Pi) },
		{ std::cos(0.8f * Pi), std::sin(0.8f * Pi) }, { std::cos(1.0f * Pi), std::sin(1.0f * Pi) },
		{ std::cos(1.2f * Pi), std::sin(1.2f * Pi) }, { std::cos(1.4f * Pi), std::sin(1.4f * Pi) },
		{ std::cos(1.6f * Pi), std::sin(1.6f * Pi) }, { std::cos(1.8f * Pi), std::sin(1.8f * Pi) },
	};
	static float rads[2]{ 1.0f, 0.6f };
	static VEC2 ptvs[12]{ {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, };
	for (int i{ 0 }; i < 10; ++i) {
		ptvs[i] = (rads[i & 1] * size_) * verts[i];
		ptvs[i] = ptvs[i].Rotate(angle_);
		ptvs[i] += pos_;
	}
	ptvs[10] = ptvs[0];
	ptvs[11] = pos_;

	for (int i{ 0 }; i < 5; ++i) {
		int r = std::clamp<int>((rgba_ >> 24) + static_cast<int>(Rnd0To1() * 60.0f - 30.0f), 0, 255);
		int g = std::clamp<int>(((rgba_ >> 16) & 255) + static_cast<int>(Rnd0To1() * 60.0f - 30.0f), 0, 255);
		int b = std::clamp<int>(((rgba_ >> 8) & 255) + static_cast<int>(Rnd0To1() * 60.0f - 30.0f), 0, 255);
		int rgba = (r << 24) + (g << 16) + (b << 8) + (rgba_ & 255);
		Novice::DrawQuad(
			static_cast<int>(std::roundf(ptvs[i * 2 + 2].x)),
			static_cast<int>(std::roundf(ptvs[i * 2 + 2].y)),
			static_cast<int>(std::roundf(ptvs[i * 2 + 1].x)),
			static_cast<int>(std::roundf(ptvs[i * 2 + 1].y)),
			static_cast<int>(std::roundf(ptvs[11].x)),
			static_cast<int>(std::roundf(ptvs[11].y)),
			static_cast<int>(std::roundf(ptvs[i * 2].x)),
			static_cast<int>(std::roundf(ptvs[i * 2].y)),
			0, 0, 1, 1, 0, rgba
		);
	}
}