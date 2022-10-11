#version 460 core

out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D texture_sampler;

void main() {
    frag_color = texture(texture_sampler, tex_coord);
}