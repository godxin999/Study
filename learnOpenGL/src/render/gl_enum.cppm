export module gl_enum;

namespace Engine::inline Render{
    export enum class TextureFilteringMode{
        NEAREST					= 0x2600,
		LINEAR					= 0x2601,
		NEAREST_MIPMAP_NEAREST	= 0x2700,
		LINEAR_MIPMAP_LINEAR	= 0x2703,
		LINEAR_MIPMAP_NEAREST	= 0x2701,
		NEAREST_MIPMAP_LINEAR	= 0x2702
    };
    export enum class PixelDataFormat{
        COLOR_INDEX					= 0x1900,
		STENCIL_INDEX				= 0x1901,
		DEPTH_COMPONENT				= 0x1902,
		RED							= 0x1903,
		GREEN						= 0x1904,
		BLUE						= 0x1905,
		ALPHA						= 0x1906,
		RGB							= 0x1907,
		BGR							= 0x80E0,
		RGBA						= 0x1908,
		BGRA						= 0x80E1,
		LUMINANCE					= 0x1909,
		LUMINANCE_ALPHA				= 0x190A
    };
}