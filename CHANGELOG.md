# Changelog

## Unreleased

This version fixes a number of buffer overruns, integer overflows, and uses of uninitialised data which may be triggered by reading malformed files; all users are advised to upgrade.

### Added

- library can now easier be used as a CMake subproject
- new CMake option `BW64_PACKAGE_AND_INSTALL`
- `AxmlChunk::data()`; this allows access to the internal string, avoiding a copy when reading
- `Bw64Writer::close()`; this should be called before destruction to properly catch exceptions

### Changed

- Renamed CMake library target name from `libbw64` to `bw64`
- Renamed CMake option `UNIT_TESTS` to `BW64_UNIT_TESTS`
- Renamed CMake option `EXAMPLES` to `BW64_EXAMPLES`
- `FormatInfoChunk::formatTag` now matches the formatTag in the file, rather than always returning 1
- fmt parsing is stricter -- the chunk size must match the use of cbSize, and the presence if extra data is checked against the formatTag
- strings can be moved into `AxmlChunk` with `std::make_shared<AxmlChunk>(std:move(some_str))`, to avoid a copy when writing

### Fixed

- Fix sample rate parameter type in `writeFile()` and `BW64Writer` ctor to support 96k samplerates
- fmt extra data is now written correctly
- axml chunks greater than 4GB are now written correctly

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
