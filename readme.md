Resources Used:
learnOpenGl.com

# OpenGLViewer
[Link to Repository](https://github.com/haroonsyed/OpenGLViewer)

## Made in CAP4730: Computer Graphics at the University of Florida
### Under Dr. Aleirza Entezari

![Render Example](./docs/dolphins.png)

### Features
- Animated/Rotating View
- GPU side mesh rendering
- Separate Vertex and Fragment Shader files
- Color interpolation between vertices on face
- Loading obj files (given tri mesh structure)


### Technologies Used
- GLFW
- OPENGL
- VCPKG
- GLM
- C++
- Fundamentals of Computer Graphics 3rd Edition
- WSL2
- CMAKE

### Compilation Instructions
**Windows**
1. Clone this repo using <br>
`git clone https://github.com/haroonsyed/OpenGLViewer.git --recursive`
2. Install cmake
3. Open the folder containing repo in visual studio, run main.cpp

    or

3. CD into build folder and run `cmake ..`
4. Open generated solution (in build folder) in visual studio and run main.cpp

**Linux**
1. Clone this repo using <br>
`git clone https://github.com/haroonsyed/OpenGLViewer.git --recursive`
2. Install the following dependencies: 
libxmu-dev libxi-dev libgl-dev libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev
3. Install cmake
4. CD into build folder and run `cmake ..`
5. Run `make` to build your executable
6. Run `./main` 

### Operation Guide
Change line 111 of main to load different obj files from "data" folder.

<br>

### Outside Help
I mainly relied on https://learnopengl.com/ and the class lecture notes to understand what libraries are needed


I also used https://docs.gl/ to see information regarding the opengl library.

## Assignment Requriements Checklist:
| Feature | Status | Extra Desc. |
| ---------------------------------- | --------- | ------------------ |
| Modify vertex and frag shader to support color data | :white_check_mark: | |
| Load meshes from obj files | :white_check_mark: | Only works with tri meshes (no quads/n-gons) |
| Load vertex and frag shaders from files (.vs and .fs) | :white_check_mark: | |
| Make change to vertex/frag shader | :white_check_mark: | Passed model transformation matrix to rotate model as a function of time |
| Bonus: Implement EBO with indexed triangle structure | :x: | |
| Documentation | :x: | | 