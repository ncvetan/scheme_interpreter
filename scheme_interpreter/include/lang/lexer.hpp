#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace lexer
{

	struct Token
	{

		enum class Type {
			INVALID,
			LRB,
			RRB,
			SYMBOL,
			STRING,
			INT,
			FLOAT
		};

		Type type = Type::INVALID;

		union 
		{
			std::string symbol;
			int i_value;
			double f_value; // TEST THIS NEXT!
		};

		Token();

		Token(Token&& other) noexcept;

		Token(const Token& other) = delete;

		~Token();

		Token& operator= (const Token& other) = delete;

		Token& operator= (Token&& other) noexcept;

		friend bool operator== (const Token& lhs, const Token& rhs) noexcept;

	private:

		Token(Type type);

		friend Token make_token(Token::Type type);

		template<Token::Type type>
		friend Token make_token();
	};

	bool operator== (const Token& lhs, const Token& rhs) noexcept;
	bool operator!= (const Token& lhs, const Token& rhs) noexcept;

	/**
	 * Used to instantiate a token
	 *
	 * @tparam type: enum type representing the type of the token (e.g., right round bracket)
	 * @returns the created token
	*/
	Token make_token(Token::Type type);

	/**
	 * Used to instantiate a token
	 *
	 * @tparam type: enum type representing the type of the token (e.g., right round bracket)
	 * @returns the created token
	*/
	template<Token::Type type>
	Token make_token();

	/**
	 * Creates and determines the type of a token from a string
	 *
	 * @param token_text: string whose text will be used to create the token
	 * @returns sum of `values`, or 0.0 if `values` is empty.
	 */
	Token create_token_from_string(const std::string& token_text);

	/**
	 * Create a list of program tokens inside a vector to be parsed into an AST
	 *
	 * @param raw_text: Text containing Lisp code to be tokenized
	 * @returns a vector containing tokens
	 */
	std::vector<Token> tokenize(const std::string& raw_text);
}