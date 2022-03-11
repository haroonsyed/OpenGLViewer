#version 330 core
in vec4 vertexColor;
in vec3 transformedPos;
in vec3 transformedNorm;

out vec4 FragColor;

uniform vec3 lightPos;

void main()
{
   // Calculate vectors needed for lighting
   vec3 toLight = normalize(lightPos-transformedPos);
   vec3 viewDir = normalize(transformedPos);
   vec3 h = normalize((viewDir) + toLight);

   float ambient = 0.1;
   float specularStrength = 0.5;
   float phongExp = 10000;

   float diffuse = max(dot(transformedNorm,toLight),0);
   float specular = max(pow(dot(transformedNorm,h), 100),0);
   
   FragColor = (ambient + diffuse) * vertexColor + specular * vec4(1.0);
};