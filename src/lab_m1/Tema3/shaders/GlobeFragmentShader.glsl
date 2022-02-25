#version 330

// Input
in vec3 world_position;

// Uniform
uniform sampler2D globe_texture;
uniform vec3 disco_ball_position;
uniform float delta_time;

// Output
layout(location = 0) out vec4 out_color;

void main() {
	vec3 light_dir = world_position - disco_ball_position;
 
	vec2 texcoord;
	texcoord.x = (1.0 / (2 * 3.14159)) * atan(light_dir.x, light_dir.z) - delta_time;
	texcoord.y = (1.0 / 3.14159) * acos(light_dir.y / length(light_dir));
 
	vec3 color = texture(globe_texture, texcoord).xyz;

	out_color = vec4(color, 1);
}
