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
         * Private scripts execute in a more limited sandbox environment that by default does
         * not  allow other scripts to see their variables or functions. Variables and functions
         * in a private script can be named the same as those in other scripts without 
         * collision issues.
         *
         * Although global code cannot access private scripts, private scripts can acceess
         * global code. Similarly, private scripts can expose variables and functions to the
         * global environment using explicit notation, although the same precautions noted
         * for the GLOBAL scope should be used when doing this, to prevent naming collisions.
         *
         * PRIVATE_SHARED scripts can have at most once instance of the script loaded into
         * memory, with all objects referencing the script sharing it. This means that shared
         * scripts may execute for multiple objects and therefore do not have any instance 
         * information available to them other than what is passed through function callbacks.
         * Similarly, shared scripts should not attempt to store any per-instance state.
         */
        PRIVATE_SHARED,

        /**
         * PRIVATE_INSTANCE scripts provide the same sandboxed envrionment that PRIVATE_SHARED
         * scripts do. However, there may be multiple instances of the same PRIVATE_INSTANCE
         * script loaded into memory at the same time: one for each object that references it.
         * Because of this, instance scripts can be used to store per-instance state, since it
         * will not be shared by multiple instances.
         *
         * In some cases, such as when a script is loaded by a ScriptTarget object, the script
         * may have direct access to the owning object by using the "this" keyword.
         *
         * @see ScriptTarget
         */
        PRIVATE_INSTANCE
    };

protected:

    /**
     * Copy constructor (hidden).
     */
    Script(const Script& copy);

    /**
     * Destructor.
     */
    ~Script();

private:

    std::string _path;
    Scope _scope;
    int _env;

};

}

#endif SCRIPT_H_
