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
    
    vec4 ambient = light_ambient*material_ambient;
    
    vec4 incident_ray  = light_position-position;
    vec4 vertex_normal = normalize(normal);
    float Kd = max(dot(normalize(incident_ray),vertex_normal),0); // lambert factor  //XXX Use step() for these?
//XXX How does this work? http://www.learnopengles.com/android-lesson-two-ambient-and-diffuse-lighting/
//    float luminosity = pow(length(incident_ray),-2); // attentuation
//    Kd =  Kd/(1+(0.25*(1/luminosity)));              // attenutation (dampened)
    vec4 diffuse = Kd*light_diffuse*material_diffuse;
    
    vec4 vertex_position = normalize(position);
    vec4 normal_halfway  = normalize(normalize(incident_ray)+vertex_position);
    float Ks = pow(max(dot(vertex_normal,normal_halfway),0),material_shininess);
    vec4 specular = Ks*light_specular*material_specular;
    
     gl_Position = projection*view*model*position;
    vertex_color = color*(ambient+diffuse+specular);
}
