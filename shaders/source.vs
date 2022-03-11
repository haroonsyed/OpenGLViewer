#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aColor;

out vec4 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform vec3 light;

void main()
{
   gl_Position = view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);

   vec3 normal = vec3(model * vec4(normalize(aNorm),0.0));
   vec3 lightDir = normalize(light);

   float ambient = 0.1;
   float diffuse = max(dot(normal,lightDir),0);
   
   vertexColor = (ambient + diffuse) * vec4(aColor, 1.0);

};