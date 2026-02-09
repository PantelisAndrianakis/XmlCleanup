#include "XmlIndenter.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Function to find all XML and XSD files in a directory and its subdirectories.
std::vector<std::filesystem::path> findXmlAndXsdFiles(const std::filesystem::path& directoryPath)
{
	std::vector<std::filesystem::path> xmlFiles;

	try
	{
		// Check if the path exists and is a directory.
		if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath))
		{
			std::cerr << "Error: " << directoryPath << " does not exist or is not a directory" << std::endl;
			return xmlFiles;
		}

		// Recursively iterate through the directory and subdirectories.
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			if (entry.is_regular_file())
			{
				// Check if the file has .xml or .xsd extension.
				std::string extension = entry.path().extension().string();
				if (extension == ".xml" || extension == ".xsd")
				{
					xmlFiles.push_back(entry.path());
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error while searching for XML files: " << e.what() << std::endl;
	}

	return xmlFiles;
}

void printUsage()
{
	std::cout << "XmlCleanup - A tool for indenting XML files\n";
	std::cout << "Usage: XmlCleanup [options] <input-file> [output-file]\n";
	std::cout << "Options:\n";
	std::cout << "  -h, --help           Show this help message\n";
	std::cout << "  -t, --tabs           Use tabs for indentation (default)\n";
	std::cout << "  -s N, --spaces N     Use N spaces for indentation\n";
	std::cout << "  -i, --indent-only    Only adjust indentation, preserve linebreaks (default)\n";
	std::cout << "  -f, --full-format    Full formatting (adds linebreaks)\n";
	std::cout << "  -a, --auto-close     Auto-close empty elements (default)\n";
	std::cout << "  -n, --no-auto-close  Don't auto-close empty elements\n";
	std::cout << "\n";
	std::cout << "If no arguments are given, all XML and XSD files in the current folder and subfolders will be indented\n";
	std::cout << "using tabs for indentation and indent-only mode.\n";
	std::cout << "\n";
	std::cout << "If output-file is not specified, output is written to stdout\n";
}

std::string readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Error: Cannot open input file: " << filename << std::endl;
		exit(1);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void writeFile(const std::string& filename, const std::string& content)
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Error: Cannot open output file: " << filename << std::endl;
		exit(1);
	}

	file << content;
}

// Process a single XML file with the given formatting settings.
bool processXmlFile(const std::filesystem::path& inputPath, const std::string& indentStr, const std::string& eolStr, bool indentOnly, bool autoCloseEmptyElements)
{
	try
	{
		// Read input file.
		std::string xmlContent = readFile(inputPath.string());

		// Create XML indenter.
		XmlIndenter indenter(xmlContent, indentStr, eolStr, indentOnly, autoCloseEmptyElements);

		// Indent XML.
		std::string formattedXml = indenter.indentXML();

		// Write back to the same file.
		writeFile(inputPath.string(), formattedXml);
		std::cout << "Formatted: " << inputPath.string() << std::endl;

		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error processing " << inputPath.string() << ": " << e.what() << std::endl;
		return false;
	}
}

int main(int argc, char* argv[])
{
	// Default settings.
	std::string indentStr = "\t";
	std::string eolStr = "\n";
	bool indentOnly = true;
	bool autoCloseEmptyElements = true;
	std::string inputFile;
	std::string outputFile;

	// Check if no arguments were provided.
	if (argc == 1)
	{
		std::cout << "No arguments provided. Processing all XML and XSD files in current directory and subdirectories...\n";

		// Find all XML and XSD files in current directory and subdirectories.
		std::vector<std::filesystem::path> xmlFiles = findXmlAndXsdFiles(".");

		if (xmlFiles.empty())
		{
			std::cout << "No XML or XSD files found.\n";
			return 0;
		}

		std::cout << "Found " << xmlFiles.size() << " XML/XSD files to process.\n";

		// Process each file with default settings.
		int successCount = 0;
		for (const auto& file : xmlFiles)
		{
			if (processXmlFile(file, indentStr, eolStr, indentOnly, autoCloseEmptyElements))
			{
				successCount++;
			}
		}

		std::cout << "Successfully processed " << successCount << " out of " << xmlFiles.size() << " files.\n";

		return 0;
	}

	// Parse command-line arguments.
	std::vector<std::string> args(argv + 1, argv + argc);

	for (size_t i = 0; i < args.size(); ++i)
	{
		if (args[i] == "-h" || args[i] == "--help")
		{
			printUsage();
			return 0;
		}
		else if (args[i] == "-t" || args[i] == "--tabs")
		{
			indentStr = "\t";
		}
		else if (args[i] == "-s" || args[i] == "--spaces")
		{
			if (i + 1 < args.size() && args[i + 1][0] != '-')
			{
				int spaces = std::stoi(args[i + 1]);
				indentStr = std::string(spaces, ' ');
				i++;
			}
			else
			{
				indentStr = "  "; // Default to 2 spaces.
			}
		}
		else if (args[i] == "-i" || args[i] == "--indent-only")
		{
			indentOnly = true;
		}
		else if (args[i] == "-f" || args[i] == "--full-format")
		{
			indentOnly = false;
		}
		else if (args[i] == "-a" || args[i] == "--auto-close")
		{
			autoCloseEmptyElements = true;
		}
		else if (args[i] == "-n" || args[i] == "--no-auto-close")
		{
			autoCloseEmptyElements = false;
		}
		else if (inputFile.empty() && args[i][0] != '-')
		{
			inputFile = args[i];
		}
		else if (!inputFile.empty() && outputFile.empty() && args[i][0] != '-')
		{
			outputFile = args[i];
		}
	}

	// Check if input file is provided (we only get here if arguments were passed).
	if (inputFile.empty())
	{
		std::cerr << "Error: No valid input file specified\n";
		printUsage();
		return 1;
	}

	try
	{
		// Read input file.
		std::string xmlContent = readFile(inputFile);

		// Create XML indenter.
		XmlIndenter indenter(xmlContent, indentStr, eolStr, indentOnly, autoCloseEmptyElements);

		// Indent XML.
		std::string formattedXml = indenter.indentXML();

		// Output formatted XML.
		if (!outputFile.empty())
		{
			writeFile(outputFile, formattedXml);
			std::cout << "Formatted XML written to " << outputFile << std::endl;
		}
		else
		{
			std::cout << formattedXml;
		}

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}
