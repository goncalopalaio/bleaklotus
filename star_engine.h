
typedef struct RenderInfo {
	GLuint vao;
	GLuint texture1;
	GLuint program;
	int n_vertices;
	// Uniforms
	GLuint u_loc_model;
	GLuint u_loc_view;
	GLuint u_loc_projection;
	GLuint u_loc_tex1;
	GLuint u_loc_cam_dir;
}RenderInfo;

typedef struct BBox {
	float3 min;
	float3 max;
}BBox;

#define TYPE_CUBE 1
#define TYPE_ROBOT 2
#define TYPE_PLANE 3

typedef struct Entity {
	float3 pos;
	float3 acc;
	float3 vec;
	float3 rot;
	float3 draw_scale;
	int type;
	RenderInfo* rinfo;
	BBox bbox;
	struct Entity* next;
}Entity;

typedef struct StarWorld {
	Entity* entities;
	Entity* controllable_entities[1];
	RenderInfo* render_info_cube;
	RenderInfo* render_info_robot;
	RenderInfo* render_info_plane;
	GLuint tex_1;
	GLuint tex_2;
	GLuint tex_3;
	int entity_count;

	float time;
}StarWorld;

StarWorld world;


void _reload_shaders();
void _load_models();
Entity* _create_cube(float3 pos);
Entity* _create_robot(float3 pos);
Entity* _create_plane(float3 pos);
void _setup_shader(char* file_name, RenderInfo* info);
void _load_texture(const char* file, GLuint* tex);

void setup_world() {
	printf("Setting no entities\n");
	world.entities = NULL;
	world.entity_count = 0;
	world.time = 0;

	printf("Initializing global object properties\n");
	world.render_info_cube = (RenderInfo* ) malloc(sizeof(RenderInfo));
	world.render_info_robot = (RenderInfo* ) malloc(sizeof(RenderInfo));
	world.render_info_plane = (RenderInfo* ) malloc(sizeof(RenderInfo));
	
	printf("Loading shaders\n");
	_reload_shaders();
	
	printf("Loading models\n");
	_load_models();
}

void create_object(float3 pos, int type, int controllable) {
	Entity* new_head;
	switch (type) {
		case TYPE_CUBE:
			new_head = _create_cube(pos);
		break;
		case TYPE_ROBOT:
			new_head = _create_robot(pos);
		break;
		case TYPE_PLANE:
			new_head = _create_plane(pos);
		break;

		default:
			log("Unknown type");
	}

	if (controllable){
		world.controllable_entities[0] = new_head;
	}

	new_head->next = world.entities;
	world.entities = new_head;
}

void update_with_keyboard(KeyboardState* keys, float3* cam_pos, float3* cam_dir) {
	Entity* ent = world.controllable_entities[0];
	float inc = 0;

	if (keys->key_left){
		inc = 0.1;
	}

	if (keys->key_right){
		inc = -0.1;
	}


	if (inc != 0){
		ent->rot.y += inc;

		float3 dir;
		set_float3(&dir, sin(ent->rot.y), 0 ,cos(ent->rot.y));
		M_NORMALIZE3(dir,dir);

		scale_float3(&dir, 0.3);
		
		ent->vec = dir;


		// Update camera direction
		scale_float3(&dir, -2);

		copy_float3(cam_dir, &dir);

	}

	if (keys->key_up || keys->key_w) {
		M_SUB3(ent->pos,ent->pos,ent->vec );
	}

	// Update camera
	float3 space_between_camera;
	set_float3(&space_between_camera,0,0,0);
	add_float3(&space_between_camera,&space_between_camera, cam_dir);
	scale_float3(&space_between_camera, -32.0);

	add_float3(cam_pos, &ent->pos, &space_between_camera);

	cam_pos->y = 15.0;
}

void render_world(float* view_matrix, float* projection_matrix, float3* cam_dir) {
	Entity* tmp = world.entities;
	world.time+=0.01;
      
	float global_model_matrix[] = M_MAT4_IDENTITY();
	float global_transl_matrix[] = M_MAT4_IDENTITY();
	float global_rot_matrix[] = M_MAT4_IDENTITY();
	float global_scale_matrix[] = M_MAT4_IDENTITY();

	int i = 0;
	while(tmp != NULL) {
		//printf("%d \n", i);
		//log_float3("Updating entity at pos: ",tmp->pos);

    
        m_mat4_translation(global_transl_matrix,&tmp->pos);
		m_mat4_rotation_euler(global_rot_matrix, &tmp->rot);
		m_mat4_scale(global_scale_matrix, &tmp->draw_scale);


		m_mat4_identity(global_model_matrix);		
		m_mat4_mul(global_model_matrix, global_transl_matrix, global_rot_matrix);
		m_mat4_mul(global_model_matrix, global_model_matrix, global_scale_matrix);
		
		glUseProgram(tmp->rinfo->program);

		float sd[ ] = {1,0,0};
		glUniform3f(tmp->rinfo->u_loc_cam_dir,cam_dir->x,cam_dir->y, cam_dir->z );

		glUniformMatrix4fv(tmp->rinfo->u_loc_model, 1, GL_FALSE, global_model_matrix);		
		glUniformMatrix4fv(tmp->rinfo->u_loc_view, 1, GL_FALSE, view_matrix);
		glUniformMatrix4fv(tmp->rinfo->u_loc_projection, 1, GL_FALSE, projection_matrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tmp->rinfo->texture1);
		glUniform1i(tmp->rinfo->u_loc_tex1, 0);
	

		glBindVertexArray(tmp->rinfo->vao);
		glDrawArrays(GL_TRIANGLES, 0, tmp->rinfo->n_vertices);
		glUseProgram(0);


		tmp = tmp->next;
		i++;
	}
}

void _reload_shaders() {
	_setup_shader("shader_cube.glsl", world.render_info_cube);
	_setup_shader("shader_robot.glsl", world.render_info_robot);

	_setup_shader("shader_robot.glsl", world.render_info_plane);
}

void _load_models() {
	assert(load_mesh("models/bunny.obj", &world.render_info_cube->vao, &world.render_info_cube->n_vertices));
	assert(load_mesh("models/bleak_player.fbx", &world.render_info_robot->vao, &world.render_info_robot->n_vertices));

	load_plane(&world.render_info_plane->vao, &world.render_info_plane->n_vertices);

	_load_texture("models/Robot_01_Diffuse.png", &world.tex_1);
	_load_texture("models/bleak_player.png", &world.tex_2);
	_load_texture("models/TEX_TEST.png", &world.tex_3);
	
	world.render_info_robot->texture1 = world.tex_2;
	world.render_info_cube->texture1 = world.tex_1;

	world.render_info_plane->texture1 = world.tex_3;

}

void _load_texture(const char* file, GLuint* tex) {
	unsigned char * data;
    int width = 256;
    int height = 256;
    int bitdepth = 4;
    GLuint texture;
    data = stbi_load(file, &width, &height, &bitdepth, 0);
    // Create one OpenGL texture
    glGenTextures(1, tex);

    glBindTexture(GL_TEXTURE_2D, *tex);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    free(data);
}

void _setup_shader(char* file_name, RenderInfo* info) {
	char* buf = load_file(file_name);
	char* start_frag_shader = strstr(buf, "#FRAG");
	
	char* frag_shader = start_frag_shader + strlen("#FRAG");
	
	*start_frag_shader = '\0';
	char* vert_shader = buf;
	
	//printf("From file: VERT_SHADER: %s\n", vert_shader);
	//printf("From file: FRAG_SHADER: %s\n", frag_shader);

	info->program = compile_shader_program( vert_shader,
    										frag_shader,
    										"position",
    										"normal",
    										"uv");
	glUseProgram(info->program);
	info->u_loc_model = 	 glGetUniformLocation(info->program, "model");
    info->u_loc_view = 		 glGetUniformLocation(info->program, "view");
    info->u_loc_projection = glGetUniformLocation(info->program, "projection");
    info->u_loc_tex1 = 		 glGetUniformLocation(info->program, "tex1");
    info->u_loc_cam_dir = 	 glGetUniformLocation(info->program, "cam_dir");


	free(buf);
}

Entity* _create_cube(float3 pos) {
	// Allocate
	Entity* new_cube = (Entity*) malloc(sizeof(Entity));
	
	new_cube->type = TYPE_CUBE;

	float3 zeros = make_float3(0,0,0);
	new_cube->pos = pos;
	new_cube->acc = zeros;
	new_cube->vec = zeros;
	new_cube->rot = zeros;
	new_cube->draw_scale = make_float3(2,2,2);
	
	new_cube->bbox.min = zeros;
	new_cube->bbox.max = zeros;

	new_cube->rinfo = world.render_info_cube;

	return new_cube;
}

Entity* _create_robot(float3 pos) {
	// Allocate
	Entity* ent = (Entity*) malloc(sizeof(Entity));
	
	ent->type = TYPE_ROBOT;

	float3 zeros = make_float3(0,0,0);
	ent->pos = pos;
	ent->acc = zeros;
	ent->vec = zeros;
	ent->rot = make_float3(0,M_PI/2,0);
	ent->draw_scale = make_float3(0.01,0.01,0.01);
	
	ent->bbox.min = zeros;
	ent->bbox.max = zeros;

	ent->rinfo = world.render_info_robot;

	return ent;
}

Entity* _create_plane(float3 pos) {
	// Allocate
	Entity* ent = (Entity*) malloc(sizeof(Entity));
	
	ent->type = TYPE_PLANE;

	float3 zeros = make_float3(0,0,0);
	ent->pos = pos;
	ent->acc = zeros;
	ent->vec = zeros;
	ent->rot = make_float3(0,M_PI/2,0);
	ent->draw_scale = make_float3(0.01,0.01,0.01);
	
	ent->bbox.min = zeros;
	ent->bbox.max = zeros;

	ent->rinfo = world.render_info_plane;

	return ent;
}
