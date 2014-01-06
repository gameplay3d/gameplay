#include "Base.h"
#include "FBXSceneEncoder.h"
#include "TTFFontEncoder.h"
#include "GPBDecoder.h"
#include "EncoderArguments.h"
#include "NormalMapGenerator.h"
#include "Font.h"

using namespace gameplay;

#define FONT_SIZE_DISTANCEFIELD 48

/**
 * Prompts the user for a font size until a valid font size is entered.
 * 
 * @return A valid font size.
 */
static std::vector<unsigned int> promptUserFontSize()
{
    static const int lowerBound = 8;
    static const int upperBound = 96;
    std::vector<unsigned int> fontSizes;
    bool parseError = false;
    char buffer[4096];
    do
    {
        parseError = false;
        fontSizes.clear();

        // Prompt for font sizes
        printf("Enter a comma-separated list of font sizes (pixels) to generate:\n");
        std::cin.getline(buffer, 4096);

        // Parse comma-separated list of fonts sizes and validate
        char* ptr = const_cast<char*>(buffer);
        std::string sizeStr;
        while (ptr)
        {
            char* end = strchr(ptr, ',');
            if (end)
            {
                sizeStr = std::string(ptr, end - ptr);
                ptr = end + 1;
            }
            else
            {
                sizeStr = ptr;
                ptr = NULL;
            }
            if (sizeStr.length() > 0)
            {
                int size = atoi(sizeStr.c_str());
                if (size < lowerBound || size > upperBound)
                {
                    printf("Invalid font size: %d. Must be between %d-%d.\n", size, lowerBound, upperBound);
                    parseError = true;
                    continue;
                }
                fontSizes.push_back((unsigned int)size);
            }
        }
    } while (parseError);

    return fontSizes;
}


/**
 * Main application entry point.
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line arguments.
 *
 * usage:   gameplay-encoder[options] <file_list>
 * example: gameplay-encoder C:/assets/duck.fbx
 * example: gameplay-encoder -i boy duck.fbx
 *
 * @stod: Improve argument parsing.
 */
int main(int argc, const char** argv)
{
    EncoderArguments arguments(argc, argv);

    if (arguments.parseErrorOccured())
    {
        arguments.printUsage();
        return 0;
    }

    // Check if the file exists.
    if (!arguments.fileExists())
    {
        LOG(1, "Error: File not found: %s\n", arguments.getFilePathPointer());
        return -1;
    }

    // File exists
    LOG(1, "Encoding file: %s\n", arguments.getFilePathPointer());

    switch (arguments.getFileFormat())
    {
    case EncoderArguments::FILEFORMAT_DAE:
        {
            LOG(1, "Error: Collada support has been removed. Convert your DAE file to FBX.\n");
            return -1;
        }
    case EncoderArguments::FILEFORMAT_FBX:
        {
            std::string realpath(arguments.getFilePath());
            FBXSceneEncoder fbxEncoder;
            fbxEncoder.write(realpath, arguments);
            break;
        }
    case EncoderArguments::FILEFORMAT_TTF:
        {
            std::vector<unsigned int> fontSizes = arguments.getFontSizes();
            
            Font::FontFormat fontFormat = arguments.getFontFormat();
            if (fontFormat == Font::BITMAP)
            {
                if (fontSizes.size() == 0)
                {
                    fontSizes = promptUserFontSize();
                }
            }
            else
            {
                // Distance fields use size
                if (fontSizes.size() == 0)
                {
                    fontSizes.push_back(FONT_SIZE_DISTANCEFIELD);
                }
            }
            std::string id = getBaseName(arguments.getFilePath());
            writeFont(arguments.getFilePath().c_str(), arguments.getOutputFilePath().c_str(), fontSizes, id.c_str(), arguments.fontPreviewEnabled(), fontFormat);
            break;
        }
    case EncoderArguments::FILEFORMAT_GPB:
        {
            std::string realpath(arguments.getFilePath());
            GPBDecoder decoder;
            decoder.readBinary(realpath);
            break;
        }
    case EncoderArguments::FILEFORMAT_PNG:
    case EncoderArguments::FILEFORMAT_RAW:
        {
            if (arguments.normalMapGeneration())
            {
                int x, y;
                arguments.getHeightmapResolution(&x, &y);
                NormalMapGenerator generator(arguments.getFilePath().c_str(), arguments.getOutputFilePath().c_str(), x, y, arguments.getHeightmapWorldSize());
                generator.generate();
            }
            else
            {
                LOG(1, "Error: Nothing to do for specified file format. Did you forget an option?\n");
                return -1;
            }
            break;
        }
   default:
        {
            LOG(1, "Error: Unsupported file format: %s\n", arguments.getFilePathPointer());
            return -1;
        }
    }

    return 0;
}
