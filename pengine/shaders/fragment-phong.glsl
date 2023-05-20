#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in mat3 o_TBN;
out vec4 FragColor;

uniform sampler2D texture_snow;

// Material
struct Material {
	vec3 ambient ;
	vec3 diffuse ;
	vec3 specular ;
	float shininess ;
};
uniform Material material;

//Light
struct Light 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 pos;
};
uniform Light light;

void main() {

	vec3 N = o_normalWorld; 
	// Calcul de la couleur diffuse
	vec3 Kd = material.diffuse;
	vec3 Ld = light.diffuse;
	vec3 L = normalize ( light.pos - o_positionWorld ) ;
	vec3 Id = Kd * Ld * max(dot(N, L), 0.0);

	// Calcul de la couleur spéculaire
	vec3 Ks = material.specular;
	vec3 Ls = light.specular;
	vec3 R = reflect(-L, N);
	vec3 E = normalize(-o_positionWorld);
	float specular = pow(max(dot(R, E), 0.0), material.shininess);
	vec3 Is = Ks * Ls * specular;

	//Calcul de la couleur ambiante
	vec3 ambient = vec3(0.0, 0.0, 0.0);

	// Calcul de la couleur finale
	vec3 color = ambient + Id + Is;

    FragColor = vec4(texture(texture_snow, o_uv0).rgb * color, 1.0f);
}
