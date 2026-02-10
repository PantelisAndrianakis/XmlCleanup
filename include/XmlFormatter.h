#pragma once

#include <map>
#include <sstream>
#include <vector>

#include "XmlParser.h"

#define XPATH_MODE_BASIC            (1 << 0)
#define XPATH_MODE_WITHNAMESPACE    (1 << 1)
#define XPATH_MODE_KEEPIDATTRIBUTE  (1 << 2)
#define XPATH_MODE_WITHNODEINDEX    (1 << 3)

namespace QuickXml
{
	struct XmlFormatterParamsType
	{
		std::string indentChars = "\t";             // Indentation char(s).
		std::string eolChars = "\n";                // End of line char(s).
		size_t maxIndentLevel = 255;                // Max indent level (0 == unlimited).
		bool ensureConformity = true;               // Make the formatter respect conformity.
		bool autoCloseTags = false;                 // Make the formatter change tags like <a></a> into <a/>.
		bool indentAttributes = false;              // Make the formatter display attributes on separated lines.
		bool indentOnly = false;                    // Make the formatter keep the existing linebreaks and only adjust indentation.
		bool applySpacePreserve = false;            // Make the formatter apply the xml:space="preserve" when defined.

		std::vector<std::string> identityAttribues; // A vector of attributes considered as identity (see setIdentityAttributes).
		bool dumpIdAttributesName = true;           // Make the currentPath dump the identity attributes name (when XPATH_MODE_KEEPIDATTRIBUTE active).
	};

	struct XmlFormatterKeyValType
	{
		std::string key;
		std::string val;
	};

	struct XmlFormatterXPathEntry
	{
		std::string name;
		size_t position;
		std::string attr;                           // Last attribute parsed.
		std::vector<XmlFormatterKeyValType> attributes; // Ident attributes.
	};

	class XmlFormatter
	{
	private:
		XmlParser* parser = NULL;

		XmlFormatterParamsType params;

		std::stringstream out;
		size_t indentLevel;                         // The real applied indent level.
		size_t levelCounter;                        // The level counter.

		bool isIdentAttribute(std::string attr);

		// Adds an EOL char to output stream.
		void writeEOL();

		// Write indentations to output stream. The indentation depends on indentLevel variable.
		void writeIndentation();

		// Adds a custom string into output stream. The string can be added several times by specifying the num parameter.
		void writeElement(std::string str, size_t num = 1);

		// Change the current indentLevel. The function maintains the level in limits [0 .. params.maxIndentLevel].
		void updateIndentLevel(int change);

	public:
		// Constructor.
		XmlFormatter(const char* data, size_t length);

		// Constructor.
		XmlFormatter(const char* data, size_t length, XmlFormatterParamsType params);

		// Destructor.
		~XmlFormatter();

		// Initialize the formatter with input data.
		void init(const char* data, size_t length);

		// Initialize the formatter with input data.
		void init(const char* data, size_t length, XmlFormatterParamsType params);

		// Make internal parameters ready for formatting.
		void reset();

		// Generates a string containing a list of recognized tokens. This method has no other goal that help for debug.
		std::string debugTokens(std::string separator = "/", bool detailed = false);

		// Performs linearize formatting.
		std::stringstream* linearize();

		// Performs pretty print formatting.
		std::stringstream* prettyPrint();

		// Construct the path of given position.
		std::stringstream* currentPath(size_t position, int xpathMode = XPATH_MODE_WITHNAMESPACE);

		// Construct a default formatter parameters object.
		static XmlFormatterParamsType getDefaultParams();
	};
}
