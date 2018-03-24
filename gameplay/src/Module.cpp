#include "Module.h"

#include <iostream>
#include <dlfcn.h>


namespace gameplay
{

typedef IBehaviour* createInstance();

Module::Module(const std::string& libName, void* library)
{
    _libName = libName;
    _content = library;
}

Module::~Module()
{
    dlclose(_content);
}

IBehaviour* Module::createBehaviour(const std::string& className)
{
    using std::cerr;

    std::string functionGenerationName = "create" + className;
    createInstance* instantiateBehaviour = (createInstance*) dlsym(_content, functionGenerationName.c_str());
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return nullptr;
    }

    return instantiateBehaviour();
}

}
