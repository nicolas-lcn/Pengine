#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 o_uv0;

void main() {
  o_uv0 = uv0;
  o_uv0[1] = -uv0[1];
  vec4 positionWorld = model * vec4(position, 1.0, 1.0);
  gl_Position = projection * view * positionWorld;
}
