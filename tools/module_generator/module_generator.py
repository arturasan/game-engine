#!/usr/bin/env python3

"""
Module Generator Tool for Game Engine

This script generates boilerplate code for new engine modules.
"""

import os
import sys
import argparse
import json
from pathlib import Path
from typing import Dict, List

# Module templates
MODULE_CPP_TEMPLATE = """#include "{module_name}.hpp"
#include <iostream>

namespace GameEngine {{

// Module factory function
extern "C" IModule* Create{ModuleClassName}() {{
    return new {ModuleClassName}();
}}

{ModuleClassName}::{ModuleClassName}() {{
    std::cout << "Creating {module_name} module..." << std::endl;
}}

{ModuleClassName}::~{ModuleClassName}() {{
    std::cout << "Destroying {module_name} module..." << std::endl;
}}

bool {ModuleClassName}::Initialize(EngineCore& engine) {{
    this->engine = &engine;
    std::cout << "Initializing {module_name} module..." << std::endl;
    
    // TODO: Implement module initialization
    
    std::cout << "{module_name} module initialized successfully!" << std::endl;
    return true;
}}

void {ModuleClassName}::Shutdown() {{
    std::cout << "Shutting down {module_name} module..." << std::endl;
    
    // TODO: Implement module cleanup
    
    std::cout << "{module_name} module shutdown complete!" << std::endl;
}}

void {ModuleClassName}::Update(float deltaTime) {{
    // TODO: Implement update logic
}}

void {ModuleClassName}::Render() {{
    // TODO: Implement render logic
}}

void {ModuleClassName}::OnReload() {{
    std::cout << "Reloading {module_name} module..." << std::endl;
    
    // TODO: Implement reload logic
    
    std::cout << "{module_name} module reloaded!" << std::endl;
}}

}} // namespace GameEngine
"""

MODULE_HPP_TEMPLATE = """#pragma once

#include "core/IModule.hpp"
#include "core/Engine.hpp"

namespace GameEngine {{

class {ModuleClassName} : public IModule {{
public:
    {ModuleClassName}();
    ~{ModuleClassName}();
    
    // IModule interface
    bool Initialize(EngineCore& engine) override;
    void Shutdown() override;
    void Update(float deltaTime) override;
    void Render() override;
    void OnReload() override;
    
    const char* GetName() const override {{ return "{module_name}"; }}
    uint32_t GetVersion() const override {{ return {version_hex}; }}
    const char* GetDescription() const override {{ return "{description}"; }}
    
    // TODO: Add module-specific public methods
    
private:
    // TODO: Add module-specific private members
}};

}} // namespace GameEngine
"""

CMAKE_TEMPLATE = """# {module_name} module
add_library({module_name} MODULE
    src/{module_name}.cpp
    include/{module_name}.hpp
)

# Include directories
target_include_directories({module_name}
    PUBLIC ${{CMAKE_CURRENT_SOURCE_DIR}}/include
    PRIVATE ${{CMAKE_CURRENT_SOURCE_DIR}}/src
)

# Link libraries
target_link_libraries({module_name}
    PUBLIC
        engine_core
{dependencies}
    PRIVATE
        # Add private dependencies here
)

# Set C++ standard
target_compile_features({module_name} PRIVATE cxx_std_20)

# Set properties for module
set_target_properties({module_name} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${{CMAKE_BINARY_DIR}}/modules"
    PREFIX "" # No lib prefix on Unix
    SUFFIX ".module" # Custom suffix for modules
)

# Install
install(TARGETS {module_name}
    LIBRARY DESTINATION modules
)
"""

MODULE_JSON_TEMPLATE = """{{
    "name": "{module_name}",
    "version": "{version}",
    "api_version": {api_version},
    "dependencies": {dependencies_list},
    "entry_point": "Create{ModuleClassName}",
    "description": "{description}",
    "author": "Game Engine Team",
    "license": "MIT",
    "platforms": ["windows", "linux", "macos"],
    "tags": {tags_list}
}}
"""

def create_module_directory(module_name: str, base_path: Path) -> Path:
    """Create the module directory structure."""
    module_path = base_path / module_name
    include_path = module_path / "include"
    src_path = module_path / "src"
    
    # Create directories
    module_path.mkdir(parents=True, exist_ok=True)
    include_path.mkdir(exist_ok=True)
    src_path.mkdir(exist_ok=True)
    
    return module_path

def generate_cpp_file(module_name: str, module_class_name: str, output_path: Path):
    """Generate the module C++ implementation file."""
    content = MODULE_CPP_TEMPLATE.format(
        module_name=module_name,
        ModuleClassName=module_class_name
    )
    
    cpp_file = output_path / f"{module_name}.cpp"
    with open(cpp_file, 'w') as f:
        f.write(content)
    
    print(f"Generated: {cpp_file}")

def generate_hpp_file(module_name: str, module_class_name: str, version_hex: int, 
                     description: str, output_path: Path):
    """Generate the module header file."""
    content = MODULE_HPP_TEMPLATE.format(
        module_name=module_name,
        ModuleClassName=module_class_name,
        version_hex=version_hex,
        description=description
    )
    
    hpp_file = output_path / f"{module_name}.hpp"
    with open(hpp_file, 'w') as f:
        f.write(content)
    
    print(f"Generated: {hpp_file}")

def generate_cmake_file(module_name: str, dependencies: List[str], output_path: Path):
    """Generate the CMakeLists.txt file."""
    deps_str = ""
    if dependencies:
        deps_str = "\n".join(f"        {dep}" for dep in dependencies)
    
    content = CMAKE_TEMPLATE.format(
        module_name=module_name,
        dependencies=deps_str
    )
    
    cmake_file = output_path / "CMakeLists.txt"
    with open(cmake_file, 'w') as f:
        f.write(content)
    
    print(f"Generated: {cmake_file}")

def generate_module_json(module_name: str, module_class_name: str, version: str,
                        api_version: int, dependencies: List[str], description: str,
                        tags: List[str], output_path: Path):
    """Generate the module.json metadata file."""
    deps_json = json.dumps(dependencies) if dependencies else "[]"
    tags_json = json.dumps(tags) if tags else '["module"]'
    
    content = MODULE_JSON_TEMPLATE.format(
        module_name=module_name,
        version=version,
        api_version=api_version,
        dependencies_list=deps_json,
        ModuleClassName=module_class_name,
        description=description,
        tags_list=tags_json
    )
    
    json_file = output_path / "module.json"
    with open(json_file, 'w') as f:
        f.write(content)
    
    print(f"Generated: {json_file}")

def parse_version(version_str: str) -> int:
    """Convert version string (e.g., "1.0.0") to hex (0x01000000)."""
    parts = version_str.split('.')
    if len(parts) != 3:
        raise ValueError("Version must be in format X.Y.Z")
    
    major = int(parts[0])
    minor = int(parts[1])
    patch = int(parts[2])
    
    return (major << 16) | (minor << 8) | patch

def to_class_name(module_name: str) -> str:
    """Convert module_name to ModuleClassName."""
    parts = module_name.split('_')
    return ''.join(part.capitalize() for part in parts)

def main():
    parser = argparse.ArgumentParser(
        description="Generate a new game engine module",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --name "my_renderer" --api_version 1
  %(prog)s --name "audio_system" --version "1.0.0" --description "Audio processing system"
  %(prog)s --name "physics_module" --dependencies "renderer_interface" --tags "physics","simulation"
        """
    )
    
    parser.add_argument("--name", required=True, help="Module name (e.g., my_renderer)")
    parser.add_argument("--version", default="1.0.0", help="Module version (default: 1.0.0)")
    parser.add_argument("--api_version", type=int, default=1, help="API version (default: 1)")
    parser.add_argument("--description", default="New game engine module", help="Module description")
    parser.add_argument("--dependencies", nargs="*", default=[], help="Module dependencies")
    parser.add_argument("--tags", nargs="*", default=["module"], help="Module tags")
    parser.add_argument("--output", default="modules", help="Output directory (default: modules)")
    parser.add_argument("--force", action="store_true", help="Overwrite existing files")
    
    args = parser.parse_args()
    
    # Validate inputs
    if not args.name.replace('_', '').isalnum():
        print("Error: Module name must contain only alphanumeric characters and underscores")
        sys.exit(1)
    
    try:
        version_hex = parse_version(args.version)
    except ValueError as e:
        print(f"Error: {e}")
        sys.exit(1)
    
    # Convert inputs
    module_class_name = to_class_name(args.name)
    base_path = Path(args.output)
    
    # Check if module already exists
    module_path = base_path / args.name
    if module_path.exists() and not args.force:
        print(f"Error: Module '{args.name}' already exists at {module_path}")
        print("Use --force to overwrite existing files")
        sys.exit(1)
    
    # Create module directory
    print(f"Creating module: {args.name}")
    module_path = create_module_directory(args.name, base_path)
    
    # Generate files
    generate_cpp_file(args.name, module_class_name, module_path / "src")
    generate_hpp_file(args.name, module_class_name, version_hex, args.description, module_path / "include")
    generate_cmake_file(args.name, args.dependencies, module_path)
    generate_module_json(args.name, module_class_name, args.version, args.api_version,
                         args.dependencies, args.description, args.tags, module_path)
    
    print(f"\nModule '{args.name}' generated successfully!")
    print(f"Location: {module_path}")
    print(f"\nNext steps:")
    print(f"1. Edit {module_path}/src/{args.name}.cpp to implement your module")
    print(f"2. Edit {module_path}/include/{args.name}.hpp to define module interface")
    print(f"3. Build the module: cmake --build . --target {args.name}")

if __name__ == "__main__":
    main()