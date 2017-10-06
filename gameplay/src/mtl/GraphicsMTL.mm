#include "Base.h"
#include "GraphicsMTL.h"
#include "Game.h"
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

namespace gameplay
{

GraphicsMTL::GraphicsMTL() :
    _initialized(false),
    _resized(false),
    _width(0),
    _height(0),
    _view(nullptr),
    _device(nullptr)
{
}

GraphicsMTL::~GraphicsMTL()
{
}

void GraphicsMTL::onInitialize(unsigned long window, unsigned long connection)
{
    _view = (__bridge MTKView*)((void*)window);
    _device = _view.device;

    id<CAMetalDrawable> drawable = _view.currentDrawable;
    id<MTLTexture> texture = drawable.texture;

    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    passDescriptor.colorAttachments[0].texture = texture;
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);

    id<MTLCommandQueue> commandQueue = [_device newCommandQueue];
    id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
    id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    [commandEncoder endEncoding];

    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];

    _initialized = true;
}

bool GraphicsMTL::isInitialized()
{
    return _initialized;
}
    
void GraphicsMTL::onResize(int width, int height)
{
}
    
bool GraphicsMTL::isResized()
{
    return _resized;
}

int GraphicsMTL::getWidth()
{
    return _width;
}

int GraphicsMTL::getHeight()
{
    return _height;
}
    

std::shared_ptr<CommandPool> GraphicsMTL::createCommandPool(bool transient)
{
    return nullptr;
}
    
void GraphicsMTL::destroyCommandPool(std::shared_ptr<CommandPool> commandPool)
{
}
    
std::shared_ptr<CommandList> GraphicsMTL::createCommandList(std::shared_ptr<CommandPool> pool, bool secondary)
{
    return nullptr;
}
    
void GraphicsMTL::createCommandLists(std::shared_ptr<CommandPool> pool, bool secondary, size_t count,
                                     std::vector<std::shared_ptr<CommandList>> out)
{
}
    
void GraphicsMTL::destroyCommandList(std::shared_ptr<CommandList> commandList)
{
}
    
void GraphicsMTL::destroyCommandLists(std::vector<std::shared_ptr<CommandList>> commandLists)
{
}
    
void GraphicsMTL::submitCommands(std::shared_ptr<CommandList> commands)
{
}

void GraphicsMTL::submitCommands(std::vector<std::shared_ptr<CommandList>> commands)
{
}

void GraphicsMTL::flushCommands()
{
}

void GraphicsMTL::present()
{
}

std::shared_ptr<Buffer> GraphicsMTL::createVertexBuffer(size_t size, size_t vertexStride,
                                                        bool hostVisible)
{
    return nullptr;
}
    
std::shared_ptr<Buffer> GraphicsMTL::createIndexBuffer(size_t size, IndexFormat indexFormat,
                                                       bool hostVisible)
{
    return nullptr;
}
    
std::shared_ptr<Buffer> GraphicsMTL::createUniformBuffer(size_t size, bool hostVisible)
{
    return nullptr;
}

void GraphicsMTL::destroyBuffer(std::shared_ptr<Buffer> buffer)
{
}

std::shared_ptr<Texture> GraphicsMTL::createTexture1d(size_t width,
                                                      Format pixelFormat,
                                                      Texture::Usage usage,
                                                      Texture::SampleCount sampleCount,
                                                      bool hostVisible)
{
    return nullptr;
}

std::shared_ptr<Texture> GraphicsMTL::createTexture2d(size_t width, size_t height, size_t mipLevels,
                                                      Format pixelFormat,
                                                      Texture::Usage usage,
                                                      Texture::SampleCount sampleCount, 
                                                      bool hostVisible)
{
    return nullptr;
}

std::shared_ptr<Texture> GraphicsMTL::createTexture3d(size_t width, size_t height, size_t depth,
                                                      Format pixelFormat,
                                                      Texture::Usage usage,
                                                      Texture::SampleCount sampleCount,  
                                                      bool hostVisible)
{
    return nullptr;
}

void GraphicsMTL::destroyTexture(std::shared_ptr<Texture> texture)
{
}
    
}
