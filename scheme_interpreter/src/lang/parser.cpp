#include <lang/parser.hpp>
#include <cassert>

using namespace lexer;

namespace parser
{

	ASTExpr::ASTExpr() {}

	ASTExpr::ASTExpr(ASTExpr&& other) noexcept
	{
		type = other.type;

		switch (other.type)
		{
		case Type::INVALID:
			break;
		case Type::LIST:
			new (&children) std::vector<ASTExpr>(std::move(other.children));
			break;
		case Type::ATOM:
			new (&leaf) Token(std::move(other.leaf));
			break;
		default:
			assert(false);
		}
	}

	ASTExpr::~ASTExpr()
	{
		switch (type)
		{
		case Type::INVALID:
			break;
		case Type::LIST:
			children.~vector();
			break;
		case Type::ATOM:
			leaf.~Token();
			break;
		default:
			assert(false);
		}
	}

	ASTExpr& ASTExpr::operator= (ASTExpr&& other) noexcept
	{
		if (this == &other) return *this;

		this->~ASTExpr();
		new (this) ASTExpr(std::move(other));
		return *this;
	}

	bool operator== (const ASTExpr& lhs, const ASTExpr& rhs) noexcept
	{
		if (lhs.type == rhs.type)
		{
			switch (rhs.type)
			{
			case ASTExpr::Type::INVALID:
				return true;
			case ASTExpr::Type::LIST:
				return (lhs.children == rhs.children);
			case ASTExpr::Type::ATOM:
				return (lhs.leaf == rhs.leaf);
			default:
				assert(false);
			}
		}
		return false;
	}


	ASTExpr::ASTExpr(Type type) : type(type) {}

	template<>
	ASTExpr make_astexpr<ASTExpr::Type::INVALID>()
	{
		return ASTExpr();
	}

	template<>
	ASTExpr make_astexpr<ASTExpr::Type::LIST>()
	{
		ASTExpr expr(ASTExpr::Type::LIST);
		new (&expr.children) std::vector<ASTExpr>();
		return expr;
	}

	template<>
	ASTExpr make_astexpr<ASTExpr::Type::ATOM>()
	{
		ASTExpr expr(ASTExpr::Type::ATOM);
		new (&expr.leaf) Token();
		return expr;
	}

	using Iter = std::vector<Token>::iterator;

	ASTExpr construct_ast(std::vector<Token>&& token_arr)
	{
		if (token_arr[0].type != Token::Type::LRB)
		{
			std::cout << "Program must begin with an opening bracket" << std::endl;
			// Handle errors here...
		}

		ASTExpr ast = std::move(parse_expr(std::next(token_arr.begin()), std::prev(token_arr.end())));
		return ast;
	}

	ASTExpr parse_expr(Iter start, Iter end)
	{
		ASTExpr expr = make_astexpr<ASTExpr::Type::LIST>();
		// Break once the end of the expression is encountered
		while (start != end)
		{	
			if (start->type == Token::Type::LRB)	// New expression encountered
			{
				Iter next_end = start;
				while (next_end->type != Token::Type::RRB)
				{
					next_end = std::next(next_end);	// Find closing bracket
				}
				expr.children.push_back(parse_expr(std::next(start), next_end));
				start = next_end;
			}
			else if (start->type == Token::Type::SYMBOL || start->type == Token::Type::INT || start->type == Token::Type::FLOAT || start->type == Token::Type::STRING)
			{
				ASTExpr sub_expr = make_astexpr<ASTExpr::Type::ATOM>();
				sub_expr.leaf = std::move(*start);
				expr.children.push_back(std::move(sub_expr));
				start = std::next(start);
			}
			else
			{
				start = std::next(start);
			}
		}
		// End of expression is encountered
		return expr;
	}

	void print_ast_expr(const ASTExpr& expr, int level)
	{
		if (expr.type == ASTExpr::Type::LIST)
		{
			auto buff = "   ";
			level += 1;
			std::cout << "()\n";
			for (int j = 0; j < level; j++) std::cout << buff;
			std::cout << "|__";

			for (int i = 0; i < expr.children.size(); i++)
			{
				if (i != 0)
				{
					for (int j = 0; j < level; j++) std::cout << buff;
					std::cout << "|--";
				}
				print_ast_expr(expr.children[i], level);
				std::cout << "\n";
			}
		}
		else
		{
			switch (expr.leaf.type)
			{
			case Token::Type::SYMBOL:
				std::cout << expr.leaf.symbol << " ";
				break;
			case Token::Type::INT:
				std::cout << expr.leaf.i_value << " ";
				break;
			case Token::Type::FLOAT:
				std::cout << expr.leaf.f_value << " ";
				break;
			}
			
		}
	}

}