#include <Novice.h>
#include"GameObjects2.h"
#include"Effect.h"

namespace {
	const char kWindowTitle[] = "PG2 評価課題2";

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
}

class SCENE {
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~SCENE() = default;
};

class TITLESCENE;
class GAMEOVERSCENE;
class GAMESCENE;

namespace {
	SCENE* currentScene{ nullptr };
}

class TITLESCENE : public SCENE {
public:
	virtual void Update() override;
	virtual void Draw() override;

	virtual ~TITLESCENE() = default;
};

class GAMEOVERSCENE : public SCENE {
public:
	virtual void Update() override;
	virtual void Draw() override;

	virtual ~GAMEOVERSCENE() = default;
};

class GAMECLEARSCENE : public SCENE {
public:
	virtual void Update() override;
	virtual void Draw() override;

	virtual ~GAMECLEARSCENE() = default;
};

class GAMESCENE : public SCENE {
private:
	PLAYER player{};
	ENEMY enemies[3]{};

	int FramesLeft_{ 1800 };

	void CheckCollisionBetweenPlayerAndEnemies() {
		for (int i = 0; i < 3; ++i) {
			VEC2 d = {
				enemies[i].GetPos().x - player.GetPos().x,
				enemies[i].GetPos().y - player.GetPos().y
			};
			float d2 = player.GetSize() + enemies[i].GetSize();
			if (d.x * d.x + d.y * d.y <= d2 * d2) {
				player.ChangeHP(-0.05f);
			}
		}
	}

	void CheckCollisionBetweenPlayerBulletsAndEnemies() {
		for (int i = 0; i < 3; ++i) {
			if (enemies[i].GetHP() > 0.0f) {
				player.CheckCollisionBetweenPlayerBulletsAndEnemy(enemies[i]);
			}
		}
	}

public:
	GAMESCENE() {
		player.SetKeys(keys);
	}
	virtual ~GAMESCENE() {
		ClearEffects();
	}

	virtual void Update() override {
		if (FramesLeft_ > 0) {
			player.Update();
			for (int i = 0; i < 3; ++i) {
				enemies[i].Update();
			}
			UpdateEffects();
			CheckCollisionBetweenPlayerAndEnemies();
			CheckCollisionBetweenPlayerBulletsAndEnemies();

			--FramesLeft_;

			if (player.GetHP() <= 0.0f) {
				delete currentScene;
				currentScene = new GAMEOVERSCENE{};
				currentScene->Update();
			}
		}
		else {
			delete currentScene;
			currentScene = new GAMECLEARSCENE{};
			currentScene->Update();
		}
	}

	virtual void Draw() override {
		Novice::SetBlendMode(kBlendModeAdd);
		player.Draw();
		for (int i = 0; i < 3; ++i) {
			if (enemies[i].GetHP() > 0.0f) {
				enemies[i].Draw();
			}
		}
		DrawEffects();

		Novice::ScreenPrintf(10, 10, "Player HP : %.3f", player.GetHP());
		Novice::ScreenPrintf(10, 50, "Enemy0 HP : %.3f", enemies[0].GetHP());
		Novice::ScreenPrintf(10, 70, "Enemy1 HP : %.3f", enemies[1].GetHP());
		Novice::ScreenPrintf(10, 90, "Enemy2 HP : %.3f", enemies[2].GetHP());

		static const float inv60 = 1.0f / 60.0f;
		Novice::ScreenPrintf(10, 130, "Time Left : %.3f", static_cast<float>(FramesLeft_) * inv60);
	}
};

void TITLESCENE::Update() {
	if (preKeys[DIK_A] == 0 && keys[DIK_A] != 0) {
		delete currentScene;
		currentScene = new GAMESCENE{};
		currentScene->Update();
	}
}
void TITLESCENE::Draw() {
	Novice::ScreenPrintf(100, 100, "Title");
	Novice::ScreenPrintf(100, 120, "Press A Key to Start");
}

void GAMEOVERSCENE::Update() {
	if (preKeys[DIK_A] == 0 && keys[DIK_A] != 0) {
		delete currentScene;
		currentScene = new GAMESCENE{};
		currentScene->Update();
	}
}
void GAMEOVERSCENE::Draw() {
	Novice::ScreenPrintf(100, 100, "Game Over");
	Novice::ScreenPrintf(100, 120, "Press A Key to Restart");
}

void GAMECLEARSCENE::Update() {
	if (preKeys[DIK_A] == 0 && keys[DIK_A] != 0) {
		delete currentScene;
		currentScene = new GAMESCENE{};
		currentScene->Update();
	}
}
void GAMECLEARSCENE::Draw() {
	Novice::ScreenPrintf(100, 100, "Game Clear!");
	Novice::ScreenPrintf(100, 120, "Press A Key to Restart");
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	currentScene = new TITLESCENE{};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		currentScene->Update();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		currentScene->Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete currentScene;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
