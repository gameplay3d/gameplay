#ifndef FUNCTIONBINDING_H_
#define FUNCTIONBINDING_H_

#include "Base.h"

/**
 * Represents a binding from a function or variable to a function within the script runtime.
 */
struct FunctionBinding
{
    /**
     * Represents the type of the binding.
     */
    enum Type
    {
        GLOBAL_FUNCTION,
        GLOBAL_VARIABLE,
        GLOBAL_CONSTANT,
        STATIC_FUNCTION,
        STATIC_VARIABLE,
        STATIC_CONSTANT,
        MEMBER_FUNCTION,
        MEMBER_VARIABLE,
        MEMBER_CONSTANT
    };

    /**
     * Represents either a parameter or a return value.
     */
    struct Param
    {
        /**
         * Represents the type of parameter or return value.
         */
        enum Type
        {
            TYPE_CONSTRUCTOR,
            TYPE_DESTRUCTOR,
            TYPE_VOID,
            TYPE_BOOL,
            TYPE_CHAR,
            TYPE_SHORT,
            TYPE_INT,
            TYPE_LONG,
            TYPE_UCHAR,
            TYPE_USHORT,
            TYPE_UINT,
            TYPE_ULONG,
            TYPE_FLOAT,
            TYPE_DOUBLE,
            TYPE_STRING,
            TYPE_OBJECT,
            TYPE_ENUM,
            TYPE_UNRECOGNIZED,
            TYPE_VARARGS
        };

        /** 
         * Represents the kind or nature of the parameter/return value.
         */
        enum Kind
        {
            KIND_VALUE,
            KIND_REFERENCE,
            KIND_POINTER
        };

        /**
         * Constructor.
         * 
         * @param type The type of the parameter.
         * @param kind The kind of the parameter.
         * @param info The extra info of the parameter (optional).
         */
        Param(Type type = TYPE_VOID, Kind kind = KIND_VALUE, const string& info = "");

        /**
         * Equality operator.
         * 
         * @param p The param to compare to.
         * @return True if the parameters are equal; false otherwise.
         */
        bool operator==(const Param& p) const;

        /**
         * Inequality operator.
         * 
         * @param p The param to compare to.
         * @return True if the parameters are not equal; false otherwise.
         */
        bool operator!=(const Param& p) const;

        /** The type of the parameter. */
        Type type;
        /** The kind of the parameter. */
        Kind kind;
        /** Extra info (either the class name for a pointer 
        type or whether a string type is a std::string or a C-string). */
        string info;
        /** Whether the parameter has a default value. */
        bool hasDefaultValue;
        /** For pointer types, the number of levels of indirection */
        int levelsOfIndirection;
    };

    /**
     * Constructor.
     *
     * @param classname The name of the class that owns this binding.
     * @param uniquename The class's unique name (used only in the script bindings).
     */
    FunctionBinding(string classname = "", string uniquename = "");

    /**
     * Retrieves the C++ function name for the script binding.
     * 
     * @return The C++ function name for the script binding.
     */
    string getFunctionName() const;

    /**
     * Gets the minimum number of parameters needed to call 
     * the function that the binding corresponds to.
     * 
     * @return The minimum number of parameters.
     */
    unsigned int getMinParamCount() const;

    /**
     * Writes out the script binding for the given set of binding objects
     * (the given set corresponds to all overloaded versions of a function).
     * 
     * @param o The output stream to write to.
     * @param bindings The set of binding objects.
     */
    static void write(ostream& o, const vector<FunctionBinding>& bindings);

    /**
     * Retrieves whether the given two bindings have matching signatures.
     * 
     * @param b1 The first binding.
     * @param b2 The second binding.
     * @return True if the signatures match; false otherwise.
     */
    static bool signaturesMatch(const FunctionBinding& b1, const FunctionBinding& b2);

    /** The binding's type. */
    Type type;
    /** The binding's return value. */
    Param returnParam;
    /** The binding's parameters. */
    vector<Param> paramTypes;
    /** The name of the class that owns this binding. */
    string classname;
    /** Holds the class's unique name (used only in the script bindings). */
    string uniquename;
    /** The name of the binding (function or variable name). */
    string name;
    /** Holds the unique name of the C++ function that implements the Lua binding. */
    mutable string functionName;
    /** Holds whether the binding's return value is a object that is owned by whomever calls the function. */
    bool own;
};

ostream& operator<<(ostream& o, const FunctionBinding::Param& param);

#endif
