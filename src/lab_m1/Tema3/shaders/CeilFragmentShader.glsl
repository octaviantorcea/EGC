#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniform
uniform int is_light_from_globe;
uniform vec3 eye_position;

// self properties
uniform float ceil_Kd;
uniform float ceil_Ks;
uniform int material_shininess;

// disco_ball properties
uniform sampler2D globe_texture;
uniform vec3 disco_ball_position;
uniform float globe_intensity;
uniform float delta_time;

// Output
layout(location = 0) out vec4 out_color;

vec3 light_from_globe() {
	vec3 color = vec3(0);
	vec3 globe_color = vec3(0);

	vec3 N = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position);
    vec3 L = normalize(disco_ball_position - world_position);
    vec3 H = normalize(L + V);

	vec3 light_dir = world_position - disco_ball_position;
 
	vec2 texcoord;
	texcoord.x = (1.0 / (2 * 3.14159)) * atan(light_dir.x, light_dir.z) - delta_time;
	texcoord.y = (1.0 / 3.14159) * acos(light_dir.y / length(light_dir));
 
	globe_color = texture(globe_texture, texcoord).xyz;

	float fAtt = 1 / (1 + distance(disco_ball_position, world_position));
	float diffuseLight = ceil_Kd * max(dot(N, L), 0);
	float specularLight = 0;

    if (dot(N, L) > 0) {
		specularLight = ceil_Ks * pow(max(dot(N, H), 0), material_shininess);
    }

	color = fAtt * globe_intensity * globe_color * (diffuseLight + specularLight);

	return color;
}

void main() {
	out_color = vec4(0, 0, 0, 1);

	if (is_light_from_globe == 1) {
		out_color = vec4(light_from_globe(), 1);
	}
}
