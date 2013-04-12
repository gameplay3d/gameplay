#ifndef ENCODERARGUMENTS_H_
#define ENCODERARGUMENTS_H_

#include <set>
#include "Vector3.h"

namespace gameplay
{

/**
 * EncoderArguments handles parsing the command line arguments for the GamePlay Encoder.
 */
class EncoderArguments
{
public:

    enum FileFormat
    {
        FILEFORMAT_UNKNOWN,
        FILEFORMAT_DAE,
        FILEFORMAT_FBX,
        FILEFORMAT_TTF,
        FILEFORMAT_GPB,
        FILEFORMAT_PNG,
        FILEFORMAT_RAW
    };

    struct HeightmapOption
    {
        std::vector<std::string> nodeIds;
        std::string filename;
        bool isHighPrecision;
        int width;
        int height;
    };

    struct NormalMapOption
    {
        std::string inputFile;
        std::string outputFile;
        Vector3 worldSize;
    };

    enum AnimationGroupOption
    {
        ANIMATIONGROUP_PROMPT,
        ANIMATIONGROUP_AUTO,
        ANIMATIONGROUP_OFF
    };
    
    /**
     * Constructor.
     */
    EncoderArguments(size_t argc, const char** argv);

    /**
     * Destructor.
     */
    ~EncoderArguments(void);

    /**
     * Gets the EncoderArguments instance.
     */
    static EncoderArguments* getInstance();

    /**
     * Gets the file format from the file path based on the extension.
     */
    FileFormat getFileFormat() const;

    /**
     * Returns the file path.
     */
    const std::string& getFilePath() const;

    /**
     * Returns the char pointer to the file path string.
     */
    const char* getFilePathPointer() const;

    /**
     * Returns the output path/folder.
     * Example: "C:/dir"
     */
    std::string getOutputDirPath() const;

    /**
     * Returns the output file path.
     * Example: "C:/dir/scene.gpb"
     */
    std::string getOutputFilePath() const;

    /**
     * Returns the output file extension.
     */
    std::string getOutputFileExtension() const;

    const std::vector<std::string>& getGroupAnimationNodeId() const;
    const std::vector<std::string>& getGroupAnimationAnimationId() const;

    bool containsGroupNodeId(const std::string& nodeId) const;
    const std::string getAnimationId(const std::string& nodeId) const;

    AnimationGroupOption getAnimationGrouping() const;

    const std::vector<HeightmapOption>& getHeightmapOptions() const;

    /**
     * Returns the number of node IDs that were marked as needing to compute tangents and binormals.
     */
    unsigned int tangentBinormalIdCount() const;
    
    /**
     * Returns true if the given node ID was marked as needing to generate tangents and binormals.
     */
    bool isGenerateTangentBinormalId(const std::string& id) const;

    /**
     * Returns true if normal map generation is turned on.
     */
    bool normalMapGeneration() const;
    
    /**
     * Returns the supplied intput heightmap resolution.
     *
     * This option is only applicable for normal map generation.
     */
    void getHeightmapResolution(int* x, int* y) const;

    /**
     * Returns world size option.
     *
     * This option is only applicable for normal map generation.
     */
    const Vector3& getHeightmapWorldSize() const;
    
    /**
     * Returns true if an error occurred while parsing the command line arguments.
     */
    bool parseErrorOccured() const;

    /**
     * Tests if a file exists on the file system.
     * 
     * @return True if the file exists; false otherwise.
     */
    bool fileExists() const;

    /**
     * Prints the usage information.
     */
    void printUsage() const;

    bool fontPreviewEnabled() const;
    bool textOutputEnabled() const;
    bool optimizeAnimationsEnabled() const;
    bool outputMaterialEnabled() const;

    const char* getNodeId() const;
    unsigned int getFontSize() const;


    static std::string getRealPath(const std::string& filepath);

private:

    /**
     * Reads the command line option from the list of options starting at the given index.
     * 
     * @param options The list of command line options.
     * @param index Pointer to the index within the options list. The index will be changed
     *              if an option takes multiple arguments.
     */
    void readOption(const std::vector<std::string>& options, size_t *index);

    void setInputfilePath(const std::string& inputPath);

    /**
     * Sets the output file path that the encoder will write to.
     */
    void setOutputfilePath(const std::string& outputPath);
    

    /**
     * Replaces all instance of oldChar with newChar in str.
     */
    static void replace_char(char* str, char oldChar, char newChar);

private:
    
    std::string _filePath;
    std::string _fileOutputPath;
    std::string _nodeId;

    unsigned int _fontSize;

    bool _normalMap;
    Vector3 _heightmapWorldSize;
    int _heightmapResolution[2];

    bool _parseError;
    bool _fontPreview;
    bool _textOutput;
    bool _optimizeAnimations;
    AnimationGroupOption _animationGrouping;
    bool _outputMaterial;

    std::vector<std::string> _groupAnimationNodeId;
    std::vector<std::string> _groupAnimationAnimationId;
    std::vector<HeightmapOption> _heightmaps;
    std::set<std::string> _tangentBinormalId;

};

void unittestsEncoderArguments();

}

#endif
