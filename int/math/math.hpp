#pragma once

namespace math {
	
	vec3_t calculate_angle(vec3_t& a, vec3_t& b);
	void sin_cos(float r, float* s, float* c);
	vec3_t angle_vector(vec3_t angle);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(vec3_t&, vec3_t&);
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	
};
