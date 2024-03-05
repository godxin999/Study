export module vertex_array;

import stl;

namespace Engine::inline Render{
    export enum class DataType{
        BYTE			= 0x1400,
		UNISGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406,
		DOUBLE			= 0x140A
    };
    export class VertexArray{
    public:
        VertexArray();
        ~VertexArray();
        void Bind();
        void Unbind();
        void BindAttribute(uint32_t index, size_t size, DataType type, bool normalized, uint64_t stride, const void* pointer);
        uint32_t GetID() const{
            return m_arrayID;
        }
    private:
        uint32_t m_arrayID;
    };
}