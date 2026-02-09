#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "XmlFormatter.h"

// XmlIndenter: A wrapper class for different XML formatting engines.
class XmlIndenter
{
private:
	// The XML content to indent.
	std::string xmlContent;

	// Indentation settings.
	std::string indentStr;
	std::string eolStr;
	bool indentOnly;
	bool autoCloseEmptyElements;

public:
	// Constructor with default settings.
	XmlIndenter(const std::string& xmlContent);

	// Constructor with custom settings.
	XmlIndenter(const std::string& xmlContent, const std::string& indentStr = "\t", const std::string& eolStr = "\n", bool indentOnly = true, bool autoCloseEmptyElements = true);

	// Destructor.
	~XmlIndenter();

	// Indent XML content using QuickXml formatter.
	std::string indentXML();

	// Setters for options.
	void setIndentString(const std::string& str);
	void setEOLString(const std::string& str);
	void setIndentOnly(bool indentOnly);
	void setAutoCloseEmptyElements(bool autoClose);

	// Getters for options.
	std::string getIndentString() const;
	std::string getEOLString() const;
	bool getIndentOnly() const;
	bool getAutoCloseEmptyElements() const;

	// Static utility function to indent XML string.
	static std::string indentXMLString(const std::string& xml, const std::string& indentStr = "\t", const std::string& eolStr = "\n", bool indentOnly = true, bool autoCloseEmptyElements = true);
};
