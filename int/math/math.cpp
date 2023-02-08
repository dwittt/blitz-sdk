#include "../csgo.hpp"


//aimtux


vec3_t math::calculate_angle(vec3_t& a, vec3_t& b) {
	vec3_t angles;
	vec3_t delta;
	delta.x = (a.x - b.x);
	delta.y = (a.y - b.y);
	delta.z = (a.z - b.z);

	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);

	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;
}

void math::sin_cos(float r, float* s, float* c) {
	*s = sin(r);
	*c = cos(r);
}

vec3_t math::angle_vector(vec3_t angle) {
	auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(angle.x / 180.f * static_cast<float>(M_PI));

	return vec3_t(cp * cy, cp * sy, -sp);
}

void math::transform_vector(vec3_t& a, matrix_t& b, vec3_t& out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

void math::vector_angles(vec3_t& forward, vec3_t& angles) {
	if (forward.y == 0.0f && forward.x == 0.0f) {
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
		angles.y = 0.0f;
	}
	else {
		angles.x = atan2(-forward.z, vec3_t(forward).length()) * -180 / static_cast<float>(M_PI);
		angles.y = atan2(forward.y, forward.x) * 180 / static_cast<float>(M_PI);

		if (angles.y > 90)
			angles.y -= 180;
		else if (angles.y < 90)
			angles.y += 180;
		else if (angles.y == 90)
			angles.y = 0;
	}

	angles.z = 0.0f;
}

void math::angle_vectors(vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos(DEG2RAD(angles.x), &sp, &cp);
	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.z), &sr, &cr);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if (up) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

vec3_t math::vector_add(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t math::vector_subtract(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t math::vector_multiply(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t math::vector_divide(vec3_t& a, vec3_t& b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}




