#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <Windows.h>
#include <iostream>
using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

ostream& XM_CALLCONV operator<<(ostream& os, FXMVECTOR v) {
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);
	os << "(" << dest.x << "," << dest.y << "," << dest.z << ")";
	return os;
}

void test1() {
	XMVECTOR p = XMVectorZero();
	XMVECTOR q = XMVectorSplatOne();
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR v = XMVectorReplicate(-2.0f);
	XMVECTOR w = XMVectorSplatZ(u);
	cout << "p=" << p << endl;
	cout << "q=" << q << endl;
	cout << "u=" << u << endl;
	cout << "v=" << v << endl;
	cout << "w=" << w << endl;
}

void test2() {
	XMVECTOR n = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR u = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
	XMVECTOR v = XMVectorSet(-2.0f, 1.0f, -3.0f, 0.0f);
	XMVECTOR w = XMVectorSet(0.707f, 0.707f, 0.0f, 0.0f);
	XMVECTOR a = u + v;
	XMVECTOR b = u - v;
	XMVECTOR c = 10.0f * u;
	XMVECTOR L = XMVector3Length(u);
	XMVECTOR d = XMVector3Normalize(u);
	XMVECTOR s = XMVector3Dot(u, v);
	XMVECTOR e = XMVector3Cross(u, v);
	XMVECTOR projW;
	XMVECTOR prepW;
	XMVector3ComponentsFromNormal(&projW, &prepW, w, n);
	bool equal = XMVector3Equal(projW + prepW, w) != 0;
	bool notEqual= XMVector3NotEqual(projW + prepW, w) != 0;
	XMVECTOR angleVec = XMVector3AngleBetweenVectors(projW, prepW);
	float angleRadians = XMVectorGetX(angleVec);
	float angleDegrees = XMConvertToDegrees(angleRadians);
	cout << "u = " << u << endl;
	cout << "v = " << v << endl;
	cout << "w = " << w << endl;
	cout << "n = " << n << endl;
	cout << "a = a + v = " << a << endl;
	cout << "b = u - v = " << b << endl;
	cout << "c = 10 * u = " << c << endl;
	cout << "d = a / ||u|| = " << d << endl;
	cout << "e = u x v = " << e << endl;
	cout << "L = ||u|| = " << L << endl;
	cout << "s = u.v = " << s << endl;
	cout << "projW = " << projW << endl;
	cout << "prepW = " << prepW << endl;
	cout << "projW + prepW == w = " << equal << endl;
	cout << "projW + prepW != w = " << notEqual << endl;
	cout << "angle = " << angleDegrees << endl;
}

void test3() {
	cout.precision(8);
	XMVECTOR u = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	XMVECTOR n = XMVector3Normalize(u);
	float LU = XMVectorGetX(XMVector3Length(n));
	cout << LU << endl;
	if (LU == 1.0f) {
		cout << "Length 1" << endl;
	}
	else {
		cout << "Length not 1" << endl;
	}
	float powLU = powf(LU, 1.0e6f);
	cout << "LU^(10^6) = " << powLU << endl;
}

int main() {
	cout.setf(ios_base::boolalpha);
	if (!XMVerifyCPUSupport()) {
		cout << "DirectX math not supportted.";
		return 0;
	}
	//test1();
	//test2();
	test3();

	return 0;
}