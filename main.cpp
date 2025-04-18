#include <Novice.h>
#include <cmath>
#include <cassert>

const char kWindowTitle[] = "LC1A_10_シゲモリ_マサト_MT3";

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;
};

float cot(float radian) {
	return 1.0f / tanf(radian);
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	return {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1
	};

}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	return {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};

}

Matrix4x4 MakeRotetaXMatrix(float radius) {

	return {
		1, 0, 0, 0,
		0, std::cosf(radius), std::sinf(radius), 0,
		0, -std::sinf(radius), std::cosf(radius), 0,
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

	return { std::cosf(radius), std::sinf(radius), 0, 0,
		-std::sinf(radius), std::cosf(radius), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };

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

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	return {
		cot(fovY / 2) / aspectRatio, 0, 0, 0,
		0, cot(fovY / 2), 0, 0,
		0, 0, farClip / (farClip - nearClip), 1,
		0, 0, (-nearClip * farClip) / (farClip - nearClip), 0
	};
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	return {
		2 / (right - left), 0, 0, 0,
		0, 2 / (top - bottom), 0, 0,
		0, 0, 1 / (farClip - nearClip), 0,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1
	};
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	return {
		width / 2, 0, 0, 0,
		0, -height / 2, 0, 0,
		0, 0, maxDepth - minDepth, 0,
		left + width / 2, top + height / 2, minDepth, 1
	};
}

Matrix4x4 Inverse(const Matrix4x4& a) {

	float d = a.m[0][0] * a.m[1][1] * a.m[2][2] * a.m[3][3]
		+ a.m[0][0] * a.m[1][2] * a.m[2][3] * a.m[3][1]
		+ a.m[0][0] * a.m[1][3] * a.m[2][1] * a.m[3][2]
		- a.m[0][0] * a.m[1][3] * a.m[2][2] * a.m[3][1]
		- a.m[0][0] * a.m[1][2] * a.m[2][1] * a.m[3][3]
		- a.m[0][0] * a.m[1][1] * a.m[2][3] * a.m[3][2]
		- a.m[0][1] * a.m[1][0] * a.m[2][2] * a.m[3][3]
		- a.m[0][2] * a.m[1][0] * a.m[2][3] * a.m[3][1]
		- a.m[0][3] * a.m[1][0] * a.m[2][1] * a.m[3][2]
		+ a.m[0][3] * a.m[1][0] * a.m[2][2] * a.m[3][1]
		+ a.m[0][2] * a.m[1][0] * a.m[2][1] * a.m[3][3]
		+ a.m[0][1] * a.m[1][0] * a.m[2][3] * a.m[3][2]
		+ a.m[0][1] * a.m[1][2] * a.m[2][0] * a.m[3][3]
		+ a.m[0][2] * a.m[1][3] * a.m[2][0] * a.m[3][1]
		+ a.m[0][3] * a.m[1][1] * a.m[2][0] * a.m[3][2]
		- a.m[0][3] * a.m[1][2] * a.m[2][0] * a.m[3][1]
		- a.m[0][2] * a.m[1][1] * a.m[2][0] * a.m[3][3]
		- a.m[0][1] * a.m[1][3] * a.m[2][0] * a.m[3][2]
		- a.m[0][1] * a.m[1][2] * a.m[2][3] * a.m[3][0]
		- a.m[0][2] * a.m[1][3] * a.m[2][1] * a.m[3][0]
		- a.m[0][3] * a.m[1][1] * a.m[2][2] * a.m[3][0]
		+ a.m[0][3] * a.m[1][2] * a.m[2][1] * a.m[3][0]
		+ a.m[0][2] * a.m[1][1] * a.m[2][3] * a.m[3][0]
		+ a.m[0][1] * a.m[1][3] * a.m[2][2] * a.m[3][0];

	assert(fabsf(d) > 1e-6f);

	return {
		(a.m[1][1] * a.m[2][2] * a.m[3][3] + a.m[1][2] * a.m[2][3] * a.m[3][1] + a.m[1][3] * a.m[2][1] * a.m[3][2]
			- a.m[1][3] * a.m[2][2] * a.m[3][1] - a.m[1][2] * a.m[2][1] * a.m[3][3] - a.m[1][1] * a.m[2][3] * a.m[3][2]) / d,
		-(a.m[0][1] * a.m[2][2] * a.m[3][3] + a.m[0][2] * a.m[2][3] * a.m[3][1] + a.m[0][3] * a.m[2][1] * a.m[3][2]
			- a.m[0][3] * a.m[2][2] * a.m[3][1] - a.m[0][2] * a.m[2][1] * a.m[3][3] - a.m[0][1] * a.m[2][3] * a.m[3][2]) / d,
		(a.m[0][1] * a.m[1][2] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[3][1] + a.m[0][3] * a.m[1][1] * a.m[3][2]
			- a.m[0][3] * a.m[1][2] * a.m[3][1] - a.m[0][2] * a.m[1][1] * a.m[3][3] - a.m[0][1] * a.m[1][3] * a.m[3][2]) / d,
		-(a.m[0][1] * a.m[1][2] * a.m[2][3] + a.m[0][2] * a.m[1][3] * a.m[2][1] + a.m[0][3] * a.m[1][1] * a.m[2][2]
			- a.m[0][3] * a.m[1][2] * a.m[2][1] - a.m[0][2] * a.m[1][1] * a.m[2][3] - a.m[0][1] * a.m[1][3] * a.m[2][2]) / d,

		-(a.m[1][0] * a.m[2][2] * a.m[3][3] + a.m[1][2] * a.m[2][3] * a.m[3][0] + a.m[1][3] * a.m[2][0] * a.m[3][2]
			- a.m[1][3] * a.m[2][2] * a.m[3][0] - a.m[1][2] * a.m[2][0] * a.m[3][3] - a.m[1][0] * a.m[2][3] * a.m[3][2]) / d,
		(a.m[0][0] * a.m[2][2] * a.m[3][3] + a.m[0][2] * a.m[2][3] * a.m[3][0] + a.m[0][3] * a.m[2][0] * a.m[3][2]
			- a.m[0][3] * a.m[2][2] * a.m[3][0] - a.m[0][2] * a.m[2][0] * a.m[3][3] - a.m[0][0] * a.m[2][3] * a.m[3][2]) / d,
		-(a.m[0][0] * a.m[1][2] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[3][0] + a.m[0][3] * a.m[1][0] * a.m[3][2]
			- a.m[0][3] * a.m[1][2] * a.m[3][0] - a.m[0][2] * a.m[1][0] * a.m[3][3] - a.m[0][0] * a.m[1][3] * a.m[3][2]) / d,
		(a.m[0][0] * a.m[1][2] * a.m[2][3] + a.m[0][2] * a.m[1][3] * a.m[2][0] + a.m[0][3] * a.m[1][0] * a.m[2][2]
			- a.m[0][3] * a.m[1][2] * a.m[2][0] - a.m[0][2] * a.m[1][0] * a.m[2][3] - a.m[0][0] * a.m[1][3] * a.m[2][2]) / d,

		(a.m[1][0] * a.m[2][1] * a.m[3][3] + a.m[1][1] * a.m[2][3] * a.m[3][0] + a.m[1][3] * a.m[2][0] * a.m[3][1]
			- a.m[1][3] * a.m[2][1] * a.m[3][0] - a.m[1][1] * a.m[2][0] * a.m[3][3] - a.m[1][0] * a.m[2][3] * a.m[3][1]) / d,
		-(a.m[0][0] * a.m[2][1] * a.m[3][3] + a.m[0][1] * a.m[2][3] * a.m[3][0] + a.m[0][3] * a.m[2][0] * a.m[3][1]
			- a.m[0][3] * a.m[2][1] * a.m[3][0] - a.m[0][1] * a.m[2][0] * a.m[3][3] - a.m[0][0] * a.m[2][3] * a.m[3][1]) / d,
		(a.m[0][0] * a.m[1][1] * a.m[3][3] + a.m[0][1] * a.m[1][3] * a.m[3][0] + a.m[0][3] * a.m[1][0] * a.m[3][1]
			- a.m[0][3] * a.m[1][1] * a.m[3][0] - a.m[0][1] * a.m[1][0] * a.m[3][3] - a.m[0][0] * a.m[1][3] * a.m[3][1]) / d,
		-(a.m[0][0] * a.m[1][1] * a.m[2][3] + a.m[0][1] * a.m[1][3] * a.m[2][0] + a.m[0][3] * a.m[1][0] * a.m[2][1]
			- a.m[0][3] * a.m[1][1] * a.m[2][0] - a.m[0][1] * a.m[1][0] * a.m[2][3] - a.m[0][0] * a.m[1][3] * a.m[2][1]) / d,

		-(a.m[1][0] * a.m[2][1] * a.m[3][2] + a.m[1][1] * a.m[2][2] * a.m[3][0] + a.m[1][2] * a.m[2][0] * a.m[3][1]
			- a.m[1][2] * a.m[2][1] * a.m[3][0] - a.m[1][1] * a.m[2][0] * a.m[3][2] - a.m[1][0] * a.m[2][2] * a.m[3][1]) / d,
		(a.m[0][0] * a.m[2][1] * a.m[3][2] + a.m[0][1] * a.m[2][2] * a.m[3][0] + a.m[0][2] * a.m[2][0] * a.m[3][1]
			- a.m[0][2] * a.m[2][1] * a.m[3][0] - a.m[0][1] * a.m[2][0] * a.m[3][2] - a.m[0][0] * a.m[2][2] * a.m[3][1]) / d,
		-(a.m[0][0] * a.m[1][1] * a.m[3][2] + a.m[0][1] * a.m[1][2] * a.m[3][0] + a.m[0][2] * a.m[1][0] * a.m[3][1]
			- a.m[0][2] * a.m[1][1] * a.m[3][0] - a.m[0][1] * a.m[1][0] * a.m[3][2] - a.m[0][0] * a.m[1][2] * a.m[3][1]) / d,
		(a.m[0][0] * a.m[1][1] * a.m[2][2] + a.m[0][1] * a.m[1][2] * a.m[2][0] + a.m[0][2] * a.m[1][0] * a.m[2][1]
			- a.m[0][2] * a.m[1][1] * a.m[2][0] - a.m[0][1] * a.m[1][0] * a.m[2][2] - a.m[0][0] * a.m[1][2] * a.m[2][1]) / d
	};
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 ans;
	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 rotate{};
	Vector3 translate{};
	Vector3 cameraPosition{ 0.0f, 0.0f, -50.0f };
	Vector3 kLocalVertices[3] = {
		{ 0.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }
	};
	const float kSpeed = 0.1f;

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

		if (keys[DIK_W]) {
			translate.y += kSpeed;
		}

		if (keys[DIK_S]) {
			translate.y -= kSpeed;
		}

		if (keys[DIK_A]) {
			translate.x -= kSpeed;
		}

		if (keys[DIK_D]) {
			translate.x += kSpeed;
		}

		rotate.y += 0.1f;

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, { 0.0f, 0.0f, 0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; i++) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);

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
