#include "Base.h"
#include "GraphicsMTL.h"
#include "Game.h"
#import <simd/simd.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>


@implementation ViewDelegate
{
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView*)mtkView
{
    self = [super init];
    if(self)
    {
        _device = mtkView.device;
        _commandQueue = [_device newCommandQueue];
    }
    
    return self;
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    //
    view.clearColor = MTLClearColorMake(0, 0, 0, 1);
    
    // Create a new command buffer for each renderpass to the current drawable
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;
    if(renderPassDescriptor != nil)
    {
        
        id <MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        [renderEncoder endEncoding];
        [commandBuffer presentDrawable:view.currentDrawable];
    }
    
     [commandBuffer commit];
}

- (void)mtkView:(MTKView*)view drawableSizeWillChange:(CGSize)size
{
}
@end

namespace gameplay
{

ViewDelegate* GraphicsMTL::__viewDelegate = nullptr;
    
GraphicsMTL::GraphicsMTL() :
    _initialized(false),
    _resized(false),
    _width(0),
    _height(0)
{
}

GraphicsMTL::~GraphicsMTL()
{
}

void GraphicsMTL::onInitialize(unsigned long window, unsigned long connection)
{
    NSViewController* viewController = (__bridge NSViewController*)((void*)window);
    MTKView* mtkView = (MTKView*)viewController.view;
    id <MTLDevice> mtlDevice = MTLCreateSystemDefaultDevice();
    mtkView.device = mtlDevice;
    if(!mtkView.device)
    {
        NSLog(@"Metal is not supported on this device.");
        return;
    }
    __viewDelegate = [[ViewDelegate alloc] initWithMetalKitView:mtkView];
    mtkView.delegate = __viewDelegate;
    mtkView.preferredFramesPerSecond = 60;
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
    return _height;;
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
