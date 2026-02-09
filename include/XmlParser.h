#pragma once

#include <list>
#include <sstream>
#include <stack>

namespace QuickXml
{
	struct XmlContext
	{
		bool inOpeningTag;
		bool inClosingTag;
		size_t declarationObjects;
	};

	enum XmlTokenType
	{
		Undefined = 1 << 0,

		TagOpening = 1 << 1,        // <nx:sample
		TagClosing = 1 << 2,        // </nx:sample
		TagOpeningEnd = 1 << 3,     // > of opening tag.
		TagClosingEnd = 1 << 4,     // > of closing tag.
		TagSelfClosingEnd = 1 << 5, // /> of self closing tag.
		AttrName = 1 << 6,
		AttrValue = 1 << 7,
		Text = 1 << 8,
		Whitespace = 1 << 9,
		Instruction = 1 << 10,      // <?..?> / <%..%>
		DeclarationBeg = 1 << 11,   // <!...[
		DeclarationEnd = 1 << 12,   // ]>
		DeclarationSelfClosing = 1 << 13, // <!...>
		Comment = 1 << 14,
		CDATA = 1 << 15,
		LineBreak = 1 << 16,
		Equal = 1 << 17,

		EndOfFile = 1 << 30
	};

	typedef int XmlTokensType; // Combined tokens (ex: XmlTokenType::TagOpening | XmlTokenType::Declaration).

	struct XmlToken
	{
		XmlTokenType type;     // The token type.
		size_t pos;            // The token position in stream.
		const char* chars;     // A pointer to token chars.
		size_t size;           // The token chars length.
		XmlContext context;    // The token parsing context.
	};

	const XmlToken undefinedToken = { XmlTokenType::Undefined, 0, "", 0 };

	class XmlParser
	{
	private:
		// Constant elements (they no vary after having been set).
		const char* srcText;   // Pointer to original source text.
		size_t srcLength;      // The original source text length.

		// Varying elements.
		size_t currpos;        // The current position of the parser.
		XmlContext currcontext; // The actual parsing context.
		bool hasAttrName;      // Indicates that we got an attribute name.
		XmlToken attrnametoken; // A copy of current attribute name token.
		bool expectAttrValue;  // The parser read an = in tag, then it expects an attribute value.

		XmlToken prevtoken;    // The previous token.
		XmlToken currtoken;    // The current parsed token.
		XmlToken nexttoken;    // The following token.

		XmlToken fetchToken();

		// A queue of read tokens.
		std::list<XmlToken> buffer;

		// A stack maintaining xml:space.
		std::stack<bool> preserveSpace;

	public:
		/**
		 * Constructor.
		 * 
		 * @param data The data to parse.
		 * @param length The data length.
		 */
		XmlParser(const char* data, size_t length);

		/**
		 * Destructor.
		 */
		~XmlParser();

		/**
		 * Reset the parser settings.
		 */
		void reset();

		/**
		 * Getters.
		 */
		XmlToken getPrevToken() { return this->prevtoken; }
		XmlToken getCurrToken() { return this->currtoken; }
		XmlToken getNextToken() { return this->nexttoken; }

		/**
		 * Indicates if the current node is in xml:space="preserve" context.
		 * 
		 * @param contextualized Should the current parsing context be considered.
		 * @return True when xml:space is in preserve mode.
		 */
		bool isSpacePreserve();

		/**
		 * Get the next non-text token.
		 * This function feeds the tokens queue until it finds a structural token.
		 * The queue is popped on next "parseNext()" calls.
		 * 
		 * @return The next non-text token.
		 */
		XmlToken getNextStructureToken();

		/**
		 * Fetch next token.
		 * 
		 * @return The next recognized token.
		 */
		XmlToken parseNext();

		/**
		 * Parse input until first token of given type.
		 * 
		 * @param type The type of tokens to fetch; multiple tokens can be passed using OR operator
		 *             (ex. XmlTokenType::Declaration | XmlTokenType::TagOpening).
		 * @return The found token. The EndOfFile token is returned if no occurrence could be found.
		 */
		XmlToken parseUntil(XmlTokensType type);

		/**
		 * Reads some chars in main stream.
		 * 
		 * @param nchars The number of chars to read.
		 * @return The number of chars read (might be lower than parameter, especially when reaching the end of stream).
		 */
		size_t readChars(size_t nchars = 1);

		/**
		 * Reads the next word in main stream and update cursor position.
		 * 
		 * @return The size of word.
		 */
		size_t readNextWord(bool skipQuotedStrings = false);

		/**
		 * Reads stream (and update cursor position) until given delimiter.
		 * 
		 * @param delimiter The string to search.
		 * @param offset The number of chars to skip before checking delimiter.
		 * @param goAfter Indicates to place cursor after the delimiter.
		 * @param skipDelimiter A delimiter which introduce a segment to ignore. Let's consider following example:
		 *                      <!DOCTYPE greeting [.
		 *                        <!ELEMENT greeting (#PCDATA)>
		 *                      ]>
		 *                      Reading until delimiter ">" with skipDelimiter "<" will skip the internal <!ELEMENT..>
		 * @return Number of read chars.
		 */
		size_t readUntil(const char* delimiter, size_t offset = 0, bool goAfter = false, std::string skipDelimiter = "");

		/**
		 * Reads stream (and update cursor position) until it finds one of given characters.
		 * 
		 * @param characters Set of characters to find.
		 * @param offset The number of chars to skip before checking characters.
		 * @param goAfter Indicates to place cursor after the delimiter.
		 * @return Number of read chars.
		 */
		size_t readUntilFirstOf(const char* characters, size_t offset = 0, bool goAfter = false);

		/**
		 * Reads stream (and update cursor position) until it finds any characters which differs from given characters.
		 * 
		 * @param characters Set of characters to skip.
		 * @param offset The number of chars to skip before checking characters.
		 * @return Number of read chars.
		 */
		size_t readUntilFirstNotOf(const char* characters, size_t offset = 0);

		/**
		 * Reads stream until end of incoming declaration.
		 * 
		 * @return Number of read chars.
		 */
		size_t readDeclaration();

		/**
		 * Gets the current token name (for debug).
		 * 
		 * @return A string representation of current token.
		 */
		std::string getTokenName();
	};
}
