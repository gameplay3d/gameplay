#ifndef TILESHEET_H_
#define TILESHEET_H_

#include "SpriteBatch.h"

namespace gameplay
{

/**
 * Defines a collection of images that can be used indepentently of each other or 
 * together to form animations or large scenes.
 */
class TileSheet : public Ref
{

public:

	//TODO

	/**
     * Gets the identifier for the tilesheet.
     *
     * @return The tilesheet identifier.
     */
    const char* getId() const;

	//TODO

protected:

	/**
     * Constructor.
     */
    TileSheet(const char* id);

    /**
     * Destructor.
     */
    virtual ~TileSheet();

	//TODO

private:

    TileSheet(const TileSheet& copy);
    TileSheet& operator=(const TileSheet&);

    std::string _id;

	//TODO
};

}

#endif