#include "Base.h"
#include "GraphicsMTL.h"
#include "UtilsMTL.h"
#include "BufferMTL.h"
#include "TextureMTL.h"
#include "RenderPassMTL.h"
#include "SamplerMTL.h"
#include "ShaderMTL.h"
#include "DescriptorSetMTL.h"
#include "RenderPipelineMTL.h"
#include "CommandPoolMTL.h"
#include "CommandBufferMTL.h"
#include "SemaphoreMTL.h"
#include "FenceMTL.h"
#include "Game.h"
#include "FileSystem.h"
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

std::shared_ptr<RenderPass> GraphicsMTL::acquireNextSwapchainImage(std::shared_ptr<Fence> signalFence,
                                                                   std::shared_ptr<Semaphore> signalSemaphore)
{
    return nullptr;
}

void GraphicsMTL::present(std::vector<std::shared_ptr<Semaphore>> waitSemaphores)
{
}

void GraphicsMTL::waitIdle(std::shared_ptr<Fence> waitFence)
{
}

std::shared_ptr<CommandPool> GraphicsMTL::createCommandPool()
{
    return nullptr;
}

void GraphicsMTL::destroyCommandPool(std::shared_ptr<CommandPool> commandPool)
{
}

void GraphicsMTL::submit(std::vector<std::shared_ptr<CommandBuffer>> commandBuffers,
                         std::vector<std::shared_ptr<Semaphore>> signalSemaphores,
                         std::vector<std::shared_ptr<Semaphore>> waitSemaphores)
{
}

void GraphicsMTL::cmdBegin(std::shared_ptr<CommandBuffer> commandBuffer)
{
}

void GraphicsMTL::cmdEnd(std::shared_ptr<CommandBuffer> commandBuffer)
{
}

void GraphicsMTL::cmdBeginRender(std::shared_ptr<CommandBuffer> commandBuffer,
                                 std::shared_ptr<RenderPass> renderPass)
{
}

void GraphicsMTL::cmdEndRender(std::shared_ptr<CommandBuffer> commandBuffer)
{
}

void GraphicsMTL::cmdSetViewport(std::shared_ptr<CommandBuffer> commandBuffer,
                                 float x, float, float width, float height, 
                                 float depthMin, float depthMax)
{
}

void GraphicsMTL::cmdSetScissor(std::shared_ptr<CommandBuffer> commandBuffer,
                                size_t x, size_t y, 
                                size_t width, size_t height)
{
}

void GraphicsMTL::cmdClearColorAttachment(std::shared_ptr<CommandBuffer> commandBuffer,
                                          size_t attachmentTndex,
                                          const ClearValue& clearValue)
{
}

void GraphicsMTL::cmdBindRenderPipeline(std::shared_ptr<CommandBuffer> commandBuffer,
                                        std::shared_ptr<RenderPipeline> pipeline)
{
}

void GraphicsMTL::cmdBindDescriptorSet(std::shared_ptr<CommandBuffer> commandBuffer,
                                       std::shared_ptr<RenderPipeline> pipeline, 
                                       std::shared_ptr<DescriptorSet> descriptorSet)
{
}

void GraphicsMTL::cmdBindVertexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                                      std::shared_ptr<Buffer> vertexBuffer)
{
}

void GraphicsMTL::cmdBindVertexBuffers(std::shared_ptr<CommandBuffer> commandBuffer,
                                       std::vector<std::shared_ptr<Buffer>> vertexBuffers)
{
}

void GraphicsMTL::cmdBindIndexBuffer(std::shared_ptr<CommandBuffer> commandBuffer,
                                     std::shared_ptr<Buffer> indexBuffer)
{
}

void GraphicsMTL::cmdDraw(std::shared_ptr<CommandBuffer> commandBuffer,
                          size_t vertexCount, size_t vertexStart)
{
}

void GraphicsMTL::cmdDrawIndexed(std::shared_ptr<CommandBuffer> commandBuffer,
                                 size_t indexCount, size_t indexStart)
{
}

void GraphicsMTL::cmdTransitionImage(std::shared_ptr<CommandBuffer> commandBuffer,
                                     std::shared_ptr<Texture> texture, 
                                     Texture::Usage usagePrev, 
                                     Texture::Usage usageNext)
{
}

std::shared_ptr<Semaphore> GraphicsMTL::createSemaphore()
{
    return nullptr;
}

void GraphicsMTL::destroySemaphore(std::shared_ptr<Semaphore> semaphore)
{
}

std::shared_ptr<Fence> GraphicsMTL::createFence()
{
    return nullptr;
}

void GraphicsMTL::destroyFence(std::shared_ptr<Fence> fence)
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
                                                      const ClearValue& clearValue,
                                                      bool hostVisible)
{
    return nullptr;
}

std::shared_ptr<Texture> GraphicsMTL::createTexture2d(size_t width, size_t height, size_t mipLevels,
                                                      Format pixelFormat,
                                                      Texture::Usage usage,
                                                      Texture::SampleCount sampleCount,
                                                      const ClearValue& clearValue,
                                                      bool hostVisible)
{
    return nullptr;
}

std::shared_ptr<Texture> GraphicsMTL::createTexture3d(size_t width, size_t height, size_t depth,
                                                      Format pixelFormat,
                                                      Texture::Usage usage,
                                                      Texture::SampleCount sampleCount,
                                                      const ClearValue& clearValue,
                                                      bool hostVisible)
{
    return nullptr;
}

void GraphicsMTL::destroyTexture(std::shared_ptr<Texture> texture)
{
}

std::shared_ptr<RenderPass> GraphicsMTL::createRenderPass(size_t width, size_t height,
                                                            size_t colorAttachmentCount,
                                                            Format colorFormat,
                                                            Format depthStencilFormat,
                                                            Texture::SampleCount sampleCount)
{
    return nullptr;
}

void GraphicsMTL::destroyRenderPass(std::shared_ptr<RenderPass> renderPass)
{
}

std::shared_ptr<Sampler> GraphicsMTL::createSampler(Sampler::Filter filterMin,
                                                    Sampler::Filter filterMag,
                                                    Sampler::Filter filterMip,
                                                    Sampler::AddressMode addressModeU,
                                                    Sampler::AddressMode addressModeV,
                                                    Sampler::AddressMode addressModeW,
                                                    Sampler::BorderColor borderColor,
                                                    bool compareEnabled,
                                                    Sampler::CompareFunc compareFunc,
                                                    bool anisotropyEnabled,
                                                    float anisotropyMax,
                                                    float lodMin,
                                                    float lodMax,
                                                    float lodMipBias)
{
    return nullptr;
}

void GraphicsMTL::destroySampler(std::shared_ptr<Sampler> sampler)
{
}

std::shared_ptr<Shader> GraphicsMTL::createShader(const std::string& url)
{
    return nullptr;
}

void GraphicsMTL::destroyShader(std::shared_ptr<Shader> shader)
{
}


std::shared_ptr<DescriptorSet> GraphicsMTL::createDescriptorSet(const DescriptorSet::Descriptor* descriptors, 
                                                                size_t descriptorCount)
{
    return nullptr;
}

void GraphicsMTL::destroyDescriptorSet(std::shared_ptr<DescriptorSet> descriptorSet)
{
}

std::shared_ptr<RenderPipeline> GraphicsMTL::createRenderPipeline(RenderPipeline::PrimitiveTopology primitiveTopology,
                                                                  VertexLayout vertexLayout,
                                                                  RasterizerState rasterizerState,
                                                                  ColorBlendState colorBlendState,
                                                                  DepthStencilState depthStencilState,
                                                                  std::shared_ptr<RenderPass> renderPass,
                                                                  std::shared_ptr<DescriptorSet> descriptorSet,
                                                                  std::shared_ptr<Shader> vertShader,
                                                                  std::shared_ptr<Shader> tescShader,
                                                                  std::shared_ptr<Shader> teseShader,
                                                                  std::shared_ptr<Shader> geomShader,
                                                                  std::shared_ptr<Shader> fragShader)
{
    return nullptr;
}

void GraphicsMTL::destroyRenderPipeline(std::shared_ptr<RenderPipeline> pipeline)
{
}
    
}
