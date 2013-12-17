#ifndef	CONTROLFACTORY_H_
#define	CONTROLFACTORY_H_

#include "Theme.h"

namespace gameplay 
{	

class Properties;
class Control;

/**
 * Defines a factory for creating core controls and registered custom controls.
 *
 * @script{ignore}
 */
class ControlFactory 
{
    friend class Game;
	friend class Container;

public:

	/**
	 * The activator interface for controls that are created.
	 */
	typedef Control* (*ControlActivator)(Theme::Style*, Properties*);

	/**
	 * Gets the single instance of the control factory used to create controls and register/unregister custom controls.
	 *
	 * @return The instance of the ControlFactory.
	 */
	static ControlFactory* getInstance();

	/**
	 * Registers a custom control and specify the activator.
	 *
	 * @param typeName The name of the custom control type to register.
	 * @param activator The activator for applying the style, properties and theme to the control.
	 *
	 * @return true if the control was successfully registered.
	 */
    bool registerCustomControl(const char* typeName, ControlActivator activator);

	/**
	 * Unregisters a custom control and specify the activator.
	 *
	 * @param typeName The name of the custom control type to unregister.
	 */
    void unregisterCustomControl(const char* typeName);

private:

	/**
	 * Constructor.
	 */
	ControlFactory();

    /**
	 * Constructor.
	 */
	ControlFactory(const ControlFactory& copy);

	/**
	 * Destructor.
	 */
	~ControlFactory();

    /**
     * Cleans up resources allocated by the ControlFactory.
     */
    static void finalize();

	/**
	 * Assignment operator
	 */
	ControlFactory &operator=(const ControlFactory&);

	/**
	* Creates a controls from the set of core and custom controls registered.
	*
	* @param typeName The type of the control to create.
	* @param style The style to apply to the control.
	* @param properties A Properties object describing the control (optional).
	* @return The new control.
	*/
    Control* createControl(const char* typeName, Theme::Style *style, Properties *properties = NULL);

    /**
	 * Registers the standard (built-in) controls
	 */
	void registerStandardControls();

	std::map<std::string, ControlActivator>	_registeredControls;
};

}

#endif
