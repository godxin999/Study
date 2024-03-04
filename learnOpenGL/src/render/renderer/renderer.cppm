export module renderer;

import stl;
import glm;
import gl_enum;

namespace Engine::inline Render{
    export class Renderer{
    public:
        struct FrameInfo{
            uint64_t batchCount{0};
            uint64_t instanceCount{0};
            uint64_t polygonCount{0};
            uint64_t vertexCount{0};
        };
        Renderer();
        ~Renderer()=default;
        void SetClearColor(const glm::vec4& color);
        void Clear(bool color=true,bool depth=true,bool stencil=true);
        void SetRasterizationLinesWidth(float width);
        void SetRasterizationMode(RasterizationMode mode);
        void SetRenderCapability(RenderingCapability capability,bool enable);
        bool GetRenderCapability(RenderingCapability capability)const;
        void SetDepthTestFunction(TestFunction function);
        void SetStencilTestFunction(TestFunction function,int32_t reference,uint32_t mask);
        void SetStencilMask(uint32_t mask);
        void SetStencilOperations(Operation stencilFail=Operation::KEEP,Operation depthFail=Operation::KEEP,Operation bothPass=Operation::KEEP);
        void SetCullFaceMode(CullFaceMode mode);
        void SetDepthWriting(bool enable);
        void SetColorWriting(bool enableR,bool enableG,bool enableB,bool enableA);
        void SetColorWriting(bool enable);
        void SetViewPort(uint32_t x,uint32_t y,uint32_t width,uint32_t height);
        void ReadPixels(uint32_t x,uint32_t y,uint32_t width,uint32_t height,PixelDataFormat p_format,PixelDataType type,void* pixelData);
        
        //Get

        void ClearFrameInfo();
        const FrameInfo& GetFrameInfo()const;
    private:
        FrameInfo m_frameInfo{};
    };
}