#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <imgui.h>
#include <numbers>
#include "MyMath.h"
#include "Draw.h"
#include "Collition.h"
#include "Physics.h"

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

	Vector3 cameraPos{ 0.0f, 0.0f, 0.0f };
	Vector3 cameraPosition{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 CameraScale{ 1.0f, 1.0f, 1.0f };
	Vector3 kLocalVertices[3] = {
		{ 0.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }
	};

	CircleMotionBall ball;
	ball.radius = 0.02f;
	ball.motionRadius = 0.8f;
	ball.angularVelocity = std::numbers::pi_v<float>;

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
		
		ImGui::Begin("Camera");
		ImGui::SliderFloat3("Position", &cameraPosition.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("Rotate", &cameraRotate.x, -3.14f, 3.14f);
		ImGui::SliderFloat3("Scale", &CameraScale.x, 0.01f, 5.0f);
		ImGui::End();

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, cameraRotate, cameraPos);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(CameraScale, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		CircleMotion(ball);

		///
		/// ↑更新処理ここまで
		///
		
		///
		/// ↓描画処理ここから
		///

		Matrix4x4 normalWVPMatrix = Multiply(MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, {}, {}), Multiply(viewMatrix, projectionMatrix));

		DrawGrid(Multiply(normalWVPMatrix, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);

		DrawBall(&ball, normalWVPMatrix, viewportMatrix, 0xff0000ff);

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
