#include"Effect.h"
#include"GameObjects.h"
#include"Utilities.h"
#include<Novice.h>
#include"LinkedList.h"

namespace {
	LinkedList<Effect> Effects_{ 2048 };
}

void Effect::Update() {
	Pos_ += Vel_;
	Angle_ += AngularVel_;

	--LifeLeft_;
}

void Effect::Draw() {
	float lifeLeftRatio = static_cast<float>(LifeLeft_) / static_cast<float>(Life_);
	int alpha = (int)(lifeLeftRatio * Alpha_);
	float size = Size_ * lifeLeftRatio;
	switch (Shape) {
	case 0:
		Novice::DrawEllipse(
			static_cast<int>(Pos_.x),
			static_cast<int>(Pos_.y),
			static_cast<int>(size), static_cast<int>(size), 0.0f,
			((Color_ >> 8) << 8) + alpha, kFillModeSolid
		);
		break;

	case 1:
		DrawStar(Pos_, size, Angle_, ((Color_ >> 8) << 8) + alpha);
		break;
	}
}

TraceEffectEmitter::TraceEffectEmitter(GAMEOBJ* attached, int hsvData[6]) : EffectEmitter(attached) {
	for (int i = 0; i < 6; ++i) {
		PalatteHSVData[i] = hsvData[i];
	}
}

void TraceEffectEmitter::Update() {
	float rgb[3] = {};
	HSV2RGB(
		rgb,
		(PalatteHSVData[0] - PalatteHSVData[1]) + rndEngine() % (PalatteHSVData[1] * 2) * 1.0f,
		((PalatteHSVData[2] - PalatteHSVData[3]) + rndEngine() % (PalatteHSVData[3] * 2)) * 0.01f,
		((PalatteHSVData[4] - PalatteHSVData[5]) + rndEngine() % (PalatteHSVData[5] * 2)) * 0.01f
	);
	unsigned int rgbUINT = RGB2UINT(rgb);

	auto* eCircle = Effects_.NewElement();
	{
		eCircle->SetShape(0);
		VEC2 posCircle = Attached->GetPos();
		posCircle.x += Rnd0To1() * 15.0f - 7.5f;
		posCircle.y += Rnd0To1() * 15.0f - 7.5f;
		eCircle->SetPos(posCircle);
		eCircle->SetVel({ Rnd0To1() * 3.0f - 1.5f, Rnd0To1() * 3.0f - 1.5f });
		eCircle->SetColor((rgbUINT << 8) + 192);
		eCircle->SetSize(Rnd0To1() * 5.0f + 10.0f);
		eCircle->SetAngle(0.0f);
		eCircle->SetAngularVel(0.0f);
		eCircle->SetLife(rndEngine() % 10 + 30);
	}

	auto* eStar = Effects_.NewElement();
	{
		eStar->SetShape(1);
		VEC2 posStar = Attached->GetPos();
		posStar.x += Rnd0To1() * 25.0f - 12.5f;
		posStar.y += Rnd0To1() * 25.0f - 12.5f;
		eStar->SetPos(posStar);
		eStar->SetVel({ Rnd0To1() * 5.0f - 2.5f, Rnd0To1() * 5.0f - 2.5f });
		eStar->SetColor((rgbUINT << 8) + 192);
		eStar->SetSize(Rnd0To1() * 10.0f + 10.0f);
		eStar->SetAngle(Rnd0To1() * 128.0f);
		eStar->SetAngularVel(Rnd0To1() * 0.3f - 0.15f);
		eStar->SetLife(rndEngine() % 10 + 60);
	}
}


CollisionEffectEmitter::CollisionEffectEmitter(GAMEOBJ* attached, int hsvData[6]) : EffectEmitter(attached) {
	for (int i = 0; i < 6; ++i) {
		PalatteHSVData[i] = hsvData[i];
	}
}

void CollisionEffectEmitter::Update() {
	float rgb[3]{};
	VEC2 vel{};

	for (int i = 0; i < 30; ++i) {
		HSV2RGB(rgb, Rnd0To1() * 360.0f, 0.1f, 0.9f);

		vel.x = Rnd0To1() * 3.0f + 1.0f;
		vel.y = 0.0f;
		vel = vel.Rotate(Rnd0To1() * 2.0f * Pi);

		auto* eStar = Effects_.NewElement();
		{
			eStar->SetShape(1);
			VEC2 posStar = Attached->GetPos();
			posStar.x += Rnd0To1() * 15.0f - 7.5f;
			posStar.y += Rnd0To1() * 15.0f - 7.5f;
			eStar->SetPos(posStar);
			eStar->SetVel({ Rnd0To1() * 5.0f - 2.5f, Rnd0To1() * 5.0f - 2.5f });
			eStar->SetColor((RGB2UINT(rgb) << 8) + 192);
			eStar->SetSize(Rnd0To1() * 10.0f + 10.0f);
			eStar->SetAngle(Rnd0To1() * 128.0f);
			eStar->SetAngularVel(Rnd0To1() * 0.3f - 0.15f);
			eStar->SetLife(rndEngine() % 30 + 60);
		}
	}
}

void UpdateEffects() {
	decltype(Effects_)::Iterator it{ &Effects_ };
	for (it.Begin(); !it.End(); it.Next()) {
		if ((*it).GetLifeLeft() <= 0) {
			Effects_.Delete(it);
		}
		else {
			(*it).Update();
		}
	}
}

void DrawEffects() {
	decltype(Effects_)::Iterator it{ &Effects_ };
	for (it.Begin(); !it.End(); it.Next()) {
		(*it).Draw();
	}
}

void ClearEffects() {
	decltype(Effects_)::Iterator it{ &Effects_ };
	for (it.Begin(); !it.End(); it.Next()) {
		Effects_.Delete(it);
	}
}