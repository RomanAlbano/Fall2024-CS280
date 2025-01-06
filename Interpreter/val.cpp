#include "val.h"

Value Value::operator+(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();

    switch (a)
    {
    case VSTRING:
        switch (b)
        {
        case VSTRING:
            return Value(GetString() + op.GetString());
        case VCHAR:
            return Value(GetString() + op.GetChar());
        default:
            return Value();
        }
        break;
    case VCHAR:
        switch (b)
        {
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
        switch (b)
        {
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
        switch (b)
        {
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

Value Value::operator>(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();
    switch (a)
    {
    case VCHAR:
        switch (b)
        {
        case VINT:
            return Value(GetChar() > op.GetInt());
        case VCHAR:
            return Value(GetChar() > op.GetChar());
        default:
            return Value();
        }
        break;
    case VINT:
        switch (b)
        {
        case VINT:
            return Value(GetInt() > op.GetInt());
        case VREAL:
            return Value(GetInt() > op.GetReal());
        case VCHAR:
            return Value(GetInt() > op.GetChar());
        default:
            return Value();
        }
        break;
    case VSTRING:
        switch (b)
        {
        case VSTRING:
            return Value(GetString() > op.GetString());
        default:
            return Value();
        }
        break;
    case VREAL:
        switch (b)
        {
        case VREAL:
            return Value(GetReal() > op.GetReal());
        case VINT:
            return Value(GetReal() > op.GetInt());
        default:
            return Value();
        }
        break;
    case VBOOL:
        switch (b)
        {
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

Value Value::operator==(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();
    switch (a)
    {
    case VCHAR:
        switch (b)
        {
        case VINT:
            return Value(GetChar() == op.GetInt());
        case VCHAR:
            return Value(GetChar() == op.GetChar());
        default:
            return Value();
        }
        break;
    case VINT:
        switch (b)
        {
        case VINT:
            return Value(GetInt() == op.GetInt());
        case VREAL:
            return Value(GetInt() == op.GetReal());
        case VCHAR:
            return Value(GetInt() == op.GetChar());
        default:
            return Value();
        }
        break;
    case VSTRING:
        switch (b)
        {
        case VSTRING:
            return Value(GetString() == op.GetString());
        default:
            return Value();
        }
        break;
    case VREAL:
        switch (b)
        {
        case VREAL:
            return Value(GetReal() == op.GetReal());
        case VINT:
            return Value(GetReal() == op.GetInt());
        default:
            return Value();
        }
        break;
    case VBOOL:
        switch (b)
        {
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

Value Value::operator&&(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();
    switch (a)
    {
    case VBOOL:
        switch (b)
        {
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

Value Value::operator-(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();

    switch (a)
    {
    case VCHAR:
        switch (b)
        {
        case VCHAR:
            return Value(GetChar() - op.GetChar());
        case VINT:
            return Value(GetChar() - op.GetInt());
        case VREAL:
            return Value(GetChar() - op.GetReal());
        default:
            return Value();
        }
        break;
    case VINT:
        switch (b)
        {
        case VCHAR:
            return Value(GetInt() - op.GetChar());
        case VINT:
            return Value(GetInt() - op.GetInt());
        case VREAL:
            return Value(GetInt() - op.GetReal());
        default:
            return Value();
        }
        break;
    case VREAL:
        switch (b)
        {
        case VINT:
            return Value(GetReal() - op.GetInt());
        case VREAL:
            return Value(GetReal() - op.GetReal());
        case VCHAR:
            return Value(GetReal() - op.GetChar());
        default:
            return Value();
        }
        break;
    default:
        return Value();
    }
    return Value();
}

// numeric overloaded multiply this by op
Value Value::operator*(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();

    switch (a)
    {
    case VCHAR:
        switch (b)
        {
        case VCHAR:
            return Value(GetChar() * op.GetChar());
        case VINT:
            return Value(GetChar() * op.GetInt());
        case VREAL:
            return Value(GetChar() * op.GetReal());
        default:
            return Value();
        }
        break;
    case VINT:
        switch (b)
        {
        case VCHAR:
            return Value(GetInt() * op.GetChar());
        case VINT:
            return Value(GetInt() * op.GetInt());
        case VREAL:
            return Value(GetInt() * op.GetReal());
        default:
            return Value();
        }
        break;
    case VREAL:
        switch (b)
        {
        case VINT:
            return Value(GetReal() * op.GetInt());
        case VREAL:
            return Value(GetReal() * op.GetReal());
        case VCHAR:
            return Value(GetReal() * op.GetChar());
        default:
            return Value();
        }
        break;
    default:
        return Value();
    }
    return Value();
}

// numeric overloaded Remainder this by op
Value Value::operator%(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();

    switch (a)
    {
    case VINT:
        switch (b)
        {
        case VINT:
            return Value(GetInt() % op.GetInt());
            break;
        case VCHAR:
            return Value(GetInt() % op.GetChar());
            break;
        default:
            return Value();
        }
        break;
    case VCHAR:
        switch (b)
        {
        case VINT:
            return Value(GetChar() % op.GetInt());
            break;
        case VCHAR:
            return Value(GetChar() % op.GetChar());
            break;
        default:
            return Value();
        }
        break;
    default:
        return Value();
        break;
    }
    return Value();
}

// numeric overloaded divide this by oper
// NOTE MIGHT NEED TO THROW A RUNTIME ERROR FOR DIVISION BY 0
Value Value::operator/(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();

    switch (a)
    {
    case VCHAR:
        switch (b)
        {
        case VCHAR:
            return Value(GetChar() / op.GetChar());
        case VINT:
            if (op.GetInt() == 0)
            {
                return Value("ERROR: Cannot divide by 0");
            }
            return Value(GetChar() / op.GetInt());
        case VREAL:
            return Value(GetChar() / op.GetReal());
        default:
            return Value();
        }
        break;
    case VINT:
        switch (b)
        {
        case VCHAR:
            return Value(GetInt() / op.GetChar());
        case VINT:
            return Value(GetInt() / op.GetInt());
        case VREAL:
            return Value(GetInt() / op.GetReal());
        default:
            return Value();
        }
        break;
    case VREAL:
        switch (b)
        {
        case VINT:
            return Value(GetReal() / op.GetInt());
        case VREAL:
            return Value(GetReal() / op.GetReal());
        case VCHAR:
            return Value(GetReal() / op.GetChar());
        default:
            return Value();
        }
        break;
    default:
        return Value();
    }
    return Value();
}

// overloaded equality operator of this with op
// Value Value::operator==(const Value &op) const
// {
//     ValType a = GetType();
//     ValType b = op.GetType();

//     switch (a)
//     {
//     case VSTRING:
//         switch (b)
//         {
//         case VSTRING:
//             return Value(GetString() == op.GetString());
//         default:
//             return Value();
//         }
//         break;
//     case VCHAR:
//         switch (b)
//         {
//         case VCHAR:
//             return Value(GetChar() == op.GetChar());
//         case VINT:
//             return Value(GetChar() == op.GetInt());
//         case VREAL:
//             return Value(GetChar() == op.GetReal());
//         default:
//             return Value();
//         }
//         break;
//     case VBOOL:
//         switch(b){
//             case VBOOL:
//                 return Value(GetBool() == op.GetBool());
//             default:
//                 return Value();
//         }
//         break;
//     case VINT:
//         switch(b){
//             case VINT:
//                 return Value(GetInt() == op.GetInt());
//             case VREAL:
//                 return Value(GetInt() == op.GetReal());
//             case VCHAR:
//                 return Value(GetInt() == op.GetChar());
//             default:
//                 return Value();
//         }
//         break;
//     case VREAL:
//         switch(b){
//             case VREAL:
//                 return Value(GetReal() == op.GetReal());
//             case VINT:
//                 return Value(GetReal() == op.GetInt());
//             case VCHAR:
//                 return Value(GetReal() == op.GetChar());
//             default:
//                 return Value();
//         }
//         break;
//     default:
//         return Value();
//         break;
//     }
//     return Value();
// }
// overloaded inequality operator of this with op
Value Value::operator!=(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();

    switch (a)
    {
    case VSTRING:
        switch (b)
        {
        case VSTRING:
            return Value(GetString() != op.GetString());
        default:
            return Value();
        }
        break;
    case VCHAR:
        switch (b)
        {
        case VCHAR:
            return Value(GetChar() != op.GetChar());
        case VINT:
            return Value(GetChar() != op.GetInt());
        case VREAL:
            return Value(GetChar() != op.GetReal());
        default:
            return Value();
        }
        break;
    case VBOOL:
        switch (b)
        {
        case VBOOL:
            return Value(GetBool() != op.GetBool());
        default:
            return Value();
        }
        break;
    case VINT:
        switch (b)
        {
        case VINT:
            return Value(GetInt() != op.GetInt());
        case VREAL:
            return Value(GetInt() != op.GetReal());
        case VCHAR:
            return Value(GetInt() != op.GetChar());
        default:
            return Value();
        }
        break;
    case VREAL:
        switch (b)
        {
        case VREAL:
            return Value(GetReal() != op.GetReal());
        case VINT:
            return Value(GetReal() != op.GetInt());
        case VCHAR:
            return Value(GetReal() != op.GetChar());
        default:
            return Value();
        }
        break;
    default:
        return Value();
        break;
    }
    return Value();
}
// overloaded less than operator of this with op
Value Value::operator<(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();
    switch (a)
    {
    case VCHAR:
        switch (b)
        {
        case VINT:
            return Value(GetChar() < op.GetInt());
        case VCHAR:
            return Value(GetChar() < op.GetChar());
        default:
            return Value();
        }
        break;
    case VINT:
        switch (b)
        {
        case VINT:
            return Value(GetInt() < op.GetInt());
        case VREAL:
            return Value(GetInt() < op.GetReal());
        case VCHAR:
            return Value(GetInt() < op.GetChar());
        default:
            return Value();
        }
        break;
    case VSTRING:
        switch (b)
        {
        case VSTRING:
            return Value(GetString() < op.GetString());
        default:
            return Value();
        }
        break;
    case VREAL:
        switch (b)
        {
        case VREAL:
            return Value(GetReal() < op.GetReal());
        case VINT:
            return Value(GetReal() < op.GetInt());
        default:
            return Value();
        }
        break;
    case VBOOL:
        switch (b)
        {
        case VBOOL:
            return Value(GetBool() < op.GetBool());
        default:
            return Value();
        }
    default:
        return Value();
    }
    return Value();
}

// overloaded logical Oring operator of this with op
Value Value::operator||(const Value &op) const
{
    ValType a = GetType();
    ValType b = op.GetType();
    switch (a)
    {
    case VBOOL:
        switch (b)
        {
        case VBOOL:
            return Value(GetBool() || op.GetBool());
        default:
            return Value();
        }
    default:
        return Value();
    }
    return Value();
}
// overloaded logical Complement operator of this object
Value Value::operator!(void) const
{
    ValType a = GetType();
    if (a == VBOOL)
    {
        return Value(!GetBool());
    }
    return Value();
}