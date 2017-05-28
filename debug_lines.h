
typedef struct _DebugVectors {
	int total_vertex_elements;
	int total_vectors;
	int push_index;
	int reserved_vector_count;
	GLuint shader_program;
	GLuint uniform_view_matrix_location;
	GLuint uniform_projection_matrix_location;
	float* vertices;
	float* colors;
}_DebugVectors;


_DebugVectors _debug_vectors;
GLuint debug_vao;


char _str_debug_vectors_vert_shader[] =
    "#version 150\n"
    "in vec3 position;"
    "in vec3 color;"
    "out vec3 v_color;"
    "uniform mat4 u_view_matrix;"
    "uniform mat4 u_projection_matrix;"
    "void main(){"
    "v_color = color;"
    "gl_Position =  u_projection_matrix * u_view_matrix * vec4(position, 1.0);"
    "}";

char _str_debug_vectors_frag_shader[] =
    "#version 150\n"
    "in vec3 v_color;"
    "out vec4 frag_color;"
    "void main() {"
    "frag_color = vec4(v_color, 1.0);"
    "}";


void update_debug_vectors() {
	int total_floats = _debug_vectors.total_vertex_elements * 3;
	glBindVertexArray(debug_vao);

	GLuint vbo_pos;
	glGenBuffers(1, &vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glBufferData( GL_ARRAY_BUFFER, total_floats * sizeof(GLfloat), _debug_vectors.vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);


	GLuint vbo_colors;
	glGenBuffers(1, &vbo_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
	glBufferData( GL_ARRAY_BUFFER, total_floats * sizeof(GLfloat), _debug_vectors.colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);


}

void setup_debug_vectors(int initial_elements, int reserved_vector_count) {
	// Generate a VAO
	glGenVertexArrays(1, &debug_vao);
	glBindVertexArray(debug_vao);
	
	_debug_vectors.shader_program = compile_shader_program(_str_debug_vectors_vert_shader,
    													  _str_debug_vectors_frag_shader,
    													  "position", "color", NULL);
	
    _debug_vectors.uniform_view_matrix_location = glGetUniformLocation(_debug_vectors.shader_program, "u_view_matrix");
    _debug_vectors.uniform_projection_matrix_location = glGetUniformLocation(_debug_vectors.shader_program, "u_projection_matrix");

	_debug_vectors.total_vectors = initial_elements;
	_debug_vectors.total_vertex_elements = initial_elements * 2;
	_debug_vectors.push_index = 0;
	_debug_vectors.reserved_vector_count = reserved_vector_count;

	int total_floats = _debug_vectors.total_vertex_elements * 3;

	printf("Creating %d vectors (%d vertices) (%d floats)\n", initial_elements,_debug_vectors.total_vertex_elements, total_floats);

	_debug_vectors.vertices = (float*) malloc(total_floats * sizeof(float));
	_debug_vectors.colors 	= (float*) malloc(total_floats * sizeof(float));

	for(int idx = 0; idx < total_floats; idx+=6) {

		float xx = 10*m_randf();
		float zz = 10*m_randf();
		_debug_vectors.vertices[idx + 0] = 0;
		_debug_vectors.vertices[idx + 1] = 0;
		_debug_vectors.vertices[idx + 2] = 0;
		_debug_vectors.vertices[idx + 3] = 0;
		_debug_vectors.vertices[idx + 4] = 1;
		_debug_vectors.vertices[idx + 5] = 0;

		_debug_vectors.colors[idx + 0] = 1;
		_debug_vectors.colors[idx + 1] = 0;
		_debug_vectors.colors[idx + 2] = 0;
		_debug_vectors.colors[idx + 3] = 0;
		_debug_vectors.colors[idx + 4] = 1;
		_debug_vectors.colors[idx + 5] = 0;

		//printf("idx %d\n", idx);
		//d3("Vertex 1", *((float3*)&(_debug_vectors.vertices[idx + 0])));
		//d3("Vertex 2", *((float3*)&(_debug_vectors.vertices[idx + 3])));
		//d3("Color 1", *((float3*)&(_debug_vectors.colors[idx + 0])));
		//d3("Color 2", *((float3*)&(_debug_vectors.colors[idx + 3])));

	}

	printf("Initialized debug_vectors floats %d\n", total_floats);

//
	update_debug_vectors();

//
}

void draw_debug_vectors(float* view_matrix, float* projection_matrix) {
	update_debug_vectors();

	glUseProgram(_debug_vectors.shader_program);
	glUniformMatrix4fv(_debug_vectors.uniform_view_matrix_location, 1, GL_FALSE, view_matrix);
	glUniformMatrix4fv(_debug_vectors.uniform_projection_matrix_location, 1, GL_FALSE, projection_matrix);
	glBindVertexArray(debug_vao);

	

    glDrawArrays(GL_LINES, 0, _debug_vectors.total_vertex_elements);
    glDrawArrays(GL_POINTS, 0, _debug_vectors.total_vertex_elements);
}

void push_line_debug_vectors(float3 p1, float3 p2) {
	_debug_vectors.vertices[_debug_vectors.push_index + 0] = p1.x;
	_debug_vectors.vertices[_debug_vectors.push_index + 1] = p1.y;
	_debug_vectors.vertices[_debug_vectors.push_index + 2] = p1.z;
	_debug_vectors.vertices[_debug_vectors.push_index + 3] = p2.x;
	_debug_vectors.vertices[_debug_vectors.push_index + 4] = p2.y;
	_debug_vectors.vertices[_debug_vectors.push_index + 5] = p2.z;


	_debug_vectors.push_index+=6;
	
	int total_floats = _debug_vectors.total_vectors * 6;
	if (_debug_vectors.push_index >= total_floats){
		_debug_vectors.push_index = 0 + _debug_vectors.reserved_vector_count * 6;
	}

}

void push_vector_debug_vectors(float3 position, float3 direction, float direction_scale) {
	_debug_vectors.vertices[_debug_vectors.push_index + 0] = position.x;
	_debug_vectors.vertices[_debug_vectors.push_index + 1] = position.y;
	_debug_vectors.vertices[_debug_vectors.push_index + 2] = position.z;
	_debug_vectors.vertices[_debug_vectors.push_index + 3] = position.x + direction_scale*direction.x;
	_debug_vectors.vertices[_debug_vectors.push_index + 4] = position.y + direction_scale*direction.y;
	_debug_vectors.vertices[_debug_vectors.push_index + 5] = position.z + direction_scale*direction.z;


	_debug_vectors.push_index+=6;

	int total_floats = _debug_vectors.total_vectors * 6;
	if (_debug_vectors.push_index >= total_floats){
		_debug_vectors.push_index = 0 + _debug_vectors.reserved_vector_count * 6;
	}

}

void push_vector_debug_vectors(float3 position, float3 direction) {
	 push_vector_debug_vectors(position, direction, 1.0);
}
