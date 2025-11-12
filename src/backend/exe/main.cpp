#include <iostream>
#include <filesystem>
#include <fstream>
#include "antlr4-runtime.h"
#include "SimpleChessGameLexer.h"
#include "SimpleChessGameParser.h"
#include "SimpleChessGameBaseVisitor.h"

using namespace std;
namespace fs = std::filesystem;
using namespace antlr4;
using namespace chess;

// Custom visitor to handle parsed moves
class GameVisitor : public SimpleChessGameBaseVisitor {
public:
    // Returns a value
    std::any visitStrike(SimpleChessGameParser::StrikeContext *ctx) override {
        string from = ctx->COORD(0)->getText();
        string to = ctx->COORD(1)->getText();
        return make_pair(from, to);  // Can return data
    }
};

int main() {
    cout << "Start parsing" << endl;

    fs::path filepath("/mnt/c/Users/mathi/source/repos/cpp/sucden-fin-chess/test/game/game_01");
    
    if (!fs::exists(filepath)) {
        cerr << "Error: File does not exist: " << filepath << endl;
        return 1;
    }

    ifstream stream(filepath);
    if (!stream.is_open()) {
        cerr << "Error: Cannot open file " << filepath << endl;
        return 1;
    }

    // Create ANTLR input stream from file
    ANTLRInputStream input(stream);

    // Create lexer (tokenizer)
    SimpleChessGameLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    
    // Create parser
    SimpleChessGameParser parser(&tokens);
    
    // Parse the file (starting from 'game' rule)
    tree::ParseTree *tree = parser.game();
    
    // Walk the parse tree with our custom listener
    GameVisitor visitor;;
    auto result = visitor.visit(tree);
    
    cout << "\nParsing completed successfully!" << endl;

    return 0;
}