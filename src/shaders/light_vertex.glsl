#version 330

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec4 position;
in vec4 color;
in vec4 normal;

uniform vec4 light_position;
uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform float material_shininess;
uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;

out vec4 vertex_color;

void main ( ) {
    
    vec4 incident_ray    = light_position-position;
    vec4 incident_normal = normalize(incident_ray);
    vec4   vertex_normal = normalize(normal);
    
    vec4 ambient = light_ambient*material_ambient;
    
    float Kd = max(dot(incident_normal,vertex_normal),0); // Lambert factor
    vec4 diffuse = Kd*light_diffuse*material_diffuse;
    
    float Ks = pow(
        max(dot(vertex_normal,normalize(incident_normal+normalize(position))),0),
        material_shininess
    );
    vec4 specular = Ks*light_specular*material_specular;
    
    gl_Position  = projection*view*model*position;
    vertex_color = color*(ambient+diffuse+specular);
}
