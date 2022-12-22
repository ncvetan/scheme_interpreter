#pragma once

#include <unordered_map>
#include <memory>
#include <cmath>
#include <lang/parser.hpp>

using namespace parser;

namespace environment
{

	class Variable
	{
	public:

		enum class Type {
			INVALID,
			PROCEDURE,
			CONDITIONAL,
			DEFINITION,
			INT,
			FLOAT,
			STRING,
			BOOL,
			SYMBOL,
			LIST
		};

		Type type = Type::INVALID;

		Variable(Type type);

		virtual ~Variable() = default;

		virtual std::unique_ptr<Variable> copy() const = 0;

		virtual std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) = 0;

		friend bool operator== (const Token& lhs, const Token& rhs) noexcept;

	};

	bool operator== (const Variable& lhs, const Variable& rhs) noexcept;

	template<typename T>
	class VarCopy :
		public Variable
	{
	public:
		using Variable::Variable;

		std::unique_ptr<Variable> copy() const { return std::make_unique<T>(*static_cast<const T*>(this)); }
	};

	class Int : public VarCopy<Int>
	{
	public:
		int value;

		Int(int value);

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Float : public VarCopy<Float>
	{
	public:
		double value;

		Float(double value);

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class String : public VarCopy<String>
	{
	public:
		std::string value;

		String(std::string value);

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Bool : public VarCopy<Bool>
	{
	public:
		bool value;

		Bool(bool value);

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Symbol : public VarCopy<Symbol>
	{
	public:
		std::string value;

		Symbol(std::string value);

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class List : public Variable
	{
	public:
		std::vector<std::unique_ptr<Variable>> values;

		List(std::vector<std::unique_ptr<Variable>> values);

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Add : public VarCopy<Add>
	{
	public:
		Add();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Subtract : public VarCopy<Subtract>
	{
	public:
		Subtract();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Multiply : public VarCopy<Multiply>
	{
	public:
		Multiply();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Divide : public VarCopy<Divide>
	{
	public:
		Divide();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Mod : public VarCopy<Mod>
	{
	public:
		Mod();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class GreaterThan : public VarCopy<GreaterThan>
	{
	public:
		GreaterThan();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class GreaterThanOrEq : public VarCopy<GreaterThanOrEq>
	{
	public:
		GreaterThanOrEq();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class LessThan : public VarCopy<LessThan>
	{
	public:
		LessThan();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class LessThanOrEq : public VarCopy<LessThanOrEq>
	{
	public:
		LessThanOrEq();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Equals : public VarCopy<Equals>
	{
	public:
		Equals();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Absolute : public VarCopy<Absolute>
	{
	public:
		Absolute();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Cons : public VarCopy<Cons>
	{
	public:
		Cons();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Car : public VarCopy<Car>
	{
	public:
		Car();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Cdr : public VarCopy<Cdr>
	{
	public:
		Cdr();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Exponent : public VarCopy<Exponent>
	{
	public:
		Exponent();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Length : public VarCopy<Length>
	{
	public:
		Length();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Sin : public VarCopy<Sin>
	{
	public:
		Sin();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Cos : public VarCopy<Cos>
	{
	public:
		Cos();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Tan : public VarCopy<Tan>
	{
	public:
		Tan();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Sqrt : public VarCopy<Sqrt>
	{
	public:
		Sqrt();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Define : public VarCopy<Define>
	{
	public:
		Define();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class If : public VarCopy<If>
	{
	public:
		If();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};

	class Begin : public VarCopy<Begin>
	{
	public:
		Begin();

		std::unique_ptr<Variable> call(std::vector<ASTExpr>* args) override;
	};


	std::string get_var_type_as_string(const Variable& var);

	std::vector<std::unique_ptr<Variable>> get_variable_args(std::vector<ASTExpr>* args);

	std::string get_result_type(const std::vector<std::unique_ptr<Variable>>& args);

	class Environment
	{
	public:
		/**
		 * Initialize the environment with the built-in functions
		 *
		*/
		Environment();

		std::unordered_map<std::string, std::unique_ptr<Variable>> env_map;
	};
	
	

}