# gf3d
a collection of utlitity functions designed to facilitate creating 3D games with ~~OpenGL~~ Vulkan and SDL2
This project is specifically intended to function as an educational tool for my students taking 3D Game Programming.

Currently the project is in a WIP state.
It represents my first stab at getting vulkan rendering pipeline setup.
It is not stable and I intend to do a complete rewrite of the vulkan system as soon as I can find some time

# Additions to gf3d
The following will be a compilation of my additions to the provided gf3d base.
* Animation system
* Entity system
* Networked coop (WIP)
* Audio system
* World loading from config (WIP)
* Radius based collision detection
* 2D UI Layer (WIP)
* Text rendering (WIP)
* User control system (WIP)

## Controls
| Key | Action                                   |
|:---:|------------------------------------------|
|  W  | Move forward                             |
|  A  | Move left                                |
|  S  | Move backwards                           |
|  D  | Move right                               |
|  C  | Move up                                  |
|  Z  | Move down                                |

## Demo
[![Demo](https://i.imgur.com/a/rGRh1pB.gif)](https://i.imgur.com/a/rGRh1pB.mp4)
[Video Link](https://i.imgur.com/YqDPulW.mp4)
# Build Process
Required dependencies are:
* [SDL2-2.0.16 (devel)](https://www.libsdl.org/release/)
* [SDL2_ttf-2.0.15 (devel)](https://www.libsdl.org/projects/SDL_ttf/release/)
* [SDL2_mixer-2.0.4 (devel)](https://www.libsdl.org/projects/SDL_mixer/release/)
* [SDL2_image-2.0.5 (devel)](https://www.libsdl.org/projects/SDL_image/release/)
* [VulkanSDK_1.2.189.2](https://vulkan.lunarg.com/sdk/home)

Before you can build the example code we are providing for you, you will need to obtain the libraries required
by the source code. Some of these libraries are easy to obtain and others are provided by the vendors of hardware
that is installed in your computer. After the libraries are obtained the following sequence of steps should be
performable from the following steps from the root of the cloned git repository within a terminal. 

1. Obtain the code: `git clone <repo name>`
2. Checkout seed branch: `git checkout <branch name>`
3. Make sure ou fetch submodules: `git submodule update --init --recursive`
4. Build libraries: `pushd gfc/src; make; popd`
5. Build game: `pushd src; make; popd`

You should now have a `gf3d` binary within the root of your git repository. Executing this will start your game.
