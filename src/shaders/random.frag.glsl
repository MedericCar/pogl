#version 450

in vec3 FragPos;
in vec3 Normal;
in float coef;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 color;
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

    float mini = min(color.x, min(color.y ,color.z));
    vec3 color_center;

    if (mini == color.x)
        color_center = vec3(coef, 0, 0);
    else if (mini == color.y)
        color_center = vec3(0, coef, 0);
    else
        color_center = vec3(0, 0, coef);

    FragColor = vec4(color + color_center, 1);//clamp(coef + 0.5, 0.5, 1);
} 
