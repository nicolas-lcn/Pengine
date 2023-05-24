#version 330 core

in vec3 o_positionWorld;
in vec2 o_uv0;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D menu_texture;

void main() {
    //FragColor = color;
    FragColor = texture(menu_texture, o_uv0);
    // DEBUG: position
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // DEBUG: normal
    // FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);
}
