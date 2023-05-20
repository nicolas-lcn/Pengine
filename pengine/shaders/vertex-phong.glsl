#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv0;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;
//uniform mat4 mvp;
//uniform mat4 modelView;
//uniform mat4 normalMatrix;


out vec3 o_positionWorld;
out vec3 o_normalWorld;
out vec2 o_uv0;
out mat3 o_TBN;



void main() {
	mat3 normalMatrix = mat3(transpose(inverse(model_matrix)));
	o_uv0 = uv0;
	o_uv0[1] = -uv0[1];
	vec4 positionWorld = model_matrix * vec4(position, 1.0);
	o_positionWorld = positionWorld.xyz;
	o_normalWorld = normalMatrix * normal;
	gl_Position = proj_matrix * view_matrix * positionWorld;
	vec3 T = normalize(vec3(model_matrix * vec4(tangent, 0.0)));
	vec3 N = normalize(vec3(model_matrix * vec4(normal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	o_TBN = mat3(T, B, N);
}