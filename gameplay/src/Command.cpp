#include "Base.h"
#include "Command.h"

#include <bx/allocator.h>
#include <bx/commandline.h>
#include <bx/hash.h>
#include <bx/string.h>

namespace gameplay
{ 

struct CommandContext
{
	CommandContext()
	{
	}

	~CommandContext()
	{
	}

	void add(const char* name, ConsoleFn fn, void* userData)
	{
		uint32_t cmd = bx::hash<bx::HashMurmur2A>(name, (uint32_t)bx::strLen(name) );
		BX_CHECK(m_lookup.end() == m_lookup.find(cmd), "Command \"%s\" already exist.", name);
		Func func = { fn, userData };
		_lookup.insert(std::make_pair(cmd, func) );
	}

	void exec(const char* cmd)
	{
		for (const char* next = cmd; '\0' != *next; cmd = next)
		{
			char commandLine[1024];
			uint32_t size = sizeof(commandLine);
			int argc;
			char* argv[64];
			next = bx::tokenizeCommandLine(cmd, commandLine, size, argc, argv, BX_COUNTOF(argv), '\n');
			if (argc > 0)
			{
				int err = -1;
				uint32_t command = bx::hash<bx::HashMurmur2A>(argv[0], (uint32_t)bx::strLen(argv[0]) );
				std::unordered_map<uint32_t, Func>::iterator it = _lookup.find(command);
				if (it != _lookup.end() )
				{
					Func& fn = it->second;
					err = fn.fn(this, fn.userData, argc, argv);
				}

				switch (err)
				{
				case 0:
					break;

				case -1:
					{
						std::string tmp(cmd, next-cmd - (*next == '\0' ? 0 : 1) );
						GP_WARN("Command '%s' doesn't exist.", tmp.c_str() );
					}
					break;

				default:
					{
						std::string tmp(cmd, next-cmd - (*next == '\0' ? 0 : 1) );
						GP_WARN("Failed '%s' err: %d.", tmp.c_str(), err);
					}
					break;
				}
			}
		}
	}

	struct Func
	{
		ConsoleFn fn;
		void* userData;
	};


	std::unordered_map<uint32_t, Func>  _lookup;
};

static CommandContext* __cmdContext;

void cmdInit()
{
	__cmdContext = new CommandContext();
}

void cmdShutdown()
{
	delete __cmdContext;
}

void cmdAdd(const char* name, ConsoleFn fn, void* userData)
{
	__cmdContext->add(name, fn, userData);
}

void cmdExec(const char* format, ...)
{
	char tmp[2048];

	va_list argList;
	va_start(argList, format);
	bx::vsnprintf(tmp, BX_COUNTOF(tmp), format, argList);
	va_end(argList);

	__cmdContext->exec(tmp);
}

}


