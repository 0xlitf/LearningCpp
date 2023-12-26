# Head1stCG

## Deps

### C++

- [ ] [Modern C++ Tutorial: C++11/14/17/20 On the Fly](https://github.com/changkun/modern-cpp-tutorial)
- [ ] [CppReference C++ 20](https://en.cppreference.com/w/)

### Package Manager & Project Configuration Tools

- [ ] [Conan 2.0.10](https://conan.io/)
- [ ] [CMake 3.24.2](https://cmake.org/)

### C++ framework & Libs

- [ ] [Qt 6.6.0](https://www.qt.io/)
- [ ] [Vulkan 1.3.250.1](https://www.vulkan.org/)
- [ ] [VulkanSceneGraph 1.0.5](https://vsg-dev.github.io/vsg-dev.io/)
- [ ] [VTK 9.2.6](https://vtk.org/)
- [ ] [Poco 1.12.4](https://pocoproject.org/ "https://pocoproject.org/")

## Initial

```
git clone https://github.com/0xlitf/Head1stCG.git
```

## ChatGPT

Partial content of readme.md is from ChatGPT.

## Envs (use [RapidEE](https://www.rapidee.com/en/about "https://www.rapidee.com/en/about"))

~~CMAKE_CXX_COMPILER~~

~~C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.37.32822\bin\Hostx64\x64\cl.exe~~

#### Python

C:\envs\Python-3.10.11\Scripts\

C:\envs\Python-3.10.11\

#### VK_SDK_PATH

C:\VulkanSDK\1.3.250.1

#### VULKAN_SDK

C:\VulkanSDK\1.3.250.1

#### VCINSTALLDIR

C:\Program Files\Microsoft Visual Studio\2022\Professional\VC

#### CMake

C:\Qt\Tools\CMake_64\bin\

#### Qt

C:\Qt\6.6.0\msvc2019_64\bin

**change profiles:** C:\Users\Calcifer\.conan2\profiles\default

[settings]
arch=x86_64
build_type=Release
compiler=msvc
compiler.cppstd=**23**
compiler.runtime=dynamic
compiler.version=193
os=Windows

to solve vsg/1.0.5 install error:

**cppstd (14) is lower than the required C++ standard (17)**

## Errors record

if QtCreator can not detect the suit, change the compiler in Kits, compiler C & C++
