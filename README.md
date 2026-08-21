# Reflekt Engine 3D

## OpenGL 4.6 based rendering and game engine, uses c++ as its main language in the cpu side and glsl as the shading language.

### Rendering

Lighting: *uses Phong lighting with n number of light casters (directional, point, spot)*

Skybox: *uses cubemap to render a skybox!*

Object / Model architecture: *all geometry data is stored in meshData, and all visual data eg. textures, shininess etc is stored in materials, and mesh class includes the meshdata and material. Then a model class abstracts that and packs meshes, transform, and various other components into one that is then rendered into the scene.*

UI: uses imgui for now and has implementation of an hierarchy for objects in scene and inspector for components of an object eg transform as well as a performance overlay to monitor performance.... Tho planning on implementing custom ui using orthographic planes in future 

Rendering: Is done in multiple stages those being:

          PRE PASS
    1- Render Scene into framebuffer(Render Skybox -> Render Opaque objects -> Render transparent objects -> Render UI)
          POST PASS
    2- Perform Post process passes
    3- Render the image to screen

Shaders: **uses D3D style shaders so while writing them use any extension but .shader is kinda nice and use keywords #SHADER {type} to write all the shaders in one file! Example:** 

                #SHADER VERTEX
                ...
                #SHADER FRAGMENT
                ....
                #SHADER GEOMETRY
                ...
                #SHADER TESS_CONTROL
                ...
                #SHADER TESS_EVALUATION
                ...
                #SHADER COMPUTE
                ...

                
    And use exactly these keywords for the shader type as it is case sensitive
                
---


### Dependencies :

    ASSIMP
    
    Link: https://github.com/assimp/assimp
  
    Linking Instructions:
  
      -Clone the repo,
      -Use cmake (GUI or CLI) to build the project files, Now we need the static link files so for that while using cmake CLI enter the Command: "cmake --preset=assimp_static"
      -Now open the .slnx file, Select the project and build the solution in both Debug and Release configuration (it is necessary as for example debug config will work only with debug mode in our project so we need both if we want to use both debug and release)
      -The debug and release files will be created for ASSIMP and ZLIB we want the .lib files in out project 
      -Navigate to "/lib" and the debug and release files will be there copy paste all of them and paste those into our project in the directory "External/Assimp/lib/" and make sure we have the 3 files there being: assimp-vc145-mtd.lib, assimp-vc145-mt.lib and unit.lib
      -Navigate to "/contrib/zlib/" and there will be the debug and release folder from either one copy paste the zlibstatic.lib file into the Assimp lib folder in our project being: "External/ASSIMP/lib/" and paste it there
      -Make sure that the 4 files are there as it is and not in any other folder, and the project is set up so that those files being there should work!
      -However if you wish or need to then also replace the include folder in "External/ASSIMP" with the include folder with the ASSIMP files you downloaded.
      *External/ASSIMP/lib folder should have: assimp-vc145-mtd.lib, assimp-vc145-mt.lib, unit.lib, zlibstatic.lib
      
  (Already in repo and is linked):

    GLFW (3.3) 
    GLAD (4.6.0) 
    STB (image)
    GLM
    IMGUI

  **Note: All the libraries used are linked statically and some of them aren't included with the repo for those instructions are provided on how to get the files and link them if needed. Some demo assets will be provided to set up a scene quickly!
---
