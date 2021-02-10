#pragma once

#include "Defines.h"
#include <functional>
#include <map>
#include <cstdint>

namespace gameplay
{
/**
 * Defines a signal for an event that can be emitted.
 *
 * Multiple function slots can be registered to any signals
 * which are added to classes to represent various event signals.
 *
 * usage:
 *
 * // create new signal
 * Signal<std::string, uint32_t> signal;
 *
 * // attach a slot
 * signal.connect([](std::string arg1, uint32_t arg2)
 * {
 *     std::cout << arg1 << " " << arg2 << std::endl;
 * });
 *
 * // emit a signal event
 * signal.emit("The answer:", 42);
 */
template <typename... Args>
class GP_API Signal
{
public:

    /**
     * Constructor
     */
    Signal(){};

    /**
     * Constructor.
     */
	Signal(Signal const&) 
	{
	}

    /**
     * Constructor.
     */
	Signal(Signal&& other) noexcept :
		_slots(std::move(other._slots)),
		_current_id(other._current_id)
	{
	}

    /**
     * Destructor.
     */
    ~Signal(){};

    Signal& operator=(Signal const& other)
	{
		if (this != &other)
		{
			disconnect_all();
		}
		return *this;
	}

	Signal& operator=(Signal&& other) noexcept
	{
		if (this != &other)
		{
			_slots = std::move(other._slots);
			_current_id = other._current_id;
		}
		return *this;
	}

	/**
	 * Connects a std::function to the signal.
	 *
	 * @param slot The callback function slot.
	 * @return The slot connection id. It can be used to disconnect the function.
	 */
	uint32_t connect(std::function<void(Args...)> const& slot) const
	{
		_slots.insert(std::make_pair(++_current_id, slot));
		return _current_id;
	}

	/**
	 * Connects a member function of an object instance to this Signal.
	 */
	template <typename T>
	uint32_t connect_member(T* inst, void (T::* func)(Args...))
	{
		return connect([=](Args... args)
		{
			(inst->*func)(args...);
		});
	}

	/**
	 * Connect a const member function of an object to this Signal.
	 */
	template <typename T>
	uint32_t connect_member(T* inst, void (T::* func)(Args...) const)
	{
		return connect([=](Args... args) { (inst->*func)(args...); });
	}

	/**
	 * Disconnects a previously connected function slot.
	 */
	void disconnect(uint32_t id) const
	{
		_slots.erase(id);
	}

	/**
	 * Disconnects all previously connected function slots.
	 */
	void disconnect_all() const
	{
		_slots.clear();
	}

	/**
	 * Emits and calls all connected functions.
	 */
	void emit(Args... p)
	{
		for (auto const& it : _slots) 
		{
			it.second(p...);
		}
	}

	/**
	 * Calls only one connected function.
	 * 
	 * @param id The id of the connected slot function to be called.
	 */
	void emit_for(uint32_t id, Args... p)
	{
		auto const& it = _slots.find(id);
		if (it != _slots.end())
		{
			it->second(p...);
		}
	}

	/**
	 * Emits and calls all connected functions except for one.
	 * 
	 * @param id The connnection id to ignore.
	 */
	void emit_for_all_but_one(uint32_t id, Args... p)
	{
		for (auto const& it : _slots)
		{
			if (it.first != id)
			{
				it.second(p...);
			}
		}
	}

private:
	mutable std::map<uint32_t, std::function<void(Args...)>> _slots;
	mutable uint32_t _current_id{ 0 };
};
}
