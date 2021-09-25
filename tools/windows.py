#!/usr/bin/env python3
"""build script for windows for ride"""

import buildtools.windows as build


def main():
    """main function"""
    data = build.default_data('ride')
    build.add_dependency_sdl2(data)
    build.add_dependency_python(data)
    build.main(data)


if __name__ == "__main__":
    main()
