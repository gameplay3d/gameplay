#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an abstract graphics system.
 *
 * The graphics system manages the gpu instance, device, swapchains, queues.
 *
 * This class describes classes and enumerations to describe graphics commands.
 * Commands are recorded and then submitted for processing.
 */
class Graphics
{   
public:
    /**
     * Defines a data format for buffers, texture, etc.
     */
    enum Format
    {
        FORMAT_UNDEFINED,
        FORMAT_R8_UNORM,
        FORMAT_R16_UNORM,
        FORMAT_R16_FLOAT,
        FORMAT_R32_UINT,
        FORMAT_R32_FLOAT,
        FORMAT_R8G8_UNORM,
        FORMAT_R16G16_UNORM,
        FORMAT_R16G16_FLOAT,
        FORMAT_R32G32_UINT,
        FORMAT_R32G32_FLOAT,
        FORMAT_R8G8B8_UNORM,
        FORMAT_R16G16B16_UNORM,
        FORMAT_R16G16B16_FLOAT,
        FORMAT_R32G32B32_UINT,
        FORMAT_R32G32B32_FLOAT,
        FORMAT_B8G8R8A8_UNORM,
        FORMAT_R8G8B8A8_UNORM,
        FORMAT_R16G16B16A16_UNORM,
        FORMAT_R16G16B16A16_FLOAT,
        FORMAT_R32G32B32A32_UINT,
        FORMAT_R32G32B32A32_FLOAT,
        FORMAT_D16_UNORM,
        FORMAT_X8_D24_UNORM_PACK32,
        FORMAT_D32_FLOAT,
        FORMAT_S8_UINT,
        FORMAT_D16_UNORM_S8_UINT,
        FORMAT_D24_UNORM_S8_UINT,
        FORMAT_D32_FLOAT_S8_UINT,
        FORMAT_BC1,                    // DXT1
        FORMAT_BC2,                    // DXT3
        FORMAT_BC3,                    // DXT5
        FORMAT_BC4,                    // LATC1/ATI1
        FORMAT_BC5,                    // LATC2/ATI2
        FORMAT_BC6H,                   // BC6H
        FORMAT_BC7,                    // BC7
        FORMAT_ETC1,                   // ETC1 RGB8
        FORMAT_ETC2,                   // ETC2 RGB8
        FORMAT_ETC2A,                  // ETC2 RGBA8
        FORMAT_ETC2A1,                 // ETC2 RGB8A1
        FORMAT_PVRTC12,                // PVRTC1 RGB 2BPP
        FORMAT_PVRTC14,                // PVRTC1 RGB 4BPP
        FORMAT_PVRTC12A,               // PVRTC1 RGBA 2BPP
        FORMAT_PVRTC14A,               // PVRTC1 RGBA 4BPP
        FORMAT_PVRTC22,                // PVRTC2 RGBA 2BPP
        FORMAT_PVRTC24,                // PVRTC2 RGBA 4BPP
        FORMAT_ASTC44                  // ASTC 4x4
    };

    /**
     * Defines the type of index data.
     */
    enum IndexType
    {
        INDEX_TYPE_UINT16,
        INDEX_TYPE_UINT32
    };

    /**
     * Defines the comparison function used for
     * sampling and stencil operations.
     */
    enum CompareFunc
    {
        COMPARE_FUNC_NEVER = 0,
        COMPARE_FUNC_LESS,
        COMPARE_FUNC_EQUAL,
        COMPARE_FUNC_LESS_OR_EQUAL,
        COMPARE_FUNC_GREATER,
        COMPARE_FUNC_NOT_EQUAL,
        COMPARE_FUNC_GREATER_OR_EQUAL,
        COMPARE_FUNC_ALWAYS
    };

    /**
     * Defines the operation to the stored stencil value
     * if this or certain subsequent tests fail or pass,
     */
    enum StencilOp
    {
        STENCIL_OP_KEEP = 0,
        STENCIL_OP_ZERO,
        STENCIL_OP_REPLACE,
        STENCIL_OP_INCREMENT_AND_CLAMP,
        STENCIL_OP_DECREMENT_AND_CLAMP,
        STENCIL_OP_INVERT,
        STENCIL_OP_INCREMENT_AND_WRAP,
        STENCIL_OP_DECREMENT_AND_WRAP
    };

    /**
     * Defines the logic operations.
     */
    enum LogicOp
    {
        LOGIC_OP_CLEAR = 0,
        LOGIC_OP_AND,
        LOGIC_OP_AND_REVERSE,
        LOGIC_OP_COPY,
        LOGIC_OP_AND_INVERTED,
        LOGIC_OP_NO_OP,
        LOGIC_OP_XOR,
        LOGIC_OP_OR,
        LOGIC_OP_NOR,
        LOGIC_OP_EQUIVALENT,
        LOGIC_OP_INVERT,
        LOGIC_OP_OR_REVERSE,
        LOGIC_OP_COPY_INVERTED,
        LOGIC_OP_OR_INVERTED,
        LOGIC_OP_NAND,
        LOGIC_OP_SET
    };

    /**
     * Defines the framebuffer blending factors.
     */
    enum BlendFactor
    {
        BLEND_FACTOR_ZERO = 0,
        BLEND_FACTOR_ONE,
        BLEND_FACTOR_SRC_COLOR,
        BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
        BLEND_FACTOR_DST_COLOR,
        BLEND_FACTOR_ONE_MINUS_DST_COLOR,
        BLEND_FACTOR_SRC_ALPHA,
        BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        BLEND_FACTOR_DST_ALPHA,
        BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
        BLEND_FACTOR_CONSTANT_COLOR,
        BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
        BLEND_FACTOR_CONSTANT_ALPHA,
        BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
        BLEND_FACTOR_SRC_ALPHA_SATURATE,
        BLEND_FACTOR_SRC1_COLOR,
        BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
        BLEND_FACTOR_SRC1_ALPHA,
        BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
    };

    /**
     * Defines the framebuffer blending operations.
     */
    enum BlendOp
    {
        BLEND_OP_ADD = 0,
        BLEND_OP_SUBSTRACT,
        BLEND_OP_REVERSE_SUBTRACT,
        BLEND_OP_MIN,
        BLEND_OP_MAX
    };

    /**
     * Defines whether the final color values R, G, B and A
     * are written to the framebuffer attachment(s).
     */
    enum ColorWriteComponent : unsigned int
    {
        COLOR_WRITE_COMPONENT_RED = 0x00000001,
        COLOR_WRITE_COMPONENT_GREEN = 0x00000002,
        COLOR_WRITE_COMPONENT_BLUE = 0x00000004,
        COLOR_WRITE_COMPONENT_ALPHA = 0x00000008
    };

    /**
     * The number of samples taken
     */
    enum SampleCount
    {
        SAMPLE_COUNT_1X = 1,
        SAMPLE_COUNT_2X = 2,
        SAMPLE_COUNT_4X = 4,
        SAMPLE_COUNT_8X = 8,
        SAMPLE_COUNT_16X = 16
    };

    /**
     * Defines a linear array of data which are used for various
     * purposes by binding them to a graphics or compute
     * pipeline via descriptor sets or via certain commands or
     * by directly specifying them as parameters to certain commands.
     */
    class Buffer
    {
    public:

        /**
         * Defines the allowed usage for a buffer.
         */
        enum Usage
        {
            USAGE_UNDEFINED = 0,
            USAGE_TRANSFER_SRC,
            USAGE_TRANSFER_DST,
            USAGE_UNIFORM_TEXEL,
            USAGE_STORAGE_TEXEL,
            USAGE_UNIFORM,
            USAGE_STORAGE,
            USAGE_VERTEX,
            USAGE_INDEX
        };

        /**
         * The allowed usage for the buffer.
         */
        Usage usage;

        /**
         * The number of elements in the buffer.
         */
        size_t count;

        /**
         * The stride of each element in the buffer.
         */
        size_t stride;

        /**
         * The size (in bytes) of the buffer.
         */
        size_t size;

        /**
         * If the buffer is visible to the host.
         */
        bool hostVisible;

        /**
         * The cpu memory address.
         */
        void* cpuAddress;
    };


    /**
     * Defines a texture capable of being accessed
     * directly from gpu for processing such as samplers.
     */
    class Texture
    {
    public:

        /**
         * Defines the type of texture.
         */
        enum Type
        {
            TYPE_1D,
            TYPE_2D,
            TYPE_3D
        };

        /**
         * Defines the allowed usage for a texture.
         */
        enum Usage
        {
            USAGE_UNDEFINED,
            USAGE_TRANSFER_SRC,
            USAGE_TRANSFER_DST,
            USAGE_SAMPLER,
            USAGE_STORAGE,
            USAGE_COLOR_ATTACHMENT,
            USAGE_DEPTH_STENCIL_ATTACHMENT,
            USAGE_RESOLVE_SRC,
            USAGE_RESOLVE_DST,
            USAGE_PRESENT
        };

        /**
         * The texture format.
         */
        Graphics::Format format;

        /**
         * The width of the the texture.
         */
        int width;

        /**
         * The height of the texture.
         */
        int height;

        /**
         * The depth of the texture.
         */
        int depth;

        /**
         * The number of mip levels.
         */
        int mipLevels;

        /**
         * If the texture is visible to the host.
         */
        bool hostVisible;

        /**
         * The textures cpu memory address.
         */
        void* cpuAddress;
    };


    /**
     * Defines a filters and behaviour for sampling texture data.
     */
    class Sampler
    {
    public:

        /**
         * Specify filters used for texture lookups.
         */
        enum Filter
        {
            FILTER_NEAREST = 0,
            FILTER_LINEAR
        };

        /**
         * Defines the behavior of sampling with texture coordinates outside an image.
         */
        enum AddressMode
        {
            ADDRESS_MODE_REPEAT = 0,
            ADDRESS_MODE_MIRRORED_REPEAT,
            ADDRESS_MODE_CLAMP_TO_EDGE,
            ADDRESS_MODE_CLAMP_TO_BORDER,
            ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
        };

        /**
         * The magnification filter to apply to lookups.
         */
        Filter filterMag;

        /**
         * The minification filter to apply to lookups.
         */
        Filter filterMin;

        /**
         * The mipmap filter to apply to lookups.
         */
        Filter filterMipmap;

        /**
         *  the addressing mode for outside [0..1] range for U.
         */
        AddressMode addressModeU;

        /**
         *  the addressing mode for outside [0..1] range for V.
         */
        AddressMode addressModeV;

        /**
         *  the addressing mode for outside [0..1] range for W.
         */
        AddressMode addressModeW;

        /**
         * The bias to be added to mipmap LOD calculation and bias provided by image sampling functions
         */
        float lodMipmapBias;

        /**
         * The value used to clamp the minimum computed level-of-detail value.
         */
        float lodMin;

        /**
         * The value used to clamp the maximum computed level-of-detail value.
         */
        float lodMax;

        /**
         * Used to enable anisotropic filtering.
         */
        bool anisotropyEnabled;

        /**
         * The value used to clamp the maximum anisotropy filtering amount.
         */
        float anisotropyMax;

        /**
         * Used to enable comparison against a reference value during lookup.
         */
        bool compareFuncEnabled;

        /**
         * The comparison function to apply to fetched data before filtering.
         */
        CompareFunc compareFunc;

        /**
         * Whether to use unnormalized or normalized texel coordinates to address texel
         */
        bool unormCoords;
    };


    /**
     * Defines a shader program that contains compiled and linked modules for stages.
     */
    class ShaderProgram
    {
    public:

        /**
         * Defines a stage for processing in a pipeline.
         */
        enum Stage
        {
            /**
             * Defines the vertex processing stage of a pipeline.
             */
            STAGE_VERTEX = 0x00000001,
            /**
             * Defines the tessellation control (hull) processing stage of a pipeline.
             */
            STAGE_TESSELLATION_CONTROL = 0x00000002,
            /**
             * Defines the tessellation evaluation (domain) processing stage of a pipeline.
             */
            STAGE_TESSELLATION_EVALUATION = 0x00000004,
            /**
             * Defines the geometry processing stage of a pipeline.
             */
            STAGE_GEOMETRY = 0x00000008,
            /**
             * Defines the fragment (pixel) processing stage of a pipeline.
             */
            STAGE_FRAGMENT = 0x00000010,
            /**
             * Defines a compute processing stage of a pipeline.
             */
            STAGE_COMPUTE = 0x00000020,
            /**
             * Defines a compute processing stage of a pipeline.
             */
            STAGE_ALL = 0x00000080
        };

        /**
         * The stages compiled and linked into the shader program.
         */
        unsigned int stages;
    };


    /**
     * Defines an opaque individual (resource) desccriptor that is part of a set.
     */
    class Descriptor
    {
    public:

        /**
         * Defines the type of descriptor.
         */
        enum Type
        {
            TYPE_UNDEFINED = 0,
            TYPE_UNIFORM,
            TYPE_TEXTURE,
            TYPE_SAMPLER
        };

        /**
         * The type of resource descriptor.
         */
        Type type;

        /**
         * The binding index.
         */
        unsigned int binding;

        /**
         * The stage the resource is available in.
         */
        ShaderProgram::Stage stage;

        /**
         * The number of bound resources.
         */
        size_t count;

        /**
         * The uniforms in the descriptor
         */
        Graphics::Buffer* uniforms[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];

        /**
         * The textures in the descriptor
         */
        Graphics::Texture* textures[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];

        /**
         * The samplers in the descriptor
         */
        Graphics::Sampler* samplers[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];
    };


    /**
     * Defines a heap of (resources) descriptors to be bound.
     */
    class DescriptorSet
    {
    public:
        /**
         * The array of resource
         */
        Descriptor* descriptors;

        /**
         * The number of descriptors in the set.
         */
        size_t count;
    };


    /**
     * Defines a single vertex attribute within a vertex layout.
     */
    class VertexAttribute
    {
    public:
        /**
         * Defines the intended use for bound vertex data.
         */
        enum Semantic
        {
            SEMENTIC_UNDEFINED = 0,
            SEMENTIC_POSITION,
            SEMENTIC_NORMAL,
            SEMANTIC_COLOR,
            SEMANTIC_TANGENT,
            SEMANTIC_BINORMAL,
            SEMANTIC_BLENDWEIGHTS,
            SEMANTIC_BLENDINDICES,
            SEMANTIC_TEXCOORD0,
            SEMANTIC_TEXCOORD1,
            SEMANTIC_TEXCOORD2,
            SEMANTIC_TEXCOORD3,
            SEMANTIC_TEXCOORD4,
            SEMANTIC_TEXCOORD5,
            SEMANTIC_TEXCOORD6,
            SEMANTIC_TEXCOORD7
        };

        /**
         * Defines the rate at which vertex attributes are pulled from buffers.
         */
        enum InputRate
        {
            INPUT_RATE_PER_VERTEX = 0,
            INPUT_RATE_PER_INSTANCE
        };

        /**
         * The semantic.
         */
        Semantic semantic;

        /**
         * The semantic name.
         */
        std::string semanticName;

        /**
         * The format defining the size and type of the vertex data.
         */
        Format format;

        /**
         * The binding index.
         */
        unsigned int binding;

        /**
         * The location index.
         */
        unsigned int location;

        /**
         * The byte size of this attribute relative to the start of an element in the vertex layout.
         */
        size_t offset;

        /**
         * The rate at which vertex attributes are pulled from buffers.
         */
        InputRate inputRate;
    };

    /**
     * Defines the layout of vertex attributes.
     */
    class VertexLayout
    {
    public:
        /**
         * The number of vertex attributes in the layout.
         */
        size_t attributeCount;

        /**
         * The vertex attributes to be bound.
         */
        VertexAttribute attributes[GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX];
    };

    /**
     * Defines a collection of attachments used for
     * the output of render pass operations.
     */
    class RenderPass
    {
    public:
        /**
         * The width of the render pass and attachments.
         */
        int width;

        /**
         * The height of the render pass and attachments.
         */
        int height;

        /**
         * The sample count of the render pass.
         */
        SampleCount sampleCount;

        /**
         * The color attachments format.
         */
        Format colorFormat;

        /**
         * The color attachments.
         */
        Texture* colorAttachments[GP_GRAPHICS_RENDER_TARGET_ATTACHMENTS_MAX];

        /**
         * The multi-sapled color attachments.
         */
        Texture* colorAttachmentsMultisampling[GP_GRAPHICS_RENDER_TARGET_ATTACHMENTS_MAX];

        /**
         * The depth/stencil attachment format.
         */
        Format depthStencilFormat;

        /**
         * The depth/stencil attachment.
         */
        Texture* depthStencilAttachment;
    };

    /**
     * Defines how consecutive vertices are organized into primitives,
     */
    enum PrimitiveTopology
    {
        PRIMITIVE_TOPOLOGY_POINT_LIST = 0,
        PRIMITIVE_TOPOLOGY_LINE_LIST,
        PRIMITIVE_TOPOLOGY_LINE_STRIP,
        PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
    };

    /**
     * Defines the state by which a primitive is converted (rasterized)
     * to a two-dimensional image.
     */
    class RasterizerState
    {
    public:
        /**
         * Defines the polygon rasterization mode.
         */
        enum PolygonMode
        {
            POLYGON_MODE_SOLID = 0,
            POLYGON_MODE_WIREFRAME
        };

        /**
         * Defines the triangle culling mode.
         */
        enum CullMode
        {
            CULL_MODE_NONE = 0,
            CULL_MODE_BACK,
            CULL_MODE_FRONT,
            CULL_MODE_BOTH
        };

        /**
         * Define the polygon front-facing orientation.
         */
        enum FrontFace
        {
            FRONT_FACE_CCW = 0,
            FRONT_FACE_CW
        };

        /**
         * The polygon rasterization mode.
         */
        PolygonMode polygonMode;

        /**
         * The triangle culling mode.
         */
        CullMode cullMode;

        /**
         * The polygon front-facing orientation.
         */
        FrontFace frontFace;

        /**
         * Whether to bias fragment depth values.
         */
        unsigned int depthBias;

        /**
         * The maximum (or minimum) depth bias of a fragment.
         */
        float depthBiasClamp;

        /**
         * Whether to clip the fragment’s depth values instead of clipping primitives to the z planes of the frustum.
         */
        bool depthClipEnabled;

        /**
         * The width of rasterized line segments.
         */
        float lineWidth;
    };

    /**
     * Defines the depth stencil state for the pipeline
     */
    class DepthStencilState
    {
    public:
        /**
         * Defines the stencil state comparison function and the operations
         * when stencil test fails, passes or stencil pass but depth fails.
         * This is use for front and back stencil tests.
         */
        class StencilState
        {
        public:
            /**
             *  The comparison function used in the stencil test.
             */
            CompareFunc compareFunc;

            /**
             * The action performed on samples that fail the stencil test.
             */
            StencilOp failOp;

            /**
             * The action performed on samples that pass both the depth and stencil tests.
             */
            StencilOp passOp;

            /**
             * The action performed on samples that pass the stencil test and fail the depth test.
             */
            StencilOp passDepthFailOp;
        };

        /**
         * Whether depth testing is enabled.
         */
        bool depthTest;

        /**
         * Whether depth writes are enabled when depthTest is enabled.
         */
        bool depthWrite;

        /**
         * The comparison function used in the depth test.
         */
        CompareFunc depthCompareFunc;

        /**
         * Whether stencil testing is enabled.
         */
        bool stencilTest;

        /**
         * Selects the bits of the unsigned integer stencil values participating in the stencil test.
         */
        unsigned int stencilCompareMask;

        /**
         * Selects the bits of the unsigned integer stencil values updated by the stencil test in the stencil framebuffer attachment
         */
        unsigned int stencilComparWriteMask;

        /**
         * Stencil state of the front stencil test.
         */
        StencilState stencilStateFront;

        /**
         * Stencil state of the back stencil test.
         */
        StencilState stencilStateBack;
    };

    /**
     * Defines the blending states across all color attachments.
     */
    class BlendState
    {
    public:
        /**
         * Defines the blend state for the color attachment
         */
        class ColorBlendState
        {
        public:
            /**
             * Whether blending is enabled for the corresponding color attachment.
             * If blending is not enabled, the source fragment’s color for that
             * attachment is passed through unmodified.
             */
            bool blendEnabled;

            /**
             * Selects which blend factor is used to determine the source factors (Sr,Sg,Sb).
             */
            BlendFactor blendSrcColor;

            /**
             * Selects which blend factor is used to determine the destination factors (Dr,Dg,Db).
             */
            BlendFactor blendDstColor;

            /**
             * Selects which blend operation is used to calculate the RGB values to write to the color attachment.
             */
            BlendOp blendOpColor;

            /**
             * Selects which blend factor is used to determine the source factor Sa.
             */
            BlendFactor blendSrcAlpha;

            /**
             * Selects which blend factor is used to determine the destination factor Da.
             */
            BlendFactor blendDstAlpha;

            /**
             * Selects which blend operation is use to calculate the alpha values to write to the color attachment.
             */
            BlendOp blendOpAlpha;

            /**
             * The bitmask of ColorWriteComponent's specifying which of the R, G, B, and/or A components are enabled for writing.
             */
            unsigned int colorWriteMask;
        };

        /**
         * Whether the logic operator is processed.
         */
        bool logicOpEnable;

        /**
         * The logic operator to be applied.
         */
        LogicOp logicOp;

        /**
         * The color blending state for each color attachment.
         */
        ColorBlendState colorBlendStates[GP_GRAPHICS_RENDER_TARGET_ATTACHMENTS_MAX];
    };


    /**
     * Defines a pipeline configured with a shader program
     * vertex assembly and render pass.
     */
    class Pipeline
    {
    public:

        /**
         * The program used by the render pipeline.
         */
        ShaderProgram* shaderProgram;

        /**
         * The vertex layout.
         */
        VertexLayout vertexLayout;

        /**
         * The descriptor set of resources bound to the pipeline.
         */
        DescriptorSet* descriptorSet;

        /**
         * The render pass of attachements to target pipeline output.
         */
        RenderPass* renderPass;

        /**
         * How consecutive vertices are organized into primitives.
         */
        PrimitiveTopology primitiveTopology;

        /**
         * The state by which a primitive is converted (rasterized) to a two-dimensional image.
         */
        RasterizerState rasterizerState;

        /**
         * @brief depthStencilState
         */
        DepthStencilState depthStencilState;

        /**
         * The color blending state information.
         */
        BlendState blendState;
    };

    /**
     * Defines a list of encoded commands to be processed by the gpu.
     */
    class CommandList
    {
    public:

        /**
         * Begin recording commands into the list.
         */
        virtual void begin() = 0;

        /**
         * End recording commands.
         */
        virtual void end() = 0;

        /**
         * Begins the render pass and binds the associated attachents.
         *
         * @param pass The render pass.
         */
        virtual void beginRenderPass(RenderPass* pass) = 0;

        /**
         * End the render pass and binds the associated attachents.
         *
         * @param pass The render pass.
         */
        virtual void endRenderPass(RenderPass* pass) = 0;

        /**
         * Sets the viewport region.
         *
         * @param x The x coordinate of the viewport region.
         * @param y The y coordinate of the viewport region.
         * @param width The width of the viewport region.
         * @param height The height of the viewport region.
         * @param depthMin The minimum depth of the viewport.
         * @param depthMax The maximum depth of the viewport.
         */
        virtual void setViewport(float x, float y, float width, float height, float depthMin, float depthMax) = 0;

        /**
         * Sets the scissor region.
         *
         * @param x The x coordinate of the scissor region.
         * @param y The y coordinate of the scissor region.
         * @param width The width of the scissor region.
         * @param height The height of the scissor region.
         */
        virtual void setScissor(float x, float y, float width, float height) = 0;

        /**
         * Binds a graphics pipeline.
         *
         * @param pipeline The pipeline to be bound for use.
         */
        virtual void bindPipeline(Pipeline* pipeline) = 0;

        /**
         * Binds the specified descriptor set to a pipeline.
         *
         * @param set The descriptor set to be bound.
         */
        virtual void bindDescriptorSet(DescriptorSet* set) = 0;

        /**
         * Binds an index buffer.
         *
         * @param buffer The index buffer to be bound.
         */
        virtual void bindIndexBuffer(Buffer* buffer) = 0;

        /**
         * Binds the specified vertex buffers.
         *
         * @param buffers The vertex buffers to be bound.
         * @param bufferCount The number of vertex buffers to be bound.
         */
        virtual void bindVertexBuffers(Buffer** buffers, size_t bufferCount) = 0;

        /**
         * Draws for the specified number of vertices.
         *
         * @param vertexCount The number of vertices to be drawn.
         * @param vertexOffset The offset of the first vertex to be drawn.
         */
        virtual void draw(size_t vertexCount, size_t vertexOffset) = 0;

        /**
         * Draws instanced for the specified number of vertices.
         *
         * @param vertexCount The number of vertices to be drawn.
         * @param vertexOffset The offset of the first vertex to be drawn.
         */
        virtual void drawInstanced(size_t vertexCount, size_t vertexOffset) = 0;
    };

    /**
     * Defines a management container for pooling commands.
     */
    class CommandPool
    {
    public:
        /**
         * Creates a command list for processing render commands.
         *
         * @return A command list.
         */
        virtual Graphics::CommandList* createCommandList() = 0;

        /**
         * Destroys a command list.
         *
         * @param commandList The list of commands to be destroyed.
         */
        virtual void destroyCommandList(Graphics::CommandList* commandList) = 0;
    };

    /**
     * Defines the geometry used for rendering.
     */
    class Geometry
    {
    public:
        /**
         * Get the url from which the geometry was loaded from.
         *
         * @return The url from which the geometry was loaded from.
         */
        std::string getUrl() const;

    private:

        std::string _url;
        Graphics::Buffer* _vertexBuffer;
        Graphics::Buffer* _indexBuffer;
        std::vector<std::pair<size_t, size_t>> _indexParts;
    };

    /**
     * Defines the way a surface physically looks.
     */
    class Material
    {
    public:

        /**
         * Constructor.
         */
        Material();

        /**
         * Destructor.
         */
        ~Material();

    private:

         Graphics::Buffer* _uniformBuffer;
    };


    /**
     * Defines an abstract renderer for rendering graphics components.
     */
    class Renderer : public Component
    {
        friend class SceneObject;
        friend class Serializer::Activator;
    public:

        /**
         * The type of renderer.
         */
        enum Type
        {
            TYPE_MESH,
            TYPE_TEXT,
            TYPE_SPRITE,
            TYPE_TILESET,
            TYPE_PATH,
            TYPE_PARTICLE_EMITTER,
            TYPE_SKY,
            TYPE_TERRAIN,
            TYPE_WATER,
            TYPE_TREE,
            TYPE_DECAL,
            TYPE_VIDEO,
            TYPE_UI
        };

        /**
         * Gets the geometry for the renderer.
         *
         * @return The geometry for the index specified.
         */
        const Graphics::Geometry& getGeometry() const;

        /**
         * Gets the number of materials included in the graphics component.
         *
         * @return The number of materials included in the graphics component.
         */
        size_t getMaterialCount() const;

        /**
         * Gets the material for the index specified.
         *
         * @param index The geometry index.
         * @return The material for the index specified.
         */
        const Graphics::Material& getMaterial(size_t index) const;

    private:

        Type _type;
        Graphics::Pipeline* _pipeline;
        std::vector<Graphics::Material> _materials;
    };


    /**
     * Gets the graphics system.
     *
     * @return The graphics system.
     */
    static Graphics* getGraphics();

	/**
     * Called by platform or editor to initialize the graphics system.
	 *
	 * Exits application if fails.
	 *
	 * @param window The native window object/handle.
	 * @param connection The native connection to windowing system and application.
	 */
    virtual void initialize(unsigned long window, unsigned long connection = 0) = 0;

    /**
     * Determins if the graphics system has completed initialization.
     *
     * @return true if the graphics system is initialized, false if not.
     */
    virtual bool isInitialized() = 0;

	/**
     * Called by platform or editor on window resize event
	 *
	 * @param width The width to resize to.
	 * @param height The width to resize to.
	 */
    virtual void onResize(int width, int height) = 0;

    /**
     * Determins if the graphics system have completed resizing.
     *
     * @return true if if the graphics system have completed resizing, false if not completed.
     */
    virtual bool isResized() = 0;

    /**
     * Gets the width of the graphics sytem presentation images.
     *
     * @return The width of the graphics sytem presentation images.
     */
    virtual int getWidth() = 0;

    /**
     * Gets the height of the graphics sytem presentation images.
     *
     * @return The height of the graphics sytem presentation images.
     */
    virtual int getHeight() = 0;

    /**
     * @brief createRenderer
     * @param url
     * @return
     */
    std::shared_ptr<Renderer> createRenderer(std::string url);

protected:

    /**
     * Creates a descriptor set.
     *
     * @return The created deescriptor set.
     */
    virtual Graphics::DescriptorSet* createDescriptorSet(Descriptor* descriptors, size_t descriptorCount) = 0;

    /**
     * Creates a vertex buffer.
     *
     * @param count The number of vertex elements
     * @param hostVisible true if the host has visible access to the cpu memory, false if not.
     * @param vertexAttributes The array of vertex attribute describing the vertex layout.
     * @param vertexAttributeCount The number of vertex attributes.
     * @return The created vertex buffer.
     */
    virtual Graphics::Buffer* createVertexBuffer(size_t count, bool hostVisible, VertexLayout vertexLayout) = 0;

    /**
     * Creates an index buffer.
     *
     * @param count The number of indices.
     * @param hostVisible true if the host has visible access to the cpu memory, false if not.
     * @param indexType The type of indices.
     * @return The created index buffer.
     */
    virtual Graphics::Buffer* createIndexBuffer(size_t count, bool hostVisible, IndexType indexType) = 0;

    /**
     * Creates a uniform buffer
     *
     * @param size The size in bytes of the buffer created.
     * @param hostVisible true if the host has visible access to the cpu memory, false if not.
     * @return The created buffer with the specified usage.
     */
    virtual Graphics::Buffer* createUniformBuffer(size_t size, bool hostVisible, Buffer::Usage usage) = 0;

    /**
     * Destroys a buffer.
     *
     * @param buffer The buffer to be destroyed.
     */
    virtual void destroyBuffer(Graphics::Buffer* buffer) = 0;

    /**
     * Creates a 1 dimensional texture with the specified format and dimension.
     *
     * @param format The format of the texture to be created.
     * @param width The width of the texture.
     * @param usage The usage of the texture.
     * @param hostVisible If the texture is visible to the host.
     * @return The created texture.
     */
    virtual Graphics::Texture* createTexture1D(Graphics::Format format, size_t width, Texture::Usage usage, bool hostVisible) = 0;

    /**
     * Creates a 2 dimensional texture with the specified format, dimensions and mip levels.
     *
     * @param format The format of the texture to be created.
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param mipLevelCount The number of mip levels in the texture.
     * @param usage The usage of the texture.
     * @param hostVisible If the texture is visible to the host.
     * @return The created texture.
     */
    virtual Graphics::Texture* createTexture2D(Graphics::Format format, size_t width, size_t height, size_t mipLevelCount, Texture::Usage usage, bool hostVisible) = 0;

    /**
     * Creates a 3 dimensional texture with the specified format and dimensions.
     *
     * @param format The format of the texture to be created.
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param depth The depth of the texture.
     * @param usage The usage of the texture.
     * @param hostVisible If the texture is visible to the host.
     * @return The created texture.
     */
    virtual Graphics::Texture* createTexture3D(Graphics::Format format,  size_t width, size_t height, size_t depth, Texture::Usage usage, bool hostVisible) = 0;

    /**
     * Destroys a textures.
     *
     * @param texture The texture to be destroyed.
     */
    virtual void destroyTexture(Graphics::Texture* texture) = 0;

    /**
     * Creates a sampler.
     *
     * @return The sampler created.
     */
    virtual Graphics::Sampler* createSampler() = 0;

    /**
     * Destroys a sampler.
     *
     * @param sampler The sampler to be destroyed.
     */
    virtual void destroySampler(Graphics::Sampler* sampler) = 0;

    /**
     * Creates a shader program with the specified shader stage size, byte code and entry points.
     *
     * @param vertSize The vertex shader byte code size.
     * @param vertByteCode The vertex shader byte code.
     * @param vertEntryPoint The vertex shader entry point
     * @param tescSize The tessellation control shader byte code size.
     * @param tescByteCode The tessellation control shader byte code.
     * @param tescEntryPoint The tessellation control shader entry point
     * @param teseSize The tessellation evaluation shader byte code size.
     * @param teseByteCode The tessellation evaluation shader byte code.
     * @param teseEntryPoint The tessellation evaluation shader entry point
     * @param geomSize The geometry shader byte code size.
     * @param geomByteCode The geometry shader byte code.
     * @param geomEntryPoint The geometry shader entry point
     * @param fragSize The fragment shader byte code size.
     * @param fragByteCode The fragment shader byte code.
     * @param fragEntryPoint The fragment shader entry point
     * @return The created shader program.
     */
    virtual Graphics::ShaderProgram* createShaderProgram(size_t vertSize, const void* vertByteCode, const char* vertEntryPoint,
                                                         size_t tescSize, const void* tescByteCode, const char* tescEntryPoint,
                                                         size_t teseSize, const void* teseByteCode, const char* teseEntryPoint,
                                                         size_t geomSize, const void* geomByteCode, const char* geomEntryPoint,
                                                         size_t fragSize, const void* fragByteCode, const char* fragEntryPoint) = 0;
    /**
     * Destroys a shader program.
     *
     * @param shaderProgram The shader program to be destroyed.
     */
    virtual void destroyShaderProgram(Graphics::ShaderProgram* shaderProgram) = 0;

    /**
     * Create a render pass (framebuffer).
     * @param colorFormat The color format.
     * @param colorAttachmentCount The number of color attachments.
     * @param depthStencilFormat The depth/stencil format or undefined for none.
     * @param width The width of the render papss.
     * @param height The height of the render pass.
     * @param hostVisible If the render pass if visible to the host.
     * @return The created render pass.
     */
    virtual Graphics::RenderPass* createRenderPass(Graphics::Format colorFormat,
                                                   size_t colorAttachmentCount,
                                                   Graphics::Format depthStencilFormat,
                                                   size_t width, size_t height,
                                                   bool hostVisible) = 0;
    /**
     * Destroys a render pass.
     *
     * @param renderPass The render pass to be destroyed.
     */
    virtual void destroyRenderPass(Graphics::RenderPass* renderPass) = 0;

    /**
     * Creates a pipeline.
     *
     * @param shaderProgram The shader program
     * @param vertexLayout The vertex layout.
     * @param descriptorSet The descriptor set.
     * @param renderPass The render pass.
     * @param primitiveTopology The primitive topology.
     * @param rasterizerState The rasterizer state.
     * @param depthStencilState The depth/stencil state.
     * @param blendState The blend state.
     * @return The created pipeline.
     */
    virtual Graphics::Pipeline* createPipeline(Graphics::ShaderProgram* shaderProgram,
                                               Graphics::VertexLayout* vertexLayout,
                                               Graphics::DescriptorSet* descriptorSet,
                                               Graphics::RenderPass* renderPass,
                                               Graphics::PrimitiveTopology primitiveTopology,
                                               Graphics::RasterizerState rasterizerState,
                                               Graphics::DepthStencilState depthStencilState,
                                               Graphics::BlendState blendState) = 0;
    /**
     * Destroys a pipeline.
     *
     * @param pipeline The pipeline to be destroyed.
     */
    virtual void destroyPipeline(Graphics::Pipeline* pipeline) = 0;

    /**
     * Creates a command pool from which you can create command lists.
     *
     * @return The created command pool.
     */
    virtual Graphics::CommandPool* createCommandPool() = 0;

    /**
     * Destroys a command pool.
     *
     * @param pool The command pool to be destroyed.
     */
    virtual void destroyCommandPool(Graphics::CommandPool* pool) = 0;

    /**
     * Submit commands for processing into the queue.
     *
     * @param commandLists The command lists to be processed.
     * @param commandCount The number of command list to be processed.
     */
    virtual void submit(Graphics::CommandList** commandLists, size_t commandListCount) = 0;

    /**
     * Queues the presentation of swap images the platforms target surface.
     */
    virtual bool present() = 0;

    /**
     * Waits idle for an infinite time for all commands in the queue to be processed.
     */
    virtual void waitIdle() = 0;

    /**
     * Aquires the next swapchain image.
     */
    virtual void acquireNextSwapchainImage() = 0;

    /**
     * Called by the Game to render the currently active scene.
     *
     * TODO: Move to Game::render
     *
     * @param elapsedTime The elapsed time since the last render.
     */
    virtual void render(float elapsedTime) = 0;


    static Graphics* _graphics;

};

}
