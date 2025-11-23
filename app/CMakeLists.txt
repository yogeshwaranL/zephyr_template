# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# Find Zephyr package. This also loads Zephyr's build system.
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Define project name
project(zephyr_app)

# Add application sources
target_sources(app PRIVATE src/main.c)

# Add application include directories if needed
# target_include_directories(app PRIVATE include)
