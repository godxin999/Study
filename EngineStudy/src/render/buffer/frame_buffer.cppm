module;
#include <glad/glad.h>
export module frame_buffer;

import stl;

namespace Engine::inline Render{
    export class FrameBuffer{
    public:
        FrameBuffer(uint16_t width=0, uint16_t height=0);
        ~FrameBuffer();
        void Bind();
        void Unbind();
        void Resize(uint16_t width, uint16_t height);
        [[nodiscard]]uint32_t GetBufferID()const;
        [[nodiscard]]uint32_t GetRenderTextureID()const;
        [[nodiscard]]uint32_t GetDepthStencilBufferID()const;
    private:
        uint32_t m_bufferID{0};
        uint32_t m_renderTextureID{0};
        uint32_t m_depthStencilBufferID{0};
    };
}

module : private;

namespace Engine::inline Render{
    FrameBuffer::FrameBuffer(uint16_t width, uint16_t height){
        //创建frame buffer、render texture和depth stencil buffer
        glGenFramebuffers(1,&m_bufferID);
        glGenTextures(1,&m_renderTextureID);
        glGenRenderbuffers(1,&m_depthStencilBufferID);
        //设置render texture的参数
        glBindTexture(GL_TEXTURE_2D, m_renderTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        //将纹理对象绑定到frame buffer的颜色附件点
        Bind();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTextureID, 0);
        Unbind();

        Resize(width, height);
    }

    FrameBuffer::~FrameBuffer(){
        glDeleteFramebuffers(1, &m_bufferID);
        glDeleteTextures(1, &m_renderTextureID);
        glDeleteRenderbuffers(1, &m_depthStencilBufferID);
    }

    void FrameBuffer::Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, m_bufferID);
    }

    void FrameBuffer::Unbind(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Resize(uint16_t width, uint16_t height){
        //为纹理对象分配内存
        glBindTexture(GL_TEXTURE_2D, m_renderTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
        //为渲染(深度模板)缓冲对象分配内存
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        //将渲染缓冲对象绑定到frame buffer的深度和模板附件点
        Bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_depthStencilBufferID);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_depthStencilBufferID);
        Unbind();
    }

    uint32_t FrameBuffer::GetBufferID()const{
        return m_bufferID;
    }
    
    uint32_t FrameBuffer::GetRenderTextureID()const{
        return m_renderTextureID;
    }
    
    uint32_t FrameBuffer::GetDepthStencilBufferID()const{
        return m_depthStencilBufferID;
    }
}