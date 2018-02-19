#pragma once

namespace gameplay
{ 

struct CommandContext;
typedef int (*ConsoleFn)(CommandContext* context, void* userData, int argc, char const* const* argv);

void cmdInit();


void cmdShutdown();


void cmdAdd(const char* name, ConsoleFn fn, void* _userData = nullptr);


void cmdExec(const char* format, ...);

}