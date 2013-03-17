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
	friend class Sprite;
	friend class SpriteMap;

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
     * Gets the identifier for the tile sheet.
     *
     * @return The tile sheet identifier.
     */
    const char* getId() const;

	/**
	 * Gets the SpriteBatch that the tile sheet uses.
	 * 
	 * @return The SpriteBatch from the tile sheet.
	 */
	SpriteBatch* getSpriteBatch() const;

	/**
	 * Gets the number of strips within this tile sheet.
	 * 
	 * @return The number of strips within this tile sheet.
	 */
	unsigned int getStripCount() const;

	/**
	 * Gets the ID of a strip.
	 * 
	 * @param stripIndex The index of a strip within the tile sheet.
	 * 
	 * @return The ID of the strip.
	 */
	const char* getStripId(unsigned int stripIndex) const;

	/**
	 * Gets the number of frames within a strip.
	 * 
	 * @param id The ID of the strip.
	 * 
	 * @return The number of frames within the strip.
	 */
	unsigned int getStripFrameCount(const char* id) const;

	/**
	 * Gets the number of frames within a strip.
	 * 
	 * @param index The index of a strip within the tile sheet.
	 * 
	 * @return The number of frames within the strip.
	 */
	unsigned int getStripFrameCount(unsigned int index) const;

	/**
	 * Add a new strip to the tile sheet.
	 * 
	 * @param id The ID of the new strip.
	 * @param frameCount The number of frames in the new strip. Must be greater then zero.
	 */
	void addStrip(const char* id, unsigned int frameCount);

	/**
	 * Removes a strip from the tile sheet.
	 * 
	 * @param index The index of a strip to remove.
	 * 
	 * @return A boolean value indicating if the strip was removed or not.
	 */
	bool removeStrip(unsigned int index);

	/**
	 * Removes a strip from the tile sheet.
	 * 
	 * @param id The ID of the strip.
	 * 
	 * @return A boolean value indicating if the strip was removed or not.
	 */
	bool removeStrip(const char* id);

	/**
	 * Gets a frame from a strip.
	 * 
	 * @param stripIndex The index of a strip within the tile sheet.
	 * @param frameIndex The frame to retrieve.
	 * 
	 * @return The frame from the strip, or Rectangle::empty() if it doesn't exist.
	 */
	const Rectangle& getStripFrame(unsigned int stripIndex, unsigned int frameIndex) const;

	/**
	 * Gets a frame from a strip.
	 * 
	 * @param id The ID of the strip.
	 * @param frameIndex The frame to retrieve.
	 * 
	 * @return The frame from the strip, or Rectangle::empty() if it doesn't exist.
	 */
	const Rectangle& getStripFrame(const char* id, unsigned int frameIndex) const;

	/**
	 * Sets a frame within a strip.
	 * 
	 * @param stripIndex The index of a strip within the tile sheet.
	 * @param frameIndex The frame to set.
	 * @param frame The frame to use.
	 */
	void setStripFrame(unsigned int stripIndex, unsigned int frameIndex, const Rectangle& frame);

	/**
	 * Sets a frame within a strip.
	 * 
	 * @param id The ID of the strip.
	 * @param frameIndex The frame to set.
	 * @param frame The frame to use.
	 */
	void setStripFrame(const char* id, unsigned int frameIndex, const Rectangle& frame);

private:

	class FrameStrip
	{
	public:
		FrameStrip(const char* id, unsigned int frameCount);
		~FrameStrip();

		std::string _id;
		std::vector<Rectangle> _frames;
		unsigned int _frameCount;
	};

    TileSheet(const char* id);

    virtual ~TileSheet();

	int getStrip(const char* id) const;

    TileSheet(const TileSheet& copy);
    TileSheet& operator=(const TileSheet&);

    std::string _id;
	SpriteBatch* _batch;
	std::vector<FrameStrip> _strips;
};

}

#endif