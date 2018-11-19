# Voreeal _[Unreal Engine 4.14]_
Voreeal is a voxel library for Unreal Engine 4 powered by [PolyVox](https://bitbucket.org/volumesoffun/polyvox).

> __This project has been discontinued. If you are looking for a voxel framework I recommend [VoxelFarm](https://www.voxelfarm.com/index.html).__

> __This project is still a work in progress, a lot of things are not implemented yet.__

## Supported voxel formats:
* [Qubicle Binary (*.qb)](http://www.minddesk.com/)
* [MagicaVoxel (*.vox)](https://ephtracy.github.io/) 

## Types of Actors
Voreeal support a few different types of actors for different kind of jobs.

* **BasicVolume**: A basic volume for representing fixed volumes.
* **RegionVolume**: Allows for rendering a small part of a volume.
* **PagedVolume**: Endless volume.

## Getting Started

> When there is a stable working version of the plugin I will release it here on github so you can download a pre-compiled version.

> __Currently your project requires C++ files so it could be compiled.__

1. Clone the [repository][0] using [Git][1] into your [Game plugin folder][2]. It is __important__ that you clone because we are using submodules which is not supported by the download button!
2. Rebuild the plugin, once that is done you can just drag in any of the supported voxel formats into your project. 

All the blueprint functions are located under the category 'Voreeal'.

[0]: https://github.com/ChillyFlashER/Voreeal.git
[1]: https://help.github.com/articles/cloning-a-repository/
[2]: https://docs.unrealengine.com/latest/INT/Programming/Plugins/#pluginfolders
