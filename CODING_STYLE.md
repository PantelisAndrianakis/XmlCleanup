# Coding Style Guide

This document describes the C++-specific coding conventions for this project.

---

## Core Principles

These are the core principles that define how we write code.

### 1. TYPE INFERENCE - CONTROLLED USE OF `auto`

Type inference with `auto` is **tightly controlled** but not forbidden.

**Core Principle:** Code must be understandable without IDE assistance. The reader is more important than the writer.

**Allowed - Type is obvious from right-hand side:**
```cpp
// ALLOWED - Constructor visible.
auto players = std::vector<Player>();
auto stream = std::make_unique<FileStream>(path);
auto buffer = std::make_shared<Buffer>(1024);

// ALLOWED - Iterator types (verbose to write).
auto it = players.begin();
auto end = players.end();
```

**Forbidden - Meaning is hidden:**
```cpp
// WRONG - Return type unclear.
auto result = calculateValue(x, y, z);
auto data = getData();

// CORRECT - Explicit types are self-documenting.
int result = calculateValue(x, y, z);
std::vector<uint8_t> data = getData();
```

**Rule:** If understanding the type requires jumping to a definition, `auto` is forbidden.

**Why?** Code must be understandable without IDE assistance. The reader is more important than the writer.

### 2. SINGLE-LINE CODE - NO WRAPPING

**Code must fit in the reader's working memory. If it does not fit on one line, it does not fit in the head either.**

Control flow, conditions, and signatures must stay on single lines. This enforces **locality of understanding**: all required information must be visible in one visual frame.

```cpp
// GOOD - all parameters visible, even if line is long.
void processData(const std::string& source, const std::string& target, bool validate, int quality, ProcessingMode mode)
{
	// You can see everything. No hidden coupling. No indirection.
}

// WRONG - wrapping hides complexity.
void processData(
	const std::string& source,
	const std::string& target,
	bool validate,
	int quality,
	ProcessingMode mode)
{
	// Now you have to scan vertically. Context is distributed.
}

// ALSO WRONG - abstraction hides parameters.
struct ProcessingConfig { /* ... */ };
void processData(const ProcessingConfig& config)
{
	// Now you can't see what the function needs.
	// Coupling is hidden. Debugging is harder.
}

// CORRECT - condition visible.
if (cursor[1] == '!' && cursor[2] == '[' && cursor[3] == 'C' && cursor[4] == 'D')
{
	handleCDATA();
}
```

**Why single-line?**
- **Visibility over abstraction** - You can see all parameters/conditions directly. No indirection. No hidden coupling.
- Your brain has ~7±2 working memory slots. Single-line keeps everything in one frame.
- Wrapping distributes complexity vertically - makes you scan and reconstruct context
- Creating parameter objects to "fix" long lines makes things WORSE: hidden coupling, loss of transparency, harder debugging
- A long single line is honest. It shows the real complexity. That's good.

**Don't wrap. Don't hide. If it's long, it's long. That's the truth.**

**Constructor initialization lists follow the same rule - single line:**
```cpp
// CORRECT - initialization list on single line.
XmlIndenter::XmlIndenter(const std::string& content) : xmlContent(content), indentStr("\t"), eolStr("\n"), indentOnly(true)
{
}

// WRONG - wrapping distributes context vertically.
XmlIndenter::XmlIndenter(const std::string& content)
	: xmlContent(content),
	  indentStr("\t"),
	  eolStr("\n"),
	  indentOnly(true)
{
}
```

**Why?** Same reasons as function parameters and conditions. You need to see all initializers at once. Wrapping forces you to scan and reconstruct context. A long initialization list shows the real complexity - that's honest.

### 3. ALLMAN BRACES - ALWAYS ON NEW LINE
Opening braces `{` ALWAYS go on a new line. No exceptions (except single-line lambdas).

```cpp
// WRONG.
if (condition) {
	doSomething();
}

// CORRECT.
if (condition)
{
	doSomething();
}
```

**Why?** Visual symmetry makes code easier to scan and spot errors.

### 4. TABS FOR INDENTATION - NOT SPACES
Use tabs, period. Configure your editor properly.

Why? Because a single tab character is the true, unambiguous representation of a single indentation level. Spaces are a visual approximation; tabs are the logical unit.

### 5. COMPLETE SENTENCES IN COMMENTS
Comments start with capital letter, end with period.

```cpp
// WRONG.
// calculate average value

// CORRECT.
// Calculate the average value.
```

**Why?** Professional code looks professional. We're not writing text messages.

---

## Naming Conventions

Get the names right or the code gets rejected.

### Variables, Functions, and Members
Use **camelCase**:

```cpp
int totalCount = 0;
std::string fileName = "data.txt";

void processFile(const std::string& path)
{
	int bufferSize = 1024;
}
```

### Classes, Structs, Enums
Use **PascalCase**:

```cpp
class FileProcessor
{
	// Implementation.
};

struct ProcessingResult
{
	int originalSize;
	int newSize;
};

enum class ProcessingMode
{
	Fast,
	Balanced,
	Quality
};
```

### Constants and Macros
Use **SCREAMING_SNAKE_CASE**:

```cpp
const int MAX_BUFFER_SIZE = 10'000'000;
const std::string APP_NAME = "Application";

#define DEFAULT_TIMEOUT 30
```

---

## Formatting Rules

### Indentation
- **Tabs only** - no spaces for indentation.
- One tab per level.

### Braces Placement
Opening brace `{` on new line for:
- Functions
- Classes, structs, enums
- Namespaces
- If/else blocks
- Loops (for, while, do-while)
- Switch statements
- Try/catch blocks
- Multi-line lambdas

**Examples:**

```cpp
void processFile(const std::string& path)
{
	// Function body.
}

class FileProcessor
{
	// Class body.
};

if (condition)
{
	// If body.
}

for (int i = 0; i < count; i++)
{
	// Loop body.
}
```

**Exception - Single-line lambdas:**

```cpp
std::sort(files.begin(), files.end(), [](const File& a, const File& b) { return a.size < b.size; });
```

### Spacing Rules

**Between functions - one blank line:**
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

**Within functions - blank lines separate logic:**
```cpp
void processData()
{
	// Section 1: Read data.
	FileData data = readFile(path);
	size_t size = data.size();
	
	// Section 2: Process data.
	ProcessedData processed = transform(data);
	OptimizedData optimized = optimize(processed);
	
	// Section 3: Write results.
	writeOutput(optimized);
}
```

**Between independent control structures - blank lines:**
```cpp
// CORRECT - blank lines separate independent checks.
if (p != std::string::npos)
{
	key = attr.key.substr(p + 1);
}

if (this->params.dumpIdAttributesName)
{
	out_attr << key << "=" << attr.val;
}
```

**Related if/else stays together - no blank lines:**
```cpp
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
```

**Critical spacing rules:**
- **Never more than one blank line** anywhere.
- **No trailing spaces** at end of lines.
- **No excessive spacing** like `if (x == 0)   `.

---

## Control Flow

### When to Use If vs Switch

**Use if/else for 1-2 comparisons:**
```cpp
if (priority == 1)
{
	processHighPriority();
}
else
{
	processNormalPriority();
}
```

**Use switch for 3+ comparisons:**
```cpp
switch (priority)
{
	case 1:
	case 2:
	case 3:
		processHighPriority();
		break;
	
	case 4:
	case 5:
		processMediumPriority();
		break;
	
	default:
		processLowPriority();
		break;
}
```

### If-Else Statements
- **Always use braces** - even for single statements.
- **Keep conditions on single line** - no wrapping.
- **Include else only when both branches are meaningful** - early returns preferred for guard clauses.

```cpp
// Good - simple condition on single line.
if (cursor[1] == '!' && cursor[2] == '[' && cursor[3] == 'C' && cursor[4] == 'D')
{
	handleCDATA();
}

// Good - complex condition still on single line (even if long).
if (player.isAlive() && !player.isStunned() && player.hasMana(50) && target.isVisible() && target.isInRange(player, 1200))
{
	castSpell();
}

// Good - early return for guard clause (no else needed).
if (!isValid())
{
	return;
}

processValidData();

// Good - else when both branches are meaningful.
if (fileExists(path))
{
	loadFromFile(path);
}
else
{
	createDefaultFile(path);
}

// WRONG - no braces.
if (condition)
	doSomething();
```

### Switch Statements
- **Always include default case**.
- **Always include break** (unless fall-through is intentional and commented).
- **Only use braces when declaring variables** inside a case.

```cpp
switch (mode)
{
	case ProcessingMode::Fast:
		applyFastProcessing();
		break;
	
	case ProcessingMode::Quality:
	{
		// Braces only because we declare a variable.
		std::string config = loadQualityConfig();
		applyQualityProcessing(config);
		break;
	}
	
	default:
		std::cerr << "Unknown mode" << std::endl;
		break;
}
```

### Loops
Use the appropriate loop type:

```cpp
// For loops - known iteration count.
for (int i = 0; i < count; i++)
{
	processItem(i);
}

// Range-based for - iterating collections.
for (const std::filesystem::path& file : files)
{
	processFile(file);
}

// While loops - condition-based iteration.
while (!queue.empty())
{
	std::string item = queue.front();
	queue.pop();
	processItem(item);
}
```

---

## Type Declarations

### The Rule: Controlled Use of `auto`

Type inference with `auto` is allowed when the type is obvious, forbidden when meaning is hidden.

**Use `auto` when the type is obvious:**
```cpp
// Good - constructor clearly visible.
auto buffer = std::vector<uint8_t>(1024);
auto file = std::make_unique<FileStream>(path);

// Good - iterator types are verbose.
auto it = container.begin();
auto end = container.end();

// Good - lambda types cannot be named.
auto lambda = [](int x) { return x * 2; };
```

**Do NOT use `auto` when type is unclear:**
```cpp
// WRONG - type inference hides information.
auto result = calculateValue(x, y, z);
auto file = openFile(path);
auto data = getData();

// CORRECT - explicit types are self-documenting.
int result = calculateValue(x, y, z);
FileHandle file = openFile(path);
std::vector<uint8_t> data = getData();
```

### Language-Required `auto`

**Structured bindings (C++17) - auto is required:**
```cpp
auto [min, max] = std::minmax({1, 2, 3, 4, 5});

for (const auto& [key, value] : myMap)
{
	std::cout << key << ": " << value << std::endl;
}
```

**Generic lambdas (C++14) - auto is required:**
```cpp
auto printer = [](const auto& item) { std::cout << item << std::endl; };
```

### Const Correctness
Use `const` everywhere it applies:

```cpp
// Const parameters.
void process(const std::string& text, const std::vector<int>& data)
{
	// Use without modifying.
}

// Const member functions.
class FileInfo
{
public:
	size_t getSize() const // Doesn't modify object.
	{
		return _size;
	}
	
private:
	size_t _size;
};

// Const variables.
const int maxRetries = 3;
const std::string configFile = "config.xml";
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
int* ptr2; // Not: int* ptr1, ptr2; (ptr2 would be int, not int*).
```

---

## Comments

### General Rules
- **Start with capital letter**.
- **End with period**.
- **Use complete sentences**.
- **Use `//` for single-line** comments.
- **Use `/* */` for multi-line** comments.

```cpp
// Calculate the size reduction percentage.
double reductionPct = (1.0 - (static_cast<double>(newSize) / originalSize)) * 100.0;

/*
 * This is a multi-line comment explaining
 * a complex algorithm or process flow.
 */
```

### Documentation Comments
Use Javadoc-style `/** */` for public APIs:

```cpp
/**
 * Processes a file using the specified options.
 * @param sourcePath Path to the source file.
 * @param targetPath Path where output will be saved.
 * @param validate Whether to perform validation checks.
 * @return ProcessingResult with statistics.
 */
ProcessingResult processFile(const std::filesystem::path& sourcePath, const std::filesystem::path& targetPath, bool validate)
{
	// Implementation.
}
```

### Inline Comments
Only when clarifying non-obvious code:

```cpp
Color newPixel
{
	quantizeChannel(oldPixel.r, factor),
	quantizeChannel(oldPixel.g, factor),
	quantizeChannel(oldPixel.b, factor),
	oldPixel.a // Keep alpha unchanged.
};
```

**Don't state the obvious:**
```cpp
// WRONG - obvious comment.
int x = 5; // Set x to 5.

// CORRECT - only comment when adding value.
int retryCount = 5; // Empirically determined optimal retry count.
```

---

## Headers and Includes

### Header Guards
Use `#pragma once`:

```cpp
#pragma once

#include <string>
#include <vector>

class MyClass
{
	// Class definition.
};
```

### Include Ordering
Four groups, blank line between each:

1. Corresponding header (for .cpp files)
2. Standard library headers
3. Third-party library headers
4. Project headers

```cpp
#include "FileProcessor.h" // Corresponding header.

#include <string> // Standard library.
#include <vector>
#include <filesystem>

#include <boost/algorithm.hpp> // Third-party.

#include "utils/helpers.h" // Project headers.
#include "core/processor.h"
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

**Keep the initialization list on the same line as the constructor signature:**

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

// Avoid - assignment in constructor body.
FileProcessor::FileProcessor(const std::string& path, size_t size)
{
	filePath = path; // Less efficient.
	fileSize = size;
	isInitialized = false;
}
```

**Principle:** Keep initialization lists on a single line.

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
	// No need to define copy/move constructors, destructor.
	// Compiler-generated ones work perfectly.
};
```

**Rule of Five (when managing resources):**
If you define one, define all five:

```cpp
class ResourceManager
{
private:
	int* buffer;
	size_t size;

public:
	// Destructor.
	~ResourceManager()
	{
		delete[] buffer;
	}
	
	// Copy constructor.
	ResourceManager(const ResourceManager& other) : size(other.size)
	{
		buffer = new int[size];
		std::copy(other.buffer, other.buffer + size, buffer);
	}
	
	// Copy assignment.
	ResourceManager& operator=(const ResourceManager& other)
	{
		if (this != &other)
		{
			delete[] buffer;
			size = other.size;
			buffer = new int[size];
			std::copy(other.buffer, other.buffer + size, buffer);
		}
		return *this;
	}
	
	// Move constructor.
	ResourceManager(ResourceManager&& other) noexcept : buffer(other.buffer), size(other.size)
	{
		other.buffer = nullptr;
		other.size = 0;
	}
	
	// Move assignment.
	ResourceManager& operator=(ResourceManager&& other) noexcept
	{
		if (this != &other)
		{
			delete[] buffer;
			buffer = other.buffer;
			size = other.size;
			other.buffer = nullptr;
			other.size = 0;
		}
		return *this;
	}
};
```

---

## Error Handling

### Use Exceptions
Prefer exceptions for error handling:

```cpp
void processFile(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("File not found: " + path.string());
	}
	
	std::ifstream file(path);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + path.string());
	}
	
	// Process file.
}
```

### RAII (Resource Acquisition Is Initialization)
Use RAII for automatic resource management:

```cpp
// Good - RAII with smart pointers.
std::unique_ptr<FILE, decltype(&fclose)> file(fopen("data.txt", "r"), &fclose);

// Good - RAII with custom classes.
class FileHandle
{
private:
	FILE* file;

public:
	FileHandle(const char* path, const char* mode) : file(fopen(path, mode))
	{
		if (!file)
		{
			throw std::runtime_error("Failed to open file");
		}
	}
	
	~FileHandle()
	{
		if (file)
		{
			fclose(file);
		}
	}
	
	// Disable copying.
	FileHandle(const FileHandle&) = delete;
	FileHandle& operator=(const FileHandle&) = delete;
};
```

### Try-Catch
```cpp
try
{
	ProcessingResult result = processFile(path);
	std::cout << "Success: " << result.reductionPercent << "% reduction" << std::endl;
}
catch (const std::exception& e)
{
	std::cerr << "Error: " << e.what() << std::endl;
	return 1;
}
```

---

## String Handling

### std::string Basics
Use `std::string` for all string operations:

```cpp
std::string text = "Hello World";
std::string combined = text + " from C++";

// Substring.
std::string sub = text.substr(0, 5); // "Hello"

// Find.
size_t pos = text.find("World");
if (pos != std::string::npos)
{
	// Found.
}

// Replace.
text.replace(0, 5, "Hi"); // "Hi World"
```

### String Formatting
Use `std::to_string()` for conversions:

```cpp
int value = 42;
std::string text = "The answer is " + std::to_string(value);

double pi = 3.14159;
std::string formatted = "Pi: " + std::to_string(pi);
```

For complex formatting, use `std::ostringstream`:

```cpp
#include <sstream>
#include <iomanip>

std::ostringstream oss;
oss << "Value: " << std::setw(10) << std::setfill('0') << 42;
std::string result = oss.str(); // "Value: 0000000042"
```

---

## Numeric Formatting

### Digit Separators (C++14)
Use single quotes for digit separators:

```cpp
int largeNumber = 1'000'000;
long long veryLarge = 1'234'567'890;
double precise = 3.141'592'653;

const size_t BUFFER_SIZE = 10'000'000;
```

### Hexadecimal and Binary
```cpp
int hexValue = 0xFF'FF'FF; // Hex with separators.
int binaryValue = 0b1111'0000; // Binary with separators (C++14).
```

---

## STL Usage

### Containers

**std::vector - Dynamic array:**
```cpp
std::vector<int> numbers;
numbers.reserve(1000); // Pre-allocate to avoid reallocations.

numbers.push_back(42);
numbers.push_back(17);

for (const int& num : numbers)
{
	std::cout << num << std::endl;
}
```

**std::string - String container:**
```cpp
std::string text = "Hello";
text += " World";
```

**std::map - Ordered key-value pairs:**
```cpp
std::map<std::string, int> ages;
ages["Alice"] = 30;
ages["Bob"] = 25;

for (const auto& [name, age] : ages)
{
	std::cout << name << ": " << age << std::endl;
}
```

**std::unordered_map - Hash table:**
```cpp
std::unordered_map<std::string, int> counts;
counts["apple"] = 5;
counts["banana"] = 3;
```

**std::set - Unique sorted elements:**
```cpp
std::set<int> uniqueNumbers;
uniqueNumbers.insert(42);
uniqueNumbers.insert(17);
uniqueNumbers.insert(42); // Ignored - already exists.
```

### Algorithms

**Prefer explicit loops when control flow or allocation matters.**

**Prefer algorithms when they express intent more clearly and safely.**

```cpp
#include <algorithm>

std::vector<int> numbers = {5, 2, 8, 1, 9};

// Good - sorting is obvious and optimal.
std::sort(numbers.begin(), numbers.end());

// Good - simple search, clear intent.
std::vector<int>::iterator it = std::find(numbers.begin(), numbers.end(), 8);
if (it != numbers.end())
{
	std::cout << "Found: " << *it << std::endl;
}

// Good - erase_if is clearer and safer than manual erase loop.
std::vector<Player> players;
std::erase_if(players, [](const Player& p) { return p.health <= 0; });

// AVOID - transform hides iteration and control flow.
// std::transform(numbers.begin(), numbers.end(), doubled.begin(), [](int n) { return n * 2; });

// PREFER - explicit loop shows control flow and allocation.
std::vector<int> doubled;
doubled.reserve(numbers.size());

for (const int& n : numbers)
{
	doubled.push_back(n * 2);
}
```

**Guideline:** Use algorithms for operations where the algorithm is:
- Well-known and idiomatic (`sort`, `find`, `erase_if`, `unique`)
- Safer than manual implementation (iterator invalidation, off-by-one errors)
- Not hiding significant allocation or branching complexity

Use explicit loops when you need to see:
- Control flow
- Allocation points
- Branching logic
- Performance characteristics

### Iterators
```cpp
std::vector<std::string> files = {"file1.txt", "file2.txt", "file3.txt"};

// Forward iteration.
for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
{
	std::cout << *it << std::endl;
}

// Const iterator.
for (std::vector<std::string>::const_iterator it = files.cbegin(); it != files.cend(); ++it)
{
	std::cout << *it << std::endl;
}

// Reverse iteration.
for (std::vector<std::string>::reverse_iterator it = files.rbegin(); it != files.rend(); ++it)
{
	std::cout << *it << std::endl;
}
```

---

## Modern C++ Features

### Smart Pointers
**Never use raw pointers for ownership.**

```cpp
// WRONG - naked new/delete.
XmlParser* parser = new XmlParser(data, length);
// ... easy to leak.
delete parser;

// CORRECT - smart pointers.
std::unique_ptr<XmlParser> parser = std::make_unique<XmlParser>(data, length);
// Automatically cleaned up.

std::shared_ptr<XmlFormatter> formatter = std::make_shared<XmlFormatter>(data, length);
// Reference counted, cleaned up when last reference goes away.
```

### Range-Based For Loops
Use when iterating collections:

```cpp
std::vector<std::string> files = getFiles();

for (const std::string& file : files)
{
	process(file);
}

// Modifying elements.
for (std::string& file : files)
{
	file = normalize(file);
}
```

### std::optional (C++17)
Use for values that may not exist:

```cpp
std::optional<std::string> findFile(const std::string& name)
{
	if (fileExists(name))
	{
		return name;
	}
	return std::nullopt;
}

// Usage.
if (std::optional<std::string> file = findFile("config.xml"))
{
	std::cout << "Found: " << *file << std::endl;
}
```

### std::filesystem (C++17)
Use for file operations:

```cpp
#include <filesystem>

namespace fs = std::filesystem;

for (const fs::directory_entry& entry : fs::directory_iterator(dirPath))
{
	if (entry.is_regular_file())
	{
		std::cout << entry.path() << std::endl;
	}
}
```

### Lambdas
```cpp
// Simple lambda.
std::vector<int>::iterator it = std::find_if(numbers.begin(), numbers.end(), [](int n) { return n > 10; });

// Lambda with capture.
int threshold = 10;
std::vector<int>::iterator it2 = std::find_if(numbers.begin(), numbers.end(), [threshold](int n) { return n > threshold; });

// Lambda with explicit return type - prefer auto.
auto add = [](int a, int b) -> int { return a + b; };
```

**Avoid std::function in performance-sensitive code:**

```cpp
// BAD - type erasure, heap allocation, virtual dispatch.
std::function<int(int, int)> add = [](int a, int b) { return a + b; };

// GOOD - zero-cost, no type erasure.
auto add = [](int a, int b) { return a + b; };

// Or use templates for function parameters.
template<typename Func>
void process(Func callback)
{
	callback();
}
```

**Why?** `std::function` introduces heap allocation and virtual dispatch, violating predictable performance.

---

## Data-Oriented Design

**Memory layout and cache behavior matter more than syntax.**

### Prefer Contiguous Memory

Use `std::vector`, arrays, and `std::span`. Avoid deep object graphs and pointer chasing.

```cpp
// CORRECT - Contiguous memory, cache-friendly.
struct PlayerData
{
	std::vector<Position> positions;
	std::vector<int> healths;
	std::vector<uint8_t> levels;
};

// WRONG - Pointer-heavy, cache-hostile.
struct Player
{
	std::unique_ptr<Position> position;
	std::unique_ptr<std::vector<Item>> inventory;
	std::unique_ptr<Stats> stats;
};
```

### Flat Data Structures

Avoid excessive indirection and nesting.

```cpp
// CORRECT - Flat, direct access.
struct GameState
{
	std::vector<Player> players;
	std::vector<Npc> npcs;
	std::vector<Item> items;
};

// WRONG - Deep nesting.
struct GameState
{
	std::unique_ptr<World> world;
};

struct World
{
	std::vector<std::unique_ptr<Zone>> zones;
};

struct Zone
{
	std::vector<std::unique_ptr<Entity>> entities;
};
```

### SoA vs AoS in Hot Paths

For hot loops iterating a single field, consider Structure of Arrays.

```cpp
// AoS - good for general access.
struct Player
{
	float x;
	float y;
	int health;
	uint8_t level;
};

std::vector<Player> players;

// SoA - better for hot loops accessing one field.
struct Players
{
	std::vector<float> xs;
	std::vector<float> ys;
	std::vector<int> healths;
	std::vector<uint8_t> levels;
};

// Hot loop only needs positions - SoA wins.
for (size_t i = 0; i < players.xs.size(); i++)
{
	updatePosition(players.xs[i], players.ys[i]);
}
```

### Avoid Heap Allocation in Loops

Pre-allocate outside loops. Reuse buffers.

```cpp
// WRONG - allocates every iteration.
for (const std::string& file : files)
{
	std::vector<uint8_t> buffer(1024); // Bad.
	process(file, buffer);
}

// CORRECT - allocate once.
std::vector<uint8_t> buffer(1024);

for (const std::string& file : files)
{
	process(file, buffer);
}
```

**This grounds all other rules:** Smart pointers for ownership only, explicit loops, no algorithm pipelines—all serve cache-friendly, contiguous, predictable memory access.

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
	// Copies made on every call.
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

### Avoid Unnecessary Allocations
```cpp
// Bad - string created on every iteration.
for (int i = 0; i < 1000; ++i)
{
	std::string temp = "prefix_" + std::to_string(i);
	process(temp);
}

// Good - reuse string.
std::string temp;
for (int i = 0; i < 1000; ++i)
{
	temp = "prefix_" + std::to_string(i);
	process(temp);
}
```

---

## Anti-Patterns to Avoid

### ❌ Don't Use `auto` When Type Is Unclear
```cpp
// WRONG - unclear return type.
auto result = calculateValue(x, y, z);

// CORRECT - explicit type.
int result = calculateValue(x, y, z);

// ALLOWED - obvious constructor.
auto buffer = std::vector<uint8_t>(1024);
```

### ❌ Don't Declare Multiple Variables on Same Line
```cpp
// WRONG - confusing, unclear initialization.
int a, b = 0;
int x = 1, y = 2, z;

// CORRECT - one per line.
int a = 0;
int b = 0;
int x = 1;
int y = 2;
int z = 0;
```

### ❌ Don't Over-Engineer - Avoid Single-Use Abstractions
```cpp
// WRONG - helper function called only once, no domain meaning.
void printSeparator()
{
	std::cout << "---" << std::endl;
}

// CORRECT - inline it.
std::cout << "---" << std::endl;

// WRONG - pointless wrapper.
const int THREE = 3;
if (priority == THREE)

// CORRECT - use the value directly.
if (priority == 3)
```

**Exception:** Create abstractions when:
- Used multiple times
- Encode domain meaning (e.g., `const int MAX_PLAYERS = 100;`)
- Improve clarity significantly
- Likely to change

Single-use constants with domain meaning are allowed:

### ❌ Don't Use C-Style Casts
```cpp
// WRONG - C-style cast.
double x = (double)intValue;

// CORRECT - C++ cast.
double x = static_cast<double>(intValue);
```

### ❌ Don't Use Naked new/delete
```cpp
// WRONG - manual memory management.
int* buffer = new int[1000];
// ... easy to leak.
delete[] buffer;

// CORRECT - use containers or smart pointers.
std::vector<int> buffer(1000);
// Or
std::unique_ptr<int[]> buffer = std::make_unique<int[]>(1000);
```

### ❌ Don't Use `using namespace std;`
```cpp
// WRONG - pollutes global namespace.
using namespace std;

// CORRECT - be explicit.
std::string text;
std::vector<int> numbers;

// OK - specific using declarations in limited scope.
using std::string;
using std::vector;
```

### ❌ Don't Write Deeply Nested Code
```cpp
// WRONG - deeply nested.
if (condition1)
{
	if (condition2)
	{
		if (condition3)
		{
			// Too deep.
		}
	}
}

// CORRECT - early returns.
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

## Quick Reference Checklist

Before submitting code, verify:

- [ ] **Controlled `auto` use** - only when type is obvious from RHS
- [ ] **Explicit types when unclear** - I can see what every variable is without jumping to definitions
- [ ] **Data-oriented design** - contiguous memory, flat structures, no deep nesting
- [ ] **Algorithms vs loops** - prefer loops for control flow, algorithms for clear intent
- [ ] **Avoid std::function** - in performance-sensitive code
- [ ] **Single-line code** - all control flow, conditions, signatures, and constructor initialization lists on single lines (no wrapping)
- [ ] **Allman braces** - opening `{` on new line
- [ ] **Tabs for indentation** - not spaces
- [ ] **Complete sentences in comments** - capital letter, period
- [ ] **camelCase** for variables/functions/members
- [ ] **PascalCase** for classes/structs/enums
- [ ] **SCREAMING_SNAKE_CASE** for constants/macros
- [ ] **One blank line** between functions
- [ ] **Never more than one blank line** anywhere
- [ ] **No trailing spaces**
- [ ] **One variable per line** - no `int a, b;`
- [ ] **Switch for 3+ cases** - if/else for 1-2
- [ ] **Smart pointers** instead of raw pointers for ownership
- [ ] **RAII** for resource management
- [ ] **const correctness** everywhere applicable
- [ ] **#pragma once** for header guards
- [ ] **Include order** correct (corresponding, std, third-party, project)
- [ ] **Reserve capacity** for vectors when size known

---

## Summary

Remember these core principles:

1. **Data-Oriented** - Contiguous memory, flat structures, cache-friendly access patterns.
2. **Balanced Control Flow** - Prefer explicit loops for control flow visibility; use algorithms when they're genuinely clearer and safer.
3. **Controlled Type Inference** - Use `auto` only when type is obvious from RHS.
4. **Single-Line Code** - All control flow, conditions, signatures, and constructor initialization lists on single lines. Wrapping hides complexity instead of reducing it.
5. **Allman Braces** - Opening braces on new lines always.
6. **Complete Sentences** - Comments are documentation.
7. **Don't Over-Engineer** - YAGNI (You Ain't Gonna Need It).
8. **Modern C++ for Ownership** - Smart pointers, RAII, move semantics.
9. **Performance** - Pass by reference, reserve capacity, avoid allocations in loops.
