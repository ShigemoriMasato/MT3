#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <imgui.h>

const char kWindowTitle[] = "LC1A_10_シゲモリ_マサト_MT3";

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x;
	float y;
	float z;

	float operator*(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vector3 operator*(const float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	Vector3 operator+(const Vector3& v) const {
		return { x + v.x, y + v.y, z + v.z };
	}

	Vector3 operator-(const Vector3& v) const {
		return { x - v.x, y - v.y, z - v.z };
	}

	float Length()const {
		return std::sqrtf(x * x + y * y + z * z);
	}
};

struct Sphere {
	Vector3 center;
	float radius;
	uint32_t subdivision;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

float cot(float radian) {
	return 1.0f / tanf(radian);
}

Vector3 cross(const Vector3& a, const Vector3& b) {
	return { b.y * a.z - b.z * a.y, b.z * a.x - b.x * a.z, b.x * a.y - b.y * a.x };;
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

//debug用
unsigned int ColorLarp(unsigned int color, unsigned int targetColor, float rate) {
	unsigned int r = (color >> 24) & 0xFF;
	unsigned int g = (color >> 16) & 0xFF;
	unsigned int b = (color >> 8) & 0xFF;
	unsigned int targetR = (targetColor >> 24) & 0xFF;
	unsigned int targetG = (targetColor >> 16) & 0xFF;
	unsigned int targetB = (targetColor >> 8) & 0xFF;
	r += int((targetR - r) * rate);
	g += int((targetG - g) * rate);
	b += int((targetB - b) * rate);
	return (r << 24) | (g << 16) | (b << 8) | 0xff;
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGrigHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGrigHalfWidth * 2.0f) / kSubdivision;
	//奥から手前
	for (uint32_t i = 0; i <= kSubdivision; ++i) {
		Vector3 start = { -kGrigHalfWidth, 0.0f, -kGrigHalfWidth + kGridEvery * i };
		Vector3 end = { kGrigHalfWidth, 0.0f, -kGrigHalfWidth + kGridEvery * i };
		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xaaaaaaff);
	}

	//左から右
	for (uint32_t i = 0; i <= kSubdivision; ++i) {
		Vector3 start = { -kGrigHalfWidth + kGridEvery * i, 0.0f, -kGrigHalfWidth };
		Vector3 end = { -kGrigHalfWidth + kGridEvery * i, 0.0f, kGrigHalfWidth };
		start = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xaaaaaaff);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = sphere.subdivision;
	const float kLonEvery = 2.0f * (float)M_PI / kSubdivision;
	const float kLatEvery = (float)M_PI / kSubdivision;
	//緯度方向に分解
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = (float) - M_PI / 2 + kLatEvery * latIndex;
		//経度方向に分解
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			Vector3 a, b, c;
			a = { sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon) };
			b = { sphere.center.x + sphere.radius * std::cosf(lat + kLatEvery) * std::cosf(lon),
				sphere.center.y + sphere.radius * std::sinf(lat + kLatEvery),
				sphere.center.z + sphere.radius * std::cosf(lat + kLatEvery) * std::sinf(lon) };
			c = { sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery),
				sphere.center.y + sphere.radius * std::sinf(lat),
				sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery) };

			a = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			b = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			c = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(b.x), int(b.y), int(c.x), int(c.y), color);

			//if (a * cross(b, c) <= 0) {
			//	Novice::DrawTriangle(int(a.x), int(a.y), int(b.x), int(b.y),
			//		int(c.x), int(c.y), color, kFillModeSolid);
			//}
		}
	}
}

namespace {
	void input(float* buffer, Vector3 vec) {
		buffer[0] = vec.x;
		buffer[1] = vec.y;
		buffer[2] = vec.z;
	}

	void output(float buffer[3], Vector3& vec) {
		vec.x = buffer[0];
		vec.y = buffer[1];
		vec.z = buffer[2];
	}
}

namespace ImGui {
	void SliderVector(const char* label, Vector3& vec, float v_min, float v_max) {
		float buffer[3];
		input(buffer, vec);
		if (ImGui::SliderFloat3(label, buffer, v_min, v_max)) {
			output(buffer, vec);
		}
	}
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 ans = v2 * ((v1 * v2) / (v2.Length() * v2.Length()));
	return ans;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 segmentVec = segment.diff - segment.origin;
	Vector3 startToPoint = point - segment.origin;
	float t = startToPoint * segmentVec / (segmentVec * segmentVec);
	
	return segment.origin + segmentVec * t;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 rotate{};
	Vector3 translate{};
	Vector3 cameraPosition{ 0.0f, 1.9f, -6.49f };
	Vector3 CameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 CameraScale{ 1.0f, 1.0f, 1.0f };
	Vector3 kLocalVertices[3] = {
		{ 0.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }
	};
	const float kSpeed = 0.1f;

	Segment segment{ {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f }};
	Vector3 point{ -1.5f, 0.6f, 0.6f };

	Vector3 project = Project(point - segment.origin, segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);

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

		ImGui::Begin("Camera");
		ImGui::SliderVector("Position", cameraPosition, -10.0f, 10.0f);
		ImGui::SliderVector("Rotate", CameraRotate, -3.14f, 3.14f);
		ImGui::SliderVector("Scale", CameraScale, 0.01f, 5.0f);

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(CameraScale, CameraRotate, cameraPosition);
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

		Matrix4x4 normalWVPMatrix = Multiply(MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, {}, {}), Multiply(viewMatrix, projectionMatrix));

		Vector3 start = Transform(Transform(segment.origin, normalWVPMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(segment.diff, normalWVPMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x00ff00ff);

		Sphere pointSphere{ point, 0.01f, 24 };
		Sphere closestPointSphere{ closestPoint, 0.01f, 24 };
		DrawSphere(pointSphere, normalWVPMatrix, viewportMatrix, 0xff0000ff);
		DrawSphere(closestPointSphere, normalWVPMatrix, viewportMatrix, 0x000000ff);

		DrawGrid(Multiply(MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, {}, {}), Multiply(viewMatrix, projectionMatrix)), viewportMatrix);

		//DrawSphere({ { 0.0f, 0.0f, 0.0f }, 1.0f, 24 }, Multiply(MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, {}, {}), Multiply(viewMatrix, projectionMatrix)), viewportMatrix, 0xff);

		/*if (screenVertices[0] * cross(screenVertices[1], screenVertices[2]) <= 0) {
			Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
				int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);
		}*/

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
