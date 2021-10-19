#!/bin/bash
DIRS="examples include src tests"

find $DIRS \( -iname '*.cpp' -or -iname '*.hpp' \) -exec clang-format -style=file -i '{}' +
