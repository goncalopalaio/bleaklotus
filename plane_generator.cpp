#include "plane_generator.h"
#include "maratis_math.h"
#include "standard_libs.h"
#include "gplib.h"


float sample_plane_height(float x, float z, float max) {
	return -115 * cos(1100 * x/max) - 115 * sin(1100 * z/max);
}

void influence_plane_points(
	float domain,
	float3* a,
	float3* b,
	float3* c,
	float3* d,
	float3* e,
	float3* f) {

	a->y = sample_plane_height(a->x, a->z, domain);
	b->y = sample_plane_height(b->x, b->z, domain);
	c->y = sample_plane_height(c->x, c->z, domain);
	d->y = sample_plane_height(d->x, d->z, domain);
	e->y = sample_plane_height(e->x, e->z, domain);
	f->y = sample_plane_height(f->x, f->z, domain);

}

void load_plane(GLuint* plane_vao, int* point_count) {
	int number_per_side = PLANE_N_SQUARES;
	*point_count = number_per_side * number_per_side * 6;

    float* plane_vertices = (float*) malloc(*point_count * 3 * sizeof(float));
    float* plane_normals   = (float*) malloc(*point_count * 3 * sizeof(float));
    float* tex_coords = (float*) malloc(*point_count * 2 * sizeof(float));

    // Square side
    float s = PLANE_SIDE_SIZE;
    float base_y = -200;

    float3 a = {0,0,0};
    float3 b = {s,0,0};
    float3 c = {0,0,s};
    float3 d = {s,0,0};
    float3 e = {s,0,s};
    float3 f = {0,0,s};

    float3 normal_a = {1,0,0};
    float3 normal_b = {1,0,0};
    float3 normal_c = {1,0,0};
    float3 normal_d = {0,1,0};
    float3 normal_e = {0,1,0};
    float3 normal_f = {0,1,0};


    float3 ca;
    float3 cb;
    float3 cc;
    float3 cd;
    float3 ce;
    float3 cf;

    int idx = 0;
    int tex_idx = 0;
    float3 center_grid;
    set_float3(&center_grid, number_per_side/2.0, 0, number_per_side/2.0);

    int half_number_per_side = number_per_side / 2;
    for (int line = -half_number_per_side; line < half_number_per_side; ++line) {
        for (int col = -half_number_per_side; col < half_number_per_side; ++col) {
            
            float3 curr_offset = make_float3(line * s, base_y, col * s);  
            M_ADD3(ca, curr_offset, a);
            M_ADD3(cb, curr_offset, b);
            M_ADD3(cc, curr_offset, c);
            M_ADD3(cd, curr_offset, d);
            M_ADD3(ce, curr_offset, e);
            M_ADD3(cf, curr_offset, f);

            // Manipulate points

            influence_plane_points(number_per_side * s, &ca,&cb,&cc,&cd,&ce,&cf);

            //
            
            float3_to_array(plane_vertices + idx + 0,ca );
            float3_to_array(plane_vertices + idx + 3,cb );
            float3_to_array(plane_vertices + idx + 6,cc );
            float3_to_array(plane_vertices + idx + 9,cd );
            float3_to_array(plane_vertices + idx + 12, ce);
            float3_to_array(plane_vertices + idx + 15, cf);

            float3_to_array(plane_normals + idx + 0,  normal_a);
            float3_to_array(plane_normals + idx + 3,  normal_b);
            float3_to_array(plane_normals + idx + 6,  normal_c);
            float3_to_array(plane_normals + idx + 9,  normal_d);
            float3_to_array(plane_normals + idx + 12, normal_e);
            float3_to_array(plane_normals + idx + 15, normal_f);

            tex_coords[tex_idx + 0] = 0;
			tex_coords[tex_idx + 1] = 0;

            tex_coords[tex_idx + 2] = 1;
            tex_coords[tex_idx + 3] = 0;

            tex_coords[tex_idx + 4] = 0;
            tex_coords[tex_idx + 5] = 1;

            tex_coords[tex_idx + 6] = 1;
            tex_coords[tex_idx + 7] = 0;

            tex_coords[tex_idx + 8] = 1;
            tex_coords[tex_idx + 9] = 1;

            tex_coords[tex_idx + 10] = 0;
            tex_coords[tex_idx + 11] = 1;

            idx+=(3 * 6);
            tex_idx+= (2 * 6);
        }    
    }

    printf("Point count: %d, Idx: %d, Tex_Idx: %d, Idx/3: %d, Tex_Idx/2: %d\n", *point_count, idx, tex_idx,idx/3, tex_idx/2 );
    
    glGenVertexArrays(1, plane_vao);
    glBindVertexArray(*plane_vao);
    
	{
		printf("Uploading verts\n");
	    GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
		GL_ARRAY_BUFFER,
		3 * (*point_count) * sizeof(GLfloat),
		plane_vertices,
		GL_STATIC_DRAW);

		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		free(plane_vertices);	
    }
	
	{
		printf("Uploading normals\n");
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
		GL_ARRAY_BUFFER,
		3 * (*point_count) * sizeof(GLfloat),
		plane_normals,
		GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		free(plane_normals);
	}
	
	{
		printf("Uploading tex_coords\n");
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			2 * (*point_count) * sizeof(GLfloat),
			tex_coords,
			GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		free(tex_coords);
	}

	printf("Plane generation finished\n");
}

