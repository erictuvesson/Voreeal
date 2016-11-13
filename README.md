# Voreeal [_UnrealEngine 4.13_]
Voreeal is a voxel library for Unreal Engine 4 powered by [PolyVox](https://bitbucket.org/volumesoffun/polyvox).

> This project is still a work in progress, a lot of things are not implemented yet.

## Supported voxel formats:
* [Qubicle](http://www.minddesk.com/) Binary (*.qb) 
* [MagicaVoxel](https://ephtracy.github.io/) (*.vox)

## Types of Actors
Voreeal support a few different types of actors for different kind of jobs.

* **BasicVolume**: A basic volume for representing fixed volumes.
* **RegionVolume**: Allows for rendering a small part of a volume.
* **PagedVolume**: Endless volume.

## Getting Started

Download this repository and place it in your projects under 'Plugins' do this via GIT command line or 
a GIT client as if you download the ZIP it will not work as intended if at all this is because Polyvox is a subproject which does not download in the ZIP version. then Restart Unreal Engine 4
and it will ask you to rebuild the plugin, once that is done you can just drag in any of the supported
voxel formats into your project. 

All the blueprint functions are located under the category 'Voreeal'.
