#version 330 core
#define MAX_LIGHTS 4

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace[MAX_LIGHTS];
} fs_in;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    sampler2D depthMap;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform samplerCube skybox;
uniform vec3 skyLightDir;
uniform vec3 skyLightColor;
uniform vec3 skyLightAmbient;

uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float opacity;
    float reflectivity;
    bool hasTexture;
};

uniform Material material;

float ShadowCalculation(vec4 fragPosLightSpace, Light light);
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTexture);

void main()
{   
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);

    // Cube mapping
    vec3 I = normalize(fs_in.FragPos - viewPos);
    vec3 R = reflect(I, normal);
    vec3 envColor = texture(skybox, R).rgb;
    
    vec3 textureColor = material.hasTexture ? texture(diffuseTexture, fs_in.TexCoords).rgb : vec3(1.0);

    textureColor = mix(textureColor, envColor, material.reflectivity);
    vec3 lighting = vec3(0.0);

    for (int i = 0; i < lightCount; ++i) {
      float shadow = ShadowCalculation(fs_in.FragPosLightSpace[i], lights[i]);
      vec3 phong = CalcPointLight(lights[i], normal, fs_in.FragPos, viewDir, textureColor);
      lighting += phong * (1.0 - shadow);
    }
    
    float skyLight = min(max(dot(normal, skyLightDir), 0.0), 1.0);
    lighting += skyLightAmbient * textureColor + skyLight * skyLightColor * textureColor;
    
    FragColor = vec4(lighting, material.opacity);
}

float ShadowCalculation(vec4 fragPosLightSpace, Light light)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;
    
    float closestDepth = texture(light.depthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.depthMap, 0);
    for(int x = -4; x <= 4; ++x)
    {
        for(int y = -4; y <= 4; ++y)
        {
            float pcfDepth = texture(light.depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 81.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffuseTexture)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
    vec3 diffuse  = light.diffuse  * diff * diffuseTexture;
    vec3 specular = light.specular * spec * material.specular;

    diffuse  *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);
} 

