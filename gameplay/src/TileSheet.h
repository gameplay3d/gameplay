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

	/**
	 * Creates a new TileSheet for drawing individual sprites and tiles from.
	 * 
	 * @param id The ID for the new TileSheet.
	 * @param texture The texture that represents the TileSheet.
	 * @param initialCapacity An optional initial capacity of the SpriteBatch.
	 * 
	 * @return A new TileSheet for drawing.
	 * @script{create}
	 */
	static TileSheet* create(const char* id, Texture* texture, unsigned int initialCapacity = 0);

	//TODO: create function for using a Properties (takes a url to the properties)

	/**
     * Get a named TileSheet from its ID.
     *
     * @param id The ID of the TileSheet to search for.
     *
     * @return The TileSheet with the specified ID, or NULL if one was not found.
     */
	static TileSheet* getTileSheet(const char* id);

	/**
     * Gets the identifier for the tilesheet.
     *
     * @return The tilesheet identifier.
     */
    const char* getId() const;

	//TODO

private:

    TileSheet(const char* id);

    virtual ~TileSheet();

    TileSheet(const TileSheet& copy);
    TileSheet& operator=(const TileSheet&);

    std::string _id;
	SpriteBatch* _batch;

	//TODO
};

}

#endif