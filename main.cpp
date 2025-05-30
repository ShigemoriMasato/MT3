#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <imgui.h>
#include "MyMath.h"
#include "Draw.h"
#include "Collition.h"

using namespace MyMath;

const char kWindowTitle[] = "LE2A_06_シゲモリ_マサト_MT3";

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

	unsigned int color = 0xffffffff;

	int click = 0;
	int preClick = 0;

	AABB aabb{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } };

	Sphere sphere{ { 0.0f, 0.0f, 0.0f }, 1.0f, 16 };

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

		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Position", &cameraPosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Rotate", &cameraRotate.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("Scale", &CameraScale.x, 0.01f, 5.0f);
		ImGui::End();

		AABB worldAABB = { aabb.min + translate, aabb.max + translate };

		if (IsCollition(worldAABB, sphere)) {
			color = 0xff0000ff;
		} else {
			color = 0xffffffff;
		}

		ImGui::Begin("AABB");
		ImGui::Text("Min: (%.2f, %.2f, %.2f)", worldAABB.min.x, worldAABB.min.y, worldAABB.min.z);
		ImGui::Text("Max: (%.2f, %.2f, %.2f)", worldAABB.max.x, worldAABB.max.y, worldAABB.max.z);
		ImGui::End();

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

		DrawAABB(worldAABB, normalWVPMatrix, viewportMatrix, color);
		DrawSphere(sphere, normalWVPMatrix, viewportMatrix, 0xffffffff);

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
