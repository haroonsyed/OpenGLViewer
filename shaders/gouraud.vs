#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aColor;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform vec3 lightPos;

void main()
{
   vec3 transformedPos = vec3(model * vec4(aPos,1));
   vec3 transformedNorm = vec3(model * vec4(normalize(aNorm),0.0));

   // Calculate Position
   gl_Position = view * vec4(transformedPos,1);


   // Calculate vectors needed for lighting
   vec3 toLight = normalize(lightPos-transformedPos);
   vec3 viewDir = normalize(transformedPos);
   vec3 h = normalize((viewDir) + toLight);

   float ambient = 0.1;
   float specularStrength = 1;
   float phongExp = 100;

   float diffuse = max(dot(transformedNorm,toLight),0);
   float specular = max(pow(dot(transformedNorm,h), phongExp),0);
   
   vertexColor = (ambient + diffuse) * vec4(aColor, 1.0) + specularStrength * specular * vec4(1.0);

};