#ifndef ONCE_FPARSER_H_
#define ONCE_FPARSER_H_

#include <string>
#include <map>

class FunctionParser
{
public:
    int Parse(const std::string& Function, const std::string& Vars);
    const char* ErrorMsg(void) const;
    double Eval(const double* Vars);
    inline int EvalError(void) const { return EvalErrorType; }

    FunctionParser();
    ~FunctionParser();

//========================================================================
private:
//========================================================================
    int varAmount, ParseErrorType,EvalErrorType;

    typedef std::map<std::string, unsigned> VarMap_t;
    VarMap_t Variables;

    struct CompiledCode
    {   CompiledCode();
        CompiledCode(const CompiledCode&);
        ~CompiledCode();

        unsigned* ByteCode;
        unsigned ByteCodeSize;
        double* Immed;
        unsigned ImmedSize;
        double* Stack;
        unsigned StackSize, StackPtr;
        bool thisIsACopy;
    } Comp;

    VarMap_t::const_iterator FindVariable(const char*);
    int CheckSyntax(const char*);
    bool Compile(const char*);
    bool IsVariable(int);
    void AddCompiledByte(unsigned);
    int CompileIf(const char*, int);
    int CompileElement(const char*, int);
    int CompilePow(const char*, int);
    int CompileMult(const char*, int);
    int CompileAddition(const char*, int);
    int CompileComparison(const char*, int);
    int CompileAnd(const char*, int);
    int CompileOr(const char*, int);
    int CompileExpression(const char*, int, bool=false);

    FunctionParser(const FunctionParser&);
};

#endif
