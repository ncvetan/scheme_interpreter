#include <lang/lexer.hpp>
#include <cassert>

namespace lexer
{
	Token::Token() {}

	Token::Token(Token&& other) noexcept
	{
		type = other.type;

		switch (other.type)
		{
		case Type::INVALID:
			break;
		case Type::LRB:
			break;
		case Type::RRB:
			break;
		case Type::SYMBOL:
			new (&symbol) std::string(std::move(other.symbol));
			break;
		case Type::STRING:
			new (&symbol) std::string(std::move(other.symbol));
			break;
		case Type::INT:
			i_value = other.i_value;
			break;
		case Type::FLOAT:
			f_value = other.f_value;
			break;
		default:
			assert(false);
		}
	}

	Token::~Token()
	{
		switch (type)
		{
		case Type::INVALID:
			break;
		case Type::LRB:
			break;
		case Type::RRB:
			break;
		case Type::SYMBOL:
			symbol.~basic_string();
			break;
		case Type::STRING:
			symbol.~basic_string();
			break;
		case Type::INT:
			break;
		case Type::FLOAT:
			break;
		default:
			assert(false);
		}
	}

	Token& Token::operator= (Token&& other) noexcept
	{
		if (this == &other) return *this;

		this->~Token();
		new (this) Token(std::move(other));
		return *this;
	}

	bool operator== (const Token& lhs, const Token& rhs) noexcept
	{
		if (lhs.type == rhs.type)
		{
			switch (rhs.type)
			{
			case Token::Type::INVALID:
				return true;
			case Token::Type::LRB:
				return true;
			case Token::Type::RRB:
				return true;
			case Token::Type::SYMBOL:
				return lhs.symbol == rhs.symbol;
			case Token::Type::STRING:
				return lhs.symbol == rhs.symbol;
			case Token::Type::INT:
				return lhs.i_value == rhs.i_value;
			case Token::Type::FLOAT:
				return lhs.f_value == rhs.f_value;
			default:
				assert(false);
			}
		}
		return false;
	}

	bool operator!= (const Token& lhs, const Token& rhs) noexcept
	{
		if (lhs.type == rhs.type)
		{
			switch (rhs.type)
			{
			case Token::Type::INVALID:
				return false;
			case Token::Type::LRB:
				return false;
			case Token::Type::RRB:
				return false;
			case Token::Type::SYMBOL:
				return lhs.symbol != rhs.symbol;
			case Token::Type::STRING:
				return lhs.symbol != rhs.symbol;
			case Token::Type::INT:
				return lhs.i_value != rhs.i_value;
			case Token::Type::FLOAT:
				return lhs.f_value != rhs.f_value;
			default:
				assert(false);
			}
		}
		return true;
	}

	Token::Token(Token::Type type) : type(type) {}

	Token make_token(Token::Type type)
	{
		Token token(type);

		switch (type)
		{
		case Token::Type::INVALID:
			break;
		case Token::Type::LRB:
			break;
		case Token::Type::RRB:
			break;
		case Token::Type::SYMBOL:
			new (&token.symbol) std::string();
			break;
		case Token::Type::STRING:
			new (&token.symbol) std::string();
			break;
		case Token::Type::INT:
			token.i_value = 0;
			break;
		case Token::Type::FLOAT:
			token.f_value = 0;
			break;
		default:
			assert(false);
		}
		return token;
	}

	template<>
	Token make_token<Token::Type::INVALID>()
	{
		Token token;
		return token;
	}

	template<>
	Token make_token<Token::Type::LRB>()
	{
		Token token{ Token::Type::LRB };
		return token;
	}

	template<>
	Token make_token<Token::Type::RRB>()
	{
		Token token{ Token::Type::RRB };
		return token;
	}

	template<>
	Token make_token<Token::Type::SYMBOL>()
	{
		Token token{ Token::Type::SYMBOL };
		new (&token.symbol) std::string();
		return token;
	}

	template<>
	Token make_token<Token::Type::STRING>()
	{
		Token token{ Token::Type::STRING };
		new (&token.symbol) std::string();
		return token;
	}

	template<>
	Token make_token<Token::Type::INT>()
	{
		Token token{ Token::Type::INT };
		token.i_value = 0;
		return token;
	}

	template<>
	Token make_token<Token::Type::FLOAT>()
	{
		Token token{ Token::Type::FLOAT };
		token.f_value = 0;
		return token;
	}

	Token create_token_from_string(const std::string& token_text)
	{
		int i = 0;
		while (token_text[i] == '-') i++;
		if (isdigit(token_text[i]))
		{
			if (token_text.find('.') == std::string::npos)
			{
				Token token = std::move(make_token<Token::Type::INT>());
				token.i_value = std::stoi(token_text);
				return token;
			}
			Token token = std::move(make_token<Token::Type::FLOAT>());
			token.f_value = std::stod(token_text);
			return token;
		}
		Token token = std::move(make_token<Token::Type::SYMBOL>());
		token.symbol = token_text;
		return token;
	}

	std::vector<Token> tokenize(const std::string& raw_text)
	{
		std::vector<Token> output;

		for (int i = 0; i < raw_text.length(); ++i)
		{
			auto curr = raw_text.at(i);

			switch (curr)
			{
			case ' ':
				break;
			case '\t':
				break;
			case '\n':
				break;
			case '(':
				output.push_back(std::move(make_token<Token::Type::LRB>()));
				break;
			case ')':
				output.push_back(std::move(make_token<Token::Type::RRB>()));
				break;
			case '"':
			{
				i++;
				int start = i;
				bool done = false;

				while (i < raw_text.length() && !done) {

					switch (raw_text.at(i))
					{
					case '"':
					{
						auto token = std::move(make_token<Token::Type::STRING>());
						token.symbol = raw_text.substr(start, i - start);
						output.push_back(std::move(token));
						done = true;
						break;
					}
					default:
						i++;
					}
				}
				break;
			}
			default:
			{
				int start = i;
				bool done = false;

				while (i < raw_text.length() && !done) {

					switch (raw_text.at(i))
					{
					case ' ':
					{
						output.push_back(create_token_from_string(raw_text.substr(start, i - start)));
						done = true;
						i--;
						break;
					}
					case '(':
					{
						output.push_back(create_token_from_string(raw_text.substr(start, i - start)));
						done = true;
						i--;
						break;
					}
					case ')':
					{
						output.push_back(create_token_from_string(raw_text.substr(start, i - start)));
						done = true;
						i--;
						break;
					}
					default:
						i++;
					}
				}
			}
			}
		}
		return output;
	}
}
