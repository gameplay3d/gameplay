#ifndef __PASSWORD_TEXT_BOX__
#define __PASSWORD_TEXT_BOX__

#include "TextBox.h"

namespace gameplay
{
	/**
	 * An editable password text label.  Tap or click within the text box to bring up the
	 * virtual keyboard.
	 *
	 * Listeners can listen for a TEXT_CHANGED event, and then query the text box
	 * for the last keypress it received.
	 *
	 * The following properties are available for text boxes:

	 @verbatim
		label <labelID>
		{
			 style       = <styleID>
			 alignment   = <Control::Alignment constant> // Note: 'position' will be ignored.
			 position    = <x, y>
			 autoWidth   = <bool>
			 autoHeight  = <bool>
			 size        = <width, height>
			 width       = <width>   // Can be used in place of 'size', e.g. with 'autoHeight = true'
			 height      = <height>  // Can be used in place of 'size', e.g. with 'autoWidth = true'
			 text        = <string>
		}
	 @endverbatim
	 */
	class PasswordTextBox : public TextBox
	{
		friend class Container;

	public :

		/**
		 * Create a new password text box control.
		 *
		 * @param id The control's ID.
		 * @param style The control's style.
		 *
		 * @return The new password text box.
		 * @script{create}
		 */
		static PasswordTextBox* create(const char* id, Theme::Style* style);
		
		/**
		 * @see Control::getType
		 */
		const char* getType() const;

		/**
		 * Get the text displayed by this label.
		 *
		 * @return The text displayed by this password text box.
		 */
		const char* getText();

	protected:
		 /**
		 * Constructor.
		 */
		PasswordTextBox();

		 /**
		 * Destructor.
		 */
		~PasswordTextBox();

		/**
		 * Create a password text box with a given style and properties.
		 *
		 * @param style The style to apply to this password text box.
		 * @param properties The properties to set on this password text box.
		 *
		 * @return The new password text box.
		 */
		static PasswordTextBox* create(Theme::Style* style, Properties* properties);

		/**
		 * Handle character deletion in the textbox
		 *
		 * @param index of the deleted character
		 * @param count the number of deleted characters
		 *
		 * @return The new password text box.
		 */
		virtual void deleteChar(int index);

		/**
		 * Handle character insertion in the textbox
		 *
		 * @param index of the inserted character
		 * @param count the number of inserted characters
		 * @param characted inserted
		 *
		 * @return The new password text box.
		 */
		virtual void insertChar(int index, char character);

		/**
		 * The password behind the label displayed by this control.
		 */
		std::string _passwordText;

	private:
		PasswordTextBox(const PasswordTextBox& copy);
	};
}

#endif
