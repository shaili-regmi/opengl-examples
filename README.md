# opengl-examples

Collects opengl examples from lecture  

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
opengl-examples $ mkdir build
opengl-examples $ cd build
opengl-examples/build $ cmake -G "Visual Studio 16 2019" ..
opengl-examples/build $ start CS312-OpenGLExamples.sln
```

Your solution file should contain multiple projects, such as `gradient` and `normals`.
To run from the git bash command shell, 

```
opengl-examples/build $ ../bin/Debug/triangle
```

*macOS*

Open terminal to the directory containing this repository.

```
opengl-examples $ mkdir build
opengl-examples $ cd build
opengl-examples/build $ cmake ..
opengl-examples/build $ make
```

To run each program from build, you would type

```
opengl-examples/build $ ../bin/triangle
```

