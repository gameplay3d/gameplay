#include "Base.h"

#include "EncoderArguments.h"
#include "StringUtil.h"

#ifdef WIN32
    #define PATH_MAX    _MAX_PATH
    #define realpath(A,B)    _fullpath(B,A,PATH_MAX)
#endif

namespace gameplay
{

static EncoderArguments* __instance;

EncoderArguments::EncoderArguments(size_t argc, const char** argv) :
    _fontSize(0),
    _parseError(false),
    _fontPreview(false),
    _textOutput(false),
    _daeOutput(false)
{
    __instance = this;

    if (argc > 1)
    {
        // read the options
        std::vector<std::string> arguments;
        for (size_t i = 1; i < argc; ++i)
        {
            arguments.push_back(argv[i]);
        }
        size_t index = 0;
        for (size_t i = 0; i < arguments.size(); ++i)
        {
            if (arguments[i][0] == '-')
            {
                readOption(arguments, &i);
                index = i + 1;
            }
        }
        if (arguments.size() - index == 2)
        {
            setInputfilePath(arguments[index]);
            setOutputfilePath(arguments[index + 1]);
        }
        else if (arguments.size() - index == 1)
        {
            setInputfilePath(arguments[index]);
        }
    }
    else
    {
        _parseError = true;
    }
}

EncoderArguments::~EncoderArguments(void)
{
}

EncoderArguments* EncoderArguments::getInstance()
{
    return __instance;
}

const std::string& EncoderArguments::getFilePath() const
{
    return _filePath;
}

const char* EncoderArguments::getFilePathPointer() const
{
    return _filePath.c_str();
}

std::string EncoderArguments::getOutputDirPath() const
{
    if (_fileOutputPath.size() > 0)
    {
        int pos = _fileOutputPath.find_last_of('/');
        return (pos == -1 ? _fileOutputPath : _fileOutputPath.substr(0, pos));
    }
    else
    {
        int pos = _filePath.find_last_of('/');
        return (pos == -1 ? _filePath : _filePath.substr(0, pos));
    }
}

std::string EncoderArguments::getOutputFilePath() const
{
    if (_fileOutputPath.size() > 0)
    {
        return _fileOutputPath;
    }
    else
    {
        int pos = _filePath.find_last_of('.');
        if (pos > 0)
        {
            std::string outputFilePath(_filePath.substr(0, pos));
            outputFilePath.append(".gpb");
            return outputFilePath;
        }
        else
        {
            std::string outputFilePath(_filePath);
            outputFilePath.append(".gpb");
            return outputFilePath;
        }
    }
}

const std::string& EncoderArguments::getDAEOutputPath() const
{
    return _daeOutputPath;
}

const std::vector<std::string>& EncoderArguments::getGroupAnimationNodeId() const
{
    return _groupAnimationNodeId;
}

const std::vector<std::string>& EncoderArguments::getGroupAnimationAnimationId() const
{
    return _groupAnimationAnimationId;
}

bool EncoderArguments::containsGroupNodeId(const std::string& nodeId) const
{
    return find(_groupAnimationNodeId.begin(), _groupAnimationNodeId.end(), nodeId) != _groupAnimationNodeId.end();
}

const std::string EncoderArguments::getAnimationId(const std::string& nodeId) const
{
    std::vector<std::string>::const_iterator it = find(_groupAnimationNodeId.begin(), _groupAnimationNodeId.end(), nodeId);
    for (size_t i = 0, size = _groupAnimationNodeId.size(); i < size; ++i)
    {
        if (_groupAnimationNodeId[i].compare(nodeId) == 0)
        {
            return _groupAnimationAnimationId[i];
        }
    }
    return "";
}

const std::vector<std::string>& EncoderArguments::getHeightmapNodeIds() const
{
    return _heightmapNodeIds;
}

bool EncoderArguments::parseErrorOccured() const
{
    return _parseError;
}

bool EncoderArguments::fileExists() const
{
    if (_filePath.length() > 0)
    {
        struct stat buf;
        if (stat(_filePath.c_str(), &buf) != -1)
        {
            return true;
        }
    }
    return false;
}

void EncoderArguments::printUsage() const
{
    fprintf(stderr,"Usage: gameplay-encoder [options] <input filepath> <output filepath>\n\n");
    fprintf(stderr,"Supported file extensions:\n");
    fprintf(stderr,"  .dae\t(COLLADA)\n");
    fprintf(stderr,"  .fbx\t(FBX)\n");
    fprintf(stderr,"  .ttf\t(TrueType Font)\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"COLLADA and FBX file options:\n");
    fprintf(stderr,"  -i <id>\t\tFilter by node ID.\n");
    fprintf(stderr,"  -t\t\t\tWrite text/xml.\n");
    fprintf(stderr,"  -g <node id> <animation id>\n" \
        "\t\t\tGroup all animation channels targetting the nodes into a new animation.\n");
    fprintf(stderr,"  -heightmaps \"<node ids>\"\n" \
        "\t\t\tList of nodes to generate heightmaps for.\n" \
        "\t\t\tNode id list should be in quotes with a space between each id.\n" \
        "\t\t\tHeightmaps will be saved in files named <nodeid>.png.\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"TTF file options:\n");
    fprintf(stderr,"  -s <size of font>\tSize of the font.\n");
    fprintf(stderr,"  -p\t\t\tOutput font preview.\n");
    exit(8);
}

bool EncoderArguments::fontPreviewEnabled() const
{
    return _fontPreview;
}

bool EncoderArguments::textOutputEnabled() const
{
    return _textOutput;
}

bool EncoderArguments::DAEOutputEnabled() const
{
    return _daeOutput;
}

const char* EncoderArguments::getNodeId() const
{
    if (_nodeId.length() == 0)
    {
        return NULL;
    }
    return _nodeId.c_str();
}

unsigned int EncoderArguments::getFontSize() const
{
    return _fontSize;
}

EncoderArguments::FileFormat EncoderArguments::getFileFormat() const
{
    if (_filePath.length() < 5)
    {
        return FILEFORMAT_UNKNOWN;
    }
    // Extract the extension
    std::string ext = "";
    size_t pos = _filePath.find_last_of(".");
    if (pos != std::string::npos)
    {
        ext = _filePath.substr(pos + 1);
    }
    
    // Match every supported extension with its format constant
    if (ext.compare("dae") == 0 || ext.compare("DAE") == 0)
    {
        return FILEFORMAT_DAE;
    }
    if (ext.compare("fbx") == 0 || ext.compare("FBX") == 0)
    {
        return FILEFORMAT_FBX;
    }
    if (ext.compare("ttf") == 0 || ext.compare("TTF") == 0)
    {
        return FILEFORMAT_TTF;
    }
    if (ext.compare("gpb") == 0 || ext.compare("GPB") == 0)
    {
        return FILEFORMAT_GPB;
    }

    return FILEFORMAT_UNKNOWN;
}

void EncoderArguments::readOption(const std::vector<std::string>& options, size_t* index)
{
    const std::string& str = options[*index];
    if (str.length() == 0 && str[0] != '-')
    {
        return;
    }
    switch (str[1])
    {
    case 'd':
        if (str.compare("-dae") == 0)
        {
            // read one string, make sure not to go out of bounds
            if ((*index + 1) >= options.size())
            {
                fprintf(stderr, "Error: -dae requires 1 argument.\n");
                _parseError = true;
                return;
            }
            (*index)++;
            _daeOutputPath = options[*index];
            _daeOutput = true;
        }
        break;
    case 'g':
        if (str.compare("-groupAnimations") == 0 || str.compare("-g") == 0)
        {
            // read two strings, make sure not to go out of bounds
            if ((*index + 2) >= options.size())
            {
                fprintf(stderr, "Error: -g requires 2 arguments.\n");
                _parseError = true;
                return;
            }
            (*index)++;
            _groupAnimationNodeId.push_back(options[*index]);
            (*index)++;
            _groupAnimationAnimationId.push_back(options[*index]);
        }
        break;
    case 'i':
    case 'o':
        // Node ID
        (*index)++;
        if (*index < options.size())
        {
            _nodeId.assign(options[*index]);
        }
        else
        {
            fprintf(stderr, "Error: missing arguemnt for -%c.\n", str[1]);
            _parseError = true;
            return;
        }
        break;
    case 'h':
        {
            if (str.compare("-heightmaps") == 0)
            {
                (*index)++;
                if (*index < options.size())
                {
                    // Split node id list into tokens
                    unsigned int length = options[*index].size() + 1;
                    char* nodeIds = new char[length];
                    strcpy(nodeIds, options[*index].c_str());
                    nodeIds[length-1] = 0;
                    char* id = strtok(nodeIds, " ");
                    while (id)
                    {
                        _heightmapNodeIds.push_back(id);
                        id = strtok(NULL, " ");
                    }
                    delete[] nodeIds;
                }
                else
                {
                    fprintf(stderr, "Error: missing argument for -heightmaps.\n");
                }
            }
        }
        break;
    case 'p':
        _fontPreview = true;
        break;
    case 's':
        // Font Size

        // old format was -s##
        if (str.length() > 2)
        {
            char n = str[2];
            if (n > '0' && n <= '9')
            {
                const char* number = str.c_str() + 2;
                _fontSize = atoi(number);
                break;
            }
        }

        (*index)++;
        if (*index < options.size())
        {
            _fontSize = atoi(options[*index].c_str());
        }
        else
        {
            fprintf(stderr, "Error: missing arguemnt for -%c.\n", str[1]);
            _parseError = true;
            return;
        }
        break;
    case 't':
        _textOutput = true;
        break;
    default:
        break;
    }
}

void EncoderArguments::setInputfilePath(const std::string& inputPath)
{
    _filePath.assign(getRealPath(inputPath));
}

void EncoderArguments::setOutputfilePath(const std::string& outputPath)
{
    if (outputPath.size() > 0 && outputPath[0] != '\0')
    {
        std::string realPath = getRealPath(outputPath);
        if (endsWith(realPath.c_str(), ".gpb"))
        {
            _fileOutputPath.assign(realPath);
        }
        else if (endsWith(outputPath.c_str(), "/"))
        {
            std::string filenameNoExt = getFilenameNoExt(getFilenameFromFilePath(_filePath));

            _fileOutputPath.assign(outputPath);
            _fileOutputPath.append(filenameNoExt);
            _fileOutputPath.append(".gpb");
        }
        else
        {
            std::string filenameNoExt = getFilenameNoExt(getFilenameFromFilePath(realPath));
            int pos = realPath.find_last_of("/");
            if (pos)
            {
                _fileOutputPath = realPath.substr(0, pos);
                _fileOutputPath.append("/");
                _fileOutputPath.append(filenameNoExt);
                _fileOutputPath.append(".gpb");
            }
        }
    }
}

std::string EncoderArguments::getRealPath(const std::string& filepath)
{
    char path[PATH_MAX + 1]; /* not sure about the "+ 1" */
    realpath(filepath.c_str(), path);
    replace_char(path, '\\', '/');
    return std::string(path);
}

void EncoderArguments::replace_char(char* str, char oldChar, char newChar)
{
    for (; *str != '\0'; ++str)
    {
        if (*str == oldChar)
        {
            *str = newChar;
        }
    }
}

std::string concat(const std::string& a, const char* b)
{
    std::string str(a);
    str.append(b);
    return str;
}


void unittestsEncoderArguments()
{
    std::string dir = EncoderArguments::getRealPath(".");
    std::string exePath = EncoderArguments::getRealPath(".");
    exePath.append("/gameplay-encoder.exe");
    const char* exe = exePath.c_str();
    {
        const char* argv[] = {exe, "-g", "root", "movements", "C:\\Git\\gaming\\GamePlay\\gameplay-encoder\\res\\seymour.dae"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getAnimationId("root"), ("movements")));
        assert(equals(args.getGroupAnimationNodeId()[0], ("root")));
        assert(equals(args.getOutputFilePath(), "C:/Git/gaming/GamePlay/gameplay-encoder/res/seymour.gpb"));
    }
    {
        // Test with only input file name (relative)
        const char* argv[] = {exe, "input.dae"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.dae")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/input.gpb")));
        equals(args.getOutputDirPath(), dir);
    }
    {
        // Test specifying a relative output path
        const char* argv[] = {exe, "input.dae", "output.gpb"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.dae")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
    }
    {
        // Test specifying a relative output path
        const char* argv[] = {exe, "input.fbx", "output.gpb"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.fbx")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
    }
    {
        // Test specifying a relative output path to a directory
        const char* argv[] = {exe, "input.dae", "stuff/output.gpb"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.dae")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/stuff/output.gpb")));
    }
    {
        // Test parsing some arguments
        const char* argv[] = {exe, "-dae", "collada.dae", "-t", "input.dae", "output.gpb"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.dae")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
        assert(args.textOutputEnabled());
        //assert(equals(args.getDAEOutputPath(), concat(dir, "/collada.dae")));
    }
    {
        // Test output file with no file extension
        const char* argv[] = {exe, "input.dae", "output"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.dae")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
    }
    {
        // Test output file with wrong file extension
        const char* argv[] = {exe, "input.dae", "output.dae"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.dae")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
    }
}

}
