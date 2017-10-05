#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

uniform mat4 view, proj, model;

out vec3 fragPos, normalPos;


void main(){

  fragPos = vec3(model*vec4(vertex_position, 1.0));
  normalPos = vertex_normal;  
  gl_Position = proj * view * vec4(fragPos, 1.0);
}


  