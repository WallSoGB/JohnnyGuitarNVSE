#include "NiTypes.h"

NiMatrix33 NiMatrix33::operator*(const NiMatrix33& mat) const
{
	NiMatrix33 result;
	result.cr[0][0] =
		cr[0][0] * mat.cr[0][0] +
		cr[0][1] * mat.cr[1][0] +
		cr[0][2] * mat.cr[2][0];
	result.cr[1][0] =
		cr[1][0] * mat.cr[0][0] +
		cr[1][1] * mat.cr[1][0] +
		cr[1][2] * mat.cr[2][0];
	result.cr[2][0] =
		cr[2][0] * mat.cr[0][0] +
		cr[2][1] * mat.cr[1][0] +
		cr[2][2] * mat.cr[2][0];
	result.cr[0][1] =
		cr[0][0] * mat.cr[0][1] +
		cr[0][1] * mat.cr[1][1] +
		cr[0][2] * mat.cr[2][1];
	result.cr[1][1] =
		cr[1][0] * mat.cr[0][1] +
		cr[1][1] * mat.cr[1][1] +
		cr[1][2] * mat.cr[2][1];
	result.cr[2][1] =
		cr[2][0] * mat.cr[0][1] +
		cr[2][1] * mat.cr[1][1] +
		cr[2][2] * mat.cr[2][1];
	result.cr[0][2] =
		cr[0][0] * mat.cr[0][2] +
		cr[0][1] * mat.cr[1][2] +
		cr[0][2] * mat.cr[2][2];
	result.cr[1][2] =
		cr[1][0] * mat.cr[0][2] +
		cr[1][1] * mat.cr[1][2] +
		cr[1][2] * mat.cr[2][2];
	result.cr[2][2] =
		cr[2][0] * mat.cr[0][2] +
		cr[2][1] * mat.cr[1][2] +
		cr[2][2] * mat.cr[2][2];
	return result;
}

void NiMatrix33::operator*=(const NiMatrix33& mat) {
	NiMatrix33 result;
	result.cr[0][0] =
		cr[0][0] * mat.cr[0][0] +
		cr[0][1] * mat.cr[1][0] +
		cr[0][2] * mat.cr[2][0];
	result.cr[1][0] =
		cr[1][0] * mat.cr[0][0] +
		cr[1][1] * mat.cr[1][0] +
		cr[1][2] * mat.cr[2][0];
	result.cr[2][0] =
		cr[2][0] * mat.cr[0][0] +
		cr[2][1] * mat.cr[1][0] +
		cr[2][2] * mat.cr[2][0];
	result.cr[0][1] =
		cr[0][0] * mat.cr[0][1] +
		cr[0][1] * mat.cr[1][1] +
		cr[0][2] * mat.cr[2][1];
	result.cr[1][1] =
		cr[1][0] * mat.cr[0][1] +
		cr[1][1] * mat.cr[1][1] +
		cr[1][2] * mat.cr[2][1];
	result.cr[2][1] =
		cr[2][0] * mat.cr[0][1] +
		cr[2][1] * mat.cr[1][1] +
		cr[2][2] * mat.cr[2][1];
	result.cr[0][2] =
		cr[0][0] * mat.cr[0][2] +
		cr[0][1] * mat.cr[1][2] +
		cr[0][2] * mat.cr[2][2];
	result.cr[1][2] =
		cr[1][0] * mat.cr[0][2] +
		cr[1][1] * mat.cr[1][2] +
		cr[1][2] * mat.cr[2][2];
	result.cr[2][2] =
		cr[2][0] * mat.cr[0][2] +
		cr[2][1] * mat.cr[1][2] +
		cr[2][2] * mat.cr[2][2];
	
	*this = result;
}

NiVector3 NiMatrix33::operator*(const NiVector3& pt) const {
	return NiVector3(
		cr[0][0] * pt.x + cr[0][1] * pt.y + cr[0][2] * pt.z,
		cr[1][0] * pt.x + cr[1][1] * pt.y + cr[1][2] * pt.z,
		cr[2][0] * pt.x + cr[2][1] * pt.y + cr[2][2] * pt.z);
}

void NiMatrix33::MakeXRotation(float fAngle) {
	float sn = sin(fAngle);
	float cs = cos(fAngle);

	cr[0][0] = 1.0f;
	cr[0][1] = 0.0f;
	cr[0][2] = 0.0f;
	cr[1][0] = 0.0f;
	cr[1][1] = cs;
	cr[1][2] = sn;
	cr[2][0] = 0.0f;
	cr[2][1] = -sn;
	cr[2][2] = cs;
}

void NiMatrix33::MakeYRotation(float fAngle) {
	float sn = sin(fAngle);
	float cs = cos(fAngle);

	cr[0][0] = cs;
	cr[0][1] = 0.0f;
	cr[0][2] = -sn;
	cr[1][0] = 0.0f;
	cr[1][1] = 1.0f;
	cr[1][2] = 0.0f;
	cr[2][0] = sn;
	cr[2][1] = 0.0f;
	cr[2][2] = cs;
}

void NiMatrix33::MakeZRotation(float fAngle) {
	float sn = sin(fAngle);
	float cs = cos(fAngle);

	cr[0][0] = cs;
	cr[0][1] = sn;
	cr[0][2] = 0.0f;
	cr[1][0] = -sn;
	cr[1][1] = cs;
	cr[1][2] = 0.0f;
	cr[2][0] = 0.0f;
	cr[2][1] = 0.0f;
	cr[2][2] = 1.0f;
}

void NiMatrix33::MakeRotation(float fAngle, float x, float y, float z) {
	float sn = sin(fAngle);
	float cs = cos(fAngle);

	float omcs = 1.0f - cs;
	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float xym = x * y * omcs;
	float xzm = x * z * omcs;
	float yzm = y * z * omcs;
	float xsin = x * sn;
	float ysin = y * sn;
	float zsin = z * sn;

	cr[0][0] = x2 * omcs + cs;
	cr[0][1] = xym + zsin;
	cr[0][2] = xzm - ysin;
	cr[1][0] = xym - zsin;
	cr[1][1] = y2 * omcs + cs;
	cr[1][2] = yzm + xsin;
	cr[2][0] = xzm + ysin;
	cr[2][1] = yzm - xsin;
	cr[2][2] = z2 * omcs + cs;
}

void NiMatrix33::MakeRotation(float angle, const NiVector3& axis) {
	MakeRotation(angle, axis.x, axis.y, axis.z);
}

void NiMatrix33::FromEulerAnglesXYZ(float fXAngle, float fYAngle, float fZAngle) {
	NiMatrix33 kXRot, kYRot, kZRot;
	kXRot.MakeXRotation(fXAngle);
	kYRot.MakeYRotation(fYAngle);
	kZRot.MakeZRotation(fZAngle);
	*this = kXRot * (kYRot * kZRot);
}