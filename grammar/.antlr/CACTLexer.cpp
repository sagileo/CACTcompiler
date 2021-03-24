
    #include <vector>
    #include <string>


// Generated from /home/teacher/workspace/compiler/demo_cact_compiler/grammar/CACT.g4 by ANTLR 4.8


#include "CACTLexer.h"


using namespace antlr4;


CACTLexer::CACTLexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

CACTLexer::~CACTLexer() {
  delete _interpreter;
}

std::string CACTLexer::getGrammarFileName() const {
  return "CACT.g4";
}

const std::vector<std::string>& CACTLexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& CACTLexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& CACTLexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& CACTLexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& CACTLexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> CACTLexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& CACTLexer::getATN() const {
  return _atn;
}




// Static vars and initialization.
std::vector<dfa::DFA> CACTLexer::_decisionToDFA;
atn::PredictionContextCache CACTLexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN CACTLexer::_atn;
std::vector<uint16_t> CACTLexer::_serializedATN;

std::vector<std::string> CACTLexer::_ruleNames = {
  u8"T__0", u8"T__1", u8"T__2", u8"T__3", u8"T__4", u8"T__5", u8"BoolConst", 
  u8"Ident", u8"IdentNondigit", u8"Digit", u8"IntConst", u8"DecimalConst", 
  u8"OctalConst", u8"HexadecimalConst", u8"NonzeroDigit", u8"OctalDigit", 
  u8"HexadecimalPrefix", u8"HexadecimalDigit", u8"NewLine", u8"WhiteSpace", 
  u8"BlockComment", u8"LineComment"
};

std::vector<std::string> CACTLexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> CACTLexer::_modeNames = {
  u8"DEFAULT_MODE"
};

std::vector<std::string> CACTLexer::_literalNames = {
  "", u8"'const'", u8"','", u8"';'", u8"'int'", u8"'bool'", u8"'='"
};

std::vector<std::string> CACTLexer::_symbolicNames = {
  "", "", "", "", "", "", "", u8"BoolConst", u8"Ident", u8"IntConst", u8"NewLine", 
  u8"WhiteSpace", u8"BlockComment", u8"LineComment"
};

dfa::Vocabulary CACTLexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> CACTLexer::_tokenNames;

CACTLexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x2, 0xf, 0xaa, 0x8, 0x1, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 
    0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
    0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 
    0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 
    0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 
    0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 
    0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
    0x3, 0x8, 0x5, 0x8, 0x4e, 0xa, 0x8, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x52, 
    0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x55, 0xb, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x5e, 0xa, 0xc, 
    0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0x63, 0xa, 0xd, 0xc, 0xd, 0xe, 
    0xd, 0x66, 0xb, 0xd, 0x5, 0xd, 0x68, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 0x6, 
    0xe, 0x6c, 0xa, 0xe, 0xd, 0xe, 0xe, 0xe, 0x6d, 0x3, 0xf, 0x3, 0xf, 0x6, 
    0xf, 0x72, 0xa, 0xf, 0xd, 0xf, 0xe, 0xf, 0x73, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 
    0x12, 0x7e, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 0x5, 
    0x14, 0x84, 0xa, 0x14, 0x3, 0x14, 0x5, 0x14, 0x87, 0xa, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x3, 0x15, 0x6, 0x15, 0x8c, 0xa, 0x15, 0xd, 0x15, 0xe, 0x15, 
    0x8d, 0x3, 0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 
    0x7, 0x16, 0x96, 0xa, 0x16, 0xc, 0x16, 0xe, 0x16, 0x99, 0xb, 0x16, 0x3, 
    0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x7, 0x17, 0xa4, 0xa, 0x17, 0xc, 0x17, 0xe, 0x17, 
    0xa7, 0xb, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x97, 0x2, 0x18, 0x3, 0x3, 
    0x5, 0x4, 0x7, 0x5, 0x9, 0x6, 0xb, 0x7, 0xd, 0x8, 0xf, 0x9, 0x11, 0xa, 
    0x13, 0x2, 0x15, 0x2, 0x17, 0xb, 0x19, 0x2, 0x1b, 0x2, 0x1d, 0x2, 0x1f, 
    0x2, 0x21, 0x2, 0x23, 0x2, 0x25, 0x2, 0x27, 0xc, 0x29, 0xd, 0x2b, 0xe, 
    0x2d, 0xf, 0x3, 0x2, 0xa, 0x6, 0x2, 0x32, 0x3b, 0x43, 0x5c, 0x61, 0x61, 
    0x63, 0x7c, 0x5, 0x2, 0x43, 0x5c, 0x61, 0x61, 0x63, 0x7c, 0x3, 0x2, 
    0x32, 0x3b, 0x3, 0x2, 0x33, 0x3b, 0x3, 0x2, 0x32, 0x39, 0x5, 0x2, 0x32, 
    0x3b, 0x43, 0x48, 0x63, 0x68, 0x4, 0x2, 0xb, 0xb, 0x22, 0x22, 0x4, 0x2, 
    0xc, 0xc, 0xf, 0xf, 0x2, 0xae, 0x2, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2, 0x5, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x9, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0xb, 0x3, 0x2, 0x2, 0x2, 0x2, 0xd, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0xf, 0x3, 0x2, 0x2, 0x2, 0x2, 0x11, 0x3, 0x2, 0x2, 0x2, 0x2, 0x17, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x27, 0x3, 0x2, 0x2, 0x2, 0x2, 0x29, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2d, 0x3, 0x2, 0x2, 0x2, 
    0x3, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x5, 0x35, 0x3, 0x2, 0x2, 0x2, 0x7, 0x37, 
    0x3, 0x2, 0x2, 0x2, 0x9, 0x39, 0x3, 0x2, 0x2, 0x2, 0xb, 0x3d, 0x3, 0x2, 
    0x2, 0x2, 0xd, 0x42, 0x3, 0x2, 0x2, 0x2, 0xf, 0x4d, 0x3, 0x2, 0x2, 0x2, 
    0x11, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x13, 0x56, 0x3, 0x2, 0x2, 0x2, 0x15, 
    0x58, 0x3, 0x2, 0x2, 0x2, 0x17, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x19, 0x67, 
    0x3, 0x2, 0x2, 0x2, 0x1b, 0x69, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x6f, 0x3, 
    0x2, 0x2, 0x2, 0x1f, 0x75, 0x3, 0x2, 0x2, 0x2, 0x21, 0x77, 0x3, 0x2, 
    0x2, 0x2, 0x23, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x25, 0x7f, 0x3, 0x2, 0x2, 
    0x2, 0x27, 0x86, 0x3, 0x2, 0x2, 0x2, 0x29, 0x8b, 0x3, 0x2, 0x2, 0x2, 
    0x2b, 0x91, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x2f, 
    0x30, 0x7, 0x65, 0x2, 0x2, 0x30, 0x31, 0x7, 0x71, 0x2, 0x2, 0x31, 0x32, 
    0x7, 0x70, 0x2, 0x2, 0x32, 0x33, 0x7, 0x75, 0x2, 0x2, 0x33, 0x34, 0x7, 
    0x76, 0x2, 0x2, 0x34, 0x4, 0x3, 0x2, 0x2, 0x2, 0x35, 0x36, 0x7, 0x2e, 
    0x2, 0x2, 0x36, 0x6, 0x3, 0x2, 0x2, 0x2, 0x37, 0x38, 0x7, 0x3d, 0x2, 
    0x2, 0x38, 0x8, 0x3, 0x2, 0x2, 0x2, 0x39, 0x3a, 0x7, 0x6b, 0x2, 0x2, 
    0x3a, 0x3b, 0x7, 0x70, 0x2, 0x2, 0x3b, 0x3c, 0x7, 0x76, 0x2, 0x2, 0x3c, 
    0xa, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x7, 0x64, 0x2, 0x2, 0x3e, 0x3f, 
    0x7, 0x71, 0x2, 0x2, 0x3f, 0x40, 0x7, 0x71, 0x2, 0x2, 0x40, 0x41, 0x7, 
    0x6e, 0x2, 0x2, 0x41, 0xc, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x7, 0x3f, 
    0x2, 0x2, 0x43, 0xe, 0x3, 0x2, 0x2, 0x2, 0x44, 0x45, 0x7, 0x76, 0x2, 
    0x2, 0x45, 0x46, 0x7, 0x74, 0x2, 0x2, 0x46, 0x47, 0x7, 0x77, 0x2, 0x2, 
    0x47, 0x4e, 0x7, 0x67, 0x2, 0x2, 0x48, 0x49, 0x7, 0x68, 0x2, 0x2, 0x49, 
    0x4a, 0x7, 0x63, 0x2, 0x2, 0x4a, 0x4b, 0x7, 0x6e, 0x2, 0x2, 0x4b, 0x4c, 
    0x7, 0x75, 0x2, 0x2, 0x4c, 0x4e, 0x7, 0x67, 0x2, 0x2, 0x4d, 0x44, 0x3, 
    0x2, 0x2, 0x2, 0x4d, 0x48, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x10, 0x3, 0x2, 
    0x2, 0x2, 0x4f, 0x53, 0x5, 0x13, 0xa, 0x2, 0x50, 0x52, 0x9, 0x2, 0x2, 
    0x2, 0x51, 0x50, 0x3, 0x2, 0x2, 0x2, 0x52, 0x55, 0x3, 0x2, 0x2, 0x2, 
    0x53, 0x51, 0x3, 0x2, 0x2, 0x2, 0x53, 0x54, 0x3, 0x2, 0x2, 0x2, 0x54, 
    0x12, 0x3, 0x2, 0x2, 0x2, 0x55, 0x53, 0x3, 0x2, 0x2, 0x2, 0x56, 0x57, 
    0x9, 0x3, 0x2, 0x2, 0x57, 0x14, 0x3, 0x2, 0x2, 0x2, 0x58, 0x59, 0x9, 
    0x4, 0x2, 0x2, 0x59, 0x16, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5e, 0x5, 0x19, 
    0xd, 0x2, 0x5b, 0x5e, 0x5, 0x1b, 0xe, 0x2, 0x5c, 0x5e, 0x5, 0x1d, 0xf, 
    0x2, 0x5d, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5b, 0x3, 0x2, 0x2, 0x2, 
    0x5d, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x18, 0x3, 0x2, 0x2, 0x2, 0x5f, 
    0x68, 0x7, 0x32, 0x2, 0x2, 0x60, 0x64, 0x5, 0x1f, 0x10, 0x2, 0x61, 0x63, 
    0x5, 0x15, 0xb, 0x2, 0x62, 0x61, 0x3, 0x2, 0x2, 0x2, 0x63, 0x66, 0x3, 
    0x2, 0x2, 0x2, 0x64, 0x62, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 0x3, 0x2, 
    0x2, 0x2, 0x65, 0x68, 0x3, 0x2, 0x2, 0x2, 0x66, 0x64, 0x3, 0x2, 0x2, 
    0x2, 0x67, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x67, 0x60, 0x3, 0x2, 0x2, 0x2, 
    0x68, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x69, 0x6b, 0x7, 0x32, 0x2, 0x2, 0x6a, 
    0x6c, 0x5, 0x21, 0x11, 0x2, 0x6b, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x6d, 
    0x3, 0x2, 0x2, 0x2, 0x6d, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x6e, 0x3, 
    0x2, 0x2, 0x2, 0x6e, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x71, 0x5, 0x23, 
    0x12, 0x2, 0x70, 0x72, 0x5, 0x25, 0x13, 0x2, 0x71, 0x70, 0x3, 0x2, 0x2, 
    0x2, 0x72, 0x73, 0x3, 0x2, 0x2, 0x2, 0x73, 0x71, 0x3, 0x2, 0x2, 0x2, 
    0x73, 0x74, 0x3, 0x2, 0x2, 0x2, 0x74, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x75, 
    0x76, 0x9, 0x5, 0x2, 0x2, 0x76, 0x20, 0x3, 0x2, 0x2, 0x2, 0x77, 0x78, 
    0x9, 0x6, 0x2, 0x2, 0x78, 0x22, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7a, 0x7, 
    0x32, 0x2, 0x2, 0x7a, 0x7e, 0x7, 0x7a, 0x2, 0x2, 0x7b, 0x7c, 0x7, 0x32, 
    0x2, 0x2, 0x7c, 0x7e, 0x7, 0x5a, 0x2, 0x2, 0x7d, 0x79, 0x3, 0x2, 0x2, 
    0x2, 0x7d, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x24, 0x3, 0x2, 0x2, 0x2, 
    0x7f, 0x80, 0x9, 0x7, 0x2, 0x2, 0x80, 0x26, 0x3, 0x2, 0x2, 0x2, 0x81, 
    0x83, 0x7, 0xf, 0x2, 0x2, 0x82, 0x84, 0x7, 0xc, 0x2, 0x2, 0x83, 0x82, 
    0x3, 0x2, 0x2, 0x2, 0x83, 0x84, 0x3, 0x2, 0x2, 0x2, 0x84, 0x87, 0x3, 
    0x2, 0x2, 0x2, 0x85, 0x87, 0x7, 0xc, 0x2, 0x2, 0x86, 0x81, 0x3, 0x2, 
    0x2, 0x2, 0x86, 0x85, 0x3, 0x2, 0x2, 0x2, 0x87, 0x88, 0x3, 0x2, 0x2, 
    0x2, 0x88, 0x89, 0x8, 0x14, 0x2, 0x2, 0x89, 0x28, 0x3, 0x2, 0x2, 0x2, 
    0x8a, 0x8c, 0x9, 0x8, 0x2, 0x2, 0x8b, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x8c, 
    0x8d, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8e, 
    0x3, 0x2, 0x2, 0x2, 0x8e, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 0x8, 
    0x15, 0x2, 0x2, 0x90, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x91, 0x92, 0x7, 0x31, 
    0x2, 0x2, 0x92, 0x93, 0x7, 0x2c, 0x2, 0x2, 0x93, 0x97, 0x3, 0x2, 0x2, 
    0x2, 0x94, 0x96, 0xb, 0x2, 0x2, 0x2, 0x95, 0x94, 0x3, 0x2, 0x2, 0x2, 
    0x96, 0x99, 0x3, 0x2, 0x2, 0x2, 0x97, 0x98, 0x3, 0x2, 0x2, 0x2, 0x97, 
    0x95, 0x3, 0x2, 0x2, 0x2, 0x98, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x99, 0x97, 
    0x3, 0x2, 0x2, 0x2, 0x9a, 0x9b, 0x7, 0x2c, 0x2, 0x2, 0x9b, 0x9c, 0x7, 
    0x31, 0x2, 0x2, 0x9c, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9e, 0x8, 0x16, 
    0x2, 0x2, 0x9e, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x9f, 0xa0, 0x7, 0x31, 0x2, 
    0x2, 0xa0, 0xa1, 0x7, 0x31, 0x2, 0x2, 0xa1, 0xa5, 0x3, 0x2, 0x2, 0x2, 
    0xa2, 0xa4, 0xa, 0x9, 0x2, 0x2, 0xa3, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa4, 
    0xa7, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xa6, 
    0x3, 0x2, 0x2, 0x2, 0xa6, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa5, 0x3, 
    0x2, 0x2, 0x2, 0xa8, 0xa9, 0x8, 0x17, 0x2, 0x2, 0xa9, 0x2e, 0x3, 0x2, 
    0x2, 0x2, 0x10, 0x2, 0x4d, 0x53, 0x5d, 0x64, 0x67, 0x6d, 0x73, 0x7d, 
    0x83, 0x86, 0x8d, 0x97, 0xa5, 0x3, 0x8, 0x2, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

CACTLexer::Initializer CACTLexer::_init;
