#include "XmlIndenter.h"
#include "XmlFormatter.h"

// Constructor with default settings.
XmlIndenter::XmlIndenter(const std::string& xmlContent) : xmlContent(xmlContent), indentStr("\t"), eolStr("\n"), indentOnly(true), autoCloseEmptyElements(true)
{
}

// Constructor with custom settings.
XmlIndenter::XmlIndenter(const std::string& xmlContent, const std::string& indentStr, const std::string& eolStr, bool indentOnly, bool autoCloseEmptyElements) : xmlContent(xmlContent), indentStr(indentStr), eolStr(eolStr), indentOnly(indentOnly), autoCloseEmptyElements(autoCloseEmptyElements)
{
}

// Destructor
XmlIndenter::~XmlIndenter()
{
}

// Helper function to replace all occurrences of a string with another string.
std::string replaceAll(const std::string& source, const std::string& from, const std::string& to)
{
	std::string result = source;
	size_t pos = 0;
	while ((pos = result.find(from, pos)) != std::string::npos)
	{
		result.replace(pos, from.length(), to);
		pos += to.length();
	}
	return result;
}

// Ensure all line endings are Windows-style (\r\n).
std::string normalizeLineEndings(const std::string& content)
{
	// This implementation more closely mirrors the C# Regex.Replace approach.
	std::string result = content;

	// Replace Mac line endings (\r) not followed by \n with Windows line endings (\r\n).
	size_t pos = 0;
	while ((pos = result.find('\r', pos)) != std::string::npos)
	{
		if (pos + 1 >= result.length() || result[pos + 1] != '\n')
		{
			result.replace(pos, 1, "\r\n");
			pos += 2;
		}
		else
		{
			pos++;
		}
	}

	// Ensure all line endings are Windows format (\r\n).
	std::string finalResult;
	finalResult.reserve(result.length() + result.length() / 10); // Estimate for potential added \r characters.

	for (size_t i = 0; i < result.length(); i++)
	{
		if (result[i] == '\n' && (i == 0 || result[i - 1] != '\r'))
		{
			finalResult.append("\r\n");
		}
		else
		{
			finalResult.push_back(result[i]);
		}
	}

	return finalResult;
}

// Indent XML content using QuickXml formatter.
std::string XmlIndenter::indentXML()
{
	// Pre-process the XML content.
	std::string processedContent = xmlContent;

	// Remove all content until first < is reached.
	size_t startIndex = processedContent.find('<');
	if (startIndex != std::string::npos)
	{
		processedContent = processedContent.substr(startIndex);
	}

	// Replace Mac line endings (\r) with Windows line endings (\r\n).
	// This more closely follows the C# code's regex approach.
	processedContent = normalizeLineEndings(processedContent);

	// Create formatter parameters.
	QuickXml::XmlFormatterParamsType params;
	params.indentChars = indentStr;
	params.eolChars = eolStr;
	params.maxIndentLevel = 255; // Reasonable default.
	params.ensureConformity = true;
	params.autoCloseTags = autoCloseEmptyElements;
	params.indentAttributes = false; // Default for indent-only mode.
	params.indentOnly = indentOnly;
	params.applySpacePreserve = true; // Respect xml:space="preserve".

	// Create formatter with processed XML content.
	QuickXml::XmlFormatter formatter(processedContent.c_str(), processedContent.length(), params);

	// Format the XML.
	std::stringstream* result = formatter.prettyPrint();

	// Get the formatted string.
	std::string formattedXml = result->str();

	// Clear the stringstream (good practice).
	result->str(std::string());

	// Post-process the formatted XML.
	// Replace specific patterns.
	formattedXml = replaceAll(formattedXml, ">\t<!--", "> <!--");
	formattedXml = replaceAll(formattedXml, "><!--", "> <!--");
	formattedXml = replaceAll(formattedXml, "\"/>", "\" />");

	// Ensure all self-closing tags have a space before />.
	// First handle tags without attributes (like <flattenmapper/>).
	formattedXml = replaceAll(formattedXml, "</>", "< />");  // Just in case.

	// Handle tag names followed directly by />.
	size_t pos = 0;
	std::string resultStr = formattedXml;
	std::string pattern = "/>";
	std::string replacement = " />";

	while ((pos = resultStr.find(pattern, pos)) != std::string::npos)
	{
		// Only add space if there isn't already one and it's not part of "/>.
		if (pos > 0 && resultStr[pos - 1] != ' ' && resultStr[pos - 1] != '"')
		{
			resultStr.replace(pos, pattern.length(), replacement);
			pos += replacement.length();
		}
		else
		{
			pos += pattern.length();
		}
	}

	formattedXml = resultStr;

	// Normalize all line endings to Windows style (\r\n).
	formattedXml = normalizeLineEndings(formattedXml);

	return formattedXml;
}

// Setters for options.
void XmlIndenter::setIndentString(const std::string& str)
{
	indentStr = str;
}

void XmlIndenter::setEOLString(const std::string& str)
{
	eolStr = str;
}

void XmlIndenter::setIndentOnly(bool value)
{
	indentOnly = value;
}

void XmlIndenter::setAutoCloseEmptyElements(bool autoClose)
{
	autoCloseEmptyElements = autoClose;
}

// Getters for options.
std::string XmlIndenter::getIndentString() const
{
	return indentStr;
}

std::string XmlIndenter::getEOLString() const
{
	return eolStr;
}

bool XmlIndenter::getIndentOnly() const
{
	return indentOnly;
}

bool XmlIndenter::getAutoCloseEmptyElements() const
{
	return autoCloseEmptyElements;
}

// Static utility function to indent XML string.
std::string XmlIndenter::indentXMLString(const std::string& xml, const std::string& indentStr, const std::string& eolStr, bool indentOnly, bool autoCloseEmptyElements)
{
	XmlIndenter indenter(xml, indentStr, eolStr, indentOnly, autoCloseEmptyElements);
	return indenter.indentXML();
}
