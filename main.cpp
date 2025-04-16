#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LC1A_10_シゲモリ_マサト_MT3";

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;
};

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	return {
		{
			{1, 0 ,0, 0},
			{0, 1 ,0, 0},
			{0, 0 ,1, 0},
			{translate.x, translate.y, translate.z, 1}
		}
	};
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	return{
		{
			{scale.x, 0 ,0, 0},
			{0, scale.y ,0, 0},
			{0, 0 ,scale.z, 0},
			{0, 0 ,0, 1}
		}
	};
}

Matrix4x4 MakeRotetaXMatrix(float radius) {
	return {
	1, 0, 0, 0 ,
	0, std::cosf(radius), std::sinf(radius), 0 ,
	0, -std::sinf(radius), std::cosf(radius), 0 ,
	0, 0, 0, 1
	};
}

Matrix4x4 MakeRotateYMatrix(float radius) {
	return {
		std::cosf(radius), 0, -std::sinf(radius), 0,
		0, 1, 0, 0,
		std::sinf(radius), 0, std::cosf(radius), 0,
		0, 0, 0, 1
	};
}

Matrix4x4 MakeRotateZMatrix(float radius) {
	return {
		std::cosf(radius), std::sinf(radius), 0, 0,
		-std::sinf(radius), std::cosf(radius), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = Multiply(MakeRotetaXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	return Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& mat, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 1; i < 5; ++i) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(x + j * 80, y + i * 20, "%.2f", mat.m[i - 1][j]);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 scale{ 1.2f, 0.79f, -2.1f };
	Vector3 rotate{ 0.4f, 1.43f, -0.8f };
	Vector3 translate{ 2.7f, -4.15f, 1.57f };
	Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, worldMatrix, "worldMatrix");

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

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
