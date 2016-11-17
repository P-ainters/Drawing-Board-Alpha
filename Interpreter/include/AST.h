#ifndef GI_AST_H
#define GI_AST_H

#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <easylogging++.h>
#include <context.h>

namespace ast {
    using context::Scope;

    class ExprAST {
    public:
        virtual std::shared_ptr<ExprAST> eval(Scope &) const;

        virtual std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &, Scope &);

        virtual std::shared_ptr<ExprAST> toBool(Scope &) const;

        virtual ~ExprAST() {}
    };

    class AllExprAST : public ExprAST {
    public:
        AllExprAST(const std::vector<std::shared_ptr<ExprAST>> &v) : exprVec{v} {}

        virtual std::shared_ptr<ExprAST> eval(Scope &) const;

    private:
        std::vector<std::shared_ptr<ExprAST>> exprVec;
    };

    class BooleansAST : public ExprAST {
    public:
        BooleansAST(bool c) : booleans{c} {}

        virtual std::shared_ptr<ExprAST> eval(Scope &) const;

    private:
        bool booleans;
    };

    class NumberAST : public ExprAST {
    public:
        NumberAST(double n) : value{n} {}

        double getValue() const { return value; }

        virtual std::shared_ptr<ExprAST> toBool(Scope &) const override;

        std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        double value;
    };

    class IdentifierAST : public ExprAST {
    public:
        IdentifierAST(const std::string &tid) : id{tid} {}

        std::string getId() const { return id; }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::string id;
    };

    class IfStatementAST : public ExprAST {
    public:
        IfStatementAST(std::shared_ptr<ExprAST> c, std::shared_ptr<ExprAST> t, std::shared_ptr<ExprAST> f) :
                condition{c}, trueClause{t}, falseClause{f} {}

        std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        std::shared_ptr<ExprAST> condition;
        std::shared_ptr<ExprAST> trueClause, falseClause;
    };

    class LoadingFileAST : public ExprAST {
    public:
        LoadingFileAST(const std::string &f) : filename{f} {}

        std::shared_ptr<ExprAST> eval(Scope &) const override;

    private:
        std::string filename;
    };

    class OperatorAST : public ExprAST {
    public:
        OperatorAST(const std::vector<std::shared_ptr<ExprAST>> &v) : actualArgs{v} {}

    protected:
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

    class LessThanOperatorAST : public OperatorAST {
    public:
        LessThanOperatorAST(const std::vector<std::shared_ptr<ExprAST>> &v) : OperatorAST{v} {}

        std::shared_ptr<ExprAST> eval(Scope &s) const override;
    };

    class AddOperatorAST : public OperatorAST {
    public:
        AddOperatorAST(const std::vector<std::shared_ptr<ExprAST>> &v) : OperatorAST{v} {}

        std::shared_ptr<ExprAST> eval(Scope &s) const override;
    };

    class BindingAST : public ExprAST {
    public:
        BindingAST(const std::string &id) : identifier{id} {}

        const std::string &getIdentifier() const {
            return identifier;
        }

    private:
        std::string identifier;
    };

    class ValueBindingAST : public BindingAST {
    public:
        ValueBindingAST(const std::string &id, std::shared_ptr<ExprAST> v)
                : BindingAST(id), value{v} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::shared_ptr<ExprAST> value;
    };

    class LambdaAST : public ExprAST {
    public:
        LambdaAST(const std::vector<std::string> &v,
                  std::shared_ptr<ExprAST> expr) : formalArgs{v}, expression{expr} {}

        std::shared_ptr<ExprAST> apply(const std::vector<std::shared_ptr<ExprAST>> &actualArgs, Scope &) override;

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

        void setContext(const Scope &s) {
            context = s;
        }

    private:
        std::vector<std::string> formalArgs;
        std::shared_ptr<ExprAST> expression;
        Scope context;
    };


    class LambdaBindingAST : public BindingAST {
    public:
        LambdaBindingAST(const std::string &id,
                         const std::vector<std::string> &v,
                         std::shared_ptr<ExprAST> expr) :
                BindingAST(id), lambda{std::make_shared<LambdaAST>(v, expr)} {}

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::shared_ptr<LambdaAST> lambda;
    };


    class LambdaApplicationAST : public ExprAST {
    public:
        LambdaApplicationAST(const std::shared_ptr<ExprAST> &lam, const std::vector<std::shared_ptr<ExprAST>> &args)
                : lambdaOrIdentifier{lam}, actualArgs{args} {
        }

        std::shared_ptr<ExprAST> eval(Scope &ss) const override;

    private:
        std::shared_ptr<ExprAST> lambdaOrIdentifier;
        std::vector<std::shared_ptr<ExprAST>> actualArgs;
    };

}

#endif //GI_AST_H
