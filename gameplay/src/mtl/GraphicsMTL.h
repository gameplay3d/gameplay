#pragma once

#include "Graphics.h"
#import <MetalKit/MetalKit.h>

namespace gameplay
{

/**
 * Metal impl of Graphics
 *
 * @see Graphics
 */
class GraphicsMTL : public Graphics
{
public:
    
    /**
     * Constructor.
     */
    GraphicsMTL();
    
    /**
     * Destructor.
     */
    ~GraphicsMTL();
    
    /**
     * @see Graphics::onInitialize
     */
    void onInitialize(unsigned long window, unsigned long connection = 0);
    
    /**
     * @see Graphics::isInitialized
     */
    bool isInitialized();
    
    /**
     * @see Graphics::onResize
     */
    void onResize(int width, int height);
    
    /**
     * @see Graphics::isResized
     */
    bool isResized();
    
    /**
     * @see Graphics::getWidth
     */
    int getWidth();
    
    /**
     * @see Graphics::getHeight
     */
    int getHeight();
    
    /**
     * @see Graphics::createCommandPool
     */
    std::shared_ptr<CommandPool> createCommandPool(bool transient = false);
    
    /**
     * @see Graphics::destroyCommandPool
     */
    void destroyCommandPool(std::shared_ptr<CommandPool> commandPool);
    
    /**
     * @see Graphics::createCommandLists
     */
    std::shared_ptr<CommandList> createCommandList(std::shared_ptr<CommandPool> pool, bool secondary);
    
    /**
     * @see Graphics::createCommandLists
     */
    void createCommandLists(std::shared_ptr<CommandPool> pool, bool secondary, size_t count,
                            std::vector<std::shared_ptr<CommandList>> out);
    /**
     * @see Graphics::destroyCommandList
     */
    void destroyCommandList(std::shared_ptr<CommandList> commandList);
    
    /**
     * @see Graphics::destroyCommandLists
     */
    void destroyCommandLists(std::vector<std::shared_ptr<CommandList>> commandLists);
    
    /**
     * @see Graphics::submitCommands
     */
    void submitCommands(std::shared_ptr<CommandList> commands);
    
    /**
     * @see Graphics::submitCommands
     */
    void submitCommands(std::vector<std::shared_ptr<CommandList>> commands);
    
    /**
     * @see Graphics::flushCommands
     */
    void flushCommands();
    
    /**
     * @see Graphics::present
     */
    void present();

    /**
     * @see Graphics::createVertexBuffer
     */
    std::shared_ptr<Buffer> createVertexBuffer(size_t size, size_t vertexStride, bool hostVisible);
    
    /**
     * @see Graphics::createIndexBuffer
     */
    std::shared_ptr<Buffer> createIndexBuffer(size_t size, IndexFormat indexFormat, bool hostVisible);

    /**
     * @see Graphics::createUniformBuffer
     */
    std::shared_ptr<Buffer> createUniformBuffer(size_t size, bool hostVisible);

    /**
     * @see Graphics::destroyBuffer
     */
    void destroyBuffer(std::shared_ptr<Buffer> buffer);

    /**
     * @see Graphics::createTexture1d
     */
    std::shared_ptr<Texture> createTexture1d(size_t width, 
                                            Texture::Usage usage, 
                                            Texture::PixelFormat pixelFormat, 
                                            Texture::SampleCount sampleCount, 
                                            bool hostVisible);
    /**
     * @see Graphics::createTexture2d
     */
    std::shared_ptr<Texture> createTexture2d(size_t width, size_t height, size_t mipLevels,
                                             Texture::Usage usage, 
                                             Texture::PixelFormat pixelFormat, 
                                             Texture::SampleCount sampleCount, 
                                             bool hostVisible);
    /**
     * @see Graphics::createTexture3d
     */
    std::shared_ptr<Texture> createTexture3d(size_t width, size_t height, size_t depth, 
                                             Texture::Usage usage, 
                                             Texture::PixelFormat pixelFormat, 
                                             Texture::SampleCount sampleCount,  
                                             bool hostVisible);
    /**
     * @see Graphics::destroyTexture
     */
    void destroyTexture(std::shared_ptr<Texture> texture);
private:
    
    bool _initialized;
    bool _resized;
    uint32_t _width;
    uint32_t _height;
    bool _fullscreen;
    bool _vsync;
    MTKView* _view;
    id<MTLDevice> _device;
};

}
