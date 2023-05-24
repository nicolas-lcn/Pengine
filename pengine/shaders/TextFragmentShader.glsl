#version 330 core

in vec2 o_uv0;
out vec4 FragColor;

uniform sampler2D myTextureSampler;

void main() {
    FragColor = texture(myTextureSampler, o_uv0);
}
