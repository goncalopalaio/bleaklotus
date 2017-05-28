
GLuint vao_object_1;
GLuint vao_object_2;

GLfloat demo_triangles_points_1[] = {
	                  0.2f,  0.6f, 1.0f, 
                      0.6f,  0.2f, 1.0f, 
                      0.2f,  0.2f, 0.0f};

GLfloat demo_triangles_points_1_colors[] = {  
					  1.0f,  0.0f, 0.0f, 
                      0.0f,  0.0f, 0.0f, 
                      1.0f,  0.0f, 0.0f};

GLfloat demo_triangles_points_2[] = {  
					  0.2f,  1.6f, 1.0f, 
                      0.6f,  1.2f, 1.0f, 
                      0.2f,  1.2f, 0.0f};

GLfloat demo_triangles_points_2_colors[] = {  
					  1.0f,  1.0f, 0.0f, 
                      0.0f,  1.0f, 0.0f, 
                      1.0f,  1.0f, 0.0f};


GLuint init_demo_triangles_object(const GLfloat points[],const GLfloat colors[], const int size) {
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


    return vbo;
}

void init_demo_triangles() {

	glGenVertexArrays(1,&vao_object_1);
	glBindVertexArray(vao_object_1);

		GLuint vbo_object_1;
		glGenBuffers(1, &vbo_object_1);

	    glBindBuffer(GL_ARRAY_BUFFER, vbo_object_1);
		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);

		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), demo_triangles_points_1, GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);

//		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), demo_triangles_points_1_colors, GL_STATIC_DRAW);
//		glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 0, NULL);


    glBindVertexArray(0);


	glGenVertexArrays(1,&vao_object_2);
	glBindVertexArray(vao_object_2);

		GLuint vbo_object_2;
		glGenBuffers(1, &vbo_object_2);

	    glBindBuffer(GL_ARRAY_BUFFER, vbo_object_2);
		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);

		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), demo_triangles_points_2, GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);

//		glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), demo_triangles_points_2_colors, GL_STATIC_DRAW);
//		glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
}
