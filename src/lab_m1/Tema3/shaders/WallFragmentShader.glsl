#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniform
uniform int light_from_danceFloor;
uniform int is_light_from_spotlight;
uniform int is_light_from_globe;
uniform vec3 eye_position;

// self properties
uniform float wall_Kd;
uniform float wall_Ks;
uniform int material_shininess;

// danceFloor light
uniform vec3 danceFloor_light_position[3];
uniform float floorTileLightRadius;
uniform vec3 danceFloor_light_color[3];

// spotlights light
uniform vec3 spotlight_position[4];
uniform vec3 spotlight_direction[4];
uniform vec3 spotlight_color[4];
uniform float cutoff_angle;
uniform float spotlight_intensity;

// disco_ball properties
uniform sampler2D globe_texture;
uniform vec3 disco_ball_position;
uniform float globe_intensity;
uniform float delta_time;

// Output
layout(location = 0) out vec4 out_color;

vec3 light_from_dance_floor_tile(vec3 lightPos, vec3 lightColor) {
    vec3 color = vec3(0);

    float dist = distance(lightPos, world_position);

    if (dist < floorTileLightRadius) {
        vec3 N = normalize(world_normal);
        vec3 V = normalize(eye_position - world_position);
        vec3 L = normalize(lightPos - world_position);
        vec3 H = normalize(L + V);

        float fAtt = pow(floorTileLightRadius - dist, 2);
        float diffuseLight = wall_Kd * max(dot(N, L), 0);
        float specularLight = 0;

        if (dot(N, L) > 0) {
            specularLight = wall_Ks * pow(max(dot(N, H), 0), material_shininess);
        }

        color = fAtt * lightColor * (diffuseLight + specularLight);
    }

    return color;
}

vec3 light_from_spotlight(int index) {
    vec3 N = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position);
    vec3 L = normalize(spotlight_position[index] - world_position);
    vec3 H = normalize(L + V);

    float cut_off = radians(cutoff_angle);
    float spot_light = dot(-L, spotlight_direction[index]);
    float spot_light_limit = cos(cut_off);

    vec3 color = vec3(0);

    if (spot_light > spot_light_limit) {
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);

        float fAtt = 1 / (1 + distance(spotlight_position[index], world_position));
        float diffuseLight = wall_Kd * max(dot(N, L), 0);
        float specularLight = 0;

        if (dot(N, L) > 0) {
            specularLight = wall_Ks * pow(max(dot(N, H), 0), material_shininess);
        }

        color = fAtt * light_att_factor * spotlight_intensity * spotlight_color[index] * (diffuseLight + specularLight);
    }

    return color;
}

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
	float diffuseLight = wall_Kd * max(dot(N, L), 0);
	float specularLight = 0;

    if (dot(N, L) > 0) {
		specularLight = wall_Ks * pow(max(dot(N, H), 0), material_shininess);
    }

	color = fAtt * globe_intensity * globe_color * (diffuseLight + specularLight);

	return color;
}

void main() {
    vec3 finalColor = vec3(0);

    if (light_from_danceFloor == 1) {
        for (int i = 0; i < 3; i++) {
            finalColor +=  light_from_dance_floor_tile(danceFloor_light_position[i], danceFloor_light_color[i]);
        }
    }

    if (is_light_from_spotlight == 1) {
        for (int i = 0; i < 4; i++) {
            finalColor += light_from_spotlight(i);
        }
    }

    if (is_light_from_globe == 1) {
        finalColor += light_from_globe();
    }

    out_color = vec4(finalColor, 1);
}
