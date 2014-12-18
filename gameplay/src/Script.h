#ifndef SCRIPT_H_
#define SCRIPT_H_

#include "Base.h"
#include "Ref.h"

namespace gameplay
{

/**
 * Represents a script that has been loaded by the scripting system.
 */
class Script : public Ref
{
    friend class ScriptController;

public:

    /**
     * Defines the scope of a script environment.
     */
    enum Scope
    {
        /**
         * Global scripts execute in a single shared environment that is accessible to
         * all other scripts in the system. Global scripts can use code from other global
         * scripts and other scripts can use code from a global script.
         *
         * Scripts that execute in the global environment must be careful to not override
         * other variables or functions that already exist in the global environment (unless
         * this is intended). Because of this, script naming conventions and the use of 
         * unique namespaces is important to prevent naming collisions with other global code.
         */
        GLOBAL,

        /**
         * Protected scripts execute in a more limited sandbox environment that by default does
         * not allow other scripts to see their variables or functions. Variables and functions
         * in a protected script can be named the same as those in other scripts without 
         * collision issues.
         *
         * Although global code cannot access protected scripts, protected scripts can acceess
         * global code. Similarly, protected scripts can expose variables and functions to the
         * global environment using explicit notation, although the same precautions noted
         * for the GLOBAL scope should be used when doing this, to prevent naming collisions.
         *
         * Protected scripts are best used when associated with a single game object, since
         * these scripts are not cached the same way global scripts are. Each time a protected
         * script is loaded, a new instance of the script is loaded. This allows protected 
         * scripts to store per-instance state, since it will not be shared by multiple instances.
         *
         * @see ScriptTarget
         */
        PROTECTED
    };

    /**
     * Returns the path from which this Script was loaded.
     *
     * @return The script's path.
     */
    const char* getPath() const;

    /**
     * Returns the scope of this script.
     *
     * @return The scope of this script.
     */
    Scope getScope() const;

    /**
     * Determines if a function with the given name exists in the script.
     *
     * @param name Name of the function.
     *
     * @return True if the function exists, false otherwise.
     */
    bool functionExists(const char* name) const;

    /** 
     * Reloads this script.
     *
     * The code in the script will be reloaded and any functions and variables will be overwritten
     * into the script's envrionment.
     *
     * If the script is GLOBAL in scope, it will simply be re-run, replacing any existing global 
     * variables and functions. If the script has changed, any previous variables or functions that
     * do not exist in the updated script will remain in the global environment.
     *
     * @return True if the script is successfully reloaded, false otherwise.
     */
    bool reload();

private:

    /**
     * Constructor.
     */
    Script();

    /**
     * Copy constructor (hidden).
     */
    Script(const Script& copy);

    /**
     * Destructor.
     */
    ~Script();

    std::string _path;
    Scope _scope;
    int _env;

};

}

#endif
