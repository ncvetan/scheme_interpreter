#pragma once
#include <lang/env.hpp>
#include <functional>
#include <fstream>

using namespace environment;

namespace eval
{
	/**
	 * Evaluates an atom
	 *
	 * @param exprs: token containing the value of the atom
	 * @returns a pointer to a Variable containing the value of the atom
	*/
	std::unique_ptr<environment::Variable> eval_expr_atom(const Token& tk);

	/**
	 * Evaluates a list expression by calling a procedure based on the first value, and using the rest as arguments
	 *
	 * @param exprs: vector of ASTExprs to be evaluated
	 * @returns a pointer to a Variable containing the result of the list evaluation
	*/
	std::unique_ptr<environment::Variable> eval_expr_list(std::vector<ASTExpr>* exprs);

	/**
	 * Evaluates an expression depending on its type
	 *
	 * @param expr: expression to evaluate
	 * @returns a pointer to a Variable that either contains a value or performs a procedure
	*/
	std::unique_ptr<environment::Variable> eval_expr(ASTExpr* expr);

	/**
	 * Associate a keyword with an expression, which will be added to the environment for later usage in the program
	 *
	 * @param args: vector of ASTExprs, where the first one should be an atom containing a symbol token not present in the environment
	 * @returns a pointer to a Variable containing the expression that was added to the environment
	*/
	std::unique_ptr<Variable> define(std::vector<ASTExpr>* args);

	/**
	 * Evaluates a Scheme file passed in from the command line
	 *
	*/
	void eval_file(ASTExpr* expr);

	/**
	 * Function for beginning a read-eval-print loop, taking user input line-by-line, evaluating it, and printing the result to stdout
	 *
	*/
	void repl();
}