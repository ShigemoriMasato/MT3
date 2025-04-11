#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LC1A_10_シゲモリ_マサト_MT3";

struct Vector3 {
	float x;
	float y;
	float z;
};

Vector3 Add(const Vector3 &vec1, const Vector3 &vec2) {
	Vector3 result;
	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	result.z = vec1.z + vec2.z;
	return result;
}

Vector3 Subtract(const Vector3 &vec1, const Vector3 vec2) {
	Vector3 result;
	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	result.z = vec1.z - vec2.z;
	return result;
}

Vector3 Multiply(const Vector3 &a, const float scalar) {
	Vector3 result;
	result.x = a.x * scalar;
	result.y = a.y * scalar;
	result.z = a.z * scalar;
	return result;
}

float Dot(const Vector3 &a, const Vector3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Length(const Vector3 &a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector3 Normalize(const Vector3 &a) {
	float length = Length(a);
	Vector3 result;
	result.x = a.x / length;
	result.y = a.y / length;
	result.z = a.z / length;
	return result;
}

void ScreenPrintVector3(const int x, const int y, const Vector3 &vec, const char *label) {
	Novice::ScreenPrintf(x, y, "%f	%f	%f	: %s", vec.x, vec.y, vec.z, label);
}

void ScreenPrintFloat(const int x, const int y, const float value, const char *label) {
	Novice::ScreenPrintf(x, y, "%f	: %s", value, label);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 vec1 = { 1.0f, 3.0f, -5.0f };
	Vector3 vec2 = { 4.0f, -1.0f, 2.0f };
	float scalar = 4.0f;

	Vector3 add = Add(vec1, vec2);
	Vector3 sub = Subtract(vec1, vec2);
	Vector3 mul = Multiply(vec1, scalar);
	float dot = Dot(vec1, vec2);
	float length = Length(vec1);
	Vector3 normalized = Normalize(vec1);

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

		ScreenPrintVector3(0, 0, add, "Add");
		ScreenPrintVector3(0, 20, sub, "Subtract");
		ScreenPrintVector3(0, 40, mul, "Multiply");
		ScreenPrintFloat(0, 60, dot, "Dot");
		ScreenPrintFloat(0, 80, length, "Length");
		ScreenPrintVector3(0, 100, normalized, "Normalize");

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
