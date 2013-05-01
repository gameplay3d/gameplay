#include "Base.h"

#include "EncoderArguments.h"
#include "StringUtil.h"

#ifdef WIN32
    #define PATH_MAX    _MAX_PATH
    #define realpath(A,B)    _fullpath(B,A,PATH_MAX)
#endif

#define MAX_HEIGHTMAP_SIZE 2049

namespace gameplay
{

static EncoderArguments* __instance;

extern int __logVerbosity = 1;

EncoderArguments::EncoderArguments(size_t argc, const char** argv) :
    _fontSize(0),
    _normalMap(false),
    _parseError(false),
    _fontPreview(false),
    _textOutput(false),
    _optimizeAnimations(false),
    _animationGrouping(ANIMATIONGROUP_PROMPT),
    _outputMaterial(false)
{
    __instance = this;

    memset(_heightmapResolution, 0, sizeof(int) * 2);

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

std::string EncoderArguments::getOutputFileExtension() const
{
    switch (getFileFormat())
    {
    case FILEFORMAT_PNG:
    case FILEFORMAT_RAW:
        if (_normalMap)
            return ".png";

    default:
        return ".gpb";
    }
}

std::string EncoderArguments::getOutputFilePath() const
{
    if (_fileOutputPath.size() > 0)
    {
        // Output file explicitly set
        return _fileOutputPath;
    }
    else
    {
        // Generate an output file path
        int pos = _filePath.find_last_of('.');
        std::string outputFilePath(pos > 0 ? _filePath.substr(0, pos) : _filePath);

        // Modify the original file name if the output extension can be the same as the input
        if (_normalMap)
        {
            outputFilePath.append("_normalmap");
        }

        outputFilePath.append(getOutputFileExtension());
        return outputFilePath;
    }
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
    for (size_t i = 0, size = _groupAnimationNodeId.size(); i < size; ++i)
    {
        if (_groupAnimationNodeId[i].compare(nodeId) == 0)
        {
            return _groupAnimationAnimationId[i];
        }
    }
    return "";
}

EncoderArguments::AnimationGroupOption EncoderArguments::getAnimationGrouping() const
{
    return _animationGrouping;
}

const std::vector<EncoderArguments::HeightmapOption>& EncoderArguments::getHeightmapOptions() const
{
    return _heightmaps;
}

unsigned int EncoderArguments::tangentBinormalIdCount() const
{
    return _tangentBinormalId.size();
}

bool EncoderArguments::isGenerateTangentBinormalId(const std::string& id) const
{
    return _tangentBinormalId.find(id) != _tangentBinormalId.end();
}

bool EncoderArguments::normalMapGeneration() const
{
    return _normalMap;
}

void EncoderArguments::getHeightmapResolution(int* x, int* y) const
{
    *x = _heightmapResolution[0];
    *y = _heightmapResolution[1];
}

const Vector3& EncoderArguments::getHeightmapWorldSize() const
{
    return _heightmapWorldSize;
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

void splitString(const char* str, std::vector<std::string>* tokens)
{
    // Split node id list into tokens
    unsigned int length = strlen(str);
    char* temp = new char[length + 1];
    strcpy(temp, str);
    char* tok = strtok(temp, ",");
    while (tok)
    {
        tokens->push_back(tok);
        tok = strtok(NULL, ",");
    }
    delete[] temp;
}

void EncoderArguments::printUsage() const
{
    LOG(1, "Usage: gameplay-encoder [options] <input filepath> <output filepath>\n\n" \
    "Supported file extensions:\n" \
    "  .fbx\t(FBX)\n" \
    "  .ttf\t(TrueType Font)\n" \
    "\n" \
    "General Options:\n" \
    "  -v <verbosity>\tVerbosity level (0-4).\n" \
    "\n" \
    "FBX file options:\n" \
    "  -i <id>\tFilter by node ID.\n" \
    "  -t\t\tWrite text/xml.\n" \
    "  -g:auto\tAutomatically group animation channels into a new animation.\n" \
    "  -g:none\tDo not prompt to group animations.\n" \
    "  -g <node id> <animation id>\n" \
        "\t\tGroup all animation channels targeting the nodes into a \n" \
        "\t\tnew animation.\n" \
    "  -m\t\tOutput material file for scene.\n" \
    "  -tb <node id>\n" \
        "\t\tGenerates tangents and binormals for the given node.\n" \
    "  -oa\n" \
        "\t\tOptimizes animations by analyzing animation channel data and\n" \
        "\t\tremoving any channels that contain default/identity values\n" \
        "\t\tand removing any duplicate contiguous keyframes, which are \n" \
        "\t\tcommon when exporting baked animation data.\n" \
    "  -h <size> \"<node ids>\" <filename>\n" \
        "\t\tGenerates a single heightmap image using meshes from the \n" \
        "\t\tspecified nodes. \n" \
        "\t\t<size> is two comma-separated numbers in the format \"X,Y\", \n" \
        "\t\tindicating the dimensions of the produced heightmap image.\n" \
        "\t\t<node ids> should be in quotes with a space between each id.\n" \
        "\t\tFilename is the name of the image (PNG) to be saved.\n" \
        "\t\tMultiple -h arguments can be supplied to generate more than one \n" \
        "\t\theightmap. For 24-bit packed height data use -hp instead of -h.\n" \
    "\n" \
    "Normal map generation options:\n" \
        "  -n\t\tGenerate normal map (requires input file of type PNG or RAW)\n" \
        "  -s\t\tSize/resolution of the input heightmap image \n" \
        "    \t\t(required for RAW files)\n" \
        "  -w <size>\tSpecifies the size of an input terrain heightmap file in world\n" \
        "\t\tunits, along the X, Y and Z axes. <size> should be three \n" \
        "\t\tcomma-separated numbers in the format \"X,Y,Z\". The Y value \n" \
        "\t\trepresents the maximum possible height value of a full \n" \
        "\t\tintensity heightmap pixel.\n" \
        "\n" \
        "  Normal map generation can be used to create object-space normal maps from \n" \
        "  heightmap images. Heightmaps must be in either PNG format (where the \n" \
        "  intensity of each pixel represents a height value), or in RAW format \n" \
        "  (8 or 16-bit), which is a common headerless format supported by most \n" \
        "  terrain generation tools.\n" \
    "\n" \
    "TTF file options:\n" \
    "  -s <size>\tSize of the font.\n" \
    "  -p\t\tOutput font preview.\n" \
    "\n");
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

bool EncoderArguments::optimizeAnimationsEnabled() const
{
    return _optimizeAnimations;
}

bool EncoderArguments::outputMaterialEnabled() const
{
    return _outputMaterial;
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
    for (size_t i = 0; i < ext.size(); ++i)
        ext[i] = (char)tolower(ext[i]);
    
    // Match every supported extension with its format constant
    if (ext.compare("dae") == 0)
    {
        return FILEFORMAT_DAE;
    }
    if (ext.compare("fbx") == 0)
    {
        return FILEFORMAT_FBX;
    }
    if (ext.compare("ttf") == 0)
    {
        return FILEFORMAT_TTF;
    }
    if (ext.compare("gpb") == 0)
    {
        return FILEFORMAT_GPB;
    }
    if (ext.compare("png") == 0)
    {
        return FILEFORMAT_PNG;
    }
    if (ext.compare("raw") == 0)
    {
        return FILEFORMAT_RAW;
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
    case 'g':
        if (str.compare("-groupAnimations:auto") == 0 || str.compare("-g:auto") == 0)
        {
            _animationGrouping = ANIMATIONGROUP_AUTO;
        }
        else if (str.compare("-groupAnimations:off") == 0 || str.compare("-g:off") == 0)
        {
            _animationGrouping = ANIMATIONGROUP_OFF;
        }
        else if (str.compare("-groupAnimations") == 0 || str.compare("-g") == 0)
        {
            // read two strings, make sure not to go out of bounds
            if ((*index + 2) >= options.size())
            {
                LOG(1, "Error: -g requires 2 arguments.\n");
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
        // Node ID
        (*index)++;
        if (*index < options.size())
        {
            _nodeId.assign(options[*index]);
        }
        else
        {
            LOG(1, "Error: missing arguemnt for -%c.\n", str[1]);
            _parseError = true;
            return;
        }
        break;
    case 'o':
        // Optimization flag
        if (str == "-oa")
        {
            // Optimize animations
            _optimizeAnimations = true;
        }
        break;
    case 'h':
        {
            bool isHighPrecision = str.compare("-hp") == 0;
            if (str.compare("-heightmap") == 0 || str.compare("-h") == 0 || isHighPrecision)
            {
                (*index)++;
                if (*index < (options.size() + 2))
                {
                    _heightmaps.resize(_heightmaps.size() + 1);
                    HeightmapOption& heightmap = _heightmaps.back();
                    
                    heightmap.isHighPrecision = isHighPrecision;

                    // Read heightmap size
                    std::vector<std::string> parts;
                    splitString(options[*index].c_str(), &parts);
                    if (parts.size() != 2)
                    {
                        LOG(1, "Error: invalid size argument for -h|-heightmap.\n");
                        _parseError = true;
                        return;
                    }
                    heightmap.width = atoi(parts[0].c_str());
                    heightmap.height = atoi(parts[1].c_str());

                    // Put some artificial bounds on heightmap dimensions
                    if (heightmap.width <= 0 || heightmap.height <= 0 || heightmap.width > MAX_HEIGHTMAP_SIZE || heightmap.height > MAX_HEIGHTMAP_SIZE)
                    {
                        LOG(1, "Error: size argument for -h|-heightmap must be between (1,1) and (%d,%d).\n", (int)MAX_HEIGHTMAP_SIZE, (int)MAX_HEIGHTMAP_SIZE);
                        _parseError = true;
                        return;
                    }

                    // Split node id list into tokens
                    (*index)++;
                    splitString(options[*index].c_str(), &heightmap.nodeIds);

                    // Store output filename
                    (*index)++;
                    heightmap.filename = options[*index];
                    if (heightmap.filename.empty())
                    {
                        LOG(1, "Error: missing filename argument for -h|-heightmap.\n");
                        _parseError = true;
                        return;
                    }
                    
                    // Ensure the output filename has a .png extention
                    if (heightmap.filename.length() > 5)
                    {
                        const char* ext = heightmap.filename.c_str() + (heightmap.filename.length() - 4);
                        if (ext[0] != '.' || tolower(ext[1]) != 'p' || tolower(ext[2]) != 'n' || tolower(ext[3]) != 'g')
                            heightmap.filename += ".png";
                    }
                    else
                        heightmap.filename += ".png";
                }
                else
                {
                    LOG(1, "Error: missing argument for -h|-heightmap.\n");
                    _parseError = true;
                    return;
                }
            }
        }
        break;
    case 'm':
        if (str.compare("-m") == 0)
        {
            // generate a material file
            _outputMaterial = true;
        }
        break;
    case 'n':
        _normalMap = true;
        break;
    case 'w':
        {
            // Read world size
            (*index)++;
            if (*index >= options.size())
            {
                LOG(1, "Error: missing world size argument for -w.\n");
                _parseError = true;
                return;
            }
            std::vector<std::string> parts;
            splitString(options[*index].c_str(), &parts);
            if (parts.size() != 3)
            {
                LOG(1, "Error: invalid world size argument for -w.\n");
                _parseError = true;
                return;
            }
            _heightmapWorldSize.x = (float)atof(parts[0].c_str());
            _heightmapWorldSize.y = (float)atof(parts[1].c_str());
            _heightmapWorldSize.z = (float)atof(parts[2].c_str());
            if (_heightmapWorldSize.x == 0 || _heightmapWorldSize.y == 0 || _heightmapWorldSize.z == 0)
            {
                LOG(1, "Error: invalid world size argument for -w.\n");
                _parseError = true;
                return;
            }
        }
        break;
    case 'p':
        _fontPreview = true;
        break;
    case 's':
        if (_normalMap)
        {
            (*index)++;
            if (*index >= options.size())
            {
                LOG(1, "Error: missing argument for -s.\n");
                _parseError = true;
                return;
            }
            // Heightmap size
            std::vector<std::string> parts;
            splitString(options[*index].c_str(), &parts);
            if (parts.size() != 2 ||
                (_heightmapResolution[0] = atoi(parts[0].c_str())) <= 0 ||
                (_heightmapResolution[1] = atoi(parts[1].c_str())) <= 0)
            {
                LOG(1, "Error: invalid argument for -s.\n");
                _parseError = true;
                return;
            }
        }
        else
        {
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
                LOG(1, "Error: missing arguemnt for -%c.\n", str[1]);
                _parseError = true;
                return;
            }
        }
        break;
    case 't':
        if (str.compare("-t") == 0)
        {
            _textOutput = true;
        }
        else if (str.compare("-tb") == 0)
        {
            if ((*index + 1) >= options.size())
            {
                LOG(1, "Error: -tb requires 1 argument.\n");
                _parseError = true;
                return;
            }
            (*index)++;
            std::string nodeId = options[*index];
            if (nodeId.length() > 0)
            {
                _tangentBinormalId.insert(nodeId);
            }
        }
        break;
    case 'v':
        (*index)++;
        if (*index < options.size())
        {
            __logVerbosity = atoi(options[*index].c_str());
            if (__logVerbosity < 0)
                __logVerbosity = 0;
            else if (__logVerbosity > 4)
                __logVerbosity = 4;
        }
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
    std::string ext = getOutputFileExtension();

    if (outputPath.size() > 0 && outputPath[0] != '\0')
    {
        std::string realPath = getRealPath(outputPath);
        if (endsWith(realPath.c_str(), ext.c_str()))
        {
            _fileOutputPath.assign(realPath);
        }
        else if (endsWith(outputPath.c_str(), "/"))
        {
            std::string filenameNoExt = getFilenameNoExt(getFilenameFromFilePath(_filePath));

            _fileOutputPath.assign(outputPath);
            _fileOutputPath.append(filenameNoExt);
            _fileOutputPath.append(ext);
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
                _fileOutputPath.append(ext);
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
        const char* argv[] = {exe, "-g", "root", "movements", "C:\\Git\\gaming\\GamePlay\\gameplay-encoder\\res\\duck.fbx"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getAnimationId("root"), ("movements")));
        assert(equals(args.getGroupAnimationNodeId()[0], ("root")));
        assert(equals(args.getOutputFilePath(), "C:/Git/gaming/GamePlay/gameplay-encoder/res/duck.gpb"));
    }
    {
        // Test with only input file name (relative)
        const char* argv[] = {exe, "input.fbx"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.fbx")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/input.gpb")));
        equals(args.getOutputDirPath(), dir);
    }
    {
        // Test specifying a relative output path
        const char* argv[] = {exe, "input.fbx", "output.gpb"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.fbx")));
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
        const char* argv[] = {exe, "input.fbx", "stuff/output.gpb"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.fbx")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/stuff/output.gpb")));
    }
    {
        // Test parsing some arguments
        const char* argv[] = {exe, "test.fbx", "-t", "input.fbx", "output.gpb"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.fbx")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
        assert(args.textOutputEnabled());
    }
    {
        // Test output file with no file extension
        const char* argv[] = {exe, "input.fbx", "output"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.fbx")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
    }
    {
        // Test output file with wrong file extension
        const char* argv[] = {exe, "input.fbx", "output.fbx"};
        EncoderArguments args(sizeof(argv) / sizeof(char*), (const char**)argv);
        assert(equals(args.getFilePath(), concat(dir, "/input.fbx")));
        assert(equals(args.getOutputFilePath(), concat(dir, "/output.gpb")));
    }
}

}
