#include "../include/lang/evaluate.hpp"
#include <gtest/gtest.h>

using namespace eval;
using namespace environment;
using namespace parser;
using namespace lexer;

// TESTING THAT A TOKEN CAN BE PROPERLY CONSTRUCTED FROM A GIVEN STRING
// ====================================================================
TEST(LexerTests, create_token_from_str_case1) {
	lexer::Token token = lexer::make_token<lexer::Token::Type::INT>();
	token.i_value = 100;
	EXPECT_EQ(std::move(lexer::create_token_from_string("100")), token);
}

TEST(LexerTests, create_token_from_str_case2) {
	lexer::Token token = lexer::make_token<lexer::Token::Type::FLOAT>();
	token.f_value = 100.12;
	auto expected = lexer::create_token_from_string("100.12");
	EXPECT_EQ(std::move(expected), token);
}

TEST(LexerTests, create_token_from_str_case3) {
	lexer::Token token = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token.symbol = "testing";
	EXPECT_EQ(std::move(lexer::create_token_from_string("testing")), token);
}

TEST(LexerTests, create_token_from_str_case4) {
	lexer::Token token = lexer::make_token<lexer::Token::Type::INT>();
	token.i_value = 101;
	EXPECT_NE(std::move(lexer::create_token_from_string("100")), token);
}

TEST(LexerTests, create_token_from_str_case5) {
	lexer::Token token = lexer::make_token<lexer::Token::Type::FLOAT>();
	token.f_value = 100;
	EXPECT_NE(std::move(lexer::create_token_from_string("100")), token);
}

TEST(LexerTests, create_token_from_str_case6) {
	lexer::Token token = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token.symbol = "testing_ne";
	EXPECT_NE(std::move(lexer::create_token_from_string("testing")), token);
}

// TESTING THAT AN ASTEXPR CAN BE PROPERLY CONSTRUCTED FROM A TOKEN ARRAY
// ======================================================================
TEST(ParserTests, construct_ast_case1) {

	std::vector<Token> token_vector = std::move(tokenize("(+ 53 56)"));
	parser::ASTExpr expr = parser::construct_ast(std::move(token_vector));

	parser::ASTExpr astexpr = parser::make_astexpr<parser::ASTExpr::Type::LIST>();
	
	lexer::Token token1 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token1.symbol = "+";
	parser::ASTExpr astexpr1 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr1.leaf = std::move(token1);

	lexer::Token token2 = lexer::make_token<lexer::Token::Type::INT>();
	token2.i_value = 53;
	parser::ASTExpr astexpr2 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr2.leaf = std::move(token2);

	lexer::Token token3 = lexer::make_token<lexer::Token::Type::INT>();
	token3.i_value = 56;
	parser::ASTExpr astexpr3 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr3.leaf = std::move(token3);
	
	astexpr.children.push_back(std::move(astexpr1));
	astexpr.children.push_back(std::move(astexpr2));
	astexpr.children.push_back(std::move(astexpr3));

	EXPECT_EQ(expr, astexpr);
}

TEST(ParserTests, construct_ast_case2) {

	std::vector<Token> token_vector = std::move(tokenize("(+ 53 (- 20 56))"));
	parser::ASTExpr expr = parser::construct_ast(std::move(token_vector));

	parser::ASTExpr astexpr = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token1 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token1.symbol = "+";
	parser::ASTExpr astexpr1 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr1.leaf = std::move(token1);

	lexer::Token token2 = lexer::make_token<lexer::Token::Type::INT>();
	token2.i_value = 53;
	parser::ASTExpr astexpr2 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr2.leaf = std::move(token2);

	parser::ASTExpr sub_astexpr = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token3 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token3.symbol = "-";
	parser::ASTExpr astexpr3 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr3.leaf = std::move(token3);

	lexer::Token token4 = lexer::make_token<lexer::Token::Type::INT>();
	token4.i_value = 20;
	parser::ASTExpr astexpr4 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr4.leaf = std::move(token4);

	lexer::Token token5 = lexer::make_token<lexer::Token::Type::INT>();
	token5.i_value = 56;
	parser::ASTExpr astexpr5 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr5.leaf = std::move(token5);

	sub_astexpr.children.push_back(std::move(astexpr3));
	sub_astexpr.children.push_back(std::move(astexpr4));
	sub_astexpr.children.push_back(std::move(astexpr5));

	astexpr.children.push_back(std::move(astexpr1));
	astexpr.children.push_back(std::move(astexpr2));
	astexpr.children.push_back(std::move(sub_astexpr));

	EXPECT_EQ(expr, astexpr);
}

TEST(ParserTests, construct_ast_case3) {

	std::vector<Token> token_vector = std::move(tokenize("(+ 53.34 (- 20 56.2))"));
	parser::ASTExpr expr = parser::construct_ast(std::move(token_vector));

	parser::ASTExpr astexpr = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token1 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token1.symbol = "+";
	parser::ASTExpr astexpr1 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr1.leaf = std::move(token1);

	lexer::Token token2 = lexer::make_token<lexer::Token::Type::FLOAT>();
	token2.f_value = 53.34;
	parser::ASTExpr astexpr2 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr2.leaf = std::move(token2);

	parser::ASTExpr sub_astexpr = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token3 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token3.symbol = "-";
	parser::ASTExpr astexpr3 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr3.leaf = std::move(token3);

	lexer::Token token4 = lexer::make_token<lexer::Token::Type::INT>();
	token4.i_value = 20;
	parser::ASTExpr astexpr4 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr4.leaf = std::move(token4);

	lexer::Token token5 = lexer::make_token<lexer::Token::Type::FLOAT>();
	token5.f_value = 56.2;
	parser::ASTExpr astexpr5 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr5.leaf = std::move(token5);

	sub_astexpr.children.push_back(std::move(astexpr3));
	sub_astexpr.children.push_back(std::move(astexpr4));
	sub_astexpr.children.push_back(std::move(astexpr5));

	astexpr.children.push_back(std::move(astexpr1));
	astexpr.children.push_back(std::move(astexpr2));
	astexpr.children.push_back(std::move(sub_astexpr));

	EXPECT_EQ(expr, astexpr);
}

TEST(ParserTests, construct_ast_case4) {

	std::vector<Token> token_vector = std::move(tokenize("(func (- 20 56)(+ 42 31))"));
	parser::ASTExpr expr = parser::construct_ast(std::move(token_vector));

	parser::ASTExpr astexpr = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token1 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token1.symbol = "func";
	parser::ASTExpr astexpr1 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr1.leaf = std::move(token1);

	parser::ASTExpr sub_astexpr1 = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token2 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token2.symbol = "-";
	parser::ASTExpr astexpr2 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr2.leaf = std::move(token2);

	lexer::Token token3 = lexer::make_token<lexer::Token::Type::INT>();
	token3.i_value = 20;
	parser::ASTExpr astexpr3 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr3.leaf = std::move(token3);

	lexer::Token token4 = lexer::make_token<lexer::Token::Type::INT>();
	token4.i_value = 56;
	parser::ASTExpr astexpr4 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr4.leaf = std::move(token4);

	sub_astexpr1.children.push_back(std::move(astexpr2));
	sub_astexpr1.children.push_back(std::move(astexpr3));
	sub_astexpr1.children.push_back(std::move(astexpr4));

	parser::ASTExpr sub_astexpr2 = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token5 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token5.symbol = "+";
	parser::ASTExpr astexpr5 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr5.leaf = std::move(token5);

	lexer::Token token6 = lexer::make_token<lexer::Token::Type::INT>();
	token6.i_value = 42;
	parser::ASTExpr astexpr6 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr6.leaf = std::move(token6);

	lexer::Token token7 = lexer::make_token<lexer::Token::Type::INT>();
	token7.i_value = 31;
	parser::ASTExpr astexpr7 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr7.leaf = std::move(token7);

	sub_astexpr2.children.push_back(std::move(astexpr5));
	sub_astexpr2.children.push_back(std::move(astexpr6));
	sub_astexpr2.children.push_back(std::move(astexpr7));

	astexpr.children.push_back(std::move(astexpr1));
	astexpr.children.push_back(std::move(sub_astexpr1));
	astexpr.children.push_back(std::move(sub_astexpr2));

	EXPECT_EQ(expr, astexpr);
}

TEST(ParserTests, construct_ast_case5) {

	std::vector<Token> token_vector = std::move(tokenize("(+ \"TESTSTR1\" \"TESTSTR2\")"));
	parser::ASTExpr expr = parser::construct_ast(std::move(token_vector));

	parser::ASTExpr astexpr = parser::make_astexpr<parser::ASTExpr::Type::LIST>();

	lexer::Token token1 = lexer::make_token<lexer::Token::Type::SYMBOL>();
	token1.symbol = "+";
	parser::ASTExpr astexpr1 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr1.leaf = std::move(token1);

	lexer::Token token2 = lexer::make_token<lexer::Token::Type::STRING>();
	token2.symbol = "TESTSTR1";
	parser::ASTExpr astexpr2 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr2.leaf = std::move(token2);

	lexer::Token token3 = lexer::make_token<lexer::Token::Type::STRING>();
	token3.symbol = "TESTSTR2";
	parser::ASTExpr astexpr3 = parser::make_astexpr<parser::ASTExpr::Type::ATOM>();
	astexpr3.leaf = std::move(token3);

	astexpr.children.push_back(std::move(astexpr1));
	astexpr.children.push_back(std::move(astexpr2));
	astexpr.children.push_back(std::move(astexpr3));

	EXPECT_EQ(expr, astexpr);
}


// TESTING THE EVALUATOR AND STANDARD PROCEDURES

TEST(EvalTests, eval_expr_case1) {

	auto ast = construct_ast(std::move(tokenize("(+ 54 53)")));
	auto res = eval::eval_expr(&ast);

	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Int>(environment::Int(107));

	EXPECT_EQ(*res.get(), *expected.get());
}

TEST(EvalTests, eval_expr_case2) {

	auto ast = construct_ast(std::move(tokenize("(+ -20 10)")));
	auto res = eval::eval_expr(&ast);
	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Int>(environment::Int(-10));

	EXPECT_EQ(*res.get(), *expected.get());
}

TEST(EvalTests, eval_expr_case3) {

	auto ast = construct_ast(std::move(tokenize("(+ (- 30 20) (+ 15 10))")));
	auto res = eval::eval_expr(&ast);

	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Int>(environment::Int(35));

	EXPECT_EQ(*res.get(), *expected.get());
}

TEST(EvalTests, eval_expr_case4) {

	auto ast = construct_ast(std::move(tokenize("(+ -30.25 20)")));
	auto res = eval::eval_expr(&ast);

	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Float>(environment::Float(-10.25));

	EXPECT_EQ(*res.get(), *expected.get());
}

TEST(EvalTests, eval_expr_case5) {

	auto ast = construct_ast(std::move(tokenize("(* 5 10 5)")));
	auto res = eval::eval_expr(&ast);

	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Int>(environment::Int(250));

	EXPECT_EQ(*res.get(), *expected.get());
}

TEST(EvalTests, eval_expr_case6) {

	auto ast = construct_ast(std::move(tokenize("(* 10 (- 15 5) (/ 10 2) (/ 15 2.5))")));
	auto res = eval::eval_expr(&ast);

	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Float>(environment::Float(3000));

	EXPECT_EQ(*res.get(), *expected.get());
}

TEST(EvalTests, eval_expr_case7) {

	auto ast = construct_ast(std::move(tokenize("(> 10 5)")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, true);
}

TEST(EvalTests, eval_expr_case8) {

	auto ast = construct_ast(std::move(tokenize("(> 5 10)")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, false);
}

TEST(EvalTests, eval_expr_case9) {

	auto ast = construct_ast(std::move(tokenize("(> 5 5)")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, false);
}

TEST(EvalTests, eval_expr_case10) {

	auto ast = construct_ast(std::move(tokenize("(>= 5 5)")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, true);
}


TEST(EvalTests, eval_expr_case11) {

	auto ast = construct_ast(std::move(tokenize("(= 5 5)")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, true);
}

TEST(EvalTests, eval_expr_case12) {

	auto ast = construct_ast(std::move(tokenize("(= 6 5)")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, false);
}

TEST(EvalTests, eval_expr_case13) {

	auto ast = construct_ast(std::move(tokenize("(= \"TESTSTR1\" \"TESTSTR2\")")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, false);
}

TEST(EvalTests, eval_expr_case14) {

	auto ast = construct_ast(std::move(tokenize("(= \"TESTSTR1\" \"TESTSTR1\")")));
	auto expr = eval::eval_expr(&ast);
	auto res = static_cast<Bool*>(expr.get())->value;

	EXPECT_EQ(res, true);
}

TEST(EvalTests, eval_expr_case15) {

	auto ast = construct_ast(std::move(tokenize("(abs -15)")));
	auto res = eval::eval_expr(&ast);
	
	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Int>(environment::Int(15));

	EXPECT_EQ(*res.get(), *expected.get());
}

TEST(EvalTests, eval_expr_case16) {

	auto ast = construct_ast(std::move(tokenize("(abs -15.05)")));
	auto res = eval::eval_expr(&ast);

	std::unique_ptr<environment::Variable> expected = std::make_unique<environment::Float>(environment::Float(15.05));

	EXPECT_EQ(*res.get(), *expected.get());
}