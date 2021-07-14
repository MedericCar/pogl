#version 450

in vec3 FragPos;
in vec3 Normal;
in float coef;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float time;

void main()
{
    float ambientStrength = 0.1;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(reflect(-lightDir, norm), viewDir), 0.0), 32);

    float coef_center = 1 - max(abs(FragPos.x), abs(FragPos.y));
    vec3 color_center = vec3(0, coef_center, 0);

    //FragColor = vec4(color_center + (ambientStrength + diff + spec) * lightColor * (objectColor + color_center), 0.1);
    //FragColor = vec4(Normal, 1);
    FragColor = vec4(objectColor + color_center, 1);//clamp(coef + 0.5, 0.5, 1);
} 
