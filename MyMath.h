#pragma once
#include <Novice.h>
#include<cmath>
#include<assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

struct Vector2 {
	float x;
	float y;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

struct Sphere {
	Vector3 center;
	float radius;
};

class MyMath
{
public:
	Matrix4x4 Inverse(const Matrix4x4& m);

	Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
	
	Vector3 MultiplyVector(Vector3 v1, Vector3 v2);

	Vector3 Add(const Vector3& v1, const Vector3& v2);

	Matrix4x4 AddMatrix(const Matrix4x4& m1, const Matrix4x4& m2);

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	Matrix4x4 MakePerspectiveFovMatrix(float forY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

};