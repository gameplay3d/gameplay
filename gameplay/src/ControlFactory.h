#ifndef	CONTROLFACTORY_H_
#define	CONTROLFACTORY_H_

#include <map>
#include <string>

#include "Theme.h"

namespace gameplay {
	
	class Properties;
	class Control;

	class ControlFactory {
		
		public :
			typedef Control *(*ControlActivator)(Theme::Style *, Properties *, Theme *);

			static ControlFactory	&getInstance();

			bool					registerCustomControl(const std::string &controlName, ControlActivator controlCreator);
			void					unregisterCustomControl(const std::string &controlName);
			Control					*createControl(const std::string &controlName, Theme::Style *style, Properties *properties, Theme *theme = NULL);

		private :
			std::map<std::string, ControlActivator>	_registeredControls;
			
			ControlFactory();
			ControlFactory(const ControlFactory &);
			~ControlFactory();

			ControlFactory &operator=(const ControlFactory &);
	
			void 			registerStdControls();
	};
}

#endif	//CONTROLFACTORY_H_

