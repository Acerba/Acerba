# CHANGELOG

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
- Added SetID & GetID support for sprites
- Added CreateIndicies for GraphicsDevice
- Added Draw method for drawing array of sprites for GraphicsDevice

### Changed

- Transform members are private
- Transform stores status if it has been modified
- Sprite now uses C/C++ Vertex array instead of std::array.
- Vertex uv is now Vector3 instead of Vector2
- Tilemap now draws all tiles using one draw call.
- Tilemap now sets TileID for sprites


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

- Addded CHANGELOG.md
- Added Vector4 operators
- Added GetParent to EntityHandle
- Added LookAt and Identity to Quaternation

### Changed

- Reworked Camera
- Fixed some scene's matrix issues
- Added temporary solution for Camera's matrix calculations.