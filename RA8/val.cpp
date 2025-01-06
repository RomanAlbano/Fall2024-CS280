    #include "val.h"

    Value Value::operator+(const Value& op) const {
        ValType a = GetType();
        ValType b = op.GetType();
        
        switch(a) {
            case VSTRING:
                switch(b) {
                    case VSTRING:
                        return Value(GetString() + op.GetString());
                    case VCHAR:
                        return Value(GetString() + op.GetChar());
                    default:
                        return Value();
                }
                break;
            case VCHAR:
                switch(b) {
                    case VSTRING:
                        return Value(GetChar() + op.GetString());
                    case VCHAR:
                        return Value(GetChar() + op.GetChar());
                    case VINT:
                        return Value(GetChar() + op.GetInt());
                    case VREAL:
                        return Value(GetChar() + op.GetReal());
                    default:
                        return Value();
                }
                break;
            case VINT:
                switch(b) {
                    case VCHAR:
                        return Value(GetInt() + op.GetChar());
                    case VINT:
                        return Value(GetInt() + op.GetInt());
                    case VREAL:
                        return Value(GetInt() + op.GetReal());
                    default:
                        return Value();
                }
                break;
            case VREAL:
                switch(b) {
                    case VINT:
                        return Value(GetReal() + op.GetInt());
                    case VREAL:
                        return Value(GetReal() + op.GetReal());
                    case VCHAR:
                        return Value(GetReal() + op.GetChar());
                    default:
                        return Value();
                }
                break;
            default:
                return Value();
        }
        return Value();
    }

    Value Value::operator>(const Value& op) const {
        ValType a = GetType();
        ValType b = op.GetType();
        switch (a){
            case VCHAR:
                switch (b){
                    case VINT:
                        return Value(GetChar()> op.GetInt());
                    case VCHAR:
                        return Value(GetChar()>op.GetChar());
                    default:
                        return Value();
                }
                break;
            case VINT:
                switch(b){
                    case VINT:
                        return Value(GetInt() > op.GetInt());
                    case VREAL:
                        return Value(GetInt()>op.GetReal());
                    case VCHAR:
                        return Value(GetInt() > op.GetChar());
                    default:
                        return Value();
                }
                break;
            case VSTRING:
                switch (b){
                    case VSTRING:
                        return Value(GetString() > op.GetString());
                    default:
                        return Value();
                }
                break;
            case VREAL:
                switch(b){
                    case VREAL:
                        return Value(GetReal() > op.GetReal());
                    case VINT:
                        return Value(GetReal() > op.GetInt());
                    default:
                        return Value();
                }
                break;
            case VBOOL:
                switch(b){
                    case VBOOL:
                        return Value(GetBool() > op.GetBool());
                    default: 
                        return Value();
                }
            default:
                return Value();
        }
        return Value();
    }

    Value Value::operator==(const Value& op) const {
        ValType a = GetType();
        ValType b = op.GetType();
        switch (a){
            case VCHAR:
                switch (b){
                    case VINT:
                        return Value(GetChar()== op.GetInt());
                    case VCHAR:
                        return Value(GetChar()==op.GetChar());
                    default:
                        return Value();
                }
                break;
            case VINT:
                switch(b){
                    case VINT:
                        return Value(GetInt() == op.GetInt());
                    case VREAL:
                        return Value(GetInt()==op.GetReal());
                    case VCHAR:
                        return Value(GetInt() == op.GetChar());
                    default:
                        return Value();
                }
                break;
            case VSTRING:
                switch (b){
                    case VSTRING:
                        return Value(GetString() == op.GetString());
                    default:
                        return Value();
                }
                break;
            case VREAL:
                switch(b){
                    case VREAL:
                        return Value(GetReal() == op.GetReal());
                    case VINT:
                        return Value(GetReal() == op.GetInt());
                    default:
                        return Value();
                }
                break;
            case VBOOL:
                switch(b){
                    case VBOOL:
                        return Value(GetBool() == op.GetBool());
                    default: 
                        return Value();
                }
            default:
                return Value();
        }
        return Value();
    }

    Value Value::operator&&(const Value& op) const {
        ValType a = GetType();
        ValType b = op.GetType();
        switch (a){
            case VBOOL:
                switch(b){
                    case VBOOL:
                        return Value(GetBool() && op.GetBool());
                    default: 
                        return Value();
                }
            default:
                return Value();
        }
        return Value();
    }