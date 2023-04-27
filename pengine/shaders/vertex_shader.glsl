#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 model_matrix;
uniform mat4 view_matrix; // camera
uniform mat4 proj_matrix;

mat4 transformation_matrix;

// texture
out vec2 coord_txt;
layout(location = 2) in vec2 coord;

// heightmap
out float height;


void main(){

        transformation_matrix = proj_matrix * view_matrix * model_matrix; // MVP but inverted! (order matters)

        gl_Position = transformation_matrix * vec4(vertices_position_modelspace, 1);
        //gl_Position = vec4(vertices_position_modelspace, 1);

        coord_txt = coord; // texture

        height = vertices_position_modelspace.y;
}

