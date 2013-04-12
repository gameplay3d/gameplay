#ifndef Sampler_H_
#define Sampler_H_

#include "Base.h"
#include "Constants.h"

namespace gameplay
{

class Sampler
{
public:

    /**
     * Constructor.
     */
    Sampler(const char* id);

    /**
     * Destructor.
     */
    virtual ~Sampler(void);

    const std::string& getId() const;

    const char* getString(const std::string& name);
    void set(const std::string& name, const std::string& value);

    /**
     * Writes this sampler to a material file.
     * 
     * @param file The file pointer.
     * @param indent The number of indentation levels.
     * @param parent The parent sampler from this material's parent.
     */
    void writeMaterial(FILE* file, unsigned int indent, Sampler* parent = NULL);

private:

    void writeProperty(FILE* file, const std::string& name, unsigned int indent, Sampler* parent = NULL);

private:

    std::string _id;
    std::map<std::string, std::string> props;
};

}

#endif
