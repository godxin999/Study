module;
#include <glad/glad.h>
module renderer;

import stl;
import gl_enum;
import camera;
import glm;

namespace Engine::inline Render{
    void Renderer::SetClearColor(float r,float g,float b,float a){
        glClearColor(r,g,b,a);
    }
    void Renderer::Clear(bool color,bool depth,bool stencil){
        GLbitfield mask=0;
        if(color) mask|=GL_COLOR_BUFFER_BIT;
        if(depth) mask|=GL_DEPTH_BUFFER_BIT;
        if(stencil) mask|=GL_STENCIL_BUFFER_BIT;
        glClear(mask);
    }
    void Renderer::Clear(Camera& p_camera,bool color,bool depth,bool stencil){
        float previousClearColor[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE,previousClearColor);
        const glm::vec3& clearColor=p_camera.GetClearColor();
        SetClearColor(clearColor.r,clearColor.g,clearColor.b,1.f);
        Clear(color,depth,stencil);
        SetClearColor(previousClearColor[0],previousClearColor[1],previousClearColor[2],previousClearColor[3]);
    }

    void Renderer::SetRasterizationLinesWidth(float width){
        glLineWidth(width);
    }
    void Renderer::SetRasterizationMode(RasterizationMode mode){
        glPolygonMode(GL_FRONT_AND_BACK,static_cast<GLenum>(mode));
    }
    void Renderer::SetRenderCapability(RenderingCapability capability,bool enable){
        (enable?glEnable:glDisable)(static_cast<GLenum>(capability));
    }
    bool Renderer::GetRenderCapability(RenderingCapability capability)const{
        return glIsEnabled(static_cast<GLenum>(capability));
    }
    void Renderer::SetDepthTestFunction(TestFunction function){
        glDepthFunc(static_cast<GLenum>(function));
    }
    void Renderer::SetStencilTestFunction(TestFunction function,int32_t reference,uint32_t mask){
        glStencilFunc(static_cast<GLenum>(function),reference,mask);
    }
    void Renderer::SetStencilMask(uint32_t mask){
        glStencilMask(mask);
    }
    void Renderer::SetStencilOperations(Operation stencilFail,Operation depthFail,Operation bothPass){
        glStencilOp(static_cast<GLenum>(stencilFail),static_cast<GLenum>(depthFail),static_cast<GLenum>(bothPass));
    }
    void Renderer::SetCullFaceMode(CullFaceMode mode){
        glCullFace(static_cast<GLenum>(mode));
    }
    void Renderer::SetDepthWriting(bool enable){
        glDepthMask(enable);
    }
    void Renderer::SetColorWriting(bool enableR,bool enableG,bool enableB,bool enableA){
        glColorMask(enableR,enableG,enableB,enableA);
    }
    void Renderer::SetColorWriting(bool enable){
        glColorMask(enable,enable,enable,enable);
    }
    void Renderer::SetViewPort(uint32_t x,uint32_t y,uint32_t width,uint32_t height){
        glViewport(x,y,width,height);
    }
    void Renderer::ReadPixels(uint32_t x,uint32_t y,uint32_t width,uint32_t height,PixelDataFormat p_format,PixelDataType type,void* pixelData){
        glReadPixels(x,y,width,height,static_cast<GLenum>(p_format),static_cast<GLenum>(type),pixelData);
    }
    void Renderer::ClearFrameInfo(){
        m_frameInfo.batchCount=0;
        m_frameInfo.instanceCount=0;
        m_frameInfo.polygonCount=0;
        m_frameInfo.vertexCount=0;
    }
    const Renderer::FrameInfo& Renderer::GetFrameInfo()const{
        return m_frameInfo;
    }
}