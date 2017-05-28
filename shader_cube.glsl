#version 150

in vec3 position;
in vec3 color;
in vec2 uv_in;
in vec3 cam_pos;
in vec3 cam_dir;
out vec3 v_color;
out vec3 v_cam_pos;
out vec3 v_cam_dir;
out vec2 uv;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
void main(){
v_color = color;
uv = uv_in;
v_cam_pos = cam_pos;
v_cam_dir = cam_dir;
gl_Position =  projection * view * model * vec4(position, 1.0);
}


#FRAG

#version 150

in vec3 v_color;
out vec4 frag_color;
in vec2 uv;
in vec3 v_cam_pos;
in vec3 v_cam_dir;
uniform sampler2D texture_sampler;

void main() {
	float cosTheta = clamp( dot( v_color, vec3(0,0,1) ), 0,1 );
	frag_color = texture(texture_sampler, uv) * cosTheta;
}