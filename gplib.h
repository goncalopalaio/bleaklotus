#ifndef GPLIB_H
#define GPLIB_H

	#include "maratis_math.h"
	#include "graphics_libs.h"

	float3 make_float3(float x, float y, float z);
	void set_float3(float3* v, float x, float y, float z);
	void copy_float3(float3* dest, float3* orig);
	void copy_float3(float3* dest, float3 orig);
	void float3_to_array(float* arr, float3 f);
	void scale_float3(float3* v, float scale);
	float* malloc_mat4_identity();
	void sub_float3(float3* dest, float3* a, float3* b);
	void add_float3(float3* dest, float3* a, float3* b);
	float radians_to_degrees(float radians);
	float degrees_to_radians(float degrees);
	float dist_euclidean(float3 a, float3 b);
	float rand_float_range(float min, float max);
	float3 rand_in_unit_sphere();
	int load_mesh(const char* file_name, GLuint* vao, int* point_count);
	char* load_file(char const* path);
	GLuint compile_shader(GLenum type, const char *src);
	int compile_shader_program(const char* str_vert_shader, const char* str_frag_shader, const char* attrib_name_0, const char* attrib_name_1, const char* attrib_name_2);
	void log(const char *s);
	void log_float3(char *s, float3 f);
	void log_matrix(char*name, float matrix[]);
#endif
