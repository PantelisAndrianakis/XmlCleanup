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
		// Constructor.
		XmlParser(const char* data, size_t length);

		// Destructor.
		~XmlParser();

		// Reset the parser settings.
		void reset();

		// Getters.
		XmlToken getPrevToken() { return this->prevtoken; }
		XmlToken getCurrToken() { return this->currtoken; }
		XmlToken getNextToken() { return this->nexttoken; }

		// Indicates if the current node is in xml:space="preserve" context.
		bool isSpacePreserve();

		// Get the next non-text token. This function feeds the tokens queue until it finds a structural token. The queue is popped on next "parseNext()" calls.
		XmlToken getNextStructureToken();

		// Fetch next token.
		XmlToken parseNext();

		// Parse input until first token of given type. Multiple tokens can be passed using OR operator (ex. XmlTokenType::Declaration | XmlTokenType::TagOpening).
		XmlToken parseUntil(XmlTokensType type);

		// Reads some chars in main stream.
		size_t readChars(size_t nchars = 1);

		// Reads the next word in main stream and update cursor position.
		size_t readNextWord(bool skipQuotedStrings = false);

		// Reads stream (and update cursor position) until given delimiter. A delimiter which introduce a segment to ignore can be used with skipDelimiter parameter.
		size_t readUntil(const char* delimiter, size_t offset = 0, bool goAfter = false, std::string skipDelimiter = "");

		// Reads stream (and update cursor position) until it finds one of given characters.
		size_t readUntilFirstOf(const char* characters, size_t offset = 0, bool goAfter = false);

		// Reads stream (and update cursor position) until it finds any characters which differs from given characters.
		size_t readUntilFirstNotOf(const char* characters, size_t offset = 0);

		// Reads stream until end of incoming declaration.
		size_t readDeclaration();

		// Gets the current token name (for debug).
		std::string getTokenName();
	};
}
