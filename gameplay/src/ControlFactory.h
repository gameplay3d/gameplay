#ifndef	CONTROLFACTORY_H_
#define	CONTROLFACTORY_H_

#include "ThemeStyle.h"

namespace gameplay 
{	

class Properties;
class Control;

/**
 * Represents a factory for creating core controls and registered custom controls.
 *
 * @script{ignore}
 */
class ControlFactory 
{
    friend class Game;

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
	 * @param controlName The name of the custom control to register.
	 * @param activator The activator for applying the style and properties to the control.
	 *
	 * @return true if the control was successfully registered.
	 */
	bool registerCustomControl(const char* controlName, ControlActivator activator);

	/**
	 * Unregisters a custom control and specify the activator.
	 *
	 * @param controlName The name of the custom control to unregister.
	 */
	void unregisterCustomControl(const char* controlName);

	/**
	 * Creates a controls from the set of core and custom controls registered.
	 *
	 * @param controlName The name of the control to create.
     * @param style The style to apply to the control.
     * @param properties The Properties object containing the definition of the controlo.
     * @return The newly created control.
	 */
	Control* createControl(const char* controlName, Theme::Style *style, Properties *properties);

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
    * Called when the game is shutting down to clean up resources.
    */
    static void finalize();

	/**
	 * Assignment operator
	 */
	ControlFactory &operator=(const ControlFactory&);
	
	/**
	 * Registers the standard (built-in) controls
	 */
	void registerStandardControls();

	std::map<std::string, ControlActivator>	_registeredControls;
};

}

#endif

