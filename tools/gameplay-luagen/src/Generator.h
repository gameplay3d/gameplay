#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "Base.h"
#include "ClassBinding.h"
#include "EnumBinding.h"
#include "FunctionBinding.h"
#include "Generator.h"
#include "TypedefBinding.h"

#include <tinyxml2.h>
using namespace tinyxml2;

/**
 * Generates script bindings.
 */
class Generator
{
public:

    /**
     * Retrieves the global instance of the generator.
     * 
     * @return The global instance of the generator.
     */
    static Generator* getInstance();

    /**
     * Releases the global instance of the generator.
     */
    static void releaseInstance();

    /**
     * Retrieves the unique name for the given identifier (class, struct, enum, etc.).
     * 
     * @param name The identifier's name.
     * @return The unique name.
     */
    static string getUniqueName(string name);

    /**
     * Retrieves the unique name for the given ref id.
     * 
     * @param refId The ref id.
     * @return The unique name.
     */
    string getUniqueNameFromRef(string refId);

    /**
     * Gets the scope path for a given fully resolved classname.
     * 
     * @param classname The class name.
     * @param ns The namespace of the class if it has one.
     * @return The scope path.
     */
    static vector<string> getScopePath(string classname, string ns = string());

    /**
     * Retrieves the class/struct/enum name for the given ref id.
     * 
     * @param refId The ref id.
     * @return The class/struct/enum name.
     */
    string getIdentifier(string refId);

    /**
     * Gets the given class name (without the namespace) and stores the class's namespace in the given string pointer (if non-null).
     * 
     * @param classname The fully qualified name of the class.
     * @param ns Output variable for the namespace of the class.
     * @return The class name without the namespace.
     */
    string getClassNameAndNamespace(string classname, string* ns);

    /**
     * Sets the given ref id and class/struct/enum name pair.
     * 
     * @param refId The ref id.
     * @param classname The class/struct/enum name.
     */
    void setIdentifier(string refId, string classname);

    /**
     * Generates Lua bindings for all classes defined within the given input directory
     * and places the generated class bindings in the given output directory.
     * Note that the input directory must point to a folder containing the Doxygen XML 
     * output files for the classes that the user wants to generate bindings for.
     * 
     * @param inDir The input directory (must contain Doxygen XML output files).
     * @param outDir The output directory.
     * @param bindingNS The namespace to generate the bindings within.
     */
    void run(string inDir, string outDir, string* bindingNS = NULL);

    /**
     * Retrieves whether the given class is derived from Ref.
     * 
     * @param classname The name of the class.
     * @return True if the class is derived from Ref; false otherwise.
     */
    bool isRef(string classname);

protected:
    /**
     * Constructor.
     */
    Generator();

    /**
     * Destructor.
     */
    ~Generator();

    // Checks if the given class name specifies a class derived from the class represented by the given class binding.
    bool isDerived(const ClassBinding& c, string classname);

    // Gets the name of the namespace, file, class, or struct.
    string getCompoundName(XMLElement* node);

    // Parses the non-member functions and adds them to the list (optionally within the given namespace).
    void getFunctions(XMLElement* classNode, string ns = "");

    // Parses the namespace (with the given name).
    void getNamespace(XMLElement* nsNode, const string& name);

    // Parses the class (with the given name) and adds it to the list.
    void getClass(XMLElement* classNode, const string& name);

    // Parses all enumerations within a file (optionally within a namespace).
    void getEnums(XMLElement* fileNode, string ns = "");
    
    // Parses the enumeration type and adds it to the global list.
    void getEnum(XMLElement* e, string classname = "", string ns = "", string include = "");

    // Parses all typedefs within a file (optionall within a namespace).
    void getTypedefs(XMLElement* fileNode, string ns = "");

    // Parses the typedef and adds it to the global list.
    void getTypedef(XMLElement* e, string classname = "", string ns = "");

    // Gets the script flag if it is specified.
    string getScriptFlag(XMLElement* e);

    // Gets whether the current function has been marked as a creation function
    // (indicating that the return value is owned by the script runtime and not C++).
    void getCreateFlag(XMLElement* e, FunctionBinding& b);

    // Gets the name of the binding.
    string getName(XMLElement* e);

    // Gets whether the type is const or not.
    bool getIsConstType(XMLElement* e);

    // Gets the parameter or return value, optionally within the scope of the given class.
    FunctionBinding::Param getParam(XMLElement* e, bool isVariable = false,  string classname = "");

    // Gets all of the parameters for a function.
    void getParams(XMLElement* e, FunctionBinding& b);

    // Resolves members for all classes' derived from class 'c'.
    void resolveMembers(const ClassBinding& c);

    // Resolves the inheritance.
    void resolveInheritance();

    // Resolves includes for all classes' derived from class 'c'.
    void resolveIncludes(const ClassBinding& c);

    // Resolves inherited include files.
    void resolveInheritedIncludes();

    // Resolves the type for a single parameter/return value 
    // (also takes the function that the parameter/return value is for along
    // with the header file that the parameter/return value belongs to-the original
    // class header file for class bindings or the global Lua bindings header file
    // for global bindings).
    void resolveType(FunctionBinding::Param* param, string functionName, string header);

    // Resolves all unrecognized types.
    void resolveTypes();

    // Generates the bindings to C++ header and source files.
    void generateBindings(string* bindingNS);
    
    // Gets the set off all classes that derives from the given class.
    void getAllDerived(set<string>& derived, string classname);

    // Gets the included files for a cpp file.
    void getIncludes(XMLElement* e, string filename);

private:
    static Generator* __instance;

    const char* _file;
    map<string, string> _refIds;
    string _outDir;
    map<string, ClassBinding> _classes;
    vector<string> _topLevelBaseClasses;
    map<string, set<string> > _includes;
    map<string, vector<FunctionBinding> > _functions;
    map<string, EnumBinding> _enums;
    map<string, set<string> > _namespaces;
    map<string, TypedefBinding> _typedefs;
    set<string> __warnings;
};

#endif
