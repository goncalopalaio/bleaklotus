#version 150

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec3 out_normal;
out vec2 out_uv;
out vec3 out_cam_dir;

uniform vec3 cam_dir;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(){

out_normal = normal;
out_uv = uv;
out_cam_dir = (projection * view * normalize(vec4(2.0,1.0,2.0,1.0))).xyz;

gl_Position =  projection * view * model * vec4(position, 1.0);

}


#FRAG

#version 150

in vec3 out_cam_dir;

in vec3 out_normal;
in vec2 out_uv;

out vec4 frag_color;

uniform sampler2D tex1;

void main() {
	//float cosTheta = 1.0 + dot( out_normal, out_cam_dir) ;
	frag_color = texture(tex1, out_uv);// * cosTheta;
}