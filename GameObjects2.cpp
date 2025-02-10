#include"GameObjects2.h"
#include<Novice.h>
#include<cmath>
#include<algorithm>
#include"Utilities.h"

void PLAYERBULLET::Update() {
	Pos_ += Vel_;
	Angle_ += AngularVel_;

	float rgb[3]{};
	HSV2RGB(rgb, Pos_.x * 0.1f + 120.0f, 0.25f, 0.75f);
	Color_ = GetRGBA(RGB2UINT(rgb), 192);
}

void PLAYERBULLET::Draw() {
	DrawStar(Pos_, Size_, Angle_, Color_);
	/*Novice::DrawEllipse(
		static_cast<int>(Pos_.x),
		static_cast<int>(Pos_.y),
		static_cast<int>(Size_), static_cast<int>(Size_), 0.0f,
		Color_, kFillModeSolid
	);*/
}

void PLAYER::UpdateVel() {
	static VEC2 ds[16]{
		{ 0.0f, 0.0f },		{ 1.0f, 0.0f },				{ -1.0f, 0.0f },			{ 0.0f, 0.0f },
		{ 0.0f, 1.0f },		{ Inv_Sqrt2, Inv_Sqrt2 },	{ -Inv_Sqrt2, Inv_Sqrt2 },	{ 0.0f, 1.0f },
		{ 0.0f, -1.0f },	{ Inv_Sqrt2, -Inv_Sqrt2 },	{ -Inv_Sqrt2, -Inv_Sqrt2 },	{ 0.0f, -1.0f },
		{ 0.0f, 0.0f },		{ 1.0f, 0.0f },				{ -1.0f, 0.0f },			{ 0.0f, 0.0f },
	};

	int i{ 8 * !!Keys_[DIK_UP] + 4 * !!Keys_[DIK_DOWN] + 2 * !!Keys_[DIK_LEFT] + !!Keys_[DIK_RIGHT] };
	Vel_ = Speed_ * ds[i];
}

void PLAYER::Shoot() {
	if (Keys_[DIK_SPACE]) {
		for (int i = 0; i < 4; ++i) {
			auto* pBullet = Bullets_.NewElement();
			if (pBullet != nullptr) {
				pBullet->SetPos({ Pos_.x + 22.5f * (i >> 1), Pos_.y + 8.0f * ((i & 1) * 2 - 1) });
				pBullet->SetVel({ 45.0f, 0.0f });
				pBullet->SetSize(12.5f + 5.0f * Rnd0To1());
				pBullet->SetAngularVel(Rnd0To1() * 0.1f);
			}
		}
	}
}

void PLAYER::UpdateBullets() {
	Shoot();

	decltype(Bullets_)::Iterator it{ &Bullets_ };
	for (it.Begin(); !it.End(); it.Next()) {
		if ((*it).BorderCheck()) {
			Bullets_.Delete(it);
		}
		else {
			(*it).Update();
		}
	}
}

void PLAYER::CheckCollisionBetweenPlayerBulletsAndEnemy(ENEMY& enemy_) {
	decltype(Bullets_)::Iterator it{ &Bullets_ };
	for (it.Begin(); !it.End(); it.Next()) {
		VEC2 d = {
			enemy_.GetPos().x - (*it).GetPos().x,
			enemy_.GetPos().y - (*it).GetPos().y
		};
		float d2 = (*it).GetSize() + enemy_.GetSize();
		if (d.x * d.x + d.y * d.y <= d2 * d2) {
			enemy_.ChangeHP(-0.01f);
			Bullets_.Delete(it);
		}
	}
}

void PLAYER::Update() {
	UpdateVel();
	UpdateBullets();

	Move();
	BorderCheck();

	TraceEmitter_->Update();
}

void PLAYER::Draw() {
	static const float inv48 = 1.0f / 48.0f;
	for (int i = 0; i < 48; ++i) {
		Novice::DrawTriangle(
			static_cast<int>(std::round(Pos_.x)),
			static_cast<int>(std::round(Pos_.y)),
			static_cast<int>(std::round(Pos_.x + std::cos(i * inv48 * Pi * 2.0f + Angle_) * Size_)),
			static_cast<int>(std::round(Pos_.y + std::sin(i * inv48 * Pi * 2.0f + Angle_) * Size_)),
			static_cast<int>(std::round(Pos_.x + std::cos((i + 1) * inv48 * Pi * 2.0f + Angle_) * Size_)),
			static_cast<int>(std::round(Pos_.y + std::sin((i + 1) * inv48 * Pi * 2.0f + Angle_) * Size_)),
			ColorList_[i], kFillModeSolid
		);
	}

	Angle_ += Rnd0To1() * 0.05f;

	decltype(Bullets_)::Iterator it{ &Bullets_ };
	for (it.Begin(); !it.End(); it.Next()) {
		(*it).Draw();
	}
}

PLAYER::PLAYER() {
	Pos_ = { 200.0f, 360.0f };
	Size_ = 45.0f;

	float rgb[3]{};
	for (int i = 0; i < 48; ++i) {
		HSV2RGB(rgb, Rnd0To1() * 120.0f + 120.0f, 0.25f, 0.75f);
		ColorList_[i] = GetRGBA(RGB2UINT(rgb), 255);
	}

	int hsvData[6]{ 210, 15, 55, 5, 85, 5 };
	TraceEmitter_ = new TraceEffectEmitter{ this, hsvData };
}


void ENEMY::Update() {
	if (HP_ > 0.0f) {
		if (Cnt % 12 == 0) {
			Vel_.x += Rnd0To1() * 3.0f - 1.0f;
			if (Vel_.x > 15.0f) { Vel_.x = 15.0f; }
			else if (Vel_.x < -15.0f) Vel_.x = -15.0f;
			Vel_.y += Rnd0To1() * 3.0f - 1.0f;
			if (Vel_.y > 15.0f) { Vel_.y = 15.0f; }
			else if (Vel_.y < -15.0f) Vel_.y = -15.0f;
		}
		Cnt++;

		Move();
		int borderFlag = BorderCheck();
		if (borderFlag & 1) { Vel_.x *= -1.0f; }
		if (borderFlag & 2) { Vel_.y *= -1.0f; }
	}
	else {
		if (RespawnCountDown_ <= 0) {
			HP_ = 10.0f;
			RespawnCountDown_ = 180;
		}
		--RespawnCountDown_;
	}
}

void ENEMY::Draw() {
	static const float inv24 = 1.0f / 24.0f;
	for (int i = 0; i < 24; ++i) {
		Novice::DrawTriangle(
			static_cast<int>(std::round(Pos_.x)),
			static_cast<int>(std::round(Pos_.y)),
			static_cast<int>(std::round(Pos_.x + std::cos(i * inv24 * Pi * 2.0f + Angle_) * Size_ * 0.8f)),
			static_cast<int>(std::round(Pos_.y + std::sin(i * inv24 * Pi * 2.0f + Angle_) * Size_ * 0.8f)),
			static_cast<int>(std::round(Pos_.x + std::cos((i + 1) * inv24 * Pi * 2.0f + Angle_ + 0.01f) * Size_)),
			static_cast<int>(std::round(Pos_.y + std::sin((i + 1) * inv24 * Pi * 2.0f + Angle_ + 0.01f) * Size_)),
			ColorList_[i], kFillModeSolid
		);
	}

	Angle_ += Rnd0To1() * 0.5f;
}

ENEMY::ENEMY() {
	Pos_ = { 800.0f, 360.0f };
	Size_ = 75.0f;

	float rgb[3]{};
	for (int i = 0; i < 24; ++i) {
		HSV2RGB(rgb, Rnd0To1() * 120.0f - 120.0f, 0.25f, 0.75f);
		ColorList_[i] = GetRGBA(RGB2UINT(rgb), 255);
	}
}