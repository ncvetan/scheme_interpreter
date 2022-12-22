#pragma once

#include <lang/lexer.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace lexer;

namespace parser
{

	struct ASTExpr
	{
		enum class Type
		{
			INVALID,
			LIST,
			ATOM
		};

		Type type = Type::INVALID;

		union
		{
			Token leaf;
			std::vector<ASTExpr> children;
		};

		ASTExpr();

		ASTExpr(ASTExpr&& other) noexcept;

		ASTExpr(const ASTExpr& other) = delete;

		~ASTExpr();

		ASTExpr& operator= (const ASTExpr& other) = delete;

		ASTExpr& operator= (ASTExpr&& other) noexcept;

		friend bool operator== (const ASTExpr& lhs, const ASTExpr& rhs) noexcept;

	private:

		ASTExpr(Type type);

		template<ASTExpr::Type type>
		friend ASTExpr make_astexpr();
	};

	bool operator== (const ASTExpr& lhs, const ASTExpr& rhs) noexcept;

	ASTExpr make_astexpr(ASTExpr::Type type);

	template<ASTExpr::Type type>
	ASTExpr make_astexpr();

	using Iter = std::vector<Token>::iterator;

	/**
	 * Used to construct an abstract syntax tree by using recursive descent
	 *
	 * @param token_arr: vector containing Tokens returned from the tokenize function  
	 * @returns An ASTExpr containing the root node of the tree
	 */
	ASTExpr construct_ast(std::vector<Token>&& token_arr);

	/**
	 * Used to construct an expression using recursive descent
	 *
	 * @param start: iterator of the token for the opening bracket of an expression
	 * @param end: iterator of the token for the closing bracket of an expression
	 * @returns An ASTExpr representing the given expression of tokens
	 */
	ASTExpr parse_expr(Iter start, Iter end);

	/**
	* Very rough function for printing an AST, used for debugging purposes
	*/
	void print_ast_expr(const ASTExpr& expr, int level);
}