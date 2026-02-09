# C++ Coding Style Guide

This document describes the coding conventions for C++ projects.

## Core Principles

- **Clarity over cleverness**: Code should be immediately understandable.
- **Consistent formatting**: Use the same patterns throughout the codebase.
- **Explicit over implicit**: Prefer explicit types, clear variable names, and explicit error handling.
- **Simplicity over forced symmetry**: Don't force vertical alignment when a simple line works fine.
- **Allman style**: Opening braces on new lines for visual symmetry and easy scanning.
- **Performance-conscious**: Write efficient C++ code but prioritize readability first.
- **RAII and modern C++**: Embrace modern C++ features (C++17/20) and resource management patterns.

---

## Formatting

### Indentation
- **Use tabs for indentation** (not spaces).
- Configure your editor to use tabs.
- Continuation lines should be indented with one additional tab.

### Braces and Brackets Placement
- **Place opening braces `{` on a new line** for:
  - Function definitions.
  - Class definitions.
  - Struct definitions.
  - Enum definitions.
  - Namespace blocks.
  - If/else blocks.
  - For/while/do-while loops.
  - Switch statements.
  - Try/catch blocks.
  - Multi-line lambda expressions.

**Examples:**

```cpp
// Function definition.
void processFile(const std::filesystem::path& sourcePath, const std::filesystem::path& targetPath, const ProcessingOptions& options)
{
	// Function body.
}

// Class definition.
class FileProcessor
{
private:
	std::string filePath;
	size_t fileSize;

public:
	FileProcessor(const std::string& path);
	~FileProcessor();
	
	void process();
	size_t getSize() const;
};

// Struct definition.
struct ProcessingResult
{
	size_t originalSize;
	size_t newSize;
	double reductionPercent;
};

// Enum definition.
enum class ProcessingMode
{
	Fast,
	Balanced,
	Quality
};

// Namespace.
namespace QuickXml
{
	class XmlFormatter
	{
		// Class implementation.
	};
}

// If/else blocks.
if (args.force && args.skip)
{
	std::cerr << "Cannot use --force and --skip together" << std::endl;
	return 1;
}
else
{
	// Alternative path.
}

// For loops.
for (const auto& file : files)
{
	std::cout << "Processing: " << file.string() << std::endl;
}

// Switch statements.
switch (priority)
{
	case 1:
	case 2:
	case 3:
		std::cout << "High priority" << std::endl;
		break;
	
	case 4:
	case 5:
		std::cout << "Medium priority" << std::endl;
		break;
	
	default:
		std::cout << "Low priority" << std::endl;
		break;
}
```

**Exception - Single-line lambdas:**
Single-line lambdas can have braces on the same line:

```cpp
std::sort(files.begin(), files.end(), [](const auto& a, const auto& b) { return a.size() < b.size(); });

auto isValid = [](int x) { return x >= 0 && x <= 100; };
```

**Multi-line lambdas:**
Multi-line lambdas should have opening brace on new line:

```cpp
std::transform(files.begin(), files.end(), std::back_inserter(results), [&options](const auto& file)
{
	auto result = processFile(file, options);
	return result.success;
});

auto validator = [&validExtensions](const std::filesystem::path& path)
{
	auto ext = path.extension().string();
	return std::find(validExtensions.begin(), validExtensions.end(), ext) != validExtensions.end();
};
```

### Function Signatures
- **Write all parameters on a single line whenever possible**.
- Only break to multiple lines if the signature exceeds ~100-120 characters.
- When breaking, put each parameter on its own line with proper indentation.

**Good:**
```cpp
void processData(const std::string& source, const std::string& target, bool validate, int quality, ProcessingMode mode)
{
	// Implementation.
}
```

**When wrapping is necessary:**
```cpp
void processComplexOperation(
	const std::filesystem::path& sourcePath,
	const std::filesystem::path& targetPath,
	const ProcessingOptions& options,
	std::function<void(const std::string&)> callback
)
{
	// Implementation.
}
```

### Line Length
- Aim for **120 characters maximum** per line.
- Break longer lines at logical boundaries (after commas, operators, etc.).
- Use continuation indentation (one tab) for wrapped lines.

### When to Break Lines
**DO break lines when:**
- Line genuinely exceeds 120 characters.
- Complex nested expressions become hard to parse.
- Breaking at logical boundaries significantly improves readability.

**DON'T break lines when:**
- A line fits comfortably within 120 characters.
- Forcing vertical alignment makes code harder to scan.
- Breaking adds no clarity.
- Conditions or expressions are straightforward and readable on one line.

**Examples:**

```cpp
// Good - simple and clear, stays on one line.
if (cursor[1] == '!' && cursor[2] == '[' && cursor[3] == 'C' && cursor[4] == 'D')
{
	handleCDATA();
}

// Good - fits on one line.
auto result = calculateValue(x, y, z);

// Bad - forced vertical for no benefit.
auto result = calculateValue(
	x,
	y,
	z
);

// Bad - breaking short condition unnecessarily.
if (isValid &&
	isReady)
{
	process();
}

// Good - long line broken for readability.
auto r = quantizeChannel(pixel[0], factor);
auto g = quantizeChannel(pixel[1], factor);
auto b = quantizeChannel(pixel[2], factor);
resultBuffer.setPixel(x, y, Color{r, g, b, pixel[3]});

// Bad - unreadable single line (140+ chars).
resultBuffer.setPixel(x, y, Color{quantizeChannel(pixel[0], factor), quantizeChannel(pixel[1], factor), quantizeChannel(pixel[2], factor), pixel[3]});
```

**Principle:** Simplicity trumps symmetry. Keep code on single lines when readable. Break lines only when it genuinely helps readability or exceeds the 120-character limit.

---

## Comments

### General Comment Style
- **Begin all comment sentences with a capital letter**.
- **End all comment sentences with a period**.
- Use complete sentences for clarity.
- Use `//` for single-line comments.
- Use `/* */` for multi-line comments or block documentation.

**Examples:**
```cpp
// Calculate the size reduction percentage.
double reductionPct = (1.0 - (static_cast<double>(newSize) / originalSize)) * 100.0;

// Safety check to prevent infinite loops.
if (length > 10'000'000)
{
	break;
}

/*
 * This is a multi-line comment explaining
 * a complex algorithm or process flow.
 */
```

### Documentation Comments
Use Javadoc-style `/** */` for documenting functions, classes, and public interfaces:

```cpp
/**
 * Processes a file using a combination of techniques.
 * 
 * If validation is enabled, performs additional checks before processing.
 * Otherwise, applies standard processing with the specified quality level.
 * 
 * @param sourcePath Path to the source file.
 * @param targetPath Path where the processed file will be saved.
 * @param validate Whether to perform validation checks.
 * @return A ProcessingResult containing statistics about the operation.
 */
ProcessingResult processFile(
	const std::filesystem::path& sourcePath,
	const std::filesystem::path& targetPath,
	bool validate
)
{
	// Implementation.
}
```

### Inline Comments
- Use `//` followed by a space for inline comments.
- Place inline comments on the same line only when they're brief and clarify specific values.
- When explaining code, place comments on their own line above the code.

**Good:**
```cpp
Color newPixel
{
	quantizeChannel(oldPixel.r, factor),
	quantizeChannel(oldPixel.g, factor),
	quantizeChannel(oldPixel.b, factor),
	std::clamp(oldPixel.a, 0, 255) // Keep alpha unchanged.
};

// Distribute error to neighboring pixels (Floyd-Steinberg pattern).
if (x + 1 < width)
{
	for (int c = 0; c < 3; ++c)
	{
		workingBuffer[y][x + 1][c] += (error[c] * 7) / 16;
	}
}
```

**Avoid:**
```cpp
int x = 5; // Set x to 5.
```

---

## Headers and Includes

### Header Guards
Use `#pragma once` for header guards (modern and simpler):

```cpp
#pragma once

#include <string>
#include <vector>

class MyClass
{
	// Class definition.
};
```

**Alternative - Traditional header guards:**
If `#pragma once` is not available, use traditional guards with descriptive names:

```cpp
#ifndef PROJECT_NAME_FILE_NAME_H
#define PROJECT_NAME_FILE_NAME_H

// Header content.

#endif // PROJECT_NAME_FILE_NAME_H
```

### Include Ordering
Organize includes in the following order, with blank lines between groups:

1. Corresponding header (for `.cpp` files)
2. C++ standard library headers (alphabetically)
3. Third-party library headers (alphabetically)
4. Project headers (alphabetically)

**Example (.cpp file):**
```cpp
#include "XmlIndenter.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <rapidxml/rapidxml.hpp>

#include "XmlFormatter.h"
#include "XmlParser.h"
```

**Example (.h file):**
```cpp
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Types.h"
```

### Include Style
- Use angle brackets `< >` for standard library and third-party headers.
- Use quotes `" "` for project headers.

```cpp
#include <iostream>      // Standard library.
#include <boost/asio.hpp> // Third-party library.
#include "MyClass.h"     // Project header.
```

---

## Naming Conventions

### Variables and Functions
Use **camelCase** for variables, function parameters, and member variables:

```cpp
std::string sourcePath = "./file.txt";
size_t totalSize = 0;
bool isValid = false;

void calculateStatistics(const std::vector<uint8_t>& data, size_t width, size_t height)
{
	// Implementation.
}
```

### Classes, Structs, and Enums
Use **PascalCase**:

```cpp
class FileProcessor
{
	// Class implementation.
};

struct ProcessingResult
{
	size_t originalSize;
	size_t newSize;
};

enum class ProcessingMode
{
	Fast,
	Balanced,
	Quality
};
```

### Enum Values
Use **PascalCase** for enum class values:

```cpp
enum class Color
{
	Red,
	Green,
	Blue
};

// Usage.
auto myColor = Color::Red;
```

### Constants and Macros
Use **SCREAMING_SNAKE_CASE**:

```cpp
const char* APP_NAME = "Application";
const char* APP_VERSION = "1.0.0";
constexpr size_t MAX_BUFFER_SIZE = 10'000'000;

#define XPATH_MODE_BASIC            (1 << 0)
#define XPATH_MODE_WITHNAMESPACE    (1 << 1)
```

### Namespaces
Use **PascalCase** or **lowercase**:

```cpp
namespace QuickXml
{
	class XmlFormatter
	{
		// Implementation.
	};
}

namespace utils
{
	void helperFunction();
}
```

### Member Variables
Use **camelCase** without prefix:

```cpp
class MyClass
{
private:
	std::string fileName;     // Use camelCase.
	size_t fileSize;          // No m_ prefix.
	bool isInitialized;

public:
	// Public interface.
};
```

**Note:** Some codebases use `m_` prefix for members, but this style guide prefers clean camelCase.

---

## Type Declarations and Usage

### Type Inference with `auto`
Use `auto` when the type is obvious from context or when it improves readability:

**Good uses of auto:**
```cpp
auto result = calculateValue(x, y);  // Return type clear from function name.
auto it = myMap.find(key);           // Iterator types are verbose.
auto lambda = [](int x) { return x * 2; };

for (const auto& item : collection)  // Avoids repeating long type names.
{
	// Process item.
}
```

**Avoid auto when type is not obvious:**
```cpp
auto x = processData(); // What type is x? Not clear.

// Better - be explicit.
ProcessingResult x = processData();
```

### Const Correctness
- Use `const` for function parameters that won't be modified.
- Use `const` member functions for methods that don't modify object state.
- Use `const` references to avoid unnecessary copies.

```cpp
class FileInfo
{
private:
	std::string path;
	size_t size;

public:
	// Const reference parameter.
	FileInfo(const std::string& filePath, size_t fileSize)
		: path(filePath), size(fileSize)
	{
	}
	
	// Const member function.
	std::string getPath() const
	{
		return path;
	}
	
	size_t getSize() const
	{
		return size;
	}
};

// Const reference in function parameter.
void processFile(const FileInfo& info)
{
	std::cout << info.getPath() << std::endl;
}
```

### References and Pointers
- Prefer references over pointers when possible (no null values).
- Use pointers when null is a valid state or for polymorphism.
- Place `*` and `&` next to the type, not the variable name.

```cpp
// Good - reference next to type.
void process(const std::string& text);
int* createBuffer(size_t size);

// Avoid - reference next to variable.
void process(const std::string &text);
int *createBuffer(size_t size);

// Multiple declarations - each variable needs its own *.
int* ptr1;
int* ptr2;  // Not: int* ptr1, ptr2; (ptr2 would be int, not int*).
```

---

## Classes and Object-Oriented Design

### Class Structure
Organize class members in the following order:

1. Private members (data and methods)
2. Protected members (if applicable)
3. Public members (constructors, destructors, interface methods)

```cpp
class XmlIndenter
{
private:
	// Private member variables.
	std::string xmlContent;
	std::string indentStr;
	std::string eolStr;
	bool indentOnly;
	
	// Private helper methods.
	void preprocessContent();
	std::string normalizeLineEndings(const std::string& content);

protected:
	// Protected members (if needed).

public:
	// Constructors.
	XmlIndenter(const std::string& xmlContent);
	XmlIndenter(const std::string& xmlContent, const std::string& indentStr, const std::string& eolStr, bool indentOnly);
	
	// Destructor.
	~XmlIndenter();
	
	// Public interface.
	std::string indentXML();
	
	// Setters.
	void setIndentString(const std::string& str);
	void setEOLString(const std::string& str);
	
	// Getters (const).
	std::string getIndentString() const;
	std::string getEOLString() const;
	
	// Static methods.
	static std::string indentXMLString(const std::string& xml);
};
```

### Constructor Initialization Lists
Always use initialization lists for constructors.

**Keep the initialization list on the same line as the constructor signature whenever possible:**

```cpp
// Good - single-line initialization list.
FileProcessor::FileProcessor(const std::string& path, size_t size) : filePath(path), fileSize(size), isInitialized(false)
{
	// Constructor body for additional logic if needed.
}

// Good - even if long, keep on one line.
XmlIndenter::XmlIndenter(const std::string& xmlContent) : xmlContent(xmlContent), indentStr("\t"), eolStr("\n"), indentOnly(true), autoCloseEmptyElements(true)
{
}

// Only break to multiple lines if truly exceeding ~120 characters.
VeryLongClassName::VeryLongClassName(const std::string& veryLongParameterName, int anotherParameter, bool yetAnotherOne)
	: memberVariableWithLongName(veryLongParameterName),
	  anotherMemberVariable(anotherParameter),
	  booleanMember(yetAnotherOne)
{
}

// Avoid - assignment in constructor body.
FileProcessor::FileProcessor(const std::string& path, size_t size)
{
	filePath = path;      // Less efficient.
	fileSize = size;
	isInitialized = false;
}
```

**Principle:** Keep initialization lists on a single line unless they genuinely exceed ~120 characters.

### Rule of Five / Rule of Zero
Follow the Rule of Five or Rule of Zero:

**Rule of Zero (preferred):**
Use smart pointers and STL containers; compiler-generated special members are sufficient:

```cpp
class DataProcessor
{
private:
	std::unique_ptr<Impl> impl;
	std::vector<std::string> data;
	
public:
	// No need to define destructor, copy/move constructors/operators.
	// Compiler-generated versions work correctly.
};
```

**Rule of Five (when manual resource management is needed):**
If you define one, define all five:

```cpp
class ResourceHolder
{
private:
	int* resource;
	
public:
	// Constructor.
	ResourceHolder(int value) : resource(new int(value)) {}
	
	// Destructor.
	~ResourceHolder()
	{
		delete resource;
	}
	
	// Copy constructor.
	ResourceHolder(const ResourceHolder& other)
		: resource(new int(*other.resource))
	{
	}
	
	// Copy assignment.
	ResourceHolder& operator=(const ResourceHolder& other)
	{
		if (this != &other)
		{
			delete resource;
			resource = new int(*other.resource);
		}
		return *this;
	}
	
	// Move constructor.
	ResourceHolder(ResourceHolder&& other) noexcept
		: resource(other.resource)
	{
		other.resource = nullptr;
	}
	
	// Move assignment.
	ResourceHolder& operator=(ResourceHolder&& other) noexcept
	{
		if (this != &other)
		{
			delete resource;
			resource = other.resource;
			other.resource = nullptr;
		}
		return *this;
	}
};
```

---

## Modern C++ Features

### Smart Pointers
Prefer smart pointers over raw pointers for ownership:

```cpp
// Use std::unique_ptr for exclusive ownership.
auto parser = std::make_unique<XmlParser>(data, length);

// Use std::shared_ptr for shared ownership.
auto formatter = std::make_shared<XmlFormatter>(data, length);

// Avoid raw pointers for ownership.
XmlParser* parser = new XmlParser(data, length);  // Don't do this.
// ... (easy to forget delete).
```

### Range-Based For Loops
Use range-based for loops when possible:

```cpp
// Good - range-based for.
std::vector<std::string> files = getFiles();
for (const auto& file : files)
{
	process(file);
}

// Modifying elements.
for (auto& file : files)
{
	file = normalize(file);
}

// When you need the index, use traditional loop or enumerate.
for (size_t i = 0; i < files.size(); ++i)
{
	std::cout << i << ": " << files[i] << std::endl;
}
```

### Lambdas
Use lambdas for short, localized functions:

```cpp
// Single-line lambda.
auto isPositive = [](int x) { return x > 0; };

// Multi-line lambda with captures.
auto processor = [&options, &results](const std::string& file)
{
	auto result = processFile(file, options);
	results.push_back(result);
	return result.success;
};

// Generic lambda (C++14).
auto printer = [](const auto& item)
{
	std::cout << item << std::endl;
};
```

### Structured Bindings (C++17)
Use structured bindings to unpack tuples and pairs:

```cpp
std::map<std::string, int> myMap = {{"a", 1}, {"b", 2}};

for (const auto& [key, value] : myMap)
{
	std::cout << key << ": " << value << std::endl;
}

auto [min, max] = std::minmax({1, 2, 3, 4, 5});
```

### `std::optional` (C++17)
Use `std::optional` for values that may not exist:

```cpp
std::optional<std::string> findFile(const std::string& name)
{
	// Search logic.
	if (found)
	{
		return filename;
	}
	return std::nullopt;
}

// Usage.
if (auto file = findFile("config.xml"))
{
	std::cout << "Found: " << *file << std::endl;
}
else
{
	std::cout << "Not found" << std::endl;
}
```

### `std::filesystem` (C++17)
Use `std::filesystem` for file operations:

```cpp
#include <filesystem>

namespace fs = std::filesystem;

// Check if file exists.
if (fs::exists(path))
{
	// Process file.
}

// Iterate directory.
for (const auto& entry : fs::directory_iterator(dirPath))
{
	if (entry.is_regular_file())
	{
		std::cout << entry.path() << std::endl;
	}
}

// Recursive directory iteration.
for (const auto& entry : fs::recursive_directory_iterator(dirPath))
{
	// Process entries.
}
```

---

## Error Handling

### Exceptions
Use exceptions for error handling in most cases:

```cpp
void readFile(const std::filesystem::path& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open file: " + path.string());
	}
	
	// Read file.
}

// Catching exceptions.
try
{
	readFile(inputPath);
	processData();
}
catch (const std::runtime_error& e)
{
	std::cerr << "Error: " << e.what() << std::endl;
	return 1;
}
catch (const std::exception& e)
{
	std::cerr << "Unexpected error: " << e.what() << std::endl;
	return 1;
}
```

### Error Codes (when exceptions are not suitable)
Use error codes or `std::optional` for performance-critical code:

```cpp
enum class ErrorCode
{
	Success,
	FileNotFound,
	InvalidFormat,
	OutOfMemory
};

struct Result
{
	ErrorCode code;
	std::string message;
	std::optional<Data> data;
};

Result processFile(const std::string& path)
{
	if (!fileExists(path))
	{
		return {ErrorCode::FileNotFound, "File not found", std::nullopt};
	}
	
	// Processing logic.
	return {ErrorCode::Success, "", processedData};
}
```

### Resource Management (RAII)
Always use RAII for resource management:

```cpp
// Good - RAII with smart pointers.
{
	auto file = std::make_unique<File>(path);
	file->process();
	// Automatically cleaned up when going out of scope.
}

// Good - RAII with standard containers.
{
	std::vector<int> data(1000);
	// Automatically cleaned up.
}

// Avoid - manual resource management.
{
	int* buffer = new int[1000];
	// ... easy to forget delete[] or miss it in exception paths.
	delete[] buffer;
}
```

---

## Control Flow

### If-Else Statements
Always use braces, even for single-line statements.

**Keep the condition on a single line whenever possible** (up to ~120 characters):

```cpp
// Good - single-line condition.
if (cursor[1] == '!' && cursor[2] == '[' && cursor[3] == 'C' && cursor[4] == 'D' && cursor[5] == 'A' && cursor[6] == 'T' && cursor[7] == 'A' && cursor[8] == '[')
{
	handleCDATA();
}

// Good - braces on all branches.
if (condition)
{
	doSomething();
}
else if (anotherCondition)
{
	doSomethingElse();
}
else
{
	doDefault();
}

// Only break to multiple lines if truly exceeding 120 characters.
if (veryLongVariableName > threshold && 
	anotherVeryLongCondition == true &&
	yetAnotherLongCondition != nullptr)
{
	// Complex condition that truly doesn't fit.
}

// Avoid - no braces (error-prone when adding more statements).
if (condition)
	doSomething();
else
	doSomethingElse();

// Avoid - breaking short conditions unnecessarily.
if (x > 5 &&
	y < 10)
{
	// This is unnecessary - fits easily on one line.
}
```

**Principle:** Keep conditions on one line unless they genuinely exceed ~120 characters. Don't break lines just for aesthetics.

### Switch Statements
Use explicit `break` statements and always include a `default` case.

**Important: Only use braces within case blocks when declaring variables.**

```cpp
switch (mode)
{
	case ProcessingMode::Fast:
		applyFastProcessing();
		break;
	
	case ProcessingMode::Balanced:
		// Multiple statements - no braces needed.
		initializeBalanced();
		applyBalancedProcessing();
		logResult();
		break;
	
	case ProcessingMode::Quality:
	{
		// Braces required - variable declaration needs scope.
		std::string config = loadQualityConfig();
		applyQualityProcessing(config);
		break;
	}
	
	default:
		std::cerr << "Unknown processing mode" << std::endl;
		break;
}
```

**Avoid unnecessary braces:**

```cpp
// Bad - unnecessary braces around every case.
switch (value)
{
	case 1:
	{
		doSomething();
		break;
	}
	case 2:
	{
		doSomethingElse();
		break;
	}
}

// Good - braces only when needed for variable scope.
switch (value)
{
	case 1:
		doSomething();
		break;
	
	case 2:
		doSomethingElse();
		break;
	
	case 3:
	{
		// Braces needed for variable declaration.
		int tempValue = calculateValue();
		processTempValue(tempValue);
		break;
	}
}
```

**Fall-through cases:**

When intentionally falling through, add a comment:

```cpp
switch (token.type)
{
	case XmlTokenType::LineBreak:
		// Fall through.
	case XmlTokenType::Whitespace:
		handleWhitespace(token);
		break;
	
	case XmlTokenType::Text:
		handleText(token);
		break;
}
```

### Early Returns
Prefer early returns to reduce nesting:

```cpp
// Good - early returns.
void processData(const Data& data)
{
	if (!data.isValid())
	{
		std::cerr << "Invalid data" << std::endl;
		return;
	}
	
	if (data.isEmpty())
	{
		return;
	}
	
	// Main processing logic at top level.
	transformData(data);
	saveResults();
}

// Avoid - deep nesting.
void processData(const Data& data)
{
	if (data.isValid())
	{
		if (!data.isEmpty())
		{
			// Main logic deeply nested.
			transformData(data);
			saveResults();
		}
	}
	else
	{
		std::cerr << "Invalid data" << std::endl;
	}
}
```

---

## STL Usage

### Containers
Choose appropriate containers:

- `std::vector` - Dynamic array, default choice.
- `std::array` - Fixed-size array (C++11).
- `std::map` - Sorted key-value pairs.
- `std::unordered_map` - Hash table, faster lookups.
- `std::set` - Sorted unique elements.
- `std::unordered_set` - Hash set.

```cpp
// Vector for dynamic collections.
std::vector<std::string> files;
files.push_back("file1.xml");

// Array for fixed-size collections.
std::array<int, 5> priorities = {1, 2, 3, 4, 5};

// Map for key-value associations.
std::map<std::string, int> settings;
settings["quality"] = 85;

// Set for unique elements.
std::unordered_set<std::string> processedFiles;
processedFiles.insert("file1.xml");
```

### Algorithms
Use STL algorithms instead of manual loops when appropriate:

```cpp
#include <algorithm>

std::vector<int> numbers = {5, 2, 8, 1, 9};

// Sort.
std::sort(numbers.begin(), numbers.end());

// Find.
auto it = std::find(numbers.begin(), numbers.end(), 8);
if (it != numbers.end())
{
	std::cout << "Found: " << *it << std::endl;
}

// Transform.
std::vector<int> doubled;
std::transform(numbers.begin(), numbers.end(), std::back_inserter(doubled), [](int x) { return x * 2; });

// Remove if.
numbers.erase(std::remove_if(numbers.begin(), numbers.end(), [](int x) { return x < 5; }), numbers.end());

// Count if.
auto count = std::count_if(numbers.begin(), numbers.end(), [](int x) { return x > 5; });
```

---

## String Handling

### String Operations
Use `std::string` for strings:

```cpp
std::string text = "Hello, World!";

// Concatenation.
std::string greeting = "Hello, " + name + "!";

// Substring.
std::string sub = text.substr(0, 5); // "Hello"

// Find.
size_t pos = text.find("World");
if (pos != std::string::npos)
{
	std::cout << "Found at position: " << pos << std::endl;
}

// Replace.
text.replace(pos, 5, "C++");
```

### String Streams
Use `std::stringstream` for string building and parsing:

```cpp
#include <sstream>

// Building strings.
std::stringstream ss;
ss << "Processed: " << filename << " (" << reductionPct << "% reduction)";
std::string message = ss.str();

// Parsing strings.
std::string data = "123 456 789";
std::stringstream parser(data);
int a, b, c;
parser >> a >> b >> c;
```

### C-String Compatibility
Use `.c_str()` when interfacing with C APIs:

```cpp
std::string filename = "output.xml";
FILE* file = fopen(filename.c_str(), "w");
```

---

## Numeric Formatting

### Digit Separators (C++14)
Use single quotes `'` as digit separators for large numbers:

```cpp
constexpr size_t MAX_SIZE = 10'000'000;
constexpr double PI = 3.141'592'653'589'793;

if (length > 1'024'000)
{
	// Handle large file.
}
```

### Floating Point
Specify precision when displaying floating-point numbers:

```cpp
#include <iomanip>

double value = 3.14159265;
std::cout << std::fixed << std::setprecision(2) << value << std::endl; // Output: 3.14
```

---

## Spacing and Blank Lines

### Between Functions
Use **one blank line** between function definitions:

```cpp
void functionOne()
{
	// Implementation.
}

void functionTwo()
{
	// Implementation.
}
```

### Within Functions
Use blank lines to separate logical sections:

```cpp
void processData()
{
	// Section 1: Read data.
	auto data = readFile(path);
	size_t size = data.size();
	
	// Section 2: Process data.
	auto processed = transform(data);
	auto optimized = optimize(processed);
	
	// Section 3: Write results.
	writeOutput(optimized);
}
```

**Separate independent control flow blocks with blank lines:**

Control structures (if/while/for) that are not directly related should be separated by blank lines for clarity.

```cpp
// Good - blank lines separate independent checks.
if (p != std::string::npos)
{
	key = attr.key.substr(p + 1);
}

if (this->params.dumpIdAttributesName)
{
	out_attr << key << "=" << attr.val;
}

// Bad - blocks stuck together, hard to scan.
if (p != std::string::npos)
{
	key = attr.key.substr(p + 1);
}
if (this->params.dumpIdAttributesName)
{
	out_attr << key << "=" << attr.val;
}

// Exception - related if/else blocks stay together (no blank lines).
if (condition1)
{
	doSomething();
}
else if (condition2)
{
	doSomethingElse();
}
else
{
	doDefault();
}

// Exception - tightly coupled logic can stay together.
int x = getValue();
if (x > 10)
{
	processX(x);
}
```

**When to add blank lines:**
- Between independent if statements
- Between independent loops
- Between different types of control structures
- Between control structures and regular statements (unless tightly coupled)

**When NOT to add blank lines:**
- Between if/else if/else chains
- Between a variable declaration and its immediate use in a control structure
- Within tightly related logic blocks

### Around Class Sections
Use blank lines to separate private/protected/public sections:

```cpp
class MyClass
{
private:
	int privateData;
	void privateMethod();

public:
	MyClass();
	void publicMethod();
};
```

---

## Anti-Patterns to Avoid

### Don't Use C-Style Casts
Use C++ casts (`static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast`):

**Avoid:**
```cpp
double x = (double)intValue; // C-style cast.
```

**Better:**
```cpp
double x = static_cast<double>(intValue); // C++ cast.
```

### Don't Use Naked `new`/`delete`
Use smart pointers or containers:

**Avoid:**
```cpp
int* buffer = new int[1000];
// ... (easy to leak).
delete[] buffer;
```

**Better:**
```cpp
std::vector<int> buffer(1000);
// Or
auto buffer = std::make_unique<int[]>(1000);
```

### Don't Ignore Return Values
Always check return values from functions that can fail:

**Avoid:**
```cpp
file.open(path); // Ignoring potential failure.
```

**Better:**
```cpp
file.open(path);
if (!file.is_open())
{
	std::cerr << "Failed to open file" << std::endl;
	return;
}
```

### Don't Use `using namespace std;`
Especially not in header files:

**Avoid:**
```cpp
using namespace std;
```

**Better:**
```cpp
using std::string;
using std::vector;

// Or just use std:: prefix.
std::string text;
std::vector<int> numbers;
```

### Don't Write Overly Nested Code
Extract complex logic into separate functions:

**Avoid:**
```cpp
if (condition1)
{
	if (condition2)
	{
		if (condition3)
		{
			// Deeply nested logic.
		}
	}
}
```

**Better:**
```cpp
if (!condition1)
{
	return;
}

if (!condition2)
{
	return;
}

if (!condition3)
{
	return;
}

// Main logic at top level.
```

---

## Performance Considerations

### Pass by Const Reference
For non-trivial types, pass by const reference:

```cpp
// Good - const reference for strings, vectors, etc.
void process(const std::string& text, const std::vector<int>& data)
{
	// Use text and data without copying.
}

// Avoid - pass by value (unnecessary copy).
void process(std::string text, std::vector<int> data)
{
	// Makes copies of text and data.
}

// Note: For small types (int, double, etc.), pass by value is fine.
void calculate(int x, double y)
{
	// Passing fundamental types by value is efficient.
}
```

### Move Semantics
Use move semantics to avoid unnecessary copies:

```cpp
std::vector<int> createLargeVector()
{
	std::vector<int> data(1'000'000);
	// Fill data.
	return data; // Move, not copy (RVO/NRVO).
}

// Accept by rvalue reference when you'll consume the object.
void consumeVector(std::vector<int>&& data)
{
	processedData = std::move(data);
}
```

### Reserve Capacity
Reserve capacity for containers when size is known:

```cpp
std::vector<std::string> files;
files.reserve(1000); // Avoid multiple reallocations.

for (int i = 0; i < 1000; ++i)
{
	files.push_back(generateFilename(i));
}
```

---

## Quick Reference Checklist

When writing or reviewing code, ensure:

- [ ] Using tabs for indentation.
- [ ] Opening braces on new lines for functions, classes, control structures.
- [ ] Keep conditions and statements on single lines when they fit (≤120 chars).
- [ ] Keep constructor initialization lists on single lines when they fit.
- [ ] Only break lines when genuinely needed for readability or length.
- [ ] Comments start with capital letter and end with period.
- [ ] Using camelCase for variables/functions/members.
- [ ] Using PascalCase for classes/structs/enums.
- [ ] Using SCREAMING_SNAKE_CASE for constants/macros.
- [ ] Using `#pragma once` for header guards.
- [ ] Include order: corresponding header, std, third-party, project.
- [ ] Using const correctness (const parameters, const member functions).
- [ ] Using smart pointers instead of raw pointers for ownership.
- [ ] Using RAII for resource management.
- [ ] Proper error handling with exceptions or error codes.
- [ ] One blank line between functions.
- [ ] Blank lines to separate logical sections.
- [ ] Blank lines between independent control flow blocks (if/while/for).
- [ ] Switch case braces only when declaring variables.
- [ ] Using modern C++ features (auto, range-for, lambdas, std::optional, std::filesystem).
- [ ] Avoiding C-style casts, naked new/delete, `using namespace std;`.

---

## Summary

The C++ coding style emphasizes:
1. **Readability** - Code should be self-documenting and easy to understand.
2. **Consistency** - Use the same patterns throughout the codebase.
3. **Modern C++** - Embrace C++17/20 features for safer, more expressive code.
4. **RAII** - Resource acquisition is initialization; let destructors clean up.
5. **Const Correctness** - Use const wherever possible to express intent.
6. **Smart Pointers** - Prefer smart pointers over raw pointers for ownership.
7. **Maintainability** - Write code that's easy to modify and debug.
