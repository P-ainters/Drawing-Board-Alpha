#include <memory>
#include <gtest/gtest.h>
#include <parser.h>

using namespace lexers;
using namespace parser;

TEST(LibrariesParsingTest, BasicConsTest) {
    Scope s;
    lexers::Lexer lex("(load \"Test.scm\")");
    parseExpr(lex)->eval(s);
    ASSERT_TRUE(s.count("Cons"));
    ASSERT_TRUE(s.count("Car"));
    ASSERT_TRUE(s.count("Cdr"));

    lex.appendExp("(define p (Cons 1 2))").appendExp("(Cdr p)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(2, numPtr->getValue());

    lex.appendExp("(Car p)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
}

TEST(LibrariesParsingTest, AdvanceConsTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Test.scm\")")
            .appendExp("(define p (Cons 1 2))")
            .appendExp("(define pp (Cons 3 p))")
            .appendExp("(Car (Cdr pp))");

    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(1, numPtr->getValue());
}


TEST(LibrariesParsingTest, MinusTest) {
    Scope s;
    lexers::Lexer lex;
    lex.appendExp("(load \"Base.scm\")").appendExp("(add-list (list 7 8 9))");

    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    auto numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(24, numPtr->getValue());

    lex.appendExp("(- 5 7 8 9)");

    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<NumberAST>(res));
    numPtr = std::dynamic_pointer_cast<NumberAST>(res);
    ASSERT_EQ(-19, numPtr->getValue());
}

TEST(LibrariesParsingTest, NotTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(not (- 10 4 6))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(not (- 10 4 5))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
}

TEST(LibrariesParsingTest, AndTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(and (+ 10 4 6) (- 10 1) (+ 5 6))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(and (+ 10 4 6) (- 10 10) (+ 5 7))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
}

TEST(LibrariesParsingTest, OrTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(or (+ 10 (~ 10)) (- 10 1) (- 1 10))");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(or (+ 0 0) (- 10 10) 0)");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansFalseAST>(res));
}

TEST(LibrariesParsingTest, EqualTest) {
    Scope s;
    lexers::Lexer lex("(load \"Base.scm\")");

    lex.appendExp("(= (+ 10 (~ 10)) 0)");
    auto res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

    lex.appendExp("(= 13 (- 20 7) (+ 5 8))");
    res = parseAllExpr(lex)->eval(s);
    ASSERT_TRUE(std::dynamic_pointer_cast<BooleansTrueAST>(res));

}