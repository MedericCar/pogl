#version 450

in vec3 FragWorldPos;
in vec3 FragWorldPosDisplaced;
in vec3 Normal;
in float maxDelta;
in float delta;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float time;


void main()
{
    float ambientStrength = 0.2;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragWorldPos);
    vec3 viewDir = normalize(viewPos - FragWorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(reflect(-lightDir, norm), viewDir), 0.0), 32);

    //FragColor = vec4((ambientStrength + diff + spec) * lightColor * vec3(1, 0, 0), 1.0);
    vec3 color1 = vec3(1.0, 0.55, 0.0);
    vec3 color2 = vec3(0.6157, 0.1216, 0.0);
    vec3 color = mix(color1, color2, delta / maxDelta);

    //FragColor = vec4(delta / maxDelta, 0, 0, 1.0);
    FragColor = vec4(color, 1.0);
} 
