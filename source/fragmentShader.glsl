#version 330 core

out vec3 colour;
in vec2 UV; // Texture coordinates from vertex shader
in vec3 fragmentPosition;
in vec3 Normal;

out vec3 fragmentColour;



struct Light{
    vec3 position;
    vec3 colour;
    float constant;
    float linear;
    float quadratic;
    int type
} ---> Carry on from this 


//uniforms
uniform sampler2D diffuseMap;
uniform float ka;
uniform float kd;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform float ks;
uniform float Ns;
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform bool useLighting;

void main()
{
    vec3 objectColour = vec3(texture(diffuseMap, UV)); //texture colour
    
    
    if(!useLighting){
        fragmentColour = objectColour;
        return;
    }
    
    ////
    vec3 ambient = ka * objectColour; //ambient light reflection
    ////
    ////
    vec3 light = normalize(lightPos - fragmentPosition);
    vec3 normal = normalize(Normal);
    float cosTheta = max(dot(normal, light), 0);   ///Diffuse light reflection
    vec3 diffuse = kd * lightColour * objectColour * cosTheta;
    ////
    vec3 camera = normalize(-fragmentPosition);
    vec3 reflection = - light + 2 * dot(light, normal) * normal;
    float cosAlpha = max(dot(camera, reflection), 0);
    vec3 specular = ks * lightColour * pow(cosAlpha, Ns); //specular light reflection
    ////
    float distance = length(lightPos - fragmentPosition);
    float attenuation = 1.0 / (constant + linear *distance + quadratic *distance *distance);

    
    fragmentColour = (ambient + diffuse + specular) * attenuation;
}