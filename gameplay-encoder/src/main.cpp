#include <string>

#include "DAESceneEncoder.h"
#include "TTFFontEncoder.h"
#include "GPBDecoder.h"
#include "EncoderArguments.h"

using namespace gameplay;

std::string getFileName(const std::string& filepath)
{
    size_t index1 = filepath.find_last_of('\\');
    size_t index2 = filepath.find_last_of('/');
    size_t index = (index1 != -1 && index1 > index2 ? index1 : index2);
    size_t length = filepath.length();
    std::string filename = filepath.substr(index + 1, length);
    length = filename.length();
    std::string output = filename.substr(0, (length-4));
    return output;
}

/**
 * Main application entry point.
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line arguments.
 *
 * usage:   gameplay-encoder[options] <file_list>
 * example: gameplay-encoder C:/assets/seymour.dae
 * example: gameplay-encoder -i boy seymour.dae
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
        fprintf(stderr, "Error: File not found: %s\n", arguments.getFilePathPointer());
        return -1;
    }

    // File exists
    fprintf(stderr, "Encoding file: %s\n", arguments.getFilePathPointer());

    switch (arguments.getFileFormat())
    {
    case EncoderArguments::FILEFORMAT_DAE:
        {
            std::string realpath = arguments.getFilePath();
            DAESceneEncoder daeEncoder;
            daeEncoder.write(realpath, arguments);
            break;
        }
    case EncoderArguments::FILEFORMAT_TTF:
        {
            std::string realpath = arguments.getFilePath();
            std::string id = getFileName(realpath);
            writeFont(realpath.c_str(), arguments.getFontSize(), id.c_str(), arguments.fontPreviewEnabled());
            break;
        }
    case EncoderArguments::FILEFORMAT_GPB:
        {
            std::string realpath = arguments.getFilePath();
            GPBDecoder decoder;
            decoder.readBinary(realpath);
            break;
        }
   default:
        {
            fprintf(stderr, "Error: Unsupported file format: %s\n", arguments.getFilePathPointer());
            return -1;
        }
    }

    return 0;
}

