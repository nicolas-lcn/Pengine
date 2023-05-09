#version 330 core

// Ouput data
//(out) vec4 color;
//(out) vec4 FragColor;

// texture
uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snowrocks;
uniform sampler2D texture_snow;
in vec2 coord_txt;

// heightmap
in float height;

float coef_grass;
float coef_rock;
float coef_snowrocks;

uniform int isTerrain;

uniform vec4 mesh_color;

void main(){

        //color = vec3(0.0, 0.0, 1.0);

        if(isTerrain == 0){ // is not a terrain
                gl_FragColor = mesh_color;

        }else if(isTerrain == 1){

                //gl_FragColor = texture(texture_snow, coord_txt);
                gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
}
