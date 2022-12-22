#include <lang/env.hpp>
#include <lang/evaluate.hpp>
#include <cassert>

namespace environment
{

	Variable::Variable(Type type) : type(type) {}

	bool operator== (const Variable& lhs, const Variable& rhs) noexcept
	{
		if (lhs.type == rhs.type)
		{
			switch (lhs.type)
			{
			case Variable::Type::FLOAT:
				return(static_cast<const Float&>(lhs).value == static_cast<const Float&>(rhs).value);
			case Variable::Type::INT:
				return(static_cast<const Int&>(lhs).value == static_cast<const Int&>(rhs).value);
			}
		}
		return false;
	}

	Int::Int(int value) : VarCopy(Variable::Type::INT), value(value) {}

	std::unique_ptr<Variable> Int::call(std::vector<ASTExpr>* args)
	{
		std::cout << "Integer variable is not callable" << std::endl;
		return nullptr;
	}

	Float::Float(double value) : VarCopy(Variable::Type::FLOAT), value(value) {}

	std::unique_ptr<Variable> Float::call(std::vector<ASTExpr>* args)
	{
		std::cout << "Float variable is not callable" << std::endl;
		return nullptr;
	}

	String::String(std::string value) : VarCopy(Variable::Type::STRING), value(value) {}

	std::unique_ptr<Variable> String::call(std::vector<ASTExpr>* args)
	{
		std::cout << "String variable is not callable" << std::endl;
		return nullptr;
	}

	Bool::Bool(bool value) : VarCopy(Variable::Type::BOOL), value(value) {}

	std::unique_ptr<Variable> Bool::call(std::vector<ASTExpr>* args)
	{
		std::cout << "Boolean variable is not callable" << std::endl;
		return nullptr;
	}

	Symbol::Symbol(std::string value) : VarCopy(Variable::Type::SYMBOL), value(value) {}

	std::unique_ptr<Variable> Symbol::call(std::vector<ASTExpr>* args)
	{
		std::cout << "Symbol variable is not callable" << std::endl;
		return nullptr;
	}

	List::List(std::vector<std::unique_ptr<Variable>> values) : Variable(Variable::Type::LIST), values(std::move(values)) {}

	std::unique_ptr<Variable> List::call(std::vector<ASTExpr>* args)
	{
		std::cout << "List variable is not callable" << std::endl;
		return nullptr;
	}

	std::vector<std::unique_ptr<Variable>> get_variable_args(std::vector<ASTExpr>* args)
	{
		std::vector<std::unique_ptr<Variable>> var_args;
		for (auto& arg : *args)
		{
			var_args.push_back(std::move(eval::eval_expr(&arg)));
		}
		return var_args;
	}

	std::string get_result_type(const std::vector<std::unique_ptr<Variable>>& args)
	{
		std::string res_type{""};

		for (int i = 0; i < args.size(); i++)
		{
			if (args[i]->type == Variable::Type::INT)
			{
				if (res_type != "float") res_type = "int";
			}
			else if (args[i]->type == Variable::Type::FLOAT)
			{
				res_type = "float";
			}
			else
			{
				return get_var_type_as_string(*args[i].get());
			}
		}
		return res_type;
	}

	std::string get_var_type_as_string(const Variable& var)
	{
		switch (var.type)
		{
		case Variable::Type::PROCEDURE:
			return "Procedure";
		case Variable::Type::BOOL:
			return "Boolean";
		case Variable::Type::FLOAT:
			return "Float";
		case Variable::Type::INT:
			return "Int";
		case Variable::Type::STRING:
			return "String";
		case Variable::Type::INVALID:
			return "Invalid";
		case Variable::Type::DEFINITION:
			return "Definition";
		case Variable::Type::CONDITIONAL:
			return "Conditional";
		case Variable::Type::SYMBOL:
			return "Symbol";
		default:
			return "Unknown";
		}
	}

	Add::Add() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Add::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() < 2)
		{
			std::cout << "Add procedure expects at least 2 arguments, received: " << (*args).size() << std::endl;
			return nullptr;
		}

		auto var_args = get_variable_args(args);
		auto res_type = get_result_type(var_args);
		
		if (res_type == "int")
		{ 
			int res{ 0 };
			for (size_t i = 0; i < var_args.size(); i++)
			{
				res += static_cast<Int*>(var_args[i].get())->value;
			}
			return std::make_unique<Int>(res);
		}
		else if (res_type == "float")
		{
			double res{ 0 };
			for (size_t i = 0; i < var_args.size(); i++)
			{
				if (var_args[i]->type == Type::INT) res += static_cast<Int*>(var_args[i].get())->value;
				else if (var_args[i]->type == Type::FLOAT) res += static_cast<Float*>(var_args[i].get())->value;
			}
			return std::make_unique<Float>(res);
		}
		std::cout << "Invalid argument to add procedure, expected int or float and received: " << res_type << std::endl;
		return nullptr;
	}

	Subtract::Subtract() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Subtract::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() < 2)
		{
			std::cout << "Subtract procedure expects at least 2 arguments, received: " << (*args).size() << std::endl;
			return nullptr;
		}

		auto var_args = get_variable_args(args);
		auto res_type = get_result_type(var_args);

		if (res_type == "int")
		{
			int res = static_cast<Int*>(var_args[0].get())->value;
			for (size_t i = 1; i < var_args.size(); i++)
			{
				res -= static_cast<Int*>(var_args[i].get())->value;
			}
			return std::make_unique<Int>(res);
		}
		else if (res_type == "float")
		{

			double res{ 0 };
			if (var_args[0]->type == Type::INT) res = static_cast<Int*>(var_args[0].get())->value;
			else if (var_args[0]->type == Type::FLOAT) res = static_cast<Float*>(var_args[0].get())->value;

			for (size_t i = 1; i < var_args.size(); i++)
			{
				if (var_args[i]->type == Type::INT) res -= static_cast<Int*>(var_args[i].get())->value;
				else if (var_args[i]->type == Type::FLOAT) res -= static_cast<Float*>(var_args[i].get())->value;
			}
			return std::make_unique<Float>(res);
		}
		std::cout << "Invalid argument to subtract procedure, expected int or float and received: " << res_type << std::endl;
		return nullptr;
	}

	Multiply::Multiply() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Multiply::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() < 2)
		{
			std::cout << "Multipy procedure expects at least 2 arguments, received: " << (*args).size() << std::endl;
			return nullptr;
		}

		auto var_args = get_variable_args(args);
		auto res_type = get_result_type(var_args);

		if (res_type == "int")
		{
			int res = static_cast<Int*>(var_args[0].get())->value;
			for (size_t i = 1; i < var_args.size(); i++)
			{
				res *= static_cast<Int*>(var_args[i].get())->value;
			}
			return std::make_unique<Int>(res);
		}
		else if (res_type == "float")
		{

			double res{ 0 };
			if (var_args[0]->type == Type::INT) res = static_cast<Int*>(var_args[0].get())->value;
			else if (var_args[0]->type == Type::FLOAT) res = static_cast<Float*>(var_args[0].get())->value;

			for (size_t i = 1; i < var_args.size(); i++)
			{
				if (var_args[i]->type == Type::INT) res *= static_cast<Int*>(var_args[i].get())->value;
				else if (var_args[i]->type == Type::FLOAT) res *= static_cast<Float*>(var_args[i].get())->value;
			}
			return std::make_unique<Float>(res);
		}
		std::cout << "Invalid argument to multiply procedure, expected int or float and received: " << res_type << std::endl;
		return nullptr;
	}

	Divide::Divide() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Divide::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() < 2)
		{
			std::cout << "Divide procedure expects at least 2 arguments, received: " << (*args).size() << std::endl;
			return nullptr;
		}

		auto var_args = get_variable_args(args);
		auto res_type = get_result_type(var_args);

		if (res_type == "int")
		{
			int res = static_cast<Int*>(var_args[0].get())->value;
			for (size_t i = 1; i < var_args.size(); i++)
			{
				res /= static_cast<Int*>(var_args[i].get())->value;
			}
			return std::make_unique<Int>(res);
		}
		else if (res_type == "float")
		{

			double res{ 0 };
			if (var_args[0]->type == Type::INT) res = static_cast<Int*>(var_args[0].get())->value;
			else if (var_args[0]->type == Type::FLOAT) res = static_cast<Float*>(var_args[0].get())->value;

			for (size_t i = 1; i < var_args.size(); i++)
			{
				if (var_args[i]->type == Type::INT) res /= static_cast<Int*>(var_args[i].get())->value;
				else if (var_args[i]->type == Type::FLOAT) res /= static_cast<Float*>(var_args[i].get())->value;
			}
			return std::make_unique<Float>(res);
		}
		std::cout << "Invalid argument to divide procedure, expected int or float and received: " << res_type << std::endl;
		return nullptr;
	}

	Mod::Mod() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Mod::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() < 2)
		{
			std::cout << "Modulo procedure expects at least 2 arguments, received: " << (*args).size() << std::endl;
			return nullptr;
		}
		
		auto var_args = get_variable_args(args);
		auto res_type = get_result_type(var_args);

		if (res_type == "int")
		{
			int res = static_cast<Int*>(var_args[0].get())->value;
			for (size_t i = 1; i < var_args.size(); i++)
			{
				res %= static_cast<Int*>(var_args[i].get())->value;
			}
			return std::make_unique<Int>(res);
		}
		else if (res_type == "float")
		{

			double res{ 0 };
			if (var_args[0]->type == Type::INT) res = static_cast<Int*>(var_args[0].get())->value;
			else if (var_args[0]->type == Type::FLOAT) res = static_cast<Float*>(var_args[0].get())->value;

			for (size_t i = 1; i < var_args.size(); i++)
			{
				if (var_args[i]->type == Type::INT) res = fmod(res, static_cast<Int*>(var_args[i].get())->value);
				else if (var_args[i]->type == Type::FLOAT) res = fmod(res, static_cast<Float*>(var_args[i].get())->value);
			}
			return std::make_unique<Float>(res);
		}
		std::cout << "Invalid argument to modulo procedure, expected int or float and received: " << res_type << std::endl;
		return nullptr;
	}

	Exponent::Exponent() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Exponent::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() < 2)
		{
			std::cout << "Exponent procedure expects at least 2 arguments, received: " << (*args).size() << std::endl;
			return nullptr;
		}

		auto var_args = get_variable_args(args);
		auto res_type = get_result_type(var_args);

		if (res_type == "int")
		{
			int res = static_cast<Int*>(var_args[0].get())->value;
			for (size_t i = 1; i < var_args.size(); i++)
			{
				res = pow(res, static_cast<Int*>(var_args[i].get())->value);
			}
			return std::make_unique<Int>(res);
		}
		else if (res_type == "float")
		{

			double res{ 0 };
			if (var_args[0]->type == Type::INT) res = static_cast<Int*>(var_args[0].get())->value;
			else if (var_args[0]->type == Type::FLOAT) res = static_cast<Float*>(var_args[0].get())->value;

			for (size_t i = 1; i < var_args.size(); i++)
			{
				if (var_args[i]->type == Type::INT) res = pow(res, static_cast<Int*>(var_args[i].get())->value);
				else if (var_args[i]->type == Type::FLOAT) res = pow(res, static_cast<Float*>(var_args[i].get())->value);
			}
			std::cout << "Invalid argument to exponent procedure, expected int or float and received: " << res_type << std::endl;
			return std::make_unique<Float>(res);
		}
		return nullptr;
	}

	Absolute::Absolute() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Absolute::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 1)
		{
			std::cout << "Absolute procedure expects one argument" << std::endl;
			return nullptr;
		}

		auto arg = eval::eval_expr(&(*args)[0]);

		if (arg.get()->type == Type::INT) return std::make_unique<Int>(abs(static_cast<Int*>(arg.get())->value));
		else if (arg.get()->type == Type::FLOAT) return std::make_unique<Float>(abs(static_cast<Float*>(arg.get())->value));

		std::cout << "Invalid argument: Absolute procedure expects a number" << std::endl;
		return nullptr;
	}

	GreaterThan::GreaterThan() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> GreaterThan::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 2)
		{
			std::cout << "Greater than procedure expects two arguments" << std::endl;
			return nullptr;
		}

		auto arg0 = eval::eval_expr(&(*args)[0]);
		auto arg1 = eval::eval_expr(&(*args)[1]);

		if (arg0.get()->type == Type::PROCEDURE || arg1.get()->type == Type::PROCEDURE)
		{
			std::cout << "Greater than procedure does not accept procedure as an argument" << std::endl;
			return nullptr;
		}
		else if (arg0.get()->type == Type::INT && arg1.get()->type == Type::INT)
		{
			return std::make_unique<Bool>(static_cast<Int*>(arg0.get())->value > static_cast<Int*>(arg1.get())->value);
		}
		else if (arg0.get()->type == Type::FLOAT || arg1.get()->type == Type::FLOAT)
		{
			double lhs;
			if (arg0->type == Type::INT) lhs = static_cast<Int*>(arg0.get())->value;
			else if (arg0->type == Type::FLOAT) lhs = static_cast<Float*>(arg0.get())->value;
			else return std::make_unique<Bool>(false);

			double rhs;
			if (arg1->type == Type::INT) rhs = static_cast<Int*>(arg1.get())->value;
			else if (arg1->type == Type::FLOAT) rhs = static_cast<Float*>(arg1.get())->value;
			else return std::make_unique<Bool>(false);

			return std::make_unique<Bool>((lhs > rhs));
		}
		else return std::make_unique<Bool>(false);
	}

	GreaterThanOrEq::GreaterThanOrEq() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> GreaterThanOrEq::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 2)
		{
			std::cout << "Greater than or equals procedure expects two arguments" << std::endl;
			return nullptr;
		}

		auto arg0 = eval::eval_expr(&(*args)[0]);
		auto arg1 = eval::eval_expr(&(*args)[1]);

		if (arg0.get()->type == Type::PROCEDURE || arg1.get()->type == Type::PROCEDURE)
		{
			std::cout << "Greater than or equals procedure does not accept procedure as an argument" << std::endl;
			return nullptr;
		}
		else if (arg0.get()->type == Type::INT && arg1.get()->type == Type::INT)
		{
			return std::make_unique<Bool>(static_cast<Int*>(arg0.get())->value >= static_cast<Int*>(arg1.get())->value);
		}
		else if (arg0.get()->type == Type::FLOAT || arg1.get()->type == Type::FLOAT)
		{
			double lhs;
			if (arg0->type == Type::INT) lhs = static_cast<Int*>(arg0.get())->value;
			else if (arg0->type == Type::FLOAT) lhs = static_cast<Float*>(arg0.get())->value;
			else return std::make_unique<Bool>(false);

			double rhs;
			if (arg1->type == Type::INT) rhs = static_cast<Int*>(arg1.get())->value;
			else if (arg1->type == Type::FLOAT) rhs = static_cast<Float*>(arg1.get())->value;
			else return std::make_unique<Bool>(false);

			return std::make_unique<Bool>((lhs >= rhs));
		}
		else return std::make_unique<Bool>(false);
	}

	LessThan::LessThan() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> LessThan::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 2)
		{
			std::cout << "Less than procedure expects two arguments" << std::endl;
			return nullptr;
		}

		auto arg0 = eval::eval_expr(&(*args)[0]);
		auto arg1 = eval::eval_expr(&(*args)[1]);

		if (arg0.get()->type == Type::PROCEDURE || arg1.get()->type == Type::PROCEDURE)
		{
			std::cout << "Less than procedure does not accept procedure as an argument" << std::endl;
			return nullptr;
		}
		else if (arg0.get()->type == Type::INT && arg1.get()->type == Type::INT)
		{
			return std::make_unique<Bool>(static_cast<Int*>(arg0.get())->value < static_cast<Int*>(arg1.get())->value);
		}
		else if (arg0.get()->type == Type::FLOAT || arg1.get()->type == Type::FLOAT)
		{
			double lhs;
			if (arg0->type == Type::INT) lhs = static_cast<Int*>(arg0.get())->value;
			else if (arg0->type == Type::FLOAT) lhs = static_cast<Float*>(arg0.get())->value;
			else return std::make_unique<Bool>(false);

			double rhs;
			if (arg1->type == Type::INT) rhs = static_cast<Int*>(arg1.get())->value;
			else if (arg1->type == Type::FLOAT) rhs = static_cast<Float*>(arg1.get())->value;
			else return std::make_unique<Bool>(false);

			return std::make_unique<Bool>((lhs < rhs));
		}
		else return std::make_unique<Bool>(false);
	}

	LessThanOrEq::LessThanOrEq() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> LessThanOrEq::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 2)
		{
			std::cout << "Less than or equals procedure expects two arguments" << std::endl;
			return nullptr;
		}

		auto arg0 = eval::eval_expr(&(*args)[0]);
		auto arg1 = eval::eval_expr(&(*args)[1]);

		if (arg0.get()->type == Type::PROCEDURE || arg1.get()->type == Type::PROCEDURE)
		{
			std::cout << "Less than or equals procedure does not accept procedure as an argument" << std::endl;
			return nullptr;
		}
		else if (arg0.get()->type == Type::INT && arg1.get()->type == Type::INT)
		{
			return std::make_unique<Bool>(static_cast<Int*>(arg0.get())->value <= static_cast<Int*>(arg1.get())->value);
		}
		else if (arg0.get()->type == Type::FLOAT || arg1.get()->type == Type::FLOAT)
		{
			double lhs;
			if (arg0->type == Type::INT) lhs = static_cast<Int*>(arg0.get())->value;
			else if (arg0->type == Type::FLOAT) lhs = static_cast<Float*>(arg0.get())->value;
			else return std::make_unique<Bool>(false);

			double rhs;
			if (arg1->type == Type::INT) rhs = static_cast<Int*>(arg1.get())->value;
			else if (arg1->type == Type::FLOAT) rhs = static_cast<Float*>(arg1.get())->value;
			else return std::make_unique<Bool>(false);

			return std::make_unique<Bool>((lhs <= rhs));
		}
		else return std::make_unique<Bool>(false);
	}

	Equals::Equals() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Equals::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 2)
		{
			std::cout << "Equals procedure expects two arguments" << std::endl;
			return nullptr;
		}

		auto arg0 = eval::eval_expr(&(*args)[0]);
		auto arg1 = eval::eval_expr(&(*args)[1]);

		if (arg0.get()->type == Type::PROCEDURE || arg1.get()->type == Type::PROCEDURE)
		{
			std::cout << "Equals procedure does not accept procedure as an argument" << std::endl;
			return nullptr;
		}
		else if (arg0.get()->type == Type::INT && arg1.get()->type == Type::INT)
		{
			return std::make_unique<Bool>(static_cast<Int*>(arg0.get())->value == static_cast<Int*>(arg1.get())->value);
		}
		else if (arg0.get()->type == Type::FLOAT || arg1.get()->type == Type::FLOAT)
		{
			double lhs;
			if (arg0->type == Type::INT) lhs = static_cast<Int*>(arg0.get())->value;
			else if (arg0->type == Type::FLOAT) lhs = static_cast<Float*>(arg0.get())->value;
			else return std::make_unique<Bool>(false);

			double rhs;
			if (arg1->type == Type::INT) rhs = static_cast<Int*>(arg1.get())->value;
			else if (arg1->type == Type::FLOAT) rhs = static_cast<Float*>(arg1.get())->value;
			else return std::make_unique<Bool>(false);

			return std::make_unique<Bool>((lhs == rhs));
		}
		else if (arg0.get()->type == Type::STRING && arg1.get()->type == Type::STRING)
		{
			return std::make_unique<Bool>(static_cast<String*>(arg0.get())->value == static_cast<String*>(arg1.get())->value);
		}
		else return std::make_unique<Bool>(false);
	}

	Cons::Cons() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Cons::call(std::vector<ASTExpr>* args)
	{
		std::cout << "NOT IMPLEMENTED: cons" << std::endl;
		return nullptr;
	}

	Car::Car() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Car::call(std::vector<ASTExpr>* args)
	{
		std::cout << "NOT IMPLEMENTED: car" << std::endl;
		return nullptr;
	}

	Cdr::Cdr() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Cdr::call(std::vector<ASTExpr>* args)
	{
		std::cout << "NOT IMPLEMENTED: cdr" << std::endl;
		return nullptr;
	}

	Length::Length() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Length::call(std::vector<ASTExpr>* args)
	{
		// TODO: This needs to get the length of the list in the first arg position
		return std::make_unique<Int>((*args).size());
	}

	Sin::Sin() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Sin::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 1)
		{
			std::cout << "Sin procedure expects 1 argument" << std::endl;
			return nullptr;
		}

		auto arg = eval::eval_expr(&(*args)[0]);

		if ((*arg).type == Type::INT) return std::make_unique<Float>(sin(static_cast<Int*>(arg.get())->value));
		if ((*arg).type == Type::FLOAT) return std::make_unique<Float>(sin(static_cast<Float*>(arg.get())->value));

		std::cout << "Sin procedure received an invalid argument type: " << get_var_type_as_string((*arg)) << std::endl;
		return nullptr;
	}

	Cos::Cos() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Cos::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 1)
		{
			std::cout << "Cos procedure expects 1 argument1" << std::endl;
			return nullptr;
		}

		auto arg = eval::eval_expr(&(*args)[0]);

		if ((*arg).type == Type::INT) return std::make_unique<Float>(cos(static_cast<Int*>(arg.get())->value));
		if ((*arg).type == Type::FLOAT) return std::make_unique<Float>(cos(static_cast<Float*>(arg.get())->value));

		std::cout << "Cos procedure received an invalid argument type: " << get_var_type_as_string((*arg)) << std::endl;
		return nullptr;
	}

	Tan::Tan() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Tan::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 1)
		{
			std::cout << "Tan procedure expects 1 argument" << std::endl;
			return nullptr;
		}

		auto arg = eval::eval_expr(&(*args)[0]);

		if ((*arg).type == Type::INT) return std::make_unique<Float>(tan(static_cast<Int*>(arg.get())->value));
		if ((*arg).type == Type::FLOAT) return std::make_unique<Float>(tan(static_cast<Float*>(arg.get())->value));

		std::cout << "Tan procedure received an invalid argument type: " << get_var_type_as_string((*arg)) << std::endl;
		return nullptr;
	}

	Sqrt::Sqrt() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Sqrt::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 1)
		{
			std::cout << "Square root procedure expects 1 argument" << std::endl;
			return nullptr;
		}

		auto arg = eval::eval_expr(&(*args)[0]);

		if ((*arg).type == Type::INT)
		{
			auto sq_root_num = (long long)round((sqrt(static_cast<Int*>(arg.get())->value)));
			if (sq_root_num * sq_root_num == static_cast<Int*>(arg.get())->value) {
				return std::make_unique<Int>(sqrt(static_cast<Int*>(arg.get())->value));
			}
			else {
				return std::make_unique<Float>(sqrt(static_cast<Int*>(arg.get())->value));
			}
		}
		if ((*arg).type == Type::FLOAT)
		{
			return std::make_unique<Float>(sqrt(static_cast<Float*>(arg.get())->value));
		}

		std::cout << "Square root procedure received an invalid argument type: " << get_var_type_as_string((*arg)) << std::endl;
		return nullptr;
	}

	Define::Define() : VarCopy(Variable::Type::DEFINITION) {}

	std::unique_ptr<Variable> Define::call(std::vector<ASTExpr>* args)
	{
		std::cout << "Define variable is not callable" << std::endl;
		return nullptr;
	}

	If::If() : VarCopy(Variable::Type::CONDITIONAL) {}

	std::unique_ptr<Variable> If::call(std::vector<ASTExpr>* args)
	{
		if ((*args).size() != 3)
		{
			std::cout << "If statement expects a condition, then, and an else" << std::endl;
			return nullptr;
		}

		auto test = eval::eval_expr(&(*args)[0]);
		if (test->type != Variable::Type::BOOL)
		{
			std::cout << "If statement condition should evaluate to a boolean" << std::endl;
			return nullptr;
		}
		if (static_cast<Bool*>(test.get())->value == true)
		{
			return eval::eval_expr(&(*args)[1]);
		}
		return eval::eval_expr(&(*args)[2]);
	}

	Begin::Begin() : VarCopy(Variable::Type::PROCEDURE) {}

	std::unique_ptr<Variable> Begin::call(std::vector<ASTExpr>* args)
	{
		std::cout << "NOT IMPLEMENTED: begin" << std::endl;
		return nullptr;
	}

	Environment::Environment()
	{
		// Built-In Procedures
		env_map["+"] = std::make_unique<Add>();
		env_map["-"] = std::make_unique<Subtract>();
		env_map["*"] = std::make_unique<Multiply>();
		env_map["/"] = std::make_unique<Divide>();
		env_map["%"] = std::make_unique<Mod>();
		env_map[">"] = std::make_unique<GreaterThan>();
		env_map[">="] = std::make_unique<GreaterThanOrEq>();
		env_map["<"] = std::make_unique<LessThan>();
		env_map["<="] = std::make_unique<LessThanOrEq>();
		env_map["="] = std::make_unique<Equals>();
		env_map["abs"] = std::make_unique<Absolute>();
		env_map["car"] = std::make_unique<Cons>();
		env_map["car"] = std::make_unique<Car>();
		env_map["cdr"] = std::make_unique<Cdr>();
		env_map["expt"] = std::make_unique<Exponent>();
		env_map["length"] = std::make_unique<Length>();
		env_map["sin"] = std::make_unique<Sin>();
		env_map["cos"] = std::make_unique<Cos>();
		env_map["tan"] = std::make_unique<Tan>();
		env_map["sqrt"] = std::make_unique<Sqrt>();
	}
}