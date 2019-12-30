# Changelog

## Unreleased

### Added

- library can now easier be used as a CMake subproject
- new CMake option `BW64_PACKAGE_AND_INSTALL`

### Changed

- Renamed CMake library target name from `libbw64` to `bw64`
- Renamed CMake option `UNIT_TESTS` to `BW64_UNIT_TESTS`
- Renamed CMake option `EXAMPLES` to `BW64_EXAMPLES`

### Fixed

- Fix sample rate parameter type in `writeFile()` and `BW64Writer` ctor to support 96k samplerates

## 0.10.0 - (January 18, 2019)
### Added

- Additional unit tests

### Changed

- Use `Catch2` instead of `Boost.Test` for unit testing

### Fixed

- Fix `Bw64Reader::seek()` and `Bw64Reader::tell()` implementation
- RIFF chunk size calculation

## 0.9.0 - (July 23, 2018)

Initial release
