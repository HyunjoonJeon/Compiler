#ifndef ast_primitives_hpp
#define ast_primitives_hpp

#include <string>
#include <iostream>

#include "symbol_table.hpp"

enum TypeDef{
  INT,
  FLOAT,
  DBL
};

enum DeclType{
  CALL,
  ASSIGN,
  DECL,
  ARG
};

class Variable
    :public Expression
{
private:
    mutable std::string type;
    std::string id;
    mutable std::string address;
    std::string assignop;
    DeclType VarType;
    ExpressionPtr Expr = nullptr;
public:
//This constructor does not feel right, it's a placeholder.
    Variable() {
    }
    
    Variable(const std::string *_id) {
        VarType = CALL;
        id = *_id;
    }

    Variable(const std::string *_id, std::string *_AssignOp, ExpressionPtr _Expr) {
        VarType = ASSIGN;
        id = *_id;
        assignop = *_AssignOp;
        Expr = _Expr;
    }

    Variable(TypeDef _type, const std::string *_id, DeclType _form, ExpressionPtr _Expr = nullptr) {
        if(_form == ARG){
            VarType = ARG;
            switch(_type) {
                case INT:
                    type = "INT";
                    id = *_id;
                    break;
                case FLOAT:
                    type = "FLOAT";
                    id = *_id;
                    /*
                    StackPointer.setIncr(StackPointer.getIncr()+8);
                    address = std::to_string(StackPointer.getIncr() + 2000);
                    if(Symbol.lookUp(id) == "Error: undefined reference"){
                        Symbol.insert(type, "var", id, address);
                    }else{
                        Symbol.modify(type, "var", id, address);
                    }
                    */
                    break;
                case DBL:
                    type = "DBL";
                    id = *_id;
                    /*
                    StackPointer.setIncr(StackPointer.getIncr()+16);
                    address = std::to_string(StackPointer.getIncr() + 2000);
                    if(Symbol.lookUp(id) == "Error: undefined reference"){
                        Symbol.insert(type, "var", id, address);
                    }else{
                        Symbol.modify(type, "var", id, address);
                    }
                    */
                    break;
                default:
                    type = "something went wrong";
            }
        }else if(_form == DECL){
            VarType = DECL;
            switch(_type) {
                case INT:
                    type = "INT";
                    id = *_id;
                    Expr = _Expr;
                    break;
                case FLOAT:
                    type = "FLOAT";
                    id = *_id;
                    Expr = _Expr;
                    break;
                case DBL:
                    type = "DBL";
                    id = *_id;
                    Expr = _Expr;
                    /*
                    StackPointer.setIncr(StackPointer.getIncr()+16);
                    address = std::to_string(StackPointer.getIncr() + 2000);
                    if(Symbol.lookUp(id) == "Error: undefined reference"){
                        Symbol.insert(type, "var", id, address);
                    }else{
                        Symbol.modify(type, "var", id, address);
                    }
                    */
                    break;
                default:
                    type = "something went wrong";
            }
        }
    }
    
    const std::string getType() const
    { return type; }

    const std::string getId() const
    { return id; }

    const std::string getAddr() const
    { return address; }

    ExpressionPtr getExpr() const
    { return Expr; }

    virtual void print(std::ostream &dst) const override
    {
        switch(VarType) {
            case CALL:
                dst<<id;
                break;
            case ASSIGN:
                dst<<id;
                dst<<" ";
                dst<<assignop;
                dst<<" ";
                Expr->print(dst);
                break;
            case DECL:
                dst<<type;
                dst<<" ";
                dst<<id;
                if(Expr!=nullptr){
                    dst<<" = ";
                    Expr->print(dst);
                }
                dst<<";";
                dst<<'\n';
                break;
        }
    }

    virtual void CompileRec(std::string destReg) const override{
        switch(VarType) {
            case CALL:
                address = Symbol.lookUp(id);
                type = Symbol.getType(id);
                if(type == "INT") {
                    std::cout << "lw " << destReg << ", -" << address << "($fp)" << std::endl;
                }
                else if(type == "FLOAT") {
                    std::cout << "lwc1 " << destReg << ", -" << address << "($fp)" << std::endl;
                }
                break;
            case ASSIGN:
                address = Symbol.lookUp(id);
                type = Symbol.getType(id);
                if(assignop == "="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "sw $t0, -" << address << "($fp)" << std::endl;
                    }
                    else if(type == "FLOAT") {
                        getExpr()->CompileRec("$f0");
                        std::cout << "swc1 $f0, -" << address << "($fp)" << std::endl;  
                    }
                    else {
                        std::cout << getType() << std::endl;
                        std::cout << "ERROR: type missing" << std::endl;
                    }
                }else if(assignop == "+="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "add $t1, $t1, $t0" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                    else if(type == "FLOAT") {
                        getExpr()->CompileRec("$f0");
                        std::cout << "lwc1 $f1, -" << address << "($fp)" << std::endl;
                        std::cout << "add.s $f1, $f1, $f0" << std::endl;
                        std::cout << "swc1 $f1, -" << address << "($fp)" << std::endl;
                    }
                }else if(assignop == "-="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "sub $t1, $t1, $t0" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                    else if(type == "FLOAT") {
                        getExpr()->CompileRec("$f0");
                        std::cout << "lwc1 $f1, -" << address << "($fp)" << std::endl;
                        std::cout << "sub.s $f1, $f1, $f0" << std::endl;
                        std::cout << "swc1 $f1, -" << address << "($fp)" << std::endl;
                    }
                }else if(assignop == "/="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "div $t1, $t0" << std::endl;
                        std::cout << "mflo $t1" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                    else if(type == "FLOAT") {
                        getExpr()->CompileRec("$f0");
                        std::cout << "lwc1 $f1 -" << address << "($fp)" << std::endl;
                        std::cout << "div.s $f1, $f1, $f0" << std::endl;
                        std::cout << "swc1 $f1, -" << address << "($fp)" << std::endl;
                    }
                }else if(assignop == "*="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "mul $t1, $t1, $t0" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                    else if(type == "FLOAT") {
                        getExpr()->CompileRec("$f0");
                        std::cout << "lwc1 $f1 -" << address << "($fp)" << std::endl;
                        std::cout << "mul.s $f1, $f1, $f0" << std::endl;
                        std::cout << "swc1 $f1, -" << address << "($fp)" << std::endl;
                    }
                }else if(assignop == "%="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "div $t1, $t0" << std::endl;
                        std::cout << "mfhi $t1" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                }else if(assignop == "<<="){

                }else if(assignop == ">>="){

                }else if(assignop == "^="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "xor $t1, $t1, $t0" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                }else if(assignop == "&="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "and $t1, $t1, $t0" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                }else if(assignop == "|="){
                    if(type == "INT") {
                        getExpr()->CompileRec("$t0");
                        std::cout << "lw $t1, -" << address << "($fp)" << std::endl;
                        std::cout << "or $t1, $t1, $t0" << std::endl;
                        std::cout << "sw $t1, -" << address << "($fp)" << std::endl;
                    }
                }
                break;
            case DECL:
                if(getType()=="INT"){
                    std::cout << "addi $sp, $sp, -4" << std::endl;
                    StackPointer.setIncr(StackPointer.getIncr()+4);
                    StackPointer.setscopeIncr(StackPointer.getscopeIncr()+4);
                    address = std::to_string(StackPointer.getIncr());
                    if(Symbol.lookUp(id) == "Error: undefined reference"){
                        Symbol.insert(type, "var", id, address);
                    }else{
                        Symbol.modify(type, "var", id, address);
                    }
                    if(Expr!=nullptr){
                        getExpr()->CompileRec("$t0");
                        std::cout << "sw $t0, -" << address << "($fp)" << std::endl;
                    }
                    if(Symbol.getScope()==0){
                        std::cout << ".global " << getId() << std::endl;
                    }
                }
                else if(getType() == "FLOAT") {
                    std::cout << "addi $sp, $sp, -4" << std::endl;
                    StackPointer.setIncr(StackPointer.getIncr()+4);
                    StackPointer.setscopeIncr(StackPointer.getscopeIncr()+4);
                    address = std::to_string(StackPointer.getIncr());
                    if(Symbol.lookUp(id) == "Error: undefined reference") {
                        Symbol.insert(type, "var", id, address);
                    }else {
                        Symbol.modify(type, "var", id, address);
                    }
                    if(Expr!=nullptr) {
                        getExpr()->CompileRec("$f0");
                        std::cout << "swc1 $f0, -" << address << "($fp)" << std::endl;
                    }
                    if(Symbol.getScope()==0) {
                        std::cout << ".global " << getId() << std::endl;
                    }
                }
                break;
            case ARG:
                if(getType()=="INT"){
                    std::cout << "addi $sp, $sp, -4" << std::endl;
                    if(StackPointer.getArgc()<4){
                        std::cout << "sw $a" << StackPointer.getArgc() << ", 0($sp)"<<  std::endl;
                    }
                    StackPointer.setIncr(StackPointer.getIncr()+4);
                    StackPointer.setscopeIncr(StackPointer.getscopeIncr()+4);
                    address = std::to_string(StackPointer.getIncr());
                    if(Symbol.lookUp(id) == "Error: undefined reference"){
                        Symbol.insert(type, "var", id, address);
                    }else{
                        Symbol.modify(type, "var", id, address);
                    }
                    if(Expr!=nullptr){
                        getExpr()->CompileRec("$t0");
                        std::cout << "sw $t0, -" << address << "($fp)" << std::endl;
                    }
                    if(Symbol.getScope()==0){
                        std::cout << ".global " << getId() << std::endl;
                    }
                }
                else if(getType() == "FLOAT") {
                    std::cout << "addi $sp, $sp, -4" << std::endl;
                    if(StackPointer.getArgc()<4){
                        int float_argNum;
                        if(StackPointer.getArgc() == 0) {
                            float_argNum = 12;
                        }
                        else if(StackPointer.getArgc() == 1) {
                            float_argNum = 14;
                        }
                        std::cout << "swc1 $f" << float_argNum << ", 0($sp)"<<  std::endl;
                    }
                    StackPointer.setIncr(StackPointer.getIncr()+4);
                    StackPointer.setscopeIncr(StackPointer.getscopeIncr()+4);
                    address = std::to_string(StackPointer.getIncr());
                    if(Symbol.lookUp(id) == "Error: undefined reference"){
                        Symbol.insert(type, "var", id, address);
                    }else{
                        Symbol.modify(type, "var", id, address);
                    }
                    if(Expr!=nullptr){
                        getExpr()->CompileRec("$f0");
                        std::cout << "swc1 $f0, -" << address << "($fp)" << std::endl;
                    }
                    if(Symbol.getScope()==0){
                        std::cout << ".global " << getId() << std::endl;
                    }
                }
                break;
        }   
    }
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        return bindings.at(id);
    }    
};

class FunctionStorage
    :public Expression
{
private:
    std::string type;
    std::string id;
    mutable std::string address;
    ExpressionListPtr Args = nullptr;
    DeclType VarType;
public:
//This constructor does not feel right, it's a placeholder.
    FunctionStorage() {
    }
    
    FunctionStorage(const std::string *_id, ExpressionListPtr _Args = nullptr) {
        VarType = CALL;
        id = *_id;
        Args = _Args;
    }

    FunctionStorage(TypeDef _type, const std::string *_id) {
        VarType = DECL;
        switch(_type) {
            case INT:
                type = "INT";
                id = *_id;
                break;
            case FLOAT:
                type = "FLOAT";
                id = *_id;
                break;
            case DBL:
                type = "DBL";
                id = *_id;
                break;
            default:
                type = "something went wrong";
        }
    }
    
    const std::string getType() const
    { return type; }

    const std::string getId() const
    { return id; }

    const std::string getAddr() const
    { return address; }

    virtual void print(std::ostream &dst) const override
    {
        switch(VarType) {
            case CALL:
                dst<<id;
                break;
            case DECL:
                dst<<type;
                dst<<" ";
                dst<<id;
                dst<<"();";
                dst<<'\n';
                break;
        }
    }

    virtual void CompileRec(std::string destReg) const override{
        switch(VarType) {
            case CALL:
                address = std::to_string(std::stoi(Symbol.lookUp(id))+44);
                if(Args!=nullptr){
                    Args->CompileRec(destReg, address);
                }
                std::cout << "jal " << id << std::endl;
                break;
            case DECL:
                if(getType()=="INT"){
                   address = std::to_string(StackPointer.getIncr());
                    if(Symbol.lookUp(id) == "Error: undefined reference"){
                        Symbol.insert(type, "func", id, address);
                    }else{
                        Symbol.modify(type, "func", id, address);
                    }
                }
                break;
        }   
    }
    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        return bindings.at(id);
    }    
}; 

class DeclarationList;

typedef const DeclarationList *DeclarationListPtr;

class DeclarationList
    : public Variable
{
private:
    Variable *variable;
    DeclarationListPtr declarationList = nullptr;
public:
    DeclarationList(Variable *_variable, DeclarationListPtr _declarationList = nullptr)
        : variable(_variable)
        , declarationList(_declarationList)
    {}

    virtual ~DeclarationList() {
        delete variable;
        delete declarationList;
    }
    Variable *getVar() const
    { return variable; }

    DeclarationListPtr getdecllist() const
    { return declarationList; }

    virtual void print(std::ostream &dst) const override
    {
        variable->print(dst);
        if(declarationList!=nullptr){
            dst << ", ";
            declarationList->print(dst);
        }
    }

    virtual void CompileRec(std::string destReg) const override{
        getVar()->CompileRec(destReg);
        StackPointer.setArgc(StackPointer.getArgc()+1);
        if(declarationList!=nullptr){
            getdecllist()->CompileRec(destReg);
        }
        StackPointer.setArgc(0);
    }  
};

class Number
    : public Expression
{
private:
    double value;
public:
    Number(double _value)
        : value(_value)
    {}

    double getValue() const
    { return value; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<value;
    }

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        return value;
    }

    virtual void CompileRec(std::string destReg) const override{
        if(value == (int) value) {
            std::cout << "addi " << destReg << ", $0, " << value << std::endl;
        }
        else {
            std::cout << "addi.s " << destReg << ", $0, " << value << std::endl;
        }
    }  

};

#endif
