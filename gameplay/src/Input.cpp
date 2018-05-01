#include "Base.h"
#include "Input.h"
#include "SerializerJson.h"

namespace gameplay
{

static std::string __keyModifierNames[] =
{
    "None",
    "LeftAlt",
    "RightAlt",
    "LeftCtrl",
    "RightCtrl",
    "LeftShift",
    "RightShift",
    "LeftMeta",
    "RightMeta"
};

static std::string __mouseButtonNames[] =
{
    "None",
    "LeftMouseButton",
    "RightMouseButton",
    "MiddleMouseButton"
};

static std::string __mouseAxisNames[] =
{
    "X",
    "Y",
    "ScrollWheel"
};

static std::string __gamepadAxisNames[] =
{
    "LeftX",
    "LeftY",
    "LeftZ",
    "RightX",
    "RightY",
    "RightZ"
};

static std::string __keyNames[] =
{
    "None",
    "Esc",
    "Return",
    "Tab",
    "Space",
    "Backspace",
    "Up",
    "Down",
    "Left",
    "Right",
    "Insert",
    "Delete",
    "Home",
    "End",
    "PageUp",
    "PageDown",
    "Print",
    "Plus",
    "Minus",
    "LeftBracket",
    "RightBracket",
    "Semicolon",
    "Quote",
    "Comma",
    "Period",
    "Slash",
    "Backslash",
    "Tilde",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "NumPad0",
    "NumPad1",
    "NumPad2",
    "NumPad3",
    "NumPad4",
    "NumPad5",
    "NumPad6",
    "NumPad7",
    "NumPad8",
    "NumPad9",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "GamepadA",
    "GamepadB",
    "GamepadX",
    "GamepadY",
    "GamepadThumbL",
    "GamepadThumbR",
    "GamepadShoulderL",
    "GamepadShoulderR",
    "GamepadUp",
    "GamepadDown",
    "GamepadLeft",
    "GamepadRight",
    "GamepadBack",
    "GamepadStart",
    "GamepadGuide",
};

std::string Input::getKeyName(Key key)
{
    GP_ASSERT(key < Key::eCount);
    return __keyNames[static_cast<uint32_t>(key)];
}

std::string Input::getMouseButtonName(MouseButton mouseButton)
{
    GP_ASSERT(mouseButton < MouseButton::eCount);
    return __mouseButtonNames[static_cast<uint32_t>(mouseButton)];
}

std::string Input::getKeyModifierNames(KeyModifiers modifiers)
{
    std::string names;
    uint8_t keyModifiers = static_cast<uint8_t>(modifiers);
    for (uint8_t modifierMask = 1,  modifierIndex = 1; modifierMask < static_cast<uint32_t>(KeyModifiers::eCount); modifierMask <<= 1, ++modifierIndex)
    {
        if((modifierMask & keyModifiers) == modifierMask)
        {
            if(!names.empty())
            {
                names += " ";
            }
            names += __keyModifierNames[modifierIndex];
        }
    }
    return names;
}

std::string Input::getMouseAxisName(MouseAxis mouseAxis)
{
    return __mouseAxisNames[static_cast<uint32_t>(mouseAxis)];
}

std::string Input::getGamepadAxisName(GamepadAxis gamepadAxis)
{
    return __gamepadAxisNames[static_cast<uint32_t>(gamepadAxis)];
}

std::string Input::enumToString(const std::string& enumName, int value)
{
    std::string result;
    if (enumName.compare("gameplay::Input::KeyModifiers") == 0)
    {
        std::vector<std::string> names;
        if (value == static_cast<int>(KeyModifiers::eNone))
        {
            names.push_back("eNone");
        }
        else
        {
            for(int i = 1; i < static_cast<int>(KeyModifiers::eCount); i <<= 1)
            {
                switch (i & value)
                {
                case static_cast<int>(KeyModifiers::eLeftAlt):
                    names.push_back("eLeftAlt");
                    break;
                case static_cast<int>(KeyModifiers::eRightAlt):
                    names.push_back("eRightAlt");
                    break;
                case static_cast<int>(KeyModifiers::eLeftCtrl):
                    names.push_back("eLeftCtrl");
                    break;
                case static_cast<int>(KeyModifiers::eRightCtrl):
                    names.push_back("eRightCtrl");
                    break;
                case static_cast<int>(KeyModifiers::eLeftShift):
                    names.push_back("eLeftShift");
                    break;
                case static_cast<int>(KeyModifiers::eRightShift):
                    names.push_back("eRightShift");
                    break;
                case static_cast<int>(KeyModifiers::eLeftMeta):
                    names.push_back("eLeftMeta");
                    break;
                case static_cast<int>(KeyModifiers::eRightMeta):
                    names.push_back("eRightMeta");
                    break;
                }
            }
        }
        if (names.empty())
        {
            names.push_back("eNone");
        }
        for(auto itr = names.begin(); itr != names.end(); ++itr)
        {
            if (itr != names.begin())
            {
                result += '|';
            }
            result += *itr;
        }
    }
    else if (enumName.compare("gameplay::Input::GamepadAxis") == 0)
    {
        switch (value)
        {
            case static_cast<int>(GamepadAxis::eLeftX):
                result = "eLeftX";
                break;
            case static_cast<int>(GamepadAxis::eLeftY):
                result = "eLeftY";
                break;
            case static_cast<int>(GamepadAxis::eLeftZ):
                result = "eLeftZ";
                break;
            case static_cast<int>(GamepadAxis::eRightX):
                result = "eRightX";
                break;
            case static_cast<int>(GamepadAxis::eRightY):
                result = "eRightY";
                break;
            case static_cast<int>(GamepadAxis::eRightZ):
                result = "eRightZ";
                break;
            default:
                result = "eLeftX";
                break;
        }
    }
    else if (enumName.compare("gameplay::Input::MouseAxis") == 0)
    {
        switch (value)
        {
            case static_cast<int>(MouseAxis::eX):
                result = "eX";
                break;
            case static_cast<int>(MouseAxis::eY):
                result = "eY";
                break;
            case static_cast<int>(MouseAxis::eScrollWheel):
                result = "eScrollWheel";
                break;
            default:
                result = "eX";
                break;
        }
    }
    else if (enumName.compare("gameplay::Input::Mapping::Type") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Mapping::Type::eKey):
                result = "eKey";
                break;
            case static_cast<int>(Mapping::Type::eMouseButton):
                result = "eMouseButton";
                break;
            case static_cast<int>(Mapping::Type::eMouseAxis):
                result = "eMouseAxis";
                break;
            case static_cast<int>(Mapping::Type::eGamepadAxis):
                result = "eGamepadAxis";
                break;
            default:
                result = "eKey";
                break;
        }
    }
    else if (enumName.compare("gameplay::Input::Mapping::Action") == 0)
    {
        switch (value)
        {
        case static_cast<int>(Mapping::Action::ePositive) :
            result = "ePositive";
            break;
        case static_cast<int>(Mapping::Action::eNegative) :
            result = "eNegative";
            break;
        default:
            result = "ePositive";
            break;
        }
    }
    else if (enumName.compare("gameplay::Input::Mapping::ActionProfile") == 0)
    {
        switch (value)
        {
        case static_cast<int>(Mapping::ActionProfile::ePrimary) :
            result = "ePrimary";
            break;
        case static_cast<int>(Mapping::ActionProfile::eSecondary) :
            result = "eSecondary";
            break;
        default:
            result = "ePrimary";
            break;
        }
    }
    else if (enumName.compare("gameplay::Input::MouseButton") == 0)
    {
        switch (value)
        {
            case static_cast<int>(MouseButton::eNone):
                result = "eNone";
                break;
            case static_cast<int>(MouseButton::eLeft):
                result = "eLeft";
                break;
            case static_cast<int>(MouseButton::eRight):
                result = "eRight";
                break;
            case static_cast<int>(MouseButton::eMiddle):
                result = "eMiddle";
                break;
            default:
                result = "eNone";
                break;
        }
    }
    else if (enumName.compare("gameplay::Input::Key") == 0)
    {
        switch (value)
        {
            case static_cast<int>(Key::eNone):
                result = "eNone";
                break;
            case static_cast<int>(Key::eEsc):
                result = "eEsc";
                break;
            case static_cast<int>(Key::eReturn):
                result = "eReturn";
                break;
            case static_cast<int>(Key::eTab):
                result = "eTab";
                break;
            case static_cast<int>(Key::eSpace):
                result = "eSpace";
                break;
            case static_cast<int>(Key::eBackspace):
                result = "eBackspace";
                break;
            case static_cast<int>(Key::eUp):
                result = "eUp";
                break;
            case static_cast<int>(Key::eDown):
                result = "eDown";
                break;
            case static_cast<int>(Key::eLeft):
                result = "eLeft";
                break;
            case static_cast<int>(Key::eRight):
                result = "eRight";
                break;
            case static_cast<int>(Key::eInsert):
                result = "eInsert";
                break;
            case static_cast<int>(Key::eDelete):
                result = "eDelete";
                break;
            case static_cast<int>(Key::eHome):
                result = "eHome";
                break;
            case static_cast<int>(Key::eEnd):
                result = "eEnd";
                break;
            case static_cast<int>(Key::ePageUp):
                result = "ePageUp";
                break;
            case static_cast<int>(Key::ePageDown):
                result = "ePageDown";
                break;
            case static_cast<int>(Key::ePrint):
                result = "ePrint";
                break;
            case static_cast<int>(Key::ePlus):
                result = "ePlus";
                break;
            case static_cast<int>(Key::eMinus):
                result = "eMinus";
                break;
            case static_cast<int>(Key::eLeftBracket):
                result = "eLeftBracket";
                break;
            case static_cast<int>(Key::eRightBracket):
                result = "eRightBracket";
                break;
            case static_cast<int>(Key::eSemiColon):
                result = "eSemiColon";
                break;
            case static_cast<int>(Key::eQuote):
                result = "eQuote";
                break;
            case static_cast<int>(Key::eComma):
                result = "eComma";
                break;
            case static_cast<int>(Key::ePeriod):
                result = "ePeriod";
                break;
            case static_cast<int>(Key::eSlash):
                result = "eSlash";
                break;
            case static_cast<int>(Key::eBackslash):
                result = "eBackslash";
                break;
            case static_cast<int>(Key::eTilde):
                result = "eTilde";
                break;
            case static_cast<int>(Key::eF1):
                result = "eF1";
                break;
            case static_cast<int>(Key::eF2):
                result = "eF2";
                break;
            case static_cast<int>(Key::eF3):
                result = "eF3";
                break;
            case static_cast<int>(Key::eF4):
                result = "eF4";
                break;
            case static_cast<int>(Key::eF5):
                result = "eF5";
                break;
            case static_cast<int>(Key::eF6):
                result = "eF6";
                break;
            case static_cast<int>(Key::eF7):
                result = "eF7";
                break;
            case static_cast<int>(Key::eF8):
                result = "eF8";
                break;
            case static_cast<int>(Key::eF9):
                result = "eF9";
                break;
            case static_cast<int>(Key::eF10):
                result = "eF10";
                break;
            case static_cast<int>(Key::eF11):
                result = "eF11";
                break;
            case static_cast<int>(Key::eF12):
                result = "eF12";
                break;
            case static_cast<int>(Key::eNumpad0):
                result = "eNumpad0";
                break;
            case static_cast<int>(Key::eNumpad1):
                result = "eNumpad1";
                break;
            case static_cast<int>(Key::eNumpad2):
                result = "eNumpad2";
                break;
            case static_cast<int>(Key::eNumpad3):
                result = "eNumpad3";
                break;
            case static_cast<int>(Key::eNumpad4):
                result = "eNumpad4";
                break;
            case static_cast<int>(Key::eNumpad5):
                result = "eNumpad5";
                break;
            case static_cast<int>(Key::eNumpad6):
                result = "eNumpad6";
                break;
            case static_cast<int>(Key::eNumpad7):
                result = "eNumpad7";
                break;
            case static_cast<int>(Key::eNumpad8):
                result = "eNumpad8";
                break;
            case static_cast<int>(Key::eNumpad9):
                result = "eNumpad9";
                break;
            case static_cast<int>(Key::e0):
                result = "e0";
                break;
            case static_cast<int>(Key::e1):
                result = "e1";
                break;
            case static_cast<int>(Key::e2):
                result = "e2";
                break;
            case static_cast<int>(Key::e3):
                result = "e3";
                break;
            case static_cast<int>(Key::e4):
                result = "e4";
                break;
            case static_cast<int>(Key::e5):
                result = "e5";
                break;
            case static_cast<int>(Key::e6):
                result = "e6";
                break;
            case static_cast<int>(Key::e7):
                result = "e7";
                break;
            case static_cast<int>(Key::e8):
                result = "e8";
                break;
            case static_cast<int>(Key::e9):
                result = "e9";
                break;
            case static_cast<int>(Key::eA):
                result = "eA";
                break;
            case static_cast<int>(Key::eB):
                result = "eB";
                break;
            case static_cast<int>(Key::eC):
                result = "eC";
                break;
            case static_cast<int>(Key::eD):
                result = "eD";
                break;
            case static_cast<int>(Key::eE):
                result = "eE";
                break;
            case static_cast<int>(Key::eF):
                result = "eF";
                break;
            case static_cast<int>(Key::eG):
                result = "eG";
                break;
            case static_cast<int>(Key::eH):
                result = "eH";
                break;
            case static_cast<int>(Key::eI):
                result = "eI";
                break;
            case static_cast<int>(Key::eJ):
                result = "eJ";
                break;
            case static_cast<int>(Key::eK):
                result = "eK";
                break;
            case static_cast<int>(Key::eL):
                result = "eL";
                break;
            case static_cast<int>(Key::eM):
                result = "eM";
                break;
            case static_cast<int>(Key::eN):
                result = "eN";
                break;
            case static_cast<int>(Key::eO):
                result = "eO";
                break;
            case static_cast<int>(Key::eP):
                result = "eP";
                break;
            case static_cast<int>(Key::eQ):
                result = "eQ";
                break;
            case static_cast<int>(Key::eR):
                result = "eR";
                break;
            case static_cast<int>(Key::eS):
                result = "eS";
                break;
            case static_cast<int>(Key::eT):
                result = "eT";
                break;
            case static_cast<int>(Key::eU):
                result = "eU";
                break;
            case static_cast<int>(Key::eV):
                result = "eV";
                break;
            case static_cast<int>(Key::eW):
                result = "eW";
                break;
            case static_cast<int>(Key::eX):
                result = "eX";
                break;
            case static_cast<int>(Key::eY):
                result = "eY";
                break;
            case static_cast<int>(Key::eZ):
                result = "eZ";
                break;
            case static_cast<int>(Key::eGamepadA):
                result = "eGamepadA";
                break;
            case static_cast<int>(Key::eGamepadB):
                result = "eGamepadB";
                break;
            case static_cast<int>(Key::eGamepadX):
                result = "eGamepadX";
                break;
            case static_cast<int>(Key::eGamepadY):
                result = "eGamepadY";
                break;
            case static_cast<int>(Key::eGamepadThumbLeft):
                result = "eGamepadThumbLeft";
                break;
            case static_cast<int>(Key::eGamepadThumbRight):
                result = "eGamepadThumbRight";
                break;
            case static_cast<int>(Key::eGamepadShoulderLeft):
                result = "eGamepadShoulderLeft";
                break;
            case static_cast<int>(Key::eGamepadShoulderRight):
                result = "eGamepadShoulderRight";
                break;
            case static_cast<int>(Key::eGamepadUp):
                result = "eGamepadUp";
                break;
            case static_cast<int>(Key::eGamepadDown):
                result = "eGamepadDown";
                break;
            case static_cast<int>(Key::eGamepadLeft):
                result = "eGamepadLeft";
                break;
            case static_cast<int>(Key::eGamepadRight):
                result = "eGamepadRight";
                break;
            case static_cast<int>(Key::eGamepadBack):
                result = "eGamepadBack";
                break;
            case static_cast<int>(Key::eGamepadStart):
                result = "eGamepadStart";
                break;
            case static_cast<int>(Key::eGamepadGuide):
                result = "eGamepadGuide";
                break;
            default:
                result = "eNone";
                break;
        }
    }
    return result;
}

int Input::enumParse(const std::string& enumName, const std::string& str)
{
    int result = 0;
    if (enumName.compare("gameplay::Input::KeyModifiers") == 0)
    {
        std::string nameBuffer;
        std::vector<std::string> names;
        for (const char& token : str)
        {
            nameBuffer += token;
            if (token == '|')
            {
                names.push_back(nameBuffer);
                nameBuffer.clear();
            }
        }
        if (!nameBuffer.empty())
        {
            names.push_back(nameBuffer);
        }
        for (const std::string& name : names)
        {
            if (name.compare("eNone") == 0)
                result |= static_cast<int>(KeyModifiers::eNone);
            else if (name.compare("eLeftAlt") == 0)
                result |= static_cast<int>(KeyModifiers::eLeftAlt);
            else if (name.compare("eRightAlt") == 0)
                result |= static_cast<int>(KeyModifiers::eRightAlt);
            else if (name.compare("eLeftCtrl") == 0)
                result |= static_cast<int>(KeyModifiers::eLeftCtrl);
            else if (name.compare("eRightCtrl") == 0)
                result |= static_cast<int>(KeyModifiers::eRightCtrl);
            else if (name.compare("eLeftShift") == 0)
                result |= static_cast<int>(KeyModifiers::eLeftShift);
            else if (name.compare("eRightShift") == 0)
                result |= static_cast<int>(KeyModifiers::eRightShift);
            else if (name.compare("eLeftMeta") == 0)
                result |= static_cast<int>(KeyModifiers::eLeftMeta);
            else if (name.compare("eRightMeta") == 0)
                result |= static_cast<int>(KeyModifiers::eRightMeta);
        }
    }
    else if (enumName.compare("gameplay::Input::GamepadAxis") == 0)
    {
        if (str.compare("eLeftX") == 0)
            result = static_cast<int>(GamepadAxis::eLeftX);
        else if (str.compare("eLeftY") == 0)
            result = static_cast<int>(GamepadAxis::eLeftY);
        else if (str.compare("eLeftZ") == 0)
            result = static_cast<int>(GamepadAxis::eLeftZ);
        else if (str.compare("eRightX") == 0)
            result = static_cast<int>(GamepadAxis::eRightX);
        else if (str.compare("eRightY") == 0)
            result = static_cast<int>(GamepadAxis::eRightY);
        else if (str.compare("eRightZ") == 0)
            result = static_cast<int>(GamepadAxis::eRightZ);
    }
    else if (enumName.compare("gameplay::Input::MouseAxis") == 0)
    {
        if (str.compare("eX") == 0)
            result = static_cast<int>(MouseAxis::eX);
        else if (str.compare("eY") == 0)
            result = static_cast<int>(MouseAxis::eY);
        else if (str.compare("eScrollWheel") == 0)
            result = static_cast<int>(MouseAxis::eScrollWheel);
    }
    else if (enumName.compare("gameplay::Input::Mapping::Type") == 0)
    {
        if (str.compare("eKey") == 0)
            result = static_cast<int>(Mapping::Type::eKey);
        else if (str.compare("eMouseButton") == 0)
            result = static_cast<int>(Mapping::Type::eMouseButton);
        else if (str.compare("eMouseAxis") == 0)
            result = static_cast<int>(Mapping::Type::eMouseAxis);
        else if (str.compare("eGamepadAxis") == 0)
            result = static_cast<int>(Mapping::Type::eGamepadAxis);
    }
    else if (enumName.compare("gameplay::Input::Mapping::Action") == 0)
    {
        if (str.compare("ePositive") == 0)
            result = static_cast<int>(Mapping::Action::ePositive);
        else if (str.compare("eNegative") == 0)
            result = static_cast<int>(Mapping::Action::eNegative);
    }
    else if (enumName.compare("gameplay::Input::Mapping::ActionProfile") == 0)
    {
        if (str.compare("ePrimary") == 0)
            result = static_cast<int>(Mapping::ActionProfile::ePrimary);
        else if (str.compare("eSecondary") == 0)
            result = static_cast<int>(Mapping::ActionProfile::eSecondary);
    }
    else if (enumName.compare("gameplay::Input::MouseButton") == 0)
    {
        if (str.compare("eLeft") == 0)
            result = static_cast<int>(MouseButton::eLeft);
        else if (str.compare("eRight") == 0)
            result = static_cast<int>(MouseButton::eRight);
        else if (str.compare("eMiddle") == 0)
            result = static_cast<int>(MouseButton::eMiddle);
    }
    else if (enumName.compare("gameplay::Input::Key") == 0)
    {
        if (str.compare("eNone") == 0)
            result = static_cast<int>(Key::eNone);
        else if (str.compare("eEsc") == 0)
            result = static_cast<int>(Key::eEsc);
        else if (str.compare("eReturn") == 0)
            result = static_cast<int>(Key::eReturn);
        else if (str.compare("eTab") == 0)
            result = static_cast<int>(Key::eTab);
        else if (str.compare("eSpace") == 0)
            result = static_cast<int>(Key::eSpace);
        else if (str.compare("eBackspace") == 0)
            result = static_cast<int>(Key::eBackspace);
        else if (str.compare("eUp") == 0)
            result = static_cast<int>(Key::eUp);
        else if (str.compare("eDown") == 0)
            result = static_cast<int>(Key::eDown);
        else if (str.compare("eLeft") == 0)
            result = static_cast<int>(Key::eLeft);
        else if (str.compare("eRight") == 0)
            result = static_cast<int>(Key::eRight);
        else if (str.compare("eInsert") == 0)
            result = static_cast<int>(Key::eInsert);
        else if (str.compare("eDelete") == 0)
            result = static_cast<int>(Key::eDelete);
        else if (str.compare("eHome") == 0)
            result = static_cast<int>(Key::eHome);
        else if (str.compare("eEnd") == 0)
            result = static_cast<int>(Key::eEnd);
        else if (str.compare("ePageUp") == 0)
            result = static_cast<int>(Key::ePageUp);
        else if (str.compare("ePageDown") == 0)
            result = static_cast<int>(Key::ePageDown);
        else if (str.compare("ePrint") == 0)
            result = static_cast<int>(Key::ePrint);
        else if (str.compare("ePlus") == 0)
            result = static_cast<int>(Key::ePlus);
        else if (str.compare("eMinus") == 0)
            result = static_cast<int>(Key::eMinus);
        else if (str.compare("eLeftBracket") == 0)
            result = static_cast<int>(Key::eLeftBracket);
        else if (str.compare("eRightBracket") == 0)
            result = static_cast<int>(Key::eRightBracket);
        else if (str.compare("eSemiColon") == 0)
            result = static_cast<int>(Key::eSemiColon);
        else if (str.compare("eQuote") == 0)
            result = static_cast<int>(Key::eQuote);
        else if (str.compare("eComma") == 0)
            result = static_cast<int>(Key::eComma);
        else if (str.compare("ePeriod") == 0)
            result = static_cast<int>(Key::ePeriod);
        else if (str.compare("eSlash") == 0)
            result = static_cast<int>(Key::eSlash);
        else if (str.compare("eBackslash") == 0)
            result = static_cast<int>(Key::eBackslash);
        else if (str.compare("eTilde") == 0)
            result = static_cast<int>(Key::eTilde);
        else if (str.compare("eF1") == 0)
            result = static_cast<int>(Key::eF1);
        else if (str.compare("eF2") == 0)
            result = static_cast<int>(Key::eF2);
        else if (str.compare("eF3") == 0)
            result = static_cast<int>(Key::eF3);
        else if (str.compare("eF4") == 0)
            result = static_cast<int>(Key::eF4);
        else if (str.compare("eF5") == 0)
            result = static_cast<int>(Key::eF5);
        else if (str.compare("eF6") == 0)
            result = static_cast<int>(Key::eF6);
        else if (str.compare("eF7") == 0)
            result = static_cast<int>(Key::eF7);
        else if (str.compare("eF8") == 0)
            result = static_cast<int>(Key::eF8);
        else if (str.compare("eF9") == 0)
            result = static_cast<int>(Key::eF9);
        else if (str.compare("eF10") == 0)
            result = static_cast<int>(Key::eF10);
        else if (str.compare("eF11") == 0)
            result = static_cast<int>(Key::eF11);
        else if (str.compare("eF12") == 0)
            result = static_cast<int>(Key::eF12);
        else if (str.compare("eNumpad0") == 0)
            result = static_cast<int>(Key::eNumpad0);
        else if (str.compare("eNumpad1") == 0)
            result = static_cast<int>(Key::eNumpad1);
        else if (str.compare("eNumpad2") == 0)
            result = static_cast<int>(Key::eNumpad2);
        else if (str.compare("eNumpad3") == 0)
            result = static_cast<int>(Key::eNumpad3);
        else if (str.compare("eNumpad4") == 0)
            result = static_cast<int>(Key::eNumpad4);
        else if (str.compare("eNumpad5") == 0)
            result = static_cast<int>(Key::eNumpad5);
        else if (str.compare("eNumpad6") == 0)
            result = static_cast<int>(Key::eNumpad6);
        else if (str.compare("eNumpad7") == 0)
            result = static_cast<int>(Key::eNumpad7);
        else if (str.compare("eNumpad8") == 0)
            result = static_cast<int>(Key::eNumpad8);
        else if (str.compare("eNumpad9") == 0)
            result = static_cast<int>(Key::eNumpad9);
        else if (str.compare("e0") == 0)
            result = static_cast<int>(Key::e0);
        else if (str.compare("e1") == 0)
            result = static_cast<int>(Key::e1);
        else if (str.compare("e2") == 0)
            result = static_cast<int>(Key::e2);
        else if (str.compare("e3") == 0)
            result = static_cast<int>(Key::e3);
        else if (str.compare("e4") == 0)
            result = static_cast<int>(Key::e4);
        else if (str.compare("e5") == 0)
            result = static_cast<int>(Key::e5);
        else if (str.compare("e6") == 0)
            result = static_cast<int>(Key::e6);
        else if (str.compare("e7") == 0)
            result = static_cast<int>(Key::e7);
        else if (str.compare("e8") == 0)
            result = static_cast<int>(Key::e8);
        else if (str.compare("e9") == 0)
            result = static_cast<int>(Key::e9);
        else if (str.compare("eA") == 0)
            result = static_cast<int>(Key::eA);
        else if (str.compare("eB") == 0)
            result = static_cast<int>(Key::eB);
        else if (str.compare("eC") == 0)
            result = static_cast<int>(Key::eC);
        else if (str.compare("eD") == 0)
            result = static_cast<int>(Key::eD);
        else if (str.compare("eE") == 0)
            result = static_cast<int>(Key::eE);
        else if (str.compare("eF") == 0)
            result = static_cast<int>(Key::eF);
        else if (str.compare("eG") == 0)
            result = static_cast<int>(Key::eG);
        else if (str.compare("eH") == 0)
            result = static_cast<int>(Key::eH);
        else if (str.compare("eI") == 0)
            result = static_cast<int>(Key::eI);
        else if (str.compare("eJ") == 0)
            result = static_cast<int>(Key::eJ);
        else if (str.compare("eK") == 0)
            result = static_cast<int>(Key::eK);
        else if (str.compare("eL") == 0)
            result = static_cast<int>(Key::eL);
        else if (str.compare("eM") == 0)
            result = static_cast<int>(Key::eM);
        else if (str.compare("eN") == 0)
            result = static_cast<int>(Key::eN);
        else if (str.compare("eO") == 0)
            result = static_cast<int>(Key::eO);
        else if (str.compare("eP") == 0)
            result = static_cast<int>(Key::eP);
        else if (str.compare("eQ") == 0)
            result = static_cast<int>(Key::eQ);
        else if (str.compare("eR") == 0)
            result = static_cast<int>(Key::eR);
        else if (str.compare("eS") == 0)
            result = static_cast<int>(Key::eS);
        else if (str.compare("eT") == 0)
            result = static_cast<int>(Key::eT);
        else if (str.compare("eU") == 0)
            result = static_cast<int>(Key::eU);
        else if (str.compare("eV") == 0)
            result = static_cast<int>(Key::eV);
        else if (str.compare("eW") == 0)
            result = static_cast<int>(Key::eW);
        else if (str.compare("eX") == 0)
            result = static_cast<int>(Key::eX);
        else if (str.compare("eY") == 0)
            result = static_cast<int>(Key::eY);
        else if (str.compare("eZ") == 0)
            result = static_cast<int>(Key::eZ);
        else if (str.compare("eGamepadA") == 0)
            result = static_cast<int>(Key::eGamepadA);
        else if (str.compare("eGamepadB") == 0)
            result = static_cast<int>(Key::eGamepadB);
        else if (str.compare("eGamepadX") == 0)
            result = static_cast<int>(Key::eGamepadX);
        else if (str.compare("eGamepadY") == 0)
            result = static_cast<int>(Key::eGamepadY);
        else if (str.compare("eGamepadThumbLeft") == 0)
            result = static_cast<int>(Key::eGamepadThumbLeft);
        else if (str.compare("eGamepadThumbRight") == 0)
            result = static_cast<int>(Key::eGamepadThumbRight);
        else if (str.compare("eGamepadShoulderLeft") == 0)
            result = static_cast<int>(Key::eGamepadShoulderLeft);
        else if (str.compare("eGamepadShoulderRight") == 0)
            result = static_cast<int>(Key::eGamepadShoulderRight);
        else if (str.compare("eGamepadUp") == 0)
            result = static_cast<int>(Key::eGamepadUp);
        else if (str.compare("eGamepadDown") == 0)
            result = static_cast<int>(Key::eGamepadDown);
        else if (str.compare("eGamepadLeft") == 0)
            result = static_cast<int>(Key::eGamepadLeft);
        else if (str.compare("eGamepadRight") == 0)
            result = static_cast<int>(Key::eGamepadRight);
        else if (str.compare("eGamepadBack") == 0)
            result = static_cast<int>(Key::eGamepadBack);
        else if (str.compare("eGamepadStart") == 0)
            result = static_cast<int>(Key::eGamepadStart);
        else if (str.compare("eGamepadGuide") == 0)
            result = static_cast<int>(Key::eGamepadGuide);
    }
    return result;
}

bool isKeyboard(Input::Key key)
{
    return key <= Input::Key::eZ;
}

bool isGamepad(Input::Key key)
{
    return key < Input::Key::eCount && !isKeyboard(key);
}

bool useAnyGamepad(uint32_t gamepadIndex)
{
    return gamepadIndex == Input::kGamepadsAnyIndex;
}

bool isValidIndex(int index, size_t size)
{
    return index >= 0 && index < size;
}

char Input::getKeyCode(Input::Key key, Input::KeyModifiers modifiers)
{
    const bool isAscii = (Key::e0 <= key && key <= Key::eZ) || (Key::eEsc  <= key && key <= Key::eMinus);
    if (!isAscii)
    {
        return '\0';
    }
    const bool isNumber = (Key::e0 <= key && key <= Key::e9);
    if (isNumber)
    {
        return '0' + char(static_cast<uint32_t>(key) - static_cast<uint32_t>(Key::e0));
    }
    const bool isChar = (Key::eA <= key && key <= Key::eZ);
    if (isChar)
    {
        bool shift = bool(modifiers & Input::KeyModifiers::eLeftShift | KeyModifiers::eRightShift);
        return (shift ? 'A' : 'a') + char(static_cast<uint32_t>(key) - static_cast<uint32_t>(Key::eA));
    }

    switch (key)
    {
    case Key::eEsc:
        return 0x1b;
    case Key::eReturn:
        return '\n';
    case Key::eTab:
        return '\t';
    case Key::eSpace:
        return ' ';
    case Key::eBackspace:
        return 0x08;
    case Key::ePlus:
        return '+';
    case Key::eMinus:
        return '-';
    default: 
        break;
    }
    return '\0';
}

bool Input::isKeyHeld(Key key, KeyModifiers modifiers, uint32_t gamepadIndex)
{
    return getInput()->isKey(key, modifiers, gamepadIndex, false, false, true);
}

bool Input::isKeyDown(Key key, KeyModifiers modifiers, uint32_t gamepadIndex)
{
    return getInput()->isKey(key, modifiers, gamepadIndex, false, true, false);
}

bool Input::isKeyUp(Key key, KeyModifiers modifiers, uint32_t gamepadIndex)
{
    return getInput()->isKey(key, modifiers, gamepadIndex, true, false, false);
}

bool Input::isActionHeld(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).held;
}

bool Input::isActionDown(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).down;
}

bool Input::isActionUp(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).up;
}

float Input::getAxis(const std::string& mappingName)
{
    return getInput()->getMappingState(mappingName).axis;
}

bool Input::isMouseButtonHeld(MouseButton button)
{
    return getInput()->_mouseButtons.isHeld(static_cast<uint32_t>(button));
}

bool Input::isMouseButtonDown(MouseButton button)
{
    return getInput()->_mouseButtons.isDown(static_cast<uint32_t>(button));
}

bool Input::isMouseButtonUp(MouseButton button)
{
    return getInput()->_mouseButtons.isUp(static_cast<uint32_t>(button));
}

const Vector2& Input::getMousePosition()
{
    return getInput()->_mousePosition;
}

float Input::getMouseAxis(MouseAxis mouseAxis)
{
    return getInput()->getMouseAxisValue(mouseAxis);
}

float Input::getGamepadAxis(GamepadAxis gamepadAxis, uint32_t gamepadIndex)
{
    return getInput()->getGamepadAxisValue(gamepadAxis, gamepadIndex);
}

float Input::getMouseScroll()
{
    return getInput()->_mouseScroll;
}

bool Input::isMouseLocked()
{
    return getInput()->_mouseLocked;
}

void Input::setMouseLocked(bool lock)
{
    getInput()->_mouseLocked = lock;
}

std::shared_ptr<Input::Mapping> Input::createMapping()
{
    return std::make_shared<Mapping>();
}

bool Input::addMapping(const std::shared_ptr<Mapping>& mapping)
{
    return getInput()->_mappings->add(mapping);
}

bool Input::removeMapping(const std::shared_ptr<Mapping>& mapping)
{
    return getInput()->_mappings->remove(mapping);
}

bool Input::removeMappings(const std::string& name)
{
    return getInput()->_mappings->remove(name);
}

Input::Input() :
    _mouseLocked(false),
    _mouseScroll(0)
{
}

Input::~Input()
{
}

Input* Input::getInput()
{
    static Input input;
    return &input;
}

void Input::initialize()
{
    if (Serializer* reader = Serializer::createReader(GP_ENGINE_INPUT))
    {
        std::shared_ptr<Serializable> mappings = reader->readObject(nullptr);
        if (mappings)
        {
            _mappings = std::static_pointer_cast<Mappings>(mappings);
        }
        else
        {
            _mappings = std::make_shared<Mappings>();
        }
        reader->close();
        GP_SAFE_DELETE(reader);
    }
    else
    {
        _mappings = std::make_shared<Mappings>();
        Serializer* writer = SerializerJson::createWriter(GP_ENGINE_INPUT);
        writer->writeObject(nullptr, _mappings);
        writer->close();
        GP_SAFE_DELETE(writer);
    }
    _keys.initialize(static_cast<uint32_t>(Key::eCount));
    _mouseButtons.initialize(static_cast<uint32_t>(MouseButton::eCount));
    for (int i = 0; i < _gamepads.size(); ++i)
    {
        GamepadState& gamepad = _gamepads[i];
        gamepad.index = i;
        gamepad.buttons.initialize(static_cast<uint32_t>(Key::eCount));
    }
}

void Input::update()
{
    _keys.update();
    _mouseButtons.update();
    for (GamepadState& gamepad : _gamepads)
    {
        gamepad.buttons.update();
    }
    _mouseScroll = 0;
    _previousMousePosition = _mousePosition;
}

float Input::getGamepadAxisValue(GamepadAxis axis, uint32_t gamepadIndex) const
{
    const uint32_t gamepadAxis = static_cast<uint32_t>(axis);
    if (gamepadAxis >= static_cast<uint32_t>(GamepadAxis::eCount))
    {
        return 0.0f;
    }
    if (useAnyGamepad(gamepadIndex))
    {
        for(const GamepadState& gamepad : _gamepads)
        {
            const float value = gamepad.axes[gamepadAxis];
            const bool isActive = value != 0.0f;
            if(isActive)
            {
                return value;
            }
        }
    }
    else if (isValidIndex(gamepadIndex, _gamepads.size()))
    {
        return _gamepads[gamepadIndex].axes[gamepadAxis];
    }
    return 0.0f;
}

float Input::getMouseAxisValue(MouseAxis mouseAxis) const
{
    const Vector2 mouseDelta = _mousePosition - _previousMousePosition;
    switch (mouseAxis)
    {
    case MouseAxis::eX:
        return mouseDelta.x > 0 ? 1.0f : mouseDelta.x < 0 ? - 1.0f : 0.0f;
    case MouseAxis::eY:
        return mouseDelta.y < 0 ? 1.0f : mouseDelta.y > 0 ? - 1.0f : 0.0f;
    case MouseAxis::eScrollWheel:
        return _mouseScroll;
    }
    return 0.0f;
}

bool Input::isKey(Key key, KeyModifiers modifiers, const ActionState& state, bool up, bool down, bool held) const
{
    const uint32_t slot = static_cast<uint32_t>(key);
    if (up)
    {
        return state.isUp(slot, modifiers);
    }
    else if (down)
    {
        return state.isDown(slot, modifiers);
    }
    else if (held)
    {
        return state.isHeld(slot, modifiers);
    }
    return false;
}

bool Input::isKey(Key key, KeyModifiers modifiers, uint32_t gamepadIndex, bool up, bool down, bool held) const
{
    if (isKeyboard(key))
    {
        return isKey(key, modifiers, _keys, up, down, held);
    }
    else
    {
        if (gamepadIndex == kGamepadsAnyIndex)
        {
            for (const GamepadState& gamePad : _gamepads)
            {
                if(isKey(key, modifiers, gamePad.buttons, up, down, held))
                {
                    return true;
                }
            }
        }
        else if (gamepadIndex < kGamepadsMax)
        {
            return isKey(key, modifiers, _gamepads[gamepadIndex].buttons, up, down, held);
        }
    }
    return false;
}

void Input::getActionState(Mapping::Action action, const Mapping& mapping, bool& up, bool& down, bool& held) const
{
    up = false;
    down = false;
    held = false;
    const uint32_t actionIndex = static_cast<uint32_t>(action);
    for (uint32_t profileIndex = 0; profileIndex < static_cast<uint32_t>(Mapping::ActionProfile::eCount); ++profileIndex)
    {
        const bool mouse = mapping.getType() == Mapping::Type::eMouseButton;
        const Mapping::ActionProfileMap& map = mouse ? mapping.getMouseButtonActions() : mapping.getKeyActions();
        const Mapping::ActionData& data = *map[profileIndex][actionIndex];
        if (data.slotIndex == 0 && data.modifiers == KeyModifiers::eNone)
        {
            continue;
        }
        const ActionState* deviceState = nullptr;
        const Key key = static_cast<Key>(data.slotIndex);
        if (mouse)
        {
            deviceState = &_mouseButtons;
        }
        else if(isKeyboard(key))
        {
            deviceState = &_keys;
        }
        else if (isGamepad(key))
        {
            if (useAnyGamepad(data.gamepadIndex))
            {
                for(int i = 0; i < _gamepads.size(); ++i)
                {
                    const ActionState& gamepadState = _gamepads[i].buttons;
                    if(gamepadState.isActive(data.slotIndex))
                    {
                        deviceState = &gamepadState;
                        break;
                    }
                }
            }
            else if (isValidIndex(data.gamepadIndex, _gamepads.size()))
            {
                 deviceState = &_gamepads[data.gamepadIndex].buttons;
            }
        }
        if (!deviceState)
        {
            continue;
        }
        if (deviceState->isActive(data.slotIndex, data.modifiers))
        {
            up = deviceState->isUp(data.slotIndex, data.modifiers);
            down = deviceState->isDown(data.slotIndex, data.modifiers);
            held = deviceState->isHeld(data.slotIndex, data.modifiers);
            break;
        }
    }
}

void Input::getActionState(const Mapping& mapping, MappingState& state) const
{
    bool positiveUp = false;
    bool positiveDown = false;
    bool positiveHeld = false;
    getActionState(Mapping::Action::ePositive, mapping, positiveUp, positiveDown, positiveHeld);
    bool negativeUp = false;
    bool negativeDown = false;
    bool negativeHeld = false;
    getActionState(Mapping::Action::eNegative, mapping, negativeUp, negativeDown, negativeHeld);
    const bool neutral = positiveHeld && negativeHeld;
    state.held = positiveHeld || negativeHeld;
    state.up = !state.held && (positiveUp || negativeUp);
    state.down = !neutral && (positiveDown || negativeDown);
    if (!neutral && state.held)
    {
        state.axis = positiveHeld ? 1.0f : -1.0f;
    }
}

Input::MappingState Input::getMappingState(const std::string& name) const
{
    MappingState state;
    state.axis = 0.0f;
    state.up = false;
    state.down = false;
    state.held = false;
    for (const std::shared_ptr<Mapping>& mapping : _mappings->mappings)
    {
        if (mapping->getName() == name)
        {
            switch (mapping->getType())
            {
            case Mapping::Type::eGamepadAxis:
                state.axis = getGamepadAxisValue(mapping->getGamepadAxis(), mapping->getGamepadAxisIndex());
                break;
            case Mapping::Type::eMouseAxis:
                state.axis = getMouseAxisValue(mapping->getMouseAxis());
                break;
            case Mapping::Type::eKey:
            case Mapping::Type::eMouseButton:
                getActionState(*mapping, state);
                break;
            }
            if (mapping->getInverted())
            {
                state.axis *= -1.0f;
            }
            const bool isInputActive = state.axis != 0.0f || state.down || state.down || state.held;
            if (isInputActive)
            {
                break;
            }
        }
    }
    return state;
}

void Input::postGamepadConnectionEvent(uint32_t gamepadIndex, bool connected)
{
    _gamepads[gamepadIndex].connected = connected;
}

void Input::postGamepadAxisEvent(uint32_t gamepadIndex, GamepadAxis axis, int value)
{
    value *= axis == GamepadAxis::eLeftY || axis == GamepadAxis::eRightY ? -1 : 1;
    _gamepads[gamepadIndex].axes[static_cast<uint32_t>(axis)] = (1.0f / static_cast<float>(std::numeric_limits<int16_t>::max())) * static_cast<float>(value);
}

void Input::postMouseMotionEvent(int mx, int my)
{
    if (!_mouseLocked)
    {
        _mousePosition.x = mx;
        _mousePosition.y = my;
    }
}

void Input::postMouseWheelEvent(int value)
{
    if (!_mouseLocked)
    {
        _mouseScroll = value;
    }
}

void Input::postMousePressEvent(int mx, int my, MouseButton button, bool down)
{
    _mouseButtons.postEvent(static_cast<uint32_t>(button), down);
}

void Input::postKeyCharEvent(char)
{
}

void Input::postKeyPressEvent(Key key, KeyModifiers modifiers, bool down, uint32_t gamepadIndex)
{
    const uint32_t slot = static_cast<uint32_t>(key);
    if(isKeyboard(key))
    {
        _keys.postEvent(slot, down, modifiers);
    }
    else if(gamepadIndex < kGamepadsMax)
    {
        _gamepads[gamepadIndex].buttons.postEvent(slot, down);
    }
}

Input::GamepadState::GamepadState() :
    connected(false)
{
}

Input::ActionState::ActionState() :
    dirty(false)
{
}

void Input::ActionState::initialize(int buttonCount)
{
    actionSlots.resize(buttonCount);
}

void Input::ActionState::update()
{
    if(dirty)
    {
        for(ActionSlot& slot : actionSlots)
        {
            if (slot.bits.any())
            {
                slot.bits.reset(static_cast<uint32_t>(ActionSlot::Bits::eDown));
                slot.bits.reset(static_cast<uint32_t>(ActionSlot::Bits::eUp));
                slot.modifiers = slot.bits.test(static_cast<uint32_t>(ActionSlot::Bits::eHeld)) ? slot.modifiers : KeyModifiers::eNone;
            }
        }
        dirty = false;
    }
}

bool isModifierOnlyButton(uint32_t button)
{
    return button == 0;
}

bool isModifierMatch(uint32_t slot, Input::KeyModifiers modifiers , Input::KeyModifiers stateModifiers)
{
    const bool areModifiersActive = (modifiers & stateModifiers) == modifiers;
    const bool areOnlyModifiersActive = modifiers == stateModifiers;
    return isModifierOnlyButton(slot) ? areModifiersActive : areOnlyModifiersActive;
}

void Input::ActionState::postEvent(uint32_t button, bool down, KeyModifiers modifiers)
{
    ActionSlot& slot = actionSlots[button];
    const bool wasHeld = slot.bits.test(static_cast<uint32_t>(ActionSlot::Bits::eHeld));
    const bool isReleasing = wasHeld && !down;
    const bool hasModifiersRemaining = modifiers != KeyModifiers::eNone;
    const bool isModifierUpdate = isModifierOnlyButton(button) && isReleasing && hasModifiersRemaining;
    if(isModifierUpdate)
    {
        slot.modifiers = modifiers;
    }
    else
    {
        dirty = true;
        slot.bits.set(static_cast<uint32_t>(ActionSlot::Bits::eDown), !wasHeld && down);
        slot.bits.set(static_cast<uint32_t>(ActionSlot::Bits::eHeld), down);
        slot.bits.set(static_cast<uint32_t>(ActionSlot::Bits::eUp), wasHeld && !down);
        slot.modifiers = down ? modifiers : slot.modifiers;
    }
}

bool Input::ActionState::test(uint32_t slot, KeyModifiers modifiers, ActionSlot::Bits bit) const
{
    const ActionSlot& actionSlotstate = actionSlots[slot];
    return actionSlotstate.bits.test(static_cast<uint32_t>(bit)) && isModifierMatch(slot, modifiers, actionSlotstate.modifiers);
}

bool Input::ActionState::isActive(uint32_t slot, KeyModifiers modifiers) const
{
    const ActionSlot& actionSlotstate = actionSlots[slot];
    return actionSlotstate.bits.any() && isModifierMatch(slot, modifiers, actionSlotstate.modifiers);
}

bool Input::ActionState::isUp(uint32_t slot, KeyModifiers modifiers) const
{
    return test(slot, modifiers, ActionSlot::Bits::eUp);
}

bool Input::ActionState::isDown(uint32_t slot, KeyModifiers modifiers) const
{
    return test(slot, modifiers, ActionSlot::Bits::eDown);
}

bool Input::ActionState::isHeld(uint32_t slot, KeyModifiers modifiers) const
{
    return test(slot, modifiers, ActionSlot::Bits::eHeld);
}

Input::ActionState::ActionSlot::ActionSlot()
    : modifiers(KeyModifiers::eNone)
{
}

std::string Input::Mappings::getClassName()
{
    return "gameplay::Input::Mappings";
}

std::shared_ptr<Serializable> Input::Mappings::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Mappings>());
}

void Input::Mappings::onSerialize(Serializer* serializer)
{
    serializer->writeObjectList("values", mappings.size());
    for (const std::shared_ptr<Mapping>& mapping : mappings)
    {
        serializer->writeObject("", mapping);
    }
}

void Input::Mappings::onDeserialize(Serializer* serializer)
{
    const size_t size = serializer->readObjectList("values");
    for (size_t i = 0; i < size; ++i)
    {
        mappings.push_back(std::static_pointer_cast<Mapping>(serializer->readObject("")));
    }
}

bool Input::Mappings::add(const std::shared_ptr<Mapping>& mapping)
{
    auto itr = std::find(mappings.begin(), mappings.end(), mapping);
    if (itr == mappings.end())
    {
        mappings.push_back(mapping);
        return true;
    }
    return false;
}

bool Input::Mappings::remove(const std::string& name)
{
    const size_t count = mappings.size();
    auto itr = mappings.begin();
    while (itr != mappings.end())
    {
        if ((*itr)->getName() == name)
        {
            itr = mappings.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
    return count != mappings.size();
}

bool Input::Mappings::remove(const std::shared_ptr<Mapping>& mapping)
{
    auto itr = std::find(mappings.begin(), mappings.end(), mapping);
    if (itr != mappings.end())
    {
        mappings.erase(itr);
        return true;
    }
    return false;
}

Input::Mapping::Mapping() :
    _inverted(false),
    _gamepadIndex(kGamepadsAnyIndex)
{
    for (int profileIndex = 0; profileIndex < static_cast<int>(ActionProfile::eCount); ++profileIndex)
    {
        for (int actionIndex = 0; actionIndex < static_cast<int>(Action::eCount); ++actionIndex)
        {
            const Action action = static_cast<Action>(actionIndex);
            const ActionProfile profile = static_cast<ActionProfile>(profileIndex);
            _keyActions[profileIndex][actionIndex] = std::make_shared<KeyActionData>(profile, action);
            _mouseButtonActions[profileIndex][actionIndex] = std::make_shared<MouseButtonActionData>(profile, action);
        }
    }
}

std::string Input::Mapping::getClassName()
{
    return "gameplay::Input::Mapping";
}

void Input::Mapping::serialize(Serializer* serializer, const ActionProfileMap& map)
{
    std::vector<std::shared_ptr<ActionData>> output;
    for (const Actions& actions : map)
    {
        for (const std::shared_ptr<ActionData>& data : actions)
        {
            if (data->slotIndex != 0 || data->modifiers != KeyModifiers::eNone)
            {
                output.push_back(data);
            }
        }
    }
    if (!output.empty())
    {
        serializer->writeObjectList("actions", output.size());
        for (const std::shared_ptr<Serializable>& data : output)
        {
            serializer->writeObject("", data);
        }
    }
}

void Input::Mapping::deserialize(Serializer* serializer, ActionProfileMap& map)
{
    const size_t size = serializer->readObjectList("actions");
    for (int i = 0; i < size; ++i)
    {
        std::shared_ptr<ActionData> data = std::static_pointer_cast<ActionData>(serializer->readObject(""));
        map[static_cast<int>(data->profile)][static_cast<int>(data->action)] = data;
    }
}

void Input::Mapping::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("type", "gameplay::Input::Mapping::Type", static_cast<int>(_type), static_cast<int>(Type::eCount));
    serializer->writeBool("inverted", _inverted, false);
    serializer->writeString("name", _name.c_str(), "");
    serializer->writeString("description", _description.c_str(), "");
    switch (_type)
    {
    case Type::eMouseAxis:
        serializer->writeEnum("mouseAxis", "gameplay::Input::MouseAxis", static_cast<int>(_mouseAxis), static_cast<int>(MouseAxis::eCount));
        break;
    case Type::eGamepadAxis:
        serializer->writeInt("gamepadIndex", _gamepadIndex, kGamepadsAnyIndex);
        serializer->writeEnum("gamepadAxis", "gameplay::Input::GamepadAxis", static_cast<int>(_gamepadAxis), static_cast<int>(GamepadAxis::eCount));
        break;
    case Type::eKey:
        serializer->writeInt("gamepadIndex", _gamepadIndex, kGamepadsAnyIndex);
        serialize(serializer, _keyActions);
        break;
    case Type::eMouseButton:
        serialize(serializer, _mouseButtonActions);
        break;
    }
}

void Input::Mapping::onDeserialize(Serializer* serializer)
{
    _type = static_cast<Type>(serializer->readEnum("type", "gameplay::Input::Mapping::Type", static_cast<int>(Type::eCount)));
    _inverted = serializer->readBool("inverted", false);
    serializer->readString("name", _name, "");
    serializer->readString("description", _description, "");
    switch (_type)
    {
    case Type::eMouseAxis:
        _mouseAxis = static_cast<MouseAxis>(serializer->readEnum("mouseAxis", "gameplay::Input::MouseAxis", static_cast<int>(MouseAxis::eCount)));
        break;
    case Type::eGamepadAxis:
        _gamepadIndex = serializer->readInt("gamepadIndex", kGamepadsAnyIndex);
        _gamepadAxis = static_cast<GamepadAxis>(serializer->readEnum("gamepadAxis", "gameplay::Input::GamepadAxis", static_cast<int>(GamepadAxis::eCount)));
        break;
    case Type::eKey:
        _gamepadIndex = serializer->readInt("gamepadIndex", kGamepadsAnyIndex);
        deserialize(serializer, _keyActions);
        break;
    case Type::eMouseButton:
        deserialize(serializer, _mouseButtonActions);
        break;
    }
}

std::shared_ptr<Serializable> Input::Mapping::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Mapping>());
}

std::string Input::Mapping::getName() const
{
    return _name;
}

void Input::Mapping::setName(const std::string& name)
{
    _name = name;
}

std::string Input::Mapping::getDescription() const
{
    return _description;
}


void Input::Mapping::setDescription(const std::string& description)
{
    _description = description;
}

Input::Mapping::Type Input::Mapping::getType() const
{
    return _type;
}

void Input::Mapping::setType(Type type)
{
    _type = type;
}

bool Input::Mapping::getInverted() const
{
    return _inverted;
}

void Input::Mapping::setInverted(bool inverted)
{
    _inverted = inverted;
}

Input::Key Input::Mapping::getKeyAction(ActionProfile profile, Action action) const
{
    return static_cast<Key>(_keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->slotIndex);
}

void Input::Mapping::setKeyAction(ActionProfile profile, Action action, Key key)
{
    _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->slotIndex = static_cast<uint32_t>(key);
}

Input::KeyModifiers Input::Mapping::getKeyActionModifiers(ActionProfile profile, Action action) const
{
    return _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->modifiers;
}

void Input::Mapping::setKeyActionModifiers(ActionProfile profile, Action action, KeyModifiers modifiers)
{
    _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->modifiers = modifiers;
}

Input::MouseButton Input::Mapping::getMouseButtonAction(ActionProfile profile, Action action) const
{
    return static_cast<MouseButton>(_mouseButtonActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->slotIndex);
}

void Input::Mapping::setMouseButtonAction(ActionProfile profile, Action action, MouseButton button)
{
    _mouseButtonActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->slotIndex = static_cast<uint32_t>(button);
}

Input::MouseAxis Input::Mapping::getMouseAxis() const
{
    return _mouseAxis;
}

void Input::Mapping::setMouseAxis(MouseAxis axis)
{
    _mouseAxis = axis;
}

Input::GamepadAxis Input::Mapping::getGamepadAxis() const
{
    return _gamepadAxis;
}

void Input::Mapping::setGamepadAxis(GamepadAxis axis)
{
    _gamepadAxis = axis;
}

uint32_t Input::Mapping::getKeyActionIndex(ActionProfile profile, Action action) const
{
    return _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->gamepadIndex;
}

void Input::Mapping::setKeyActionIndex(ActionProfile profile, Action action, uint32_t gamepadIndex)
{
    _keyActions[static_cast<uint32_t>(profile)][static_cast<uint32_t>(action)]->gamepadIndex = gamepadIndex;
}

uint32_t Input::Mapping::getGamepadAxisIndex() const
{
    return _gamepadIndex;
}

void Input::Mapping::setGamepadAxisIndex(uint32_t gamepadIndex)
{
    _gamepadIndex = gamepadIndex;
}

const Input::Mapping::ActionProfileMap& Input::Mapping::getKeyActions() const
{
    return _keyActions;
}

const Input::Mapping::ActionProfileMap& Input::Mapping::getMouseButtonActions() const
{
    return _mouseButtonActions;
}

Input::Mapping::ActionData::ActionData(ActionProfile profile, Action action) :
    action(action),
    profile(profile),
    slotIndex(0),
    gamepadIndex(kGamepadsAnyIndex),
    modifiers(KeyModifiers::eNone)
{
}

void Input::Mapping::ActionData::onSerialize(Serializer* serializer)
{
    serializer->writeEnum("profile", "gameplay::Input::Mapping::ActionProfile", static_cast<int>(profile), static_cast<int>(ActionProfile::eCount));
    serializer->writeEnum("action", "gameplay::Input::Mapping::Action", static_cast<int>(action), static_cast<int>(Action::eCount));
}

void Input::Mapping::ActionData::onDeserialize(Serializer* serializer)
{
    profile = static_cast<ActionProfile>(serializer->readEnum("profile", "gameplay::Input::Mapping::ActionProfile", static_cast<int>(ActionProfile::eCount)));
    action = static_cast<Action>(serializer->readEnum("action", "gameplay::Input::Mapping::Action", static_cast<int>(Action::eCount)));
}

Input::Mapping::KeyActionData::KeyActionData(ActionProfile profile, Action action) : ActionData(profile, action)
{
}

std::string Input::Mapping::KeyActionData::getClassName()
{
    return "gameplay::Input::Mapping::KeyActionData";
}

void Input::Mapping::KeyActionData::onSerialize(Serializer* serializer)
{
    ActionData::onSerialize(serializer);
    serializer->writeEnum("key", "gameplay::Input::Key", slotIndex, static_cast<int>(Key::eNone));
    serializer->writeEnum("modifiers", "gameplay::Input::KeyModifiers", static_cast<int>(modifiers), static_cast<int>(KeyModifiers::eNone));
    serializer->writeInt("gamepadIndex", gamepadIndex, kGamepadsAnyIndex);
}

void Input::Mapping::KeyActionData::onDeserialize(Serializer* serializer)
{
    ActionData::onDeserialize(serializer);
    slotIndex = serializer->readEnum("key", "gameplay::Input::Key", static_cast<int>(Key::eNone));
    modifiers = static_cast<KeyModifiers>(serializer->readEnum("modifiers", "gameplay::Input::KeyModifiers", static_cast<int>(KeyModifiers::eNone)));
    gamepadIndex = serializer->readInt("gamepadIndex", kGamepadsAnyIndex);
}

std::shared_ptr<Serializable> Input::Mapping::KeyActionData::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<KeyActionData>(ActionProfile::ePrimary, Action::ePositive));
}

Input::Mapping::MouseButtonActionData::MouseButtonActionData(ActionProfile profile, Action action) : ActionData(profile, action)
{
}

std::string Input::Mapping::MouseButtonActionData::getClassName()
{
    return "gameplay::Input::Mapping::MouseButtonActionData";
}

void Input::Mapping::MouseButtonActionData::onSerialize(Serializer* serializer)
{
    ActionData::onSerialize(serializer);
    serializer->writeEnum("mouseButton", "gameplay::Input::MouseButton", slotIndex, static_cast<int>(MouseButton::eNone));
}

void Input::Mapping::MouseButtonActionData::onDeserialize(Serializer* serializer)
{
    ActionData::onDeserialize(serializer);
    slotIndex = serializer->readEnum("mouseButton", "gameplay::Input::MouseButton", static_cast<int>(MouseButton::eNone));
}

std::shared_ptr<Serializable> Input::Mapping::MouseButtonActionData::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<MouseButtonActionData>(ActionProfile::ePrimary, Action::ePositive));
}

}
