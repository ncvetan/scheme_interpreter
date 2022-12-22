#include <lang/evaluate.hpp>
#include <cassert>

using namespace environment;

namespace eval
{
	Environment env = environment::Environment();

	void print_variable(std::unique_ptr<Variable> var)
	{
		switch (var->type)
		{
		case Variable::Type::BOOL:
			std::cout << static_cast<Bool*>(var.get())->value << std::endl;
			break;
		case Variable::Type::INT:
			std::cout << static_cast<Int*>(var.get())->value << std::endl;
			break;
		case Variable::Type::FLOAT:
			std::cout << static_cast<Float*>(var.get())->value << std::endl;
			break;
		case Variable::Type::STRING:
			std::cout << static_cast<String*>(var.get())->value << std::endl;
			break;
		default:
			std::cout << "Invalid result encountered" << std::endl;
		}
	}

	std::unique_ptr<Variable> define(std::vector<ASTExpr>* args)
	{
		if (args->size() != 2)
		{
			std::cout << "Definition expects two arguments, a name and a value" << std::endl;
			return nullptr;
		}
		auto key = eval::eval_expr(&(*args)[0]);
		auto value = eval::eval_expr(&(*args)[1]);

		if (key->type != Variable::Type::SYMBOL)
		{
			std::cout << "Define expects a unique symbol as the first argument, received: " << get_var_type_as_string((*key)) << std::endl;
			return nullptr;
		}

		env.env_map.insert({static_cast<Symbol*>(key.get())->value, value->copy()});
		return nullptr;
	}

	std::unique_ptr<Variable> eval_expr_atom(const Token& tk)
	{
		switch (tk.type)
		{
		case Token::Type::INT:
			return std::make_unique<Int>(Int(tk.i_value));
		case Token::Type::FLOAT:
			return std::make_unique<Float>(Float(tk.f_value));
		case Token::Type::STRING:
			return std::make_unique<String>(String(tk.symbol));
		case Token::Type::SYMBOL:
		{
			if (tk.symbol == "define")
			{
				return std::make_unique<Define>(Define());
			}
			if (tk.symbol == "if")
			{
				return std::make_unique<If>(If());
			}
			auto it = env.env_map.find(tk.symbol);
			if (it == env.env_map.end())
			{
				// Symbol is not in the current environment
				return std::make_unique<Symbol>(Symbol(tk.symbol));
			}
			return it->second->copy();
		}
		}
		return nullptr;
	}

	std::unique_ptr<Variable> eval_expr_list(std::vector<ASTExpr>* exprs)
	{
		if ((*exprs).size() == 0)
		{
			std::cout << "Empty list encountered" << std::endl;
			return nullptr;
		}

		if ((*exprs)[0].type != ASTExpr::Type::ATOM)
		{
			std::cout << "List must begin with a symbol" << std::endl;
			return nullptr;
		}

		std::unique_ptr<Variable> fn = eval_expr_atom((*exprs)[0].leaf);

		std::vector<ASTExpr> args;

		for (int i = 1; i < (*exprs).size(); ++i) args.push_back(std::move((*exprs)[i]));

		if (fn->type == Variable::Type::DEFINITION)
		{
			return define(&args);
		}
		if (fn->type == Variable::Type::CONDITIONAL || fn->type == Variable::Type::PROCEDURE)
		{
			return fn->call(&args);
		}
		
		std::cout << "Unknown argument encountered in first list position: " << static_cast<Symbol*>(fn.get())->value << std::endl;
		return nullptr;
	}

	std::unique_ptr<Variable> eval_expr(ASTExpr* expr)
	{
		switch ((*expr).type)
		{
		case ASTExpr::Type::ATOM:
			return eval_expr_atom((*expr).leaf);
		case ASTExpr::Type::LIST:
			return eval_expr_list(&(*expr).children);
		default:
			std::cout << "Invalid ASTExpr encountered" << std::endl;
			return nullptr;
		}
	}

	void eval_file(ASTExpr* expr)
	{
		std::cout << "NOT IMPLEMENTED: Evaluating a file" << std::endl;
	}

	void repl()
	{
		std::cout << "A Scheme interpreter by @ncvetan\nEnter 'exit' to close the program\n";
		while (true)
		{
			std::string line;
			std::cout << ">> ";
			std::getline(std::cin, line);
			if (line == "exit") break;
			
			auto ast = construct_ast(std::move(tokenize(line)));
			auto result = eval_expr(&ast);

			if (result != nullptr)
			{
				print_variable(std::move(result));
			}
		}
	}
}
