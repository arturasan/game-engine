#!/usr/bin/env python3

"""
Asset Cooker Tool for Game Engine

This script processes raw assets and converts them to engine-ready formats.
"""

import os
import sys
import argparse
import json
import shutil
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from PIL import Image
import numpy as np

class AssetProcessor:
    """Base class for asset processors."""
    
    def __init__(self):
        self.supported_extensions = []
    
    def can_process(self, file_path: Path) -> bool:
        """Check if this processor can handle the given file."""
        return file_path.suffix.lower() in self.supported_extensions
    
    def process(self, input_path: Path, output_path: Path) -> bool:
        """Process the asset file."""
        raise NotImplementedError("Subclasses must implement process()")

class TextureProcessor(AssetProcessor):
    """Processor for texture assets."""
    
    def __init__(self):
        super().__init__()
        self.supported_extensions = ['.png', '.jpg', '.jpeg', '.bmp', '.tga']
    
    def process(self, input_path: Path, output_path: Path) -> bool:
        """Process texture files."""
        try:
            # Load image
            image = Image.open(input_path)
            
            # Convert to RGBA if needed
            if image.mode != 'RGBA':
                image = image.convert('RGBA')
            
            # Create output directory
            output_path.parent.mkdir(parents=True, exist_ok=True)
            
            # Save as PNG (lossless)
            image.save(output_path, 'PNG')
            
            # Generate metadata
            metadata = {
                'type': 'texture',
                'format': 'rgba8',
                'width': image.width,
                'height': image.height,
                'channels': 4,
                'mipmaps': True,
                'source': str(input_path)
            }
            
            # Save metadata
            metadata_path = output_path.with_suffix('.json')
            with open(metadata_path, 'w') as f:
                json.dump(metadata, f, indent=2)
            
            print(f"Processed texture: {input_path} -> {output_path}")
            return True
            
        except Exception as e:
            print(f"Error processing texture {input_path}: {e}")
            return False

class ModelProcessor(AssetProcessor):
    """Processor for 3D model assets."""
    
    def __init__(self):
        super().__init__()
        self.supported_extensions = ['.obj', '.gltf', '.glb', '.fbx']
    
    def process(self, input_path: Path, output_path: Path) -> bool:
        """Process model files."""
        try:
            # For now, just copy the file and generate metadata
            # In a real implementation, you'd use a library like Assimp
            
            # Create output directory
            output_path.parent.mkdir(parents=True, exist_ok=True)
            
            # Copy file
            shutil.copy2(input_path, output_path)
            
            # Generate metadata
            metadata = {
                'type': 'model',
                'format': input_path.suffix.lower()[1:],  # Remove the dot
                'source': str(input_path),
                'animations': [],
                'materials': [],
                'meshes': []
            }
            
            # Save metadata
            metadata_path = output_path.with_suffix('.json')
            with open(metadata_path, 'w') as f:
                json.dump(metadata, f, indent=2)
            
            print(f"Processed model: {input_path} -> {output_path}")
            return True
            
        except Exception as e:
            print(f"Error processing model {input_path}: {e}")
            return False

class AudioProcessor(AssetProcessor):
    """Processor for audio assets."""
    
    def __init__(self):
        super().__init__()
        self.supported_extensions = ['.wav', '.mp3', '.ogg', '.flac']
    
    def process(self, input_path: Path, output_path: Path) -> bool:
        """Process audio files."""
        try:
            # For now, just copy the file and generate metadata
            # In a real implementation, you'd use a library like libsndfile
            
            # Create output directory
            output_path.parent.mkdir(parents=True, exist_ok=True)
            
            # Copy file
            shutil.copy2(input_path, output_path)
            
            # Generate metadata
            metadata = {
                'type': 'audio',
                'format': input_path.suffix.lower()[1:],  # Remove the dot
                'source': str(input_path),
                'duration': 0.0,  # Would need audio library to calculate
                'channels': 0,
                'sample_rate': 0,
                'bit_depth': 0
            }
            
            # Save metadata
            metadata_path = output_path.with_suffix('.json')
            with open(metadata_path, 'w') as f:
                json.dump(metadata, f, indent=2)
            
            print(f"Processed audio: {input_path} -> {output_path}")
            return True
            
        except Exception as e:
            print(f"Error processing audio {input_path}: {e}")
            return False

class ShaderProcessor(AssetProcessor):
    """Processor for shader assets."""
    
    def __init__(self):
        super().__init__()
        self.supported_extensions = ['.vert', '.frag', '.geom', '.tesc', '.tese', '.comp']
    
    def process(self, input_path: Path, output_path: Path) -> bool:
        """Process shader files."""
        try:
            # Read shader source
            with open(input_path, 'r') as f:
                source = f.read()
            
            # Create output directory
            output_path.parent.mkdir(parents=True, exist_ok=True)
            
            # Copy shader source
            with open(output_path, 'w') as f:
                f.write(source)
            
            # Generate metadata
            shader_type = self._get_shader_type(input_path.suffix)
            metadata = {
                'type': 'shader',
                'shader_type': shader_type,
                'source': str(input_path),
                'defines': [],
                'includes': []
            }
            
            # Save metadata
            metadata_path = output_path.with_suffix('.json')
            with open(metadata_path, 'w') as f:
                json.dump(metadata, f, indent=2)
            
            print(f"Processed shader: {input_path} -> {output_path}")
            return True
            
        except Exception as e:
            print(f"Error processing shader {input_path}: {e}")
            return False
    
    def _get_shader_type(self, extension: str) -> str:
        """Get shader type from file extension."""
        type_map = {
            '.vert': 'vertex',
            '.frag': 'fragment',
            '.geom': 'geometry',
            '.tesc': 'tessellation_control',
            '.tese': 'tessellation_evaluation',
            '.comp': 'compute'
        }
        return type_map.get(extension.lower(), 'unknown')

class AssetCooker:
    """Main asset cooker class."""
    
    def __init__(self):
        self.processors = [
            TextureProcessor(),
            ModelProcessor(),
            AudioProcessor(),
            ShaderProcessor()
        ]
    
    def get_processor(self, file_path: Path) -> Optional[AssetProcessor]:
        """Get the appropriate processor for a file."""
        for processor in self.processors:
            if processor.can_process(file_path):
                return processor
        return None
    
    def cook_asset(self, input_path: Path, output_path: Path) -> bool:
        """Cook a single asset file."""
        processor = self.get_processor(input_path)
        if not processor:
            print(f"No processor found for: {input_path}")
            return False
        
        return processor.process(input_path, output_path)
    
    def cook_directory(self, input_dir: Path, output_dir: Path, recursive: bool = True) -> Tuple[int, int]:
        """Cook all assets in a directory."""
        if not input_dir.exists():
            print(f"Input directory does not exist: {input_dir}")
            return 0, 0
        
        processed_count = 0
        failed_count = 0
        
        # Get all files to process
        if recursive:
            files = [f for f in input_dir.rglob('*') if f.is_file()]
        else:
            files = [f for f in input_dir.iterdir() if f.is_file()]
        
        for file_path in files:
            # Calculate relative path and output path
            rel_path = file_path.relative_to(input_dir)
            output_path = output_dir / rel_path
            
            # Skip metadata files
            if file_path.suffix == '.json':
                continue
            
            print(f"Processing: {file_path}")
            if self.cook_asset(file_path, output_path):
                processed_count += 1
            else:
                failed_count += 1
        
        return processed_count, failed_count
    
    def generate_asset_manifest(self, output_dir: Path) -> bool:
        """Generate an asset manifest file."""
        try:
            assets = []
            
            # Find all asset metadata files
            for metadata_file in output_dir.rglob('*.json'):
                # Skip if it's not an asset metadata file
                if metadata_file.name == 'manifest.json':
                    continue
                
                try:
                    with open(metadata_file, 'r') as f:
                        metadata = json.load(f)
                    
                    # Add file path to metadata
                    asset_file = metadata_file.with_suffix('')
                    metadata['path'] = str(asset_file.relative_to(output_dir))
                    assets.append(metadata)
                    
                except Exception as e:
                    print(f"Error reading metadata {metadata_file}: {e}")
            
            # Create manifest
            manifest = {
                'version': '1.0',
                'asset_count': len(assets),
                'assets': assets
            }
            
            # Save manifest
            manifest_path = output_dir / 'manifest.json'
            with open(manifest_path, 'w') as f:
                json.dump(manifest, f, indent=2)
            
            print(f"Generated asset manifest: {manifest_path}")
            return True
            
        except Exception as e:
            print(f"Error generating asset manifest: {e}")
            return False

def main():
    parser = argparse.ArgumentParser(
        description="Cook assets for the game engine",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s assets/ cooked_assets/
  %(prog)s assets/textures/ cooked_assets/ --no-recursive
  %(prog)s assets/ cooked_assets/ --generate-manifest
        """
    )
    
    parser.add_argument("input", help="Input directory or file")
    parser.add_argument("output", help="Output directory or file")
    parser.add_argument("--recursive", action="store_true", default=True,
                       help="Process directories recursively (default: True)")
    parser.add_argument("--no-recursive", action="store_false", dest="recursive",
                       help="Do not process directories recursively")
    parser.add_argument("--generate-manifest", action="store_true",
                       help="Generate asset manifest file")
    parser.add_argument("--force", action="store_true",
                       help="Overwrite existing files")
    parser.add_argument("--verbose", action="store_true",
                       help="Enable verbose output")
    
    args = parser.parse_args()
    
    input_path = Path(args.input)
    output_path = Path(args.output)
    
    # Create asset cooker
    cooker = AssetCooker()
    
    # Process single file
    if input_path.is_file():
        if args.verbose:
            print(f"Processing single file: {input_path}")
        
        success = cooker.cook_asset(input_path, output_path)
        if success:
            print("Asset cooked successfully!")
        else:
            print("Failed to cook asset!")
            sys.exit(1)
    
    # Process directory
    elif input_path.is_dir():
        if args.verbose:
            print(f"Processing directory: {input_path}")
        
        processed, failed = cooker.cook_directory(input_path, output_path, args.recursive)
        
        print(f"\nProcessing complete!")
        print(f"Successfully processed: {processed} assets")
        print(f"Failed to process: {failed} assets")
        
        if args.generate_manifest:
            cooker.generate_asset_manifest(output_path)
        
        if failed > 0:
            print(f"Warning: {failed} assets failed to process")
            sys.exit(1)
    
    else:
        print(f"Error: Input path does not exist: {input_path}")
        sys.exit(1)

if __name__ == "__main__":
    main()