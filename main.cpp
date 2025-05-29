#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <imgui.h>
#include "MyMath.h"

using namespace MyMath;

const char kWindowTitle[] = "LE2A_06_シゲモリ_マサト_MT3";

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

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 ans = v2 * ((v1 * v2) / (v2.Length() * v2.Length()));
	return ans;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 segmentVec = segment.diff - segment.origin;
	Vector3 startToPoint = point - segment.origin;
	float t = dot(startToPoint, segmentVec / (segmentVec * segmentVec));
	
	return segment.origin + segmentVec * t;
}

bool IsCollition(const Sphere& sphere, const Plane& plane) {
	float distance = dot(sphere.center, plane.normal) - plane.distance;
	if (distance < sphere.radius && -sphere.radius < distance) {
		return true;
	}
	return false;
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

void MakeTrianglePoint(Triangle& triangle, const Plane& plane) {
	Vector3 center = plane.normal * plane.distance;
	Vector3 perpendiculars[3];
	perpendiculars[0] = Perpendicular(plane.normal).Normalize();
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = cross(plane.normal, perpendiculars[0]);

	for (int32_t i = 0; i < 3; ++i) {
		Vector3 extend = perpendiculars[i] * 2.0f;
		Vector3 point = center + extend;
		triangle.vertices[i] = point;
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = plane.normal * plane.distance;
	Vector3 perpendiculars[4];
	perpendiculars[0] = Perpendicular(plane.normal).Normalize();
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t i = 0; i < 4; ++i) {
		Vector3 extend = perpendiculars[i] * 2.0f;
		Vector3 point = center + extend;
		points[i] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	//Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	//↓これの0を3に書き換えると四角形になる
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = segment.origin * viewProjectionMatrix * viewportMatrix;
	Vector3 end = (segment.origin + segment.diff) * viewProjectionMatrix * viewportMatrix;
	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}

bool IsCollition(const Segment& segment, const Plane& plane) {
	float bn = dot(segment.diff, plane.normal);
	if (bn == 0.0f) {
		return false;
	}
	float on = dot(segment.origin, plane.normal);
	float t = (plane.distance - on) / bn;
	if (t < 0.0f || t > 1.0f) {
		return false;
	}
	return true;
}

bool IsCollition(const Segment& segment, const Triangle& triangle) {
	Plane plane;
	plane.normal = cross(triangle.vertices[1] - triangle.vertices[0], triangle.vertices[2] - triangle.vertices[1]).Normalize();
	plane.distance = dot(plane.normal, triangle.vertices[0]);

	float bn = dot(segment.diff, plane.normal);
	if (bn == 0.0f) {
		return false;
	}
	float on = dot(segment.origin, plane.normal);
	float t = (plane.distance - on) / bn;
	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	Vector3 hitpoint = segment.origin + segment.diff * t;
	
	Vector3 center = (triangle.vertices[0] + triangle.vertices[1] + triangle.vertices[2]) / 3.0f;
	Vector3 center01 = center - (triangle.vertices[0] + triangle.vertices[1]) / 2;
	Vector3 center12 = center - (triangle.vertices[1] + triangle.vertices[2]) / 2;
	Vector3 center20 = center - (triangle.vertices[2] + triangle.vertices[0]) / 2;
	Vector3 p01 = hitpoint - (triangle.vertices[0] + triangle.vertices[1]) / 2;
	Vector3 p12 = hitpoint - (triangle.vertices[1] + triangle.vertices[2]) / 2;
	Vector3 p20 = hitpoint - (triangle.vertices[2] + triangle.vertices[0]) / 2;
	
	float a = dot(center01.Normalize(), p01.Normalize());
	float b = dot(center12.Normalize(), p12.Normalize());
	float c = dot(center20.Normalize(), p20.Normalize());

	ImGui::Begin("Segment");
	ImGui::Text("hitpoint: (%.2f, %.2f, %.2f)", hitpoint.x, hitpoint.y, hitpoint.z);
	ImGui::Text("a: %.2f", a);
	ImGui::Text("b: %.2f", b);
	ImGui::Text("c: %.2f", c);
	ImGui::End();

	if (a >= 0.0f &&
		b >= 0.0f &&
		c >= 0.0f) {
		return true;
	}

	return false;
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	for(int i = 0; i < 3; ++i) {
		Vector3 start = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(triangle.vertices[(i + 1) % 3], viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 rotate{};
	Vector3 translate{0.0f, 0.0f, 0.0f};
	Vector3 cameraPosition{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 CameraScale{ 1.0f, 1.0f, 1.0f };
	Vector3 kLocalVertices[3] = {
		{ 0.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }
	};
	const float kSpeed = 0.1f;

	Segment segment{ {-1.0f, -1.0f, 0.5f}, {2.0f, 2.0f, 0.0f }};

	Sphere sphere{ { 0.0f, 0.0f, 0.0f }, 0.3f, 24 };
	unsigned int color = 0xffffffff;

	Triangle trianglePoint{};

	int click = 0;
	int preClick = 0;

	Vector2 defaultMousePosition{};
	bool a = false;

	Plane plane{ { 0.0f, 1.0f, 0.0f }, 0.0f };

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

		// マウスの状態を取得
		preClick = click;
		click = Novice::IsPressMouse(0);

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

		if (keys[DIK_Q]) {
			translate.z += kSpeed;
		}

		if (keys[DIK_E]) {
			translate.z -= kSpeed;
		}

		if (keys[DIK_LSHIFT] && click) {
			int x;
			int y;
			if (!preClick || !preKeys[DIK_LSHIFT]) {
				Novice::GetMousePosition(&x, &y);
				defaultMousePosition = { (float)x, (float)y };
			}
			Novice::GetMousePosition(&x, &y);
			Vector2 nowMousePosition = { (float)x, (float)y };

			Matrix4x4 cameraRotateMatrix = Multiply(Multiply(MakeRotateXMatrix(cameraRotate.x), MakeRotateYMatrix(cameraRotate.y)), MakeRotateZMatrix(cameraRotate.z));

			float buffer = (nowMousePosition.x - defaultMousePosition.x) * 0.01f;
			cameraPosition = cameraPosition + Transform({ buffer, 0, 0 }, cameraRotateMatrix);
			buffer = (nowMousePosition.y - defaultMousePosition.y) * 0.01f;
			cameraPosition = cameraPosition - Transform({ 0, buffer, 0 }, cameraRotateMatrix);

			defaultMousePosition = nowMousePosition;
		}

		if (keys[DIK_LCONTROL] && click) {
			int x;
			int y;

			if (a) {
				a = false;
			} else {
				a = true;
			}

			if (!preClick || !preKeys[DIK_LCONTROL]) {
				Novice::GetMousePosition(&x, &y);
				defaultMousePosition = { (float)x, (float)y };
			}
			Novice::GetMousePosition(&x, &y);
			Vector2 nowMousePosition = { (float)x, (float)y };

			cameraRotate.y = cameraRotate.y + (nowMousePosition.x - defaultMousePosition.x) * 0.003f;
			cameraRotate.x = cameraRotate.x + (nowMousePosition.y - defaultMousePosition.y) * 0.003f;

			defaultMousePosition = nowMousePosition;
		}

		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Position", &cameraPosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Rotate", &cameraRotate.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("Scale", &CameraScale.x, 0.01f, 5.0f);
		ImGui::SliderFloat3("PlaneNormal", &plane.normal.x, -1.0f, 1.0f);
		ImGui::SliderFloat("PlaneDistance", &plane.distance, -10.0f, 10.0f);
		ImGui::End();

		plane.normal.Normalize();

		Segment seg;
		seg.diff = segment.diff;
		seg.origin = segment.origin + translate;

		ImGui::Begin("Segment");
		ImGui::Text("Origin: (%.2f, %.2f, %.2f)", seg.origin.x, seg.origin.y, seg.origin.z);
		ImGui::Text("Diff: (%.2f, %.2f, %.2f)", seg.diff.x, seg.diff.y, seg.diff.z);
		ImGui::End();

		MakeTrianglePoint(trianglePoint, plane);

		if (IsCollition(seg, trianglePoint)) {
			color = 0xff0000ff;
		} else {
			color = 0xffffffff;
		}

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(CameraScale, cameraRotate, cameraPosition);
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

		DrawGrid(Multiply(normalWVPMatrix, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);

		DrawSegment(seg, normalWVPMatrix, viewportMatrix, color);

		DrawPlane(plane, Multiply(MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, {}, {0.0f, 0.0f, 0.0f}), Multiply(viewMatrix, projectionMatrix)), viewportMatrix, 0xffffffff);

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
