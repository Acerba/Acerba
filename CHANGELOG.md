# CHANGELOG

## 11.11.2017
### Added
- arm64-v8a support 

### Fixed
- TMXLite android paths
    - **NOTE 1:** .tmx files doens't need "source=map.tsx" to "source=assets/map.tsx" edit
    - **NOTE 2:** This means that .tsx files will be searched from **same directory** where .tmx files are

## 10.11.2017
### Added
- Added UI Material
- Camera added SetUISize and GetUISize methods

### Changed
- Updated SceneDrawing examples
- Fixed EnittyManger's GetComponents method

## 27.10.2017
### Added

- Added SetID & GetID support for sprites
- Added CreateIndicies for GraphicsDevice
- Added Draw method for drawing array of sprites for GraphicsDevice

### Changed

- Sprite now uses C/C++ Vertex array instead of std::array.
- Vertex uv is now Vector3 instead of Vector2
- Tilemap now draws all tiles using one draw call.
- Tilemap now sets TileID for sprites


## 1.11.2017

### Changed

- Improved support for static entities
- Updated CollisionExample



## 30.10.2017

### Added

- Animation Support
- Animation Component
- Animation Example
- Added TotalTime method for Time
- Added GetScale method for Sprite



## 29.10.2017

### Added

- Proper CollisionExample

### Changed

- Collidables work when used without ECS



## 27.10.2017

### Added

- Support for static entities

### Changed

- Transform members are private
- Transform stores status if it has been modified



## 20.10.2017

### Changed

- Changed Matrix, Quaternion and Vector implementation.



## 13.10.2017

### Added

- Drawable base
- Tilemap support

### Changed

- Spritesheet
- Keyboards
- Materials & Uniforms, reworked
- BaseComponent, fixed constants
- Path, added operators and absolute path



## 29.09.2017

### Added

- Added CHANGELOG.md
- Added Vector4 operators
- Added GetParent to EntityHandle
- Added LookAt and Identity to Quaternation

### Changed

- Reworked Camera
- Fixed some scene's matrix issues
- Added temporary solution for Camera's matrix calculations.