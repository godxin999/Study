export module gl_enum;

namespace Engine::inline Render{
    //纹理过滤模式
    export enum class TextureFilteringMode{
        NEAREST					= 0x2600, //邻近过滤
		LINEAR					= 0x2601, //线性过滤
		NEAREST_MIPMAP_NEAREST	= 0x2700, //使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
		LINEAR_MIPMAP_NEAREST	= 0x2701, //使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
		NEAREST_MIPMAP_LINEAR	= 0x2702,  //在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
		LINEAR_MIPMAP_LINEAR	= 0x2703 //在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
    };
    //像素数据格式
    export enum class PixelDataFormat{
        COLOR_INDEX					= 0x1900,
		STENCIL_INDEX				= 0x1901,
		DEPTH_COMPONENT				= 0x1902,
		RED							= 0x1903,
		GREEN						= 0x1904,
		BLUE						= 0x1905,
		ALPHA						= 0x1906,
		RGB							= 0x1907,
		RGBA						= 0x1908,
		LUMINANCE					= 0x1909,
		LUMINANCE_ALPHA				= 0x190A,
		BGR							= 0x80E0,
		BGRA						= 0x80E1
    };
    //像素数据类型
    export enum class PixelDataType{
        BYTE						= 0x1400,
		UNSIGNED_BYTE				= 0x1401,
		BITMAP						= 0x1A00,
		SHORT						= 0x1402,
		UNSIGNED_SHORT				= 0x1403,
		INT							= 0x1404,
		UNSIGNED_INT				= 0x1405,
		FLOAT						= 0x1406,
		UNSIGNED_BYTE_3_3_2			= 0x8032,
		UNSIGNED_SHORT_4_4_4_4		= 0x8033,
		UNSIGNED_SHORT_5_5_5_1		= 0x8034,
		UNSIGNED_INT_8_8_8_8		= 0x8035,
		UNSIGNED_INT_10_10_10_2		= 0x8036,
		UNSIGNED_BYTE_2_3_3_REV		= 0x8362,
		UNSIGNED_SHORT_5_6_5		= 0x8363,
		UNSIGNED_SHORT_5_6_5_REV	= 0x8364,
		UNSIGNED_SHORT_4_4_4_4_REV	= 0x8365,
		UNSIGNED_SHORT_1_5_5_5_REV	= 0x8366,
		UNSIGNED_INT_8_8_8_8_REV	= 0x8367,
		UNSIGNED_INT_2_10_10_10_REV	= 0x8368
    };
    //数据缓冲区访问方式
    export enum class AccessSpecifier{
        //数据每次绘制都会发生变化
        STREAM_DRAW		= 0x88E0,
		STREAM_READ		= 0x88E1,
		STREAM_COPY		= 0x88E2,
        //数据每次绘制都会改变很多
		DYNAMIC_DRAW	= 0x88E8,
		DYNAMIC_READ	= 0x88E9,
		DYNAMIC_COPY	= 0x88EA,
        //数据每次绘制都不会改变或者几乎不会改变
		STATIC_DRAW		= 0x88E4,
		STATIC_READ		= 0x88E5,
		STATIC_COPY		= 0x88E6
    };
    //渲染功能
    export enum class RenderingCapability{
		CULL_FACE					= 0x0B44, //面剔除
		DEPTH_TEST					= 0x0B71, //深度测试
		STENCIL_TEST				= 0x0B90, //模板测试
		DITHER						= 0x0BD0, //颜色抖动
        BLEND						= 0x0BE2, //颜色混合(像素颜色和颜色缓冲区颜色混合)
		SCISSOR_TEST				= 0x0C11, //裁剪测试
		POLYGON_OFFSET_FILL			= 0x8037, //多边形偏移填充
		SAMPLE_ALPHA_TO_COVERAGE	= 0x809E, //样本alpha覆盖
		MULTISAMPLE					= 0x809D, //多重采样
		SAMPLE_COVERAGE				= 0x80A0  //样本覆盖
    };
    //光栅化方式
    export enum class RasterizationMode{
        POINT	= 0x1B00,
		LINE	= 0x1B01,
		FILL	= 0x1B02
    };
    //基元类型
    export enum class PrimitiveMode{
        POINTS						= 0x0000,
		LINES						= 0x0001,
		LINE_LOOP					= 0x0002,
		LINE_STRIP					= 0x0003,
		TRIANGLES					= 0x0004,
		TRIANGLE_STRIP				= 0x0005,
		TRIANGLE_FAN				= 0x0006,
		LINES_ADJACENCY				= 0x000A,
		LINE_STRIP_ADJACENCY		= 0x000B,
		TRIANGLES_ADJACENCY			= 0x000C,
		TRIANGLE_STRIP_ADJACENCY	= 0x000D,
		PATCHES						= 0xE
    };
    //数值操作
    export enum class Operation{
		ZERO			= 0,
        KEEP			= 0x1E00,
		REPLACE			= 0x1E01,
		INCREMENT		= 0x1E02,
		DECREMENT		= 0x1E03,
		INVERT			= 0x150A,
		INCREMENT_WRAP	= 0x8507,
		DECREMENT_WRAP	= 0x8508
    };
    //面剔除方式
    export enum class CullFaceMode{
        FRONT			= 0x0404,
		BACK			= 0x0405,
		FRONT_AND_BACK	= 0x0408
    };
    //深度/模板测试函数
    export enum class TestFunction{
        NEVER			= 0x0200,
		LESS			= 0x0201,
		EQUAL			= 0x0202,
		LESS_EQUAL		= 0x0203,
		GREATER			= 0x0204,
		NOTEQUAL		= 0x0205,
		GREATER_EQUAL	= 0x0206,
		ALWAYS			= 0x0207
    };
}