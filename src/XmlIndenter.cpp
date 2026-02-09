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

// Destructor.
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

/**
 * Formats single-line XML comments to ensure consistent spacing.
 * Adds one space after <!-- and one space before --> for better readability.
 * Normalizes multiple consecutive spaces within comment text to a single space.
 * Only affects single-line comments; multi-line comments remain unchanged.
 */
std::string formatSingleLineComments(const std::string& xml)
{
	std::string result = xml;
	size_t pos = 0;

	while ((pos = result.find("<!--", pos)) != std::string::npos)
	{
		// Find the end of this comment.
		size_t endPos = result.find("-->", pos);
		if (endPos == std::string::npos)
		{
			// No end tag found, move past this one.
			pos += 4;
			continue;
		}

		// Check if this is a single-line comment (no newlines between start and end).
		std::string commentText = result.substr(pos, endPos - pos + 3);
		if (commentText.find('\n') == std::string::npos && commentText.find('\r') == std::string::npos)
		{
			// Extract the comment content (between <!-- and -->).
			std::string commentContent = result.substr(pos + 4, endPos - (pos + 4));

			// Trim leading and trailing spaces.
			size_t startTrim = commentContent.find_first_not_of(' ');
			size_t endTrim = commentContent.find_last_not_of(' ');

			if (startTrim != std::string::npos && endTrim != std::string::npos)
			{
				commentContent = commentContent.substr(startTrim, endTrim - startTrim + 1);
			}
			else if (startTrim != std::string::npos)
			{
				commentContent = commentContent.substr(startTrim);
			}
			else if (endTrim != std::string::npos)
			{
				commentContent = commentContent.substr(0, endTrim + 1);
			}
			else
			{
				commentContent = ""; // Comment was all spaces.
			}

			// Normalize multiple spaces to single space within the comment content.
			std::string normalizedContent;
			normalizedContent.reserve(commentContent.length());
			bool lastWasSpace = false;

			for (char c : commentContent)
			{
				if (c == ' ')
				{
					if (!lastWasSpace)
					{
						normalizedContent.push_back(c);
						lastWasSpace = true;
					}
				}
				else
				{
					normalizedContent.push_back(c);
					lastWasSpace = false;
				}
			}

			// Replace the original comment with the normalized one.
			std::string newComment;
			if (normalizedContent.empty())
			{
				// For empty comments, use only one space between tags.
				newComment = "<!-- -->";
			}
			else
			{
				newComment = "<!-- " + normalizedContent + " -->";
			}
			result.replace(pos, endPos - pos + 3, newComment);

			// Adjust position based on the new comment length.
			pos += newComment.length();
		}
		else
		{
			// This is a multi-line comment, skip it.
			pos = endPos + 3;
		}
	}

	return result;
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

	// Format single-line XML comments to ensure proper spacing.
	formattedXml = formatSingleLineComments(formattedXml);

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
