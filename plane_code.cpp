  GLuint plane_vao;
    GLuint plane_shader_program;
    GLuint plane_uniform_view_matrix_location;
    GLuint plane_uniform_projection_matrix_location;
    int square_side = 10;
    int plane_total_vertices = square_side * square_side * 6;
    {

    glGenVertexArrays(1, &plane_vao);
    glBindVertexArray(plane_vao);
    
    plane_shader_program = compile_shader_program(str_points_vert_shader, str_points_frag_shader
                                                  ,"position", "color");
    
    plane_uniform_view_matrix_location = glGetUniformLocation(plane_shader_program, "u_view_matrix");
    plane_uniform_projection_matrix_location = glGetUniformLocation(plane_shader_program, "u_projection_matrix");

    float* plane_vertices = (float*) malloc(square_side * square_side * 6 * 3 * sizeof(float));
    float* plane_colors   = (float*) malloc(square_side * square_side * 6 * 3 * sizeof(float));


    float3 a = {0,0,0};
    float3 b = {1,0,0};
    float3 c = {0,0,1};
    float3 d = {1,0,0};
    float3 e = {1,0,1};
    float3 f = {0,0,1};

    float3 color_a = {1,0,0};
    float3 color_b = {1,0,0};
    float3 color_c = {1,0,0};
    float3 color_d = {0,1,0};
    float3 color_e = {0,1,0};
    float3 color_f = {0,1,0};


    float3 ca;
    float3 cb;
    float3 cc;
    float3 cd;
    float3 ce;
    float3 cf;

    int idx = 0;

    float3 center_grid;
    set_float3(&center_grid, square_side/2.0, 0, square_side/2.0);

    int half_square_side = square_side / 2;
    for (int line = -half_square_side; line < half_square_side; ++line) {
        for (int col = -half_square_side; col < half_square_side; ++col) {
            float3 curr_offset = make_float3(line, 0, col);
            M_ADD3(ca, curr_offset, a);
            M_ADD3(cb, curr_offset, b);
            M_ADD3(cc, curr_offset, c);
            M_ADD3(cd, curr_offset, d);
            M_ADD3(ce, curr_offset, e);
            M_ADD3(cf, curr_offset, f);

            //ca.y = -dist_euclidean(ca, center_grid) / 3.0;
            //cb.y = -dist_euclidean(cb, center_grid) / 3.0;
            //cc.y = -dist_euclidean(cc, center_grid) / 3.0;
            //cd.y = -dist_euclidean(cd, center_grid) / 3.0;
            //ce.y = -dist_euclidean(ce, center_grid) / 3.0;
            //cf.y = -dist_euclidean(cf, center_grid) / 3.0;

            
            set_float3(plane_vertices + idx + 0,ca );
            set_float3(plane_vertices + idx + 3,cb );
            set_float3(plane_vertices + idx + 6,cc );
            set_float3(plane_vertices + idx + 9,cd );
            set_float3(plane_vertices + idx + 12, ce);
            set_float3(plane_vertices + idx + 15, cf);

            set_float3(plane_colors + idx + 0,  color_a);
            set_float3(plane_colors + idx + 3,  color_b);
            set_float3(plane_colors + idx + 6,  color_c);
            set_float3(plane_colors + idx + 9,  color_d);
            set_float3(plane_colors + idx + 12, color_e);
            set_float3(plane_colors + idx + 15, color_f);

            idx+=18;
        }    
    }
    printf("Initialized plane vao\n");
    


    int total_floats = plane_total_vertices * 3;
    glBindVertexArray(plane_vao);

    GLuint vbo_pos;
    glGenBuffers(1, &vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
    glBufferData( GL_ARRAY_BUFFER, total_floats * sizeof(GLfloat), plane_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);


    GLuint vbo_colors;
    glGenBuffers(1, &vbo_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData( GL_ARRAY_BUFFER, total_floats * sizeof(GLfloat), plane_colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    }


    
        if(1) {            
            glUseProgram(plane_shader_program);
            glUniformMatrix4fv(plane_uniform_view_matrix_location, 1, GL_FALSE, view_matrix);
            glUniformMatrix4fv(plane_uniform_projection_matrix_location, 1, GL_FALSE, projection_matrix);


            glBindVertexArray(plane_vao);
            glDrawArrays(GL_TRIANGLES, 0,  plane_total_vertices);

        }
