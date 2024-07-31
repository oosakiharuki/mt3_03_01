#include <Novice.h>
#include "MyMath.h"
#include<imgui.h>

const char kWindowTitle[] = "LE2C_07_オオサキ_ハルキ_タイトル";

MyMath* myMath_ = new MyMath();


Vector3 operator*(const Vector3& v1, const Vector3& v2) { return myMath_->MultiplyVector(v1, v2); }
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return myMath_->Multiply(m1, m2); }
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return myMath_->AddMatrix(m1, m2); }

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHandleWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHandleWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHandleWidth + (xIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ x,0.0f,-kGridHandleWidth };
		Vector3 end{ x,0.0f,kGridHandleWidth };

		Vector3 startScreen = myMath_->Transform(myMath_->Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = myMath_->Transform(myMath_->Transform(end, viewProjectionMatrix), viewportMatrix);

		if (x == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);
	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHandleWidth + (zIndex * kGridEvery);
		unsigned int color = 0xAAAAAAFF;
		Vector3 start{ -kGridHandleWidth,0.0f,z };
		Vector3 end{ kGridHandleWidth,0.0f,z };

		Vector3 startScreen = myMath_->Transform(myMath_->Transform(start, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = myMath_->Transform(myMath_->Transform(end, viewProjectionMatrix), viewportMatrix);

		if (z == 0.0f) {
			color = BLACK;
		}
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, color);

	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 translates[3]{
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f}
	};

	Vector3 rotates[3]{
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f}
	};

	Vector3 scales[3]{
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f}
	};


	Vector3 cameraPosition = { 0.0f ,0.0f,-20.0f };
	Vector3 cameraTranslate = { 0.0f,-1.0f,-8.49f };
	Vector3 cameraRotate = {- 0.2f,0.0f,0.0f };




		
	Sphere sphere[3] = {
		{0.0,0.0f,0.0f ,0.1f},
		{0.0,0.0f,0.0f ,0.1f},
		{0.0,0.0f,0.0f ,0.1f}
	};





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
		


		Matrix4x4 shoulder = myMath_->MakeAffineMatrix(scales[0],rotates[0],translates[0]);
		Matrix4x4 elbow = myMath_->MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		Matrix4x4 hand = myMath_->MakeAffineMatrix(scales[2], rotates[2],translates[2]);


		Matrix4x4 worldMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 Ws = shoulder * worldMatrix;
		Matrix4x4 We = elbow * Ws;
		Matrix4x4 Wh = hand * We;

		Matrix4x4 cameraMatrix = myMath_->MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = myMath_->Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = myMath_->MakePerspectiveFovMatrix(0.45f, float(1280.0f) / float(720.0f), 0.1f, 100.0f);		


		Matrix4x4 WorldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
		Matrix4x4 SWorldViewProjectionMatrix = Ws * viewMatrix * projectionMatrix;
		Matrix4x4 EWorldViewProjectionMatrix = We * viewMatrix * projectionMatrix;
		Matrix4x4 HWorldViewProjectionMatrix = Wh * viewMatrix * projectionMatrix;


		Matrix4x4 viewportMatrix = myMath_->MakeViewportMatrix(0, 0, float(1280.0f), float(720.0f), 0.0f, 1.0f);







		Vector3 shoulderScreen = myMath_->Transform(myMath_->Transform(sphere[0].center, SWorldViewProjectionMatrix), viewportMatrix);
		Vector3 elbowScreen = myMath_->Transform(myMath_->Transform(sphere[1].center, EWorldViewProjectionMatrix), viewportMatrix);
		Vector3 handScreen = myMath_->Transform(myMath_->Transform(sphere[2].center, HWorldViewProjectionMatrix), viewportMatrix);



		DrawGrid(WorldViewProjectionMatrix, viewportMatrix);


		//sphere[0].center = { shoulderScreen.x,shoulderScreen.y,shoulderScreen.z };
		//sphere[1].center = { elbowScreen.x,elbowScreen.y,elbowScreen.z };
		//sphere[2].center = { handScreen.x,handScreen.y,handScreen.z };

		myMath_->DrawSphere(sphere[0], SWorldViewProjectionMatrix, viewportMatrix, RED);
		myMath_->DrawSphere(sphere[1], EWorldViewProjectionMatrix, viewportMatrix, GREEN);
		myMath_->DrawSphere(sphere[2], HWorldViewProjectionMatrix, viewportMatrix, BLUE);
		
		Novice::DrawLine((int)shoulderScreen.x, (int)shoulderScreen.y, (int)elbowScreen.x, (int)elbowScreen.y, WHITE);
		Novice::DrawLine((int)elbowScreen.x, (int)elbowScreen.y, (int)handScreen.x, (int)handScreen.y, WHITE);



		ImGui::Begin("window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		
		ImGui::Text("Red");
		ImGui::DragFloat3("transformRed", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotateRed", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scaleRed", &scales[0].x, 0.01f);

		ImGui::Text("Green");
		ImGui::DragFloat3("transformGreen", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotateGreen", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scaleGreen", &scales[1].x, 0.01f);

		ImGui::Text("Blue");
		ImGui::DragFloat3("transformBlue", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotateBlue", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scaleBlue", &scales[2].x, 0.01f);



		ImGui::End();




		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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
