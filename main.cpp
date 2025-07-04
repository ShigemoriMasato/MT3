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

	Vector3 cameraPos{ 0.0f, 0.0f, 0.0f };
	Vector3 cameraPosition{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 CameraScale{ 1.0f, 1.0f, 1.0f };
	Vector3 kLocalVertices[3] = {
		{ 0.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }
	};

	Vector3 a = { 0.2f, 1.0f, 0.0f };
	Vector3 b = { 2.4f, 3.1f, 1.2f };
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;
	Vector3 rotate = { 0.4f, 1.43f, -0.8f };
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

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

		ImGui::Begin("Answer");
		ImGui::Text("c : %f, %f, %f", c.x, c.y, c.z);
		ImGui::Text("d : %f, %f, %f", d.x, d.y, d.z);
		ImGui::Text("e : %f, %f, %f", e.x, e.y, e.z);
		ImGui::Text("rotateMatrix");
		ImGui::Text("  %f, %f, %f, %f", rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
		ImGui::Text("  %f, %f, %f, %f", rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
		ImGui::Text("  %f, %f, %f, %f", rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
		ImGui::Text("  %f, %f, %f, %f", rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);
		ImGui::End();

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, cameraRotate, cameraPos);
		Matrix4x4 cameraMatrix = MakeAffineMatrix(CameraScale, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///
		
		///
		/// ↓描画処理ここから
		///

		Matrix4x4 normalWVPMatrix = Multiply(MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, {}, {}), Multiply(viewMatrix, projectionMatrix));

		DrawGrid(Multiply(normalWVPMatrix, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);

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
