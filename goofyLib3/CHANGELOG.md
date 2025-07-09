# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- built in test shaders or fallback one
- many lights at once or inbuilt lighting (idk if i will really add)
- more mesh file support
- better texture generation
- actual error handling ( Ihate crashes)
- MAYBE flexible buffers (like different values)

### Changed
- optimize batch rendering with dynamic buffer resizing and less CPU-GPU sync
- refactor API for even simpler mesh creation and rendering

### Fixed
- improve error handling for shader compilation and resource loading
- fix edge cases in mesh transformation functions

### Deprecated
- legacy mesh loading functions (to be replaced by new high-level asset loader)

### Removed
- none

### Fixed
- none

### Security
- none


## [Release 1.0.0 "The start"] - 2025-07-07

### Added
- core library with window creation and OpenGL setup
- easy shader compilation
- some mesh functions for easy usage
- batch rendering
- a global texture array
- basic helper functions for transforms and debugging

### Changed
- change internal buffer system to support multiple meshes with batching offsets
- change `GOOFY_BUFFER` structure to track multiple meshes and offsets for efficient batching
- rename `GOOFY_VERTICE` to `GOOFY_VERTEX` for naming consistency
- simplify `goofy_renderMesh` API to remove manual offset parameters; offsets now managed internally
- add `goofy_drawAllMeshes` for drawing all buffered meshes in one call
- ername and reorganized function names for clarity and consistency like objMesh
- add constants for `MAX_MESHES` and expanded internal arrays to support larger batch sizes

### Deprecated
- manual vertex and index offset management in `goofy_renderMesh` (replaced with internal batching)
- original simple `GOOFY_BUFFER` struct with only VAO, VBO, EBO (replaced with extended struct)

### Removed
- offset parameters from `goofy_renderMesh` function signature
- old version of `GOOFY_BUFFER` struct without batching support

### Fixed
- improved memory management with safer mesh reallocation functions
- fixed potential bugs in mesh appending logic by copying vertex and index counts correctly

### Security
- none

## [Release 1.0.1 "One tiny mistake"] - 2025-07-07

### Added
- manual version setting for window creation

### Changed
- improved internal code and bug fixes
- added improved comments to header file

### Deprecated
- none

### Removed
- none

### Fixed
- fixed bug where malloc wasnt used in variable sized table structs causing bug

### Security
- none

## [Release 1.1.0 "some improvements!"] - 2025-07-08

### Added
- primitive mesh creation functions
- added automatic trash collection system
- goofy_terminate now frees all trash
- organized functions into sections for easier searching

### Changed
- changed deleteBuffer function name to freeBuffer
- changed some int values into short or char to save space

### Deprecated
- none

### Removed
- none

### Fixed
- bug where glwfMakeContextcurrent wasnt set for each window resize causing it to not resize or resize wrong window

### Security
- none

## [Release 1.2.0 "Deprecated stuff"] - 2025-07-09

### Added
- goofy_cubeMesh function added. Returns a cube mesh.
- comments to .h file
- Independent texture arrays, summary of all objects is in changed section
- TextureArrays can now be added to trash collection.
- added GOOFY_TEXTURE_ARRAY object.
- added mesh rotation function
- added mesh texture setting function
- added sphere creation
- added mesh resizing
- simplified primitives creation

### Changed
- goofy_initTextures now requires an ID argument and returns an independent texture array instance.
- You now need to pass a texture array to drawAllMeshes.
- goofy_initTrashBatch nows returns a trashBatch object, i don't know why i didnt do this earlier.
- window sizes and version parameters changed back to int

### Deprecated
- none

### Removed
- shaderProgram argument in goofy_renderMesh function. It does nothing.

### Fixed
- bug with invalid error handling in trash

### Security
- none

