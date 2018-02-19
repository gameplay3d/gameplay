#include "Editor.h"

namespace gameplay
{

Editor editor;

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::onInitialize(int argc, const char* const* argv)
{
	Game::onInitialize(argc, argv);

	static const Input::Binding inputBindings[] =
	{
		{ Input::Key::KEY_Q,				Input::KeyModifier::KEY_MODIFIER_LEFT_CTRL,  1, nullptr, "exit"                              },
		{ Input::Key::KEY_Q,				Input::KeyModifier::KEY_MODIFIER_RIGHT_CTRL, 1, nullptr, "exit"                              },
		{ Input::Key::KEY_F,				Input::KeyModifier::KEY_MODIFIER_LEFT_CTRL,  1, nullptr, "graphics fullscreen"               },
		{ Input::Key::KEY_F,				Input::KeyModifier::KEY_MODIFIER_RIGHT_CTRL, 1, nullptr, "graphics fullscreen"               },
		{ Input::Key::KEY_RETURN,			Input::KeyModifier::KEY_MODIFIER_RIGHT_ALT,  1, nullptr, "graphics fullscreen"               },
		{ Input::Key::KEY_F1,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics stats"                    },
		{ Input::Key::KEY_F1,				Input::KeyModifier::KEY_MODIFIER_LEFT_CTRL,  1, nullptr, "graphics ifh"                      },
		{ Input::Key::KEY_GAMEPAD_START,	Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics stats"                    },
		{ Input::Key::KEY_F1,				Input::KeyModifier::KEY_MODIFIER_LEFT_SHIFT, 1, nullptr, "graphics stats 0\ngraphics text 0" },
		{ Input::Key::KEY_F3,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics wireframe"                },
		{ Input::Key::KEY_F4,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics hmd"                      },
		{ Input::Key::KEY_F4,				Input::KeyModifier::KEY_MODIFIER_LEFT_SHIFT, 1, nullptr, "graphics hmdrecenter"              },
		{ Input::Key::KEY_F4,				Input::KeyModifier::KEY_MODIFIER_LEFT_CTRL,  1, nullptr, "graphics hmddbg"                   },
		{ Input::Key::KEY_F6,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics profiler"                 },
		{ Input::Key::KEY_F7,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics vsync"                    },
		{ Input::Key::KEY_F8,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics msaa"                     },
		{ Input::Key::KEY_F9,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics flush"                    },
		{ Input::Key::KEY_F10,				Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics hidpi"                    },
		{ Input::Key::KEY_PRINT,			Input::KeyModifier::KEY_MODIFIER_NONE,       1, nullptr, "graphics screenshot"               },
		{ Input::Key::KEY_P,				Input::KeyModifier::KEY_MODIFIER_LEFT_CTRL,  1, nullptr, "graphics screenshot"               },
		{ Input::Key::KEY_NONE,				Input::KeyModifier::KEY_MODIFIER_NONE,		 0, nullptr,  nullptr }
	};
	Input::addBindings("system", inputBindings);

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
	
	// TODO:
}

void Editor::onFinalize()
{
	// TODO:

	Game::onFinalize();
}

void Editor::onResize(size_t width, size_t height)
{
	Game::onResize(width, height);

	// TODO:
}

void Editor::onUpdate(float elapsedTime)
{
	Game::onUpdate(elapsedTime);

	// TODO:

	bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()) );
	bgfx::touch(0);
	bgfx::frame();
}

}

GP_IMPLEMENT_MAIN(Editor);
