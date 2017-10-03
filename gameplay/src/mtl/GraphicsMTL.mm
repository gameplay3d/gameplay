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
    _view = (MTKView*)window;
    _device = (id<MTLDevice>)connection;

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

    [commandQueue release];
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
    
std::shared_ptr<Buffer> GraphicsMTL::createVertexBuffer(const VertexFormat& vertexFormat, size_t vertexCount,
                                                        bool hostVisible)
{
    return nullptr;
}
    
std::shared_ptr<Buffer> GraphicsMTL::createIndexBuffer(IndexFormat indexFormat, size_t indexCount,
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
    
}
