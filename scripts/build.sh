#!/bin/bash

# Game Engine Build Script

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
BUILD_TYPE="Release"
BUILD_DIR="build"
VCPKG_ROOT=""
VERBOSE=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --release)
            BUILD_TYPE="Release"
            shift
            ;;
        --vcpkg-root)
            VCPKG_ROOT="$2"
            shift 2
            ;;
        --verbose)
            VERBOSE=true
            shift
            ;;
        --help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  --debug         Build in Debug mode"
            echo "  --release       Build in Release mode (default)"
            echo "  --vcpkg-root    Path to vcpkg installation"
            echo "  --verbose       Enable verbose output"
            echo "  --help          Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Function to print colored output
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [[ ! -f "CMakeLists.txt" ]]; then
    print_error "CMakeLists.txt not found. Please run this script from the game-engine root directory."
    exit 1
fi

# Try to find vcpkg if not specified
if [[ -z "$VCPKG_ROOT" ]]; then
    # Common vcpkg locations
    possible_paths=(
        "$HOME/vcpkg"
        "/opt/vcpkg"
        "/usr/local/vcpkg"
        "$HOME/.vcpkg"
    )
    
    for path in "${possible_paths[@]}"; do
        if [[ -f "$path/scripts/buildsystems/vcpkg.cmake" ]]; then
            VCPKG_ROOT="$path"
            break
        fi
    done
    
    if [[ -z "$VCPKG_ROOT" ]]; then
        print_error "vcpkg not found. Please specify --vcpkg-root or install vcpkg."
        exit 1
    fi
fi

print_info "Using vcpkg at: $VCPKG_ROOT"

# Check if vcpkg toolchain file exists
VCPKG_TOOLCHAIN="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
if [[ ! -f "$VCPKG_TOOLCHAIN" ]]; then
    print_error "vcpkg toolchain file not found at: $VCPKG_TOOLCHAIN"
    exit 1
fi

# Create build directory
print_info "Creating build directory..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake
print_info "Configuring CMake..."
CMAKE_ARGS=(
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN"
    -DBUILD_EXAMPLES=ON
    -DBUILD_TESTS=ON
    -DBUILD_TOOLS=ON
    -DENABLE_AI=ON
)

if [[ "$VERBOSE" == true ]]; then
    CMAKE_ARGS+=(-DCMAKE_VERBOSE_MAKEFILE=ON)
fi

cmake "${CMAKE_ARGS[@]}" ..

# Build the project
print_info "Building project..."
CMAKE_BUILD_ARGS=()
if [[ "$VERBOSE" == true ]]; then
    CMAKE_BUILD_ARGS+=(--verbose)
fi

cmake --build . --config "$BUILD_TYPE" "${CMAKE_BUILD_ARGS[@]}"

# Run tests if built
if [[ -f "bin/tests" ]] || [[ -f "bin/Release/tests.exe" ]]; then
    print_info "Running tests..."
    ctest --output-on-failure -C "$BUILD_TYPE"
fi

print_success "Build completed successfully!"
print_info "Binaries are available in: $BUILD_DIR/bin/"

# Print example commands
print_info "To run examples:"
echo "  $BUILD_DIR/bin/examples/basic_2d"
echo "  $BUILD_DIR/bin/examples/basic_3d"