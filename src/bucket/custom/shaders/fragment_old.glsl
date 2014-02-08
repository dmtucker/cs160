#version 330

in vec4 vertex_position;
in vec4 vertex_color;
in vec2 vertex_texture;
in vec4 vertex_normal;

uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform float material_shininess;
uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;

uniform sampler2D texture_map;

out vec4 fragment_color;

void main ( ) {
    
    vec4 ambient = light_ambient*material_ambient;
    
    vec4 normal_light  = normalize((light_position.w == 0) ?
        light_position:
        light_position-vertex_position);
    vec4 normal_normal = normalize(vertex_normal);
    float Kd = max(dot(normal_light,normal_normal),0);
    vec4 diffuse = light_diffuse*material_diffuse;
    
    vec4 normal_position = normalize(vertex_position);
    vec4 normal_halfway  = normalize(normal_light+normal_position);
    float Ks = (Kd == 0) ? 0 : pow(max(dot(normal_normal,normal_halfway),0),material_shininess);
    vec4 specular = light_specular*material_specular;
    
    fragment_color = vertex_color
                   * texture2D(texture_map,vertex_texture)
                   * ambient+(Kd*diffuse)+(Ks*specular);
}
