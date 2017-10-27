# CHANGELOG

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

## 29.09.2017
### Added

- Added CHANGELOG.md
- Added Vector4 operators
- Added GetParent to EntityHandle
- Added LookAt and Identity to Quaternation
- 

### Changed

- Reworked Camera
- Fixed some scene's matrix issues
- Added temporary solution for Camera's matrix calculations.
