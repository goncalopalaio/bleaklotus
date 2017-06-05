#include "standard_libs.h"
#include "graphics_libs.h"
#include "constants.h"

#include "gplib.h"
#include "stb_image.h"

#define WINDOW_W 600
#define WINDOW_H 1000

#include "debug_lines.h"

typedef struct Camera {
    float3 position;
    float3 direction;
    float3 up;
}Camera;

typedef struct Player {
    float3 position;
    float3 velocity;
    float rotation;
    Camera camera;
}Player;

typedef struct KeyboardState {
    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;
    bool key_w;
    bool key_s;
}KeyboardState;



#include "plane_generator.h"
#include "star_engine.h"



// World state
Player player_state;
KeyboardState keyboard_state;
//////////////////

void close_callback(GLFWwindow * window) {
    log("close_callback");
}

void size_callback(GLFWwindow * window, int width, int height) {
    log("size_callback");
}

void cursorpos_callback(GLFWwindow * window, double mx, double my) {}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // GLFW_RELEASE,GLFW_PRESS,GLFW_REPEAT

    int new_state = (action == GLFW_PRESS || action == GLFW_REPEAT) ? 1 : 0;
    switch(key) {
        case GLFW_KEY_RIGHT:
            keyboard_state.key_right = new_state;
        break;
        case GLFW_KEY_LEFT:
            keyboard_state.key_left = new_state;
        break;
        case GLFW_KEY_UP:
            keyboard_state.key_up = new_state;
        break;
        case GLFW_KEY_DOWN:
            keyboard_state.key_down = new_state;
        break;
        case GLFW_KEY_W:
            keyboard_state.key_w = new_state;
        break;
        case GLFW_KEY_S:
            keyboard_state.key_s = new_state;
        break;
    }
    

    if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

   /* printf("Camera: pos (%f %f %f) direction (%f %f %f) up (%f %f %f)\n",
         player_state.camera.position.x
        ,player_state.camera.position.y
        ,player_state.camera.position.z
        ,player_state.camera.direction.x
        ,player_state.camera.direction.y
        ,player_state.camera.direction.z
        ,player_state.camera.up.x
        ,player_state.camera.up.y
        ,player_state.camera.up.z);
    */
}

void mousebutton_callback(GLFWwindow * window, int button, int action, int mods) {}

void error_callback(int error, const char* description) {
    printf("%s\n", description);
}

int main(int argc, char const *argv[]) {
	printf("BLEAKS\n");
    GLFWwindow* window;
    if(!glfwInit()) {
        log("glfw init failed");
    } else {
        log("Glfw initialized");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_W, WINDOW_H, "BLEAKS", NULL, NULL);
    if(!window) {
        log("Create window failed");
        glfwTerminate();
        return -1;
    }
	
	log("Setting callbacks");
    glfwSetWindowCloseCallback(window, close_callback);
    glfwSetWindowSizeCallback(window, size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mousebutton_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);
    glfwSetErrorCallback(error_callback);
    log("Setting context");
    glfwMakeContextCurrent(window);


	float view_matrix[] = M_MAT4_IDENTITY();
	float projection_matrix[] = M_MAT4_IDENTITY();
	float model_matrix[] = M_MAT4_IDENTITY();

    float aspect = WINDOW_W / (float)WINDOW_H;
    m_mat4_perspective(projection_matrix, 45.0, aspect, 1, 100.0);
    m_mat4_identity(view_matrix);
    m_mat4_identity(model_matrix);

    m_mat4_rotation_axis(model_matrix, &X_AXIS, M_PI/4);

    // Initialize camera
    set_float3(&player_state.camera.position,8,4,8);
    set_float3(&player_state.camera.direction,0-player_state.camera.position.x,0-player_state.camera.position.y,0-player_state.camera.position.z);
    M_NORMALIZE3(player_state.camera.direction,player_state.camera.direction);
    set_float3(&player_state.camera.up,0,1,0);

    // Initialize player;
    set_float3(&player_state.position ,0,0,-1);
    set_float3(&player_state.velocity ,0,0,0);

     player_state.rotation = M_PI;

    char str_points_vert_shader[] =
        "#version 150\n"
        "in vec3 position;"
        "in vec3 color;"
        "in vec2 uv_in;"
        "in vec3 cam_pos;"
        "in vec3 cam_dir;"
        "out vec3 v_color;"
        "out vec3 v_cam_pos;"
        "out vec3 v_cam_dir;"
        "out vec2 uv;"
        "uniform mat4 u_view_matrix;"
        "uniform mat4 u_projection_matrix;"
        "uniform mat4 u_model_matrix;"
        "void main(){"
        "v_color = color;"
        "uv = uv_in;"
        "v_cam_pos = cam_pos;"
        "v_cam_dir = cam_dir;"
        "gl_Position =  u_projection_matrix * u_view_matrix * u_model_matrix * vec4(position, 1.0);"
        "}";

    char str_points_frag_shader[] =
        "#version 150\n"
        "in vec3 v_color;"
        "out vec4 frag_color;"
        "in vec2 uv;"
        "in vec3 v_cam_pos;"
        "in vec3 v_cam_dir;"
        "uniform sampler2D texture_sampler;"
        "void main() {"
        "float cosTheta = clamp( dot( v_color, vec3(0,0,-1) ), 0,1 );"
        "frag_color = texture(texture_sampler, uv) * cosTheta;"
        "}";

    GLuint points_shader_program = compile_shader_program(str_points_vert_shader,
    													  str_points_frag_shader,
    													  "position", "color", "uv");
    GLuint uniform_view_matrix_location = glGetUniformLocation(points_shader_program, "u_view_matrix");
    GLuint uniform_projection_matrix_location = glGetUniformLocation(points_shader_program, "u_projection_matrix");
    

    setup_debug_vectors(128, 3);

    push_vector_debug_vectors(ORIGIN,X_AXIS,15);
    push_vector_debug_vectors(ORIGIN,Y_AXIS,15);
    push_vector_debug_vectors(ORIGIN,Z_AXIS,15);


    //GLuint bunny_vao;    
    //int bunny_point_count = 0;
    //if (1){
    //    assert(load_mesh("models/Robo_01.fbx", &bunny_vao, &bunny_point_count));
    //    printf("Bunny point count %d\n", bunny_point_count);
    //}


    setup_world();
    //create_object(ORIGIN, TYPE_CUBE);
    int jf = 0;
    float3 ff = make_float3(0,0,0);
    ff.x+=2;
    create_object(ff, TYPE_CUBE,0);
    ff.x+=2;
    create_object(ff, TYPE_CUBE,0);
    ff.x+=2;
    create_object(ff, TYPE_CUBE,0);
    ff.x+=2;
    create_object(ff, TYPE_CUBE,0);
    ff = make_float3(4,0,4);
    
    create_object(ff, TYPE_ROBOT,1);

    ff = make_float3(0,0,0);
    
    create_object(ff, TYPE_PLANE,0);



    float time = 0;

    int frame_count = 0;
    float diff_millis = 0;
    float diff_millis_accumulator = 0;
    float camera_rotation = M_PI/4;


    glPointSize(16.0);
    glLineWidth(1.0);

	glEnable(GL_DEPTH_TEST);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		

        push_vector_debug_vectors(player_state.position,player_state.velocity,50);

        
        update_with_keyboard(&keyboard_state, &(player_state.camera.position), &(player_state.camera.direction));

        // Update camera
        m_mat4_lookat(view_matrix, &(player_state.camera.position), &(player_state.camera.direction), &(player_state.camera.up)); 

        render_world(view_matrix, projection_matrix, &player_state.camera.direction);

        if(1) {
            draw_debug_vectors(view_matrix, projection_matrix);
        }

		glfwSwapBuffers(window);
        glfwPollEvents();
        time +=0.001;
        ++frame_count;

    }

    glfwMakeContextCurrent(NULL);
    glfwDestroyWindow(window);

	return 0;
}
