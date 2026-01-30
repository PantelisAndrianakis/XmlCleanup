# XML Cleanup
 
A standalone tool that replicates the "Pretty print - indent only" functionality from the Notepad++ XMLTools plugin.
This utility helps maintain XML file readability while preserving the original structure.
 
## Features
 
- Preserves all existing line breaks in XML files
- Adjusts indentation based on element nesting levels
- Supports processing multiple files or entire directories
- Configurable indentation (tabs or spaces)
- Proper formatting of self-closing XML elements
- Handles XML attributes with consistent spacing
- Normalizes line endings (Windows, Unix, Mac)
- Optional automatic closing of empty elements

## Usage

```
XmlCleanup.exe [file.xml|directory] [-r] [-t|-s<num>] [-o<path>]
```

Options:
- `file.xml`: Process a single XML file
- `directory`: Process all XML files in a directory
- `-r`: Process directories recursively
- `-t`: Use tabs for indentation (default)
- `-s<num>`: Use spaces for indentation (e.g., -s2 for 2 spaces)
- `-o<path>`: Output directory (default: overwrite original files)

## Building

This project can be built using Visual Studio with the C++ compiler. Open the `XmlCleanup.sln` solution file in Visual Studio and build the solution.
