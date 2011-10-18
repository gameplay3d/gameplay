#ifndef __DOM_TYPES_H__
#define __DOM_TYPES_H__

#include <dae/daeDomTypes.h>

typedef xsDouble		domFloat;
typedef xsLong		domInt;
typedef xsUnsignedLong		domUint;
typedef xsString		domSidref;
typedef xsNCName		domSid;
typedef xsBooleanArray		domList_of_bools;
typedef daeTArray<domFloat>		domList_of_floats;
typedef xsHexBinaryArray		domList_of_hex_binary;
typedef daeTArray<domInt>		domList_of_ints;
typedef xsNameArray		domList_of_names;
typedef xsNameArray		domList_of_idrefs;
typedef daeTArray<domSidref>		domList_of_sidrefs;
typedef xsTokenArray		domList_of_tokens;
typedef daeTArray<domUint>		domList_of_uints;
typedef domList_of_bools		domBool2;
typedef domList_of_bools		domBool3;
typedef domList_of_bools		domBool4;
typedef domList_of_bools		domBool2x2;
typedef domList_of_bools		domBool2x3;
typedef domList_of_bools		domBool2x4;
typedef domList_of_bools		domBool3x2;
typedef domList_of_bools		domBool3x3;
typedef domList_of_bools		domBool3x4;
typedef domList_of_bools		domBool4x2;
typedef domList_of_bools		domBool4x3;
typedef domList_of_bools		domBool4x4;
typedef domList_of_floats		domFloat2;
typedef domList_of_floats		domFloat3;
typedef domList_of_floats		domFloat4;
typedef domList_of_floats		domFloat7;
typedef domList_of_floats		domFloat2x2;
typedef domList_of_floats		domFloat2x3;
typedef domList_of_floats		domFloat2x4;
typedef domList_of_floats		domFloat3x2;
typedef domList_of_floats		domFloat3x3;
typedef domList_of_floats		domFloat3x4;
typedef domList_of_floats		domFloat4x2;
typedef domList_of_floats		domFloat4x3;
typedef domList_of_floats		domFloat4x4;
typedef domList_of_ints		domInt2;
typedef domList_of_ints		domInt3;
typedef domList_of_ints		domInt4;
typedef domList_of_ints		domInt2x2;
typedef domList_of_ints		domInt2x3;
typedef domList_of_ints		domInt2x4;
typedef domList_of_ints		domInt3x2;
typedef domList_of_ints		domInt3x3;
typedef domList_of_ints		domInt3x4;
typedef domList_of_ints		domInt4x2;
typedef domList_of_ints		domInt4x3;
typedef domList_of_ints		domInt4x4;
typedef xsUnsignedByte		domDigits;
typedef xsShort		domMagnitude;
/**
 * This urifragment_type element is used for URI reference which can only
 * reference a resource declared within it's same document.
 */
typedef xsAnyURI		domUrifragment;
typedef domFloat4		domFx_color;
/**
 * ? GEOMETRY: [default] The geometry associated with this instance_geometry
 * or nstance_material. ? SCENE_GEOMETRY: Draw the entire scene's geometry
 * but with this effect, not the effects or materials already associated with
 * the geometry. This is for techniques such as shadow-buffer generation,
 * where you might be interested only in extracting the Z value from the light.
 * This is without regard to ordering on the assumption that ZBuffer handles
 * order. ? SCENE_IMAGE: Draw the entire scene naturally into the render targets.
 * Hense producing an image of the scene.  When used more then once, the later
 * uses must include what has been rendered so far  to the backbuffer since
 * the first render.  This is for effects that need an accurate image of the
 * scene  to work on for effects such as postprocessing blurs.  ? FULL_SCREEN_QUAD:
 * Positions are 0,0 to 1,1 and the UVs match.
 */
typedef xsString		domFx_draw;
typedef xsNonNegativeInteger		domGl_max_lights_index;
typedef xsNonNegativeInteger		domGl_max_clip_planes_index;
typedef xsNonNegativeInteger		domGl_max_texture_image_units_index;
typedef xsFloat		domGl_alpha_value;
typedef xsNonNegativeInteger		domGles_max_lights_index;
typedef xsNonNegativeInteger		domGles_max_clip_planes_index;
typedef xsNonNegativeInteger		domGles_max_texture_coords_index;
typedef xsNonNegativeInteger		domGles_max_texture_image_units_index;
typedef xsNonNegativeInteger		domGles_texcombiner_argument_index;
typedef domFloat2		domDynamic_limit;

/**
 * An enumuerated type specifying the acceptable morph methods.
 */
enum domMorph_method {
	MORPH_METHOD_NORMALIZED,
	MORPH_METHOD_RELATIVE,
	MORPH_METHOD_COUNT = 2
};

/**
 * An enumerated type specifying the acceptable node types.
 */
enum domNode_enum {
	NODE_ENUM_JOINT,
	NODE_ENUM_NODE,
	NODE_ENUM_COUNT = 2
};

/**
 * An enumerated type specifying the acceptable sampler pre and post behavior
 * attribute types.
 */
enum domSampler_behavior {
	SAMPLER_BEHAVIOR_CONSTANT,
	SAMPLER_BEHAVIOR_CYCLE,
	SAMPLER_BEHAVIOR_CYCLE_RELATIVE,
	SAMPLER_BEHAVIOR_GRADIENT,
	SAMPLER_BEHAVIOR_OSCILLATE,
	SAMPLER_BEHAVIOR_UNDEFINED,
	SAMPLER_BEHAVIOR_COUNT = 6
};

/**
 * An enumerated type specifying the acceptable up-axis values.
 */
enum domUp_axis {
	UP_AXIS_X_UP,
	UP_AXIS_Y_UP,
	UP_AXIS_Z_UP,
	UP_AXIS_COUNT = 3
};

/**
 * An enumerated type specifying the acceptable document versions.
 */
enum domVersion {
	VERSION_1_5_0,
	VERSION_COUNT = 1
};

enum domImage_face {
	IMAGE_FACE_POSITIVE_X,
	IMAGE_FACE_NEGATIVE_X,
	IMAGE_FACE_POSITIVE_Y,
	IMAGE_FACE_NEGATIVE_Y,
	IMAGE_FACE_POSITIVE_Z,
	IMAGE_FACE_NEGATIVE_Z,
	IMAGE_FACE_COUNT = 6
};

/**
 * The per-texel layout of the format.  The length of the string indicate
 * how many channels there are and the letter respresents the name of the
 * channel.  There are typically 0 to 4 channels.
 */
enum domImage_format_hint_channels {
	IMAGE_FORMAT_HINT_CHANNELS_RGB,		/**< RGB color  map */
	IMAGE_FORMAT_HINT_CHANNELS_RGBA,		/**< RGB color + Alpha map often used for color + transparency or other things packed into channel A like specular power */
	IMAGE_FORMAT_HINT_CHANNELS_RGBE,		/**< RGB color + shared exponent for HDR */
	IMAGE_FORMAT_HINT_CHANNELS_L,		/**< Luminance map often used for light mapping */
	IMAGE_FORMAT_HINT_CHANNELS_LA,		/**< Luminance+Alpha map often used for light mapping */
	IMAGE_FORMAT_HINT_CHANNELS_D,		/**< Depth map often used for displacement, parellax, relief, or shadow mapping.  Depth is depth seperate from Luminace to make special hardware considerations */
	IMAGE_FORMAT_HINT_CHANNELS_COUNT = 6
};

/**
 * Each channel of the texel has a precision.  Typically these are all linked
 * together.  An exact format lay lower the precision of an individual channel
 * but applying a higher precision by linking the channels together may still
 * convey the same information.
 */
enum domImage_format_hint_precision {
	IMAGE_FORMAT_HINT_PRECISION_DEFAULT,		/**< Designer does not care as long as it provides "reasonable" percision and performance */
	IMAGE_FORMAT_HINT_PRECISION_LOW,		/**< For integers this typically represents 8 bits.  For floats typically 16 bits. */
	IMAGE_FORMAT_HINT_PRECISION_MID,		/**< For integers this typically represents 8 to 24 bits.  For floats typically 16 to 32 bits. */
	IMAGE_FORMAT_HINT_PRECISION_HIGH,		/**< For integers this typically represents 16 to 32 bits.  For floats typically 24 to 32 bits. */
	IMAGE_FORMAT_HINT_PRECISION_MAX,		/**< Typically 32 bits or 64 bits if available.  64 bit has been seperated into it's own catagory beyond HIGH because it typically has significant performance impact and is beyond what non CAD software considers high percision. */
	IMAGE_FORMAT_HINT_PRECISION_COUNT = 5
};

/**
 * Each channel represents a range of values. Some example ranges are signed
 * or unsigned integers, or between between a clamped range such as 0.0f to
 * 1.0f, or high dynamic range via floating point
 */
enum domImage_format_hint_range {
	IMAGE_FORMAT_HINT_RANGE_SNORM,		/**< Format is representing a decimal value that remains within the -1 to 1 range. Implimentation could be integer-fixedpoint or floats. */
	IMAGE_FORMAT_HINT_RANGE_UNORM,		/**< Format is representing a decimal value that remains within the 0 to 1 range. Implimentation could be integer-fixedpoint or floats. */
	IMAGE_FORMAT_HINT_RANGE_SINT,		/**< Format is representing signed integer numbers.  (ex. 8bits = -128 to 127) */
	IMAGE_FORMAT_HINT_RANGE_UINT,		/**< Format is representing unsigned integer numbers.  (ex. 8bits = 0 to 255) */
	IMAGE_FORMAT_HINT_RANGE_FLOAT,		/**< Format should support full floating point ranges.  High precision is expected to be 32bit. Mid precision may be 16 to 32 bit.  Low precision is expected to be 16 bit. */
	IMAGE_FORMAT_HINT_RANGE_COUNT = 5
};

/**
 * The legal values for the mode attribute on the altitute element in a  geographic_location
 * element.
 */
enum domAltitude_mode {
	ALTITUDE_MODE_absolute,
	ALTITUDE_MODE_relativeToGround,
	ALTITUDE_MODE_COUNT = 2
};

enum domFx_opaque {
	FX_OPAQUE_A_ONE,		/**< When a transparent opaque attribute is set to A_ONE, it means the transparency information will be taken from the alpha channel of the color, texture, or parameter supplying the value. The value of 1.0 is opaque in this mode. */
	FX_OPAQUE_A_ZERO,		/**< When a transparent opaque attribute is set to A_ZERO, it means the transparency information will be taken from the alpha channel of the color, texture, or parameter supplying the value. The value of 0.0 is opaque in this mode. */
	FX_OPAQUE_RGB_ONE,		/**< When a transparent opaque attribute is set to RGB_ONE, it means the transparency information will be taken from the red, green, and blue channels of the color, texture, or parameter supplying the value. Each channel is modulated independently. The value of 1.0 is opaque in this mode. */
	FX_OPAQUE_RGB_ZERO,		/**< When a transparent opaque attribute is set to RGB_ZERO, it means the transparency information will be taken from the red, green, and blue channels of the color, texture, or parameter supplying the value. Each channel is modulated independently. The value of 0.0 is opaque in this mode. */
	FX_OPAQUE_COUNT = 4
};

enum domFx_sampler_wrap {
	FX_SAMPLER_WRAP_WRAP,		/**< Tile the texture at every integer junction. For example, for u values between 0 and 3, the texture is repeated three times; no mirroring is performed */
	FX_SAMPLER_WRAP_CLAMP,		/**< Same as CLAMP_TO_EDGE.  Texture coordinates reaching or exceeding the range [0.0, 1.0] are set just within 0.0 or 1.0 so that the boarder is not sampled */
	FX_SAMPLER_WRAP_BORDER,		/**< Much like clamp except texture coordinates outside the range [0.0, 1.0] are set to the border color */
	FX_SAMPLER_WRAP_MIRROR,		/**< texture is flipped at every integer junction. For u values between 0 and 1, for example, the texture is addressed normally; between 1 and 2, the texture is flipped (mirrored); between 2 and 3, the texture is normal again; and so on. */
	FX_SAMPLER_WRAP_MIRROR_ONCE,		/**< Takes the absolute value of the texture coordinate (thus, mirroring around 0), and then clamps to the maximum value */
	FX_SAMPLER_WRAP_COUNT = 5
};

enum domFx_sampler_min_filter {
	FX_SAMPLER_MIN_FILTER_NEAREST,
	FX_SAMPLER_MIN_FILTER_LINEAR,
	FX_SAMPLER_MIN_FILTER_ANISOTROPIC,
	FX_SAMPLER_MIN_FILTER_COUNT = 3
};

enum domFx_sampler_mag_filter {
	FX_SAMPLER_MAG_FILTER_NEAREST,
	FX_SAMPLER_MAG_FILTER_LINEAR,
	FX_SAMPLER_MAG_FILTER_COUNT = 2
};

enum domFx_sampler_mip_filter {
	FX_SAMPLER_MIP_FILTER_NONE,
	FX_SAMPLER_MIP_FILTER_NEAREST,
	FX_SAMPLER_MIP_FILTER_LINEAR,
	FX_SAMPLER_MIP_FILTER_COUNT = 3
};

enum domFx_modifier {
	FX_MODIFIER_CONST,
	FX_MODIFIER_UNIFORM,
	FX_MODIFIER_VARYING,
	FX_MODIFIER_STATIC,
	FX_MODIFIER_VOLATILE,
	FX_MODIFIER_EXTERN,
	FX_MODIFIER_SHARED,
	FX_MODIFIER_COUNT = 7
};

enum domFx_pipeline_stage {
	FX_PIPELINE_STAGE_TESSELLATION,
	FX_PIPELINE_STAGE_VERTEX,
	FX_PIPELINE_STAGE_GEOMETRY,
	FX_PIPELINE_STAGE_FRAGMENT,
	FX_PIPELINE_STAGE_COUNT = 4
};

enum domGl_blend {
	GL_BLEND_ZERO = 0x0,
	GL_BLEND_ONE = 0x1,
	GL_BLEND_SRC_COLOR = 0x0300,
	GL_BLEND_ONE_MINUS_SRC_COLOR = 0x0301,
	GL_BLEND_DEST_COLOR = 0x0306,
	GL_BLEND_ONE_MINUS_DEST_COLOR = 0x0307,
	GL_BLEND_SRC_ALPHA = 0x0302,
	GL_BLEND_ONE_MINUS_SRC_ALPHA = 0x0303,
	GL_BLEND_DST_ALPHA = 0x0304,
	GL_BLEND_ONE_MINUS_DST_ALPHA = 0x0305,
	GL_BLEND_CONSTANT_COLOR = 0x8001,
	GL_BLEND_ONE_MINUS_CONSTANT_COLOR = 0x8002,
	GL_BLEND_CONSTANT_ALPHA = 0x8003,
	GL_BLEND_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
	GL_BLEND_SRC_ALPHA_SATURATE = 0x0308,
	GL_BLEND_COUNT = 15
};

enum domGl_face {
	GL_FACE_FRONT = 0x0404,
	GL_FACE_BACK = 0x0405,
	GL_FACE_FRONT_AND_BACK = 0x0408,
	GL_FACE_COUNT = 3
};

enum domGl_blend_equation {
	GL_BLEND_EQUATION_FUNC_ADD = 0x8006,
	GL_BLEND_EQUATION_FUNC_SUBTRACT = 0x800A,
	GL_BLEND_EQUATION_FUNC_REVERSE_SUBTRACT = 0x800B,
	GL_BLEND_EQUATION_MIN = 0x8007,
	GL_BLEND_EQUATION_MAX = 0x8008,
	GL_BLEND_EQUATION_COUNT = 5
};

enum domGl_func {
	GL_FUNC_NEVER = 0x0200,
	GL_FUNC_LESS = 0x0201,
	GL_FUNC_LEQUAL = 0x0203,
	GL_FUNC_EQUAL = 0x0202,
	GL_FUNC_GREATER = 0x0204,
	GL_FUNC_NOTEQUAL = 0x0205,
	GL_FUNC_GEQUAL = 0x0206,
	GL_FUNC_ALWAYS = 0x0207,
	GL_FUNC_COUNT = 8
};

enum domGl_stencil_op {
	GL_STENCIL_OP_KEEP = 0x1E00,
	GL_STENCIL_OP_ZERO = 0x0,
	GL_STENCIL_OP_REPLACE = 0x1E01,
	GL_STENCIL_OP_INCR = 0x1E02,
	GL_STENCIL_OP_DECR = 0x1E03,
	GL_STENCIL_OP_INVERT = 0x150A,
	GL_STENCIL_OP_INCR_WRAP = 0x8507,
	GL_STENCIL_OP_DECR_WRAP = 0x8508,
	GL_STENCIL_OP_COUNT = 8
};

enum domGl_material {
	GL_MATERIAL_EMISSION = 0x1600,
	GL_MATERIAL_AMBIENT = 0x1200,
	GL_MATERIAL_DIFFUSE = 0x1201,
	GL_MATERIAL_SPECULAR = 0x1202,
	GL_MATERIAL_AMBIENT_AND_DIFFUSE = 0x1602,
	GL_MATERIAL_COUNT = 5
};

enum domGl_fog {
	GL_FOG_LINEAR = 0x2601,
	GL_FOG_EXP = 0x0800,
	GL_FOG_EXP2 = 0x0801,
	GL_FOG_COUNT = 3
};

enum domGl_fog_coord_src {
	GL_FOG_COORD_SRC_FOG_COORDINATE = 0x8451,
	GL_FOG_COORD_SRC_FRAGMENT_DEPTH = 0x8452,
	GL_FOG_COORD_SRC_COUNT = 2
};

enum domGl_front_face {
	GL_FRONT_FACE_CW = 0x0900,
	GL_FRONT_FACE_CCW = 0x0901,
	GL_FRONT_FACE_COUNT = 2
};

enum domGl_light_model_color_control {
	GL_LIGHT_MODEL_COLOR_CONTROL_SINGLE_COLOR = 0x81F9,
	GL_LIGHT_MODEL_COLOR_CONTROL_SEPARATE_SPECULAR_COLOR = 0x81FA,
	GL_LIGHT_MODEL_COLOR_CONTROL_COUNT = 2
};

enum domGl_logic_op {
	GL_LOGIC_OP_CLEAR = 0x1500,
	GL_LOGIC_OP_AND = 0x1501,
	GL_LOGIC_OP_AND_REVERSE = 0x1502,
	GL_LOGIC_OP_COPY = 0x1503,
	GL_LOGIC_OP_AND_INVERTED = 0x1504,
	GL_LOGIC_OP_NOOP = 0x1505,
	GL_LOGIC_OP_XOR = 0x1506,
	GL_LOGIC_OP_OR = 0x1507,
	GL_LOGIC_OP_NOR = 0x1508,
	GL_LOGIC_OP_EQUIV = 0x1509,
	GL_LOGIC_OP_INVERT = 0x150A,
	GL_LOGIC_OP_OR_REVERSE = 0x150B,
	GL_LOGIC_OP_COPY_INVERTED = 0x150C,
	GL_LOGIC_OP_NAND = 0x150E,
	GL_LOGIC_OP_SET = 0x150F,
	GL_LOGIC_OP_COUNT = 15
};

enum domGl_polygon_mode {
	GL_POLYGON_MODE_POINT = 0x1B00,
	GL_POLYGON_MODE_LINE = 0x1B01,
	GL_POLYGON_MODE_FILL = 0x1B02,
	GL_POLYGON_MODE_COUNT = 3
};

enum domGl_shade_model {
	GL_SHADE_MODEL_FLAT = 0x1D00,
	GL_SHADE_MODEL_SMOOTH = 0x1D01,
	GL_SHADE_MODEL_COUNT = 2
};

enum domGles_texenv_mode {
	GLES_TEXENV_MODE_REPLACE = 0x1E01,
	GLES_TEXENV_MODE_MODULATE = 0x2100,
	GLES_TEXENV_MODE_DECAL = 0x2101,
	GLES_TEXENV_MODE_BLEND = 0x0BE2,
	GLES_TEXENV_MODE_ADD = 0x0104,
	GLES_TEXENV_MODE_COUNT = 5
};

enum domGles_texcombiner_operator_rgb {
	GLES_TEXCOMBINER_OPERATOR_RGB_REPLACE = 0x1E01,
	GLES_TEXCOMBINER_OPERATOR_RGB_MODULATE = 0x2100,
	GLES_TEXCOMBINER_OPERATOR_RGB_ADD = 0x0104,
	GLES_TEXCOMBINER_OPERATOR_RGB_ADD_SIGNED = 0x8574,
	GLES_TEXCOMBINER_OPERATOR_RGB_INTERPOLATE = 0x8575,
	GLES_TEXCOMBINER_OPERATOR_RGB_SUBTRACT = 0x84E7,
	GLES_TEXCOMBINER_OPERATOR_RGB_DOT3_RGB = 0x86AE,
	GLES_TEXCOMBINER_OPERATOR_RGB_DOT3_RGBA = 0x86AF,
	GLES_TEXCOMBINER_OPERATOR_RGB_COUNT = 8
};

enum domGles_texcombiner_operator_alpha {
	GLES_TEXCOMBINER_OPERATOR_ALPHA_REPLACE = 0x1E01,
	GLES_TEXCOMBINER_OPERATOR_ALPHA_MODULATE = 0x2100,
	GLES_TEXCOMBINER_OPERATOR_ALPHA_ADD = 0x0104,
	GLES_TEXCOMBINER_OPERATOR_ALPHA_ADD_SIGNED = 0x8574,
	GLES_TEXCOMBINER_OPERATOR_ALPHA_INTERPOLATE = 0x8575,
	GLES_TEXCOMBINER_OPERATOR_ALPHA_SUBTRACT = 0x84E7,
	GLES_TEXCOMBINER_OPERATOR_ALPHA_COUNT = 6
};

enum domGles_texcombiner_source {
	GLES_TEXCOMBINER_SOURCE_TEXTURE = 0x1702,
	GLES_TEXCOMBINER_SOURCE_CONSTANT = 0x8576,
	GLES_TEXCOMBINER_SOURCE_PRIMARY = 0x8577,
	GLES_TEXCOMBINER_SOURCE_PREVIOUS = 0x8578,
	GLES_TEXCOMBINER_SOURCE_COUNT = 4
};

enum domGles_texcombiner_operand_rgb {
	GLES_TEXCOMBINER_OPERAND_RGB_SRC_COLOR = 0x0300,
	GLES_TEXCOMBINER_OPERAND_RGB_ONE_MINUS_SRC_COLOR = 0x0301,
	GLES_TEXCOMBINER_OPERAND_RGB_SRC_ALPHA = 0x0302,
	GLES_TEXCOMBINER_OPERAND_RGB_ONE_MINUS_SRC_ALPHA = 0x0303,
	GLES_TEXCOMBINER_OPERAND_RGB_COUNT = 4
};

enum domGles_texcombiner_operand_alpha {
	GLES_TEXCOMBINER_OPERAND_ALPHA_SRC_ALPHA = 0x0302,
	GLES_TEXCOMBINER_OPERAND_ALPHA_ONE_MINUS_SRC_ALPHA = 0x0303,
	GLES_TEXCOMBINER_OPERAND_ALPHA_COUNT = 2
};

enum domGles_sampler_wrap {
	GLES_SAMPLER_WRAP_REPEAT,
	GLES_SAMPLER_WRAP_CLAMP,
	GLES_SAMPLER_WRAP_CLAMP_TO_EDGE,
	GLES_SAMPLER_WRAP_MIRRORED_REPEAT,		/**< supported by GLES 1.1 only */
	GLES_SAMPLER_WRAP_COUNT = 4
};

enum domGles_stencil_op {
	GLES_STENCIL_OP_KEEP = 0x1E00,
	GLES_STENCIL_OP_ZERO = 0x0,
	GLES_STENCIL_OP_REPLACE = 0x1E01,
	GLES_STENCIL_OP_INCR = 0x1E02,
	GLES_STENCIL_OP_DECR = 0x1E03,
	GLES_STENCIL_OP_INVERT = 0x150A,
	GLES_STENCIL_OP_COUNT = 6
};

enum domSpring {
	SPRING_LINEAR,
	SPRING_ANGULAR,
	SPRING_COUNT = 2
};

enum domGl_enumeration {
	GL_ENUMERATION_ZERO = 0x0,
	GL_ENUMERATION_ONE = 0x1,
	GL_ENUMERATION_SRC_COLOR = 0x0300,
	GL_ENUMERATION_ONE_MINUS_SRC_COLOR = 0x0301,
	GL_ENUMERATION_DEST_COLOR = 0x0306,
	GL_ENUMERATION_ONE_MINUS_DEST_COLOR = 0x0307,
	GL_ENUMERATION_SRC_ALPHA = 0x0302,
	GL_ENUMERATION_ONE_MINUS_SRC_ALPHA = 0x0303,
	GL_ENUMERATION_DST_ALPHA = 0x0304,
	GL_ENUMERATION_ONE_MINUS_DST_ALPHA = 0x0305,
	GL_ENUMERATION_CONSTANT_COLOR = 0x8001,
	GL_ENUMERATION_ONE_MINUS_CONSTANT_COLOR = 0x8002,
	GL_ENUMERATION_CONSTANT_ALPHA = 0x8003,
	GL_ENUMERATION_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
	GL_ENUMERATION_SRC_ALPHA_SATURATE = 0x0308,
	GL_ENUMERATION_FRONT = 0x0404,
	GL_ENUMERATION_BACK = 0x0405,
	GL_ENUMERATION_FRONT_AND_BACK = 0x0408,
	GL_ENUMERATION_FUNC_ADD = 0x8006,
	GL_ENUMERATION_FUNC_SUBTRACT = 0x800A,
	GL_ENUMERATION_FUNC_REVERSE_SUBTRACT = 0x800B,
	GL_ENUMERATION_MIN = 0x8007,
	GL_ENUMERATION_MAX = 0x8008,
	GL_ENUMERATION_NEVER = 0x0200,
	GL_ENUMERATION_LESS = 0x0201,
	GL_ENUMERATION_LEQUAL = 0x0203,
	GL_ENUMERATION_EQUAL = 0x0202,
	GL_ENUMERATION_GREATER = 0x0204,
	GL_ENUMERATION_NOTEQUAL = 0x0205,
	GL_ENUMERATION_GEQUAL = 0x0206,
	GL_ENUMERATION_ALWAYS = 0x0207,
	GL_ENUMERATION_KEEP = 0x1E00,
	GL_ENUMERATION_REPLACE = 0x1E01,
	GL_ENUMERATION_INCR = 0x1E02,
	GL_ENUMERATION_DECR = 0x1E03,
	GL_ENUMERATION_INVERT = 0x150A,
	GL_ENUMERATION_INCR_WRAP = 0x8507,
	GL_ENUMERATION_DECR_WRAP = 0x8508,
	GL_ENUMERATION_EMISSION = 0x1600,
	GL_ENUMERATION_AMBIENT = 0x1200,
	GL_ENUMERATION_DIFFUSE = 0x1201,
	GL_ENUMERATION_SPECULAR = 0x1202,
	GL_ENUMERATION_AMBIENT_AND_DIFFUSE = 0x1602,
	GL_ENUMERATION_LINEAR = 0x2601,
	GL_ENUMERATION_EXP = 0x0800,
	GL_ENUMERATION_EXP2 = 0x0801,
	GL_ENUMERATION_FOG_COORDINATE = 0x8451,
	GL_ENUMERATION_FRAGMENT_DEPTH = 0x8452,
	GL_ENUMERATION_CW = 0x0900,
	GL_ENUMERATION_CCW = 0x0901,
	GL_ENUMERATION_SINGLE_COLOR = 0x81F9,
	GL_ENUMERATION_SEPARATE_SPECULAR_COLOR = 0x81FA,
	GL_ENUMERATION_CLEAR = 0x1500,
	GL_ENUMERATION_AND = 0x1501,
	GL_ENUMERATION_AND_REVERSE = 0x1502,
	GL_ENUMERATION_COPY = 0x1503,
	GL_ENUMERATION_AND_INVERTED = 0x1504,
	GL_ENUMERATION_NOOP = 0x1505,
	GL_ENUMERATION_XOR = 0x1506,
	GL_ENUMERATION_OR = 0x1507,
	GL_ENUMERATION_NOR = 0x1508,
	GL_ENUMERATION_EQUIV = 0x1509,
	GL_ENUMERATION_OR_REVERSE = 0x150B,
	GL_ENUMERATION_COPY_INVERTED = 0x150C,
	GL_ENUMERATION_NAND = 0x150E,
	GL_ENUMERATION_SET = 0x150F,
	GL_ENUMERATION_POINT = 0x1B00,
	GL_ENUMERATION_LINE = 0x1B01,
	GL_ENUMERATION_FILL = 0x1B02,
	GL_ENUMERATION_FLAT = 0x1D00,
	GL_ENUMERATION_SMOOTH = 0x1D01,
	GL_ENUMERATION_COUNT = 72
};

enum domGles_enumeration {
	GLES_ENUMERATION_ZERO = 0x0,
	GLES_ENUMERATION_ONE = 0x1,
	GLES_ENUMERATION_SRC_COLOR = 0x0300,
	GLES_ENUMERATION_ONE_MINUS_SRC_COLOR = 0x0301,
	GLES_ENUMERATION_DEST_COLOR = 0x0306,
	GLES_ENUMERATION_ONE_MINUS_DEST_COLOR = 0x0307,
	GLES_ENUMERATION_SRC_ALPHA = 0x0302,
	GLES_ENUMERATION_ONE_MINUS_SRC_ALPHA = 0x0303,
	GLES_ENUMERATION_DST_ALPHA = 0x0304,
	GLES_ENUMERATION_ONE_MINUS_DST_ALPHA = 0x0305,
	GLES_ENUMERATION_CONSTANT_COLOR = 0x8001,
	GLES_ENUMERATION_ONE_MINUS_CONSTANT_COLOR = 0x8002,
	GLES_ENUMERATION_CONSTANT_ALPHA = 0x8003,
	GLES_ENUMERATION_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
	GLES_ENUMERATION_SRC_ALPHA_SATURATE = 0x0308,
	GLES_ENUMERATION_FRONT = 0x0404,
	GLES_ENUMERATION_BACK = 0x0405,
	GLES_ENUMERATION_FRONT_AND_BACK = 0x0408,
	GLES_ENUMERATION_NEVER = 0x0200,
	GLES_ENUMERATION_LESS = 0x0201,
	GLES_ENUMERATION_LEQUAL = 0x0203,
	GLES_ENUMERATION_EQUAL = 0x0202,
	GLES_ENUMERATION_GREATER = 0x0204,
	GLES_ENUMERATION_NOTEQUAL = 0x0205,
	GLES_ENUMERATION_GEQUAL = 0x0206,
	GLES_ENUMERATION_ALWAYS = 0x0207,
	GLES_ENUMERATION_KEEP = 0x1E00,
	GLES_ENUMERATION_REPLACE = 0x1E01,
	GLES_ENUMERATION_INCR = 0x1E02,
	GLES_ENUMERATION_DECR = 0x1E03,
	GLES_ENUMERATION_INVERT = 0x150A,
	GLES_ENUMERATION_INCR_WRAP = 0x8507,
	GLES_ENUMERATION_DECR_WRAP = 0x8508,
	GLES_ENUMERATION_EMISSION = 0x1600,
	GLES_ENUMERATION_AMBIENT = 0x1200,
	GLES_ENUMERATION_DIFFUSE = 0x1201,
	GLES_ENUMERATION_SPECULAR = 0x1202,
	GLES_ENUMERATION_AMBIENT_AND_DIFFUSE = 0x1602,
	GLES_ENUMERATION_LINEAR = 0x2601,
	GLES_ENUMERATION_EXP = 0x0800,
	GLES_ENUMERATION_EXP2 = 0x0801,
	GLES_ENUMERATION_CW = 0x0900,
	GLES_ENUMERATION_CCW = 0x0901,
	GLES_ENUMERATION_SINGLE_COLOR = 0x81F9,
	GLES_ENUMERATION_SEPARATE_SPECULAR_COLOR = 0x81FA,
	GLES_ENUMERATION_CLEAR = 0x1500,
	GLES_ENUMERATION_AND = 0x1501,
	GLES_ENUMERATION_AND_REVERSE = 0x1502,
	GLES_ENUMERATION_COPY = 0x1503,
	GLES_ENUMERATION_AND_INVERTED = 0x1504,
	GLES_ENUMERATION_NOOP = 0x1505,
	GLES_ENUMERATION_XOR = 0x1506,
	GLES_ENUMERATION_OR = 0x1507,
	GLES_ENUMERATION_NOR = 0x1508,
	GLES_ENUMERATION_EQUIV = 0x1509,
	GLES_ENUMERATION_OR_REVERSE = 0x150B,
	GLES_ENUMERATION_COPY_INVERTED = 0x150C,
	GLES_ENUMERATION_NAND = 0x150E,
	GLES_ENUMERATION_SET = 0x150F,
	GLES_ENUMERATION_POINT = 0x1B00,
	GLES_ENUMERATION_LINE = 0x1B01,
	GLES_ENUMERATION_FILL = 0x1B02,
	GLES_ENUMERATION_FLAT = 0x1D00,
	GLES_ENUMERATION_SMOOTH = 0x1D01,
	GLES_ENUMERATION_COUNT = 65
};

//Element Type Enum
namespace COLLADA_TYPE
{
	const int
		NO_TYPE = 0,
		ANY = 1,
		INPUT_GLOBAL = 2,
		INPUT_LOCAL = 3,
		INPUT_LOCAL_OFFSET = 4,
		TARGETABLE_FLOAT = 5,
		TARGETABLE_FLOAT3 = 6,
		TOKEN_ARRAY = 7,
		IDREF_ARRAY = 8,
		NAME_ARRAY = 9,
		BOOL_ARRAY = 10,
		FLOAT_ARRAY = 11,
		INT_ARRAY = 12,
		PARAM = 13,
		ACCESSOR = 14,
		P = 15,
		LOOKAT = 16,
		MATRIX = 17,
		SCALE = 18,
		SKEW = 19,
		TRANSLATE = 20,
		IMAGE_SOURCE = 21,
		REF = 22,
		HEX = 23,
		IMAGE_MIPS = 24,
		CHANNEL = 25,
		SAMPLER = 26,
		TECHNIQUE = 27,
		EXTRA = 28,
		POLYLIST = 29,
		VCOUNT = 30,
		POLYGONS = 31,
		PH = 32,
		H = 33,
		LINESTRIPS = 34,
		TRISTRIPS = 35,
		LINES = 36,
		TRIANGLES = 37,
		TRIFANS = 38,
		VERTICES = 39,
		ASSET = 40,
		CONTRIBUTOR = 41,
		AUTHOR = 42,
		AUTHOR_EMAIL = 43,
		AUTHOR_WEBSITE = 44,
		AUTHORING_TOOL = 45,
		COMMENTS = 46,
		COPYRIGHT = 47,
		SOURCE_DATA = 48,
		COVERAGE = 49,
		GEOGRAPHIC_LOCATION = 50,
		LONGITUDE = 51,
		LATITUDE = 52,
		ALTITUDE = 53,
		CREATED = 54,
		KEYWORDS = 55,
		MODIFIED = 56,
		REVISION = 57,
		SUBJECT = 58,
		TITLE = 59,
		UNIT = 60,
		UP_AXIS = 61,
		IMAGE = 62,
		RENDERABLE = 63,
		INIT_FROM = 64,
		CREATE_2D = 65,
		SIZE_EXACT = 66,
		SIZE_RATIO = 67,
		UNNORMALIZED = 68,
		ARRAY = 69,
		FORMAT = 70,
		HINT = 71,
		EXACT = 72,
		CREATE_3D = 73,
		SIZE = 74,
		CREATE_CUBE = 75,
		CAMERA = 76,
		OPTICS = 77,
		TECHNIQUE_COMMON = 78,
		ORTHOGRAPHIC = 79,
		PERSPECTIVE = 80,
		IMAGER = 81,
		LIGHT = 82,
		AMBIENT = 83,
		DIRECTIONAL = 84,
		POINT = 85,
		SPOT = 86,
		INSTANCE_WITH_EXTRA = 87,
		INSTANCE_IMAGE = 88,
		INSTANCE_CAMERA = 89,
		INSTANCE_FORCE_FIELD = 90,
		INSTANCE_LIGHT = 91,
		INSTANCE_MATERIAL = 92,
		BIND = 93,
		BIND_VERTEX_INPUT = 94,
		BIND_MATERIAL = 95,
		INSTANCE_CONTROLLER = 96,
		SKELETON = 97,
		INSTANCE_GEOMETRY = 98,
		INSTANCE_NODE = 99,
		INSTANCE_PHYSICS_MATERIAL = 100,
		INSTANCE_RIGID_CONSTRAINT = 101,
		LIBRARY_CAMERAS = 102,
		LIBRARY_IMAGES = 103,
		LIBRARY_LIGHTS = 104,
		INSTANCE_EFFECT = 105,
		TECHNIQUE_HINT = 106,
		SETPARAM = 107,
		MATERIAL = 108,
		LIBRARY_MATERIALS = 109,
		FX_SAMPLER = 110,
		FX_SAMPLER1D = 111,
		FX_SAMPLER2D = 112,
		FX_SAMPLER3D = 113,
		FX_SAMPLERCUBE = 114,
		FX_SAMPLERRECT = 115,
		FX_SAMPLERDEPTH = 116,
		FX_RENDERTARGET = 117,
		FX_COLORTARGET = 118,
		FX_DEPTHTARGET = 119,
		FX_STENCILTARGET = 120,
		FX_CLEARCOLOR = 121,
		FX_CLEARDEPTH = 122,
		FX_CLEARSTENCIL = 123,
		FX_ANNOTATE = 124,
		BOOL = 125,
		BOOL2 = 126,
		BOOL3 = 127,
		BOOL4 = 128,
		INT = 129,
		INT2 = 130,
		INT3 = 131,
		INT4 = 132,
		FLOAT = 133,
		FLOAT2 = 134,
		FLOAT3 = 135,
		FLOAT4 = 136,
		FLOAT2X2 = 137,
		FLOAT3X3 = 138,
		FLOAT4X4 = 139,
		STRING = 140,
		FX_NEWPARAM = 141,
		FLOAT2X1 = 142,
		FLOAT2X3 = 143,
		FLOAT2X4 = 144,
		FLOAT3X1 = 145,
		FLOAT3X2 = 146,
		FLOAT3X4 = 147,
		FLOAT4X1 = 148,
		FLOAT4X2 = 149,
		FLOAT4X3 = 150,
		ENUM = 151,
		FX_INCLUDE = 152,
		FX_CODE = 153,
		FX_SOURCES = 154,
		INLINE = 155,
		IMPORT = 156,
		FX_TARGET = 157,
		BINARY = 158,
		FX_COMMON_FLOAT_OR_PARAM = 159,
		FX_COMMON_COLOR_OR_TEXTURE = 160,
		COLOR = 161,
		TEXTURE = 162,
		FX_COMMON_TRANSPARENT = 163,
		FX_COMMON_NEWPARAM = 164,
		SEMANTIC = 165,
		PROFILE_COMMON = 166,
		CONSTANT = 167,
		LAMBERT = 168,
		PHONG = 169,
		BLINN = 170,
		PROFILE_BRIDGE = 171,
		GLES2_NEWPARAM = 172,
		MODIFIER = 173,
		GLES2_SHADER = 174,
		SOURCES = 175,
		GLES2_PROGRAM = 176,
		BIND_ATTRIBUTE = 177,
		BIND_UNIFORM = 178,
		GLES2_PASS = 179,
		STATES = 180,
		EVALUATE = 181,
		DRAW = 182,
		PROFILE_GLES2 = 183,
		NEWPARAM = 184,
		GLSL_ARRAY = 185,
		GLSL_NEWPARAM = 186,
		GLSL_SHADER = 187,
		GLSL_PROGRAM = 188,
		PROFILE_GLSL = 189,
		PASS = 190,
		CG_ARRAY = 191,
		CG_SETPARAM = 192,
		CG_USER = 193,
		CG_NEWPARAM = 194,
		CG_PASS = 195,
		PROGRAM = 196,
		SHADER = 197,
		PROFILE_CG = 198,
		GLES_TEXTURE_CONSTANT = 199,
		GLES_TEXENV_COMMAND = 200,
		GLES_TEXCOMBINER_ARGUMENT_RGB = 201,
		GLES_TEXCOMBINER_ARGUMENT_ALPHA = 202,
		GLES_TEXCOMBINER_COMMAND_RGB = 203,
		GLES_TEXCOMBINER_COMMAND_ALPHA = 204,
		GLES_TEXCOMBINER_COMMAND = 205,
		GLES_TEXTURE_PIPELINE = 206,
		GLES_SAMPLER = 207,
		TEXCOORD = 208,
		GLES_NEWPARAM = 209,
		PROFILE_GLES = 210,
		EFFECT = 211,
		LIBRARY_EFFECTS = 212,
		BOX = 213,
		HALF_EXTENTS = 214,
		PLANE = 215,
		EQUATION = 216,
		SPHERE = 217,
		RADIUS = 218,
		ELLIPSOID = 219,
		CYLINDER = 220,
		HEIGHT = 221,
		CAPSULE = 222,
		FORCE_FIELD = 223,
		LIBRARY_FORCE_FIELDS = 224,
		PHYSICS_MATERIAL = 225,
		LIBRARY_PHYSICS_MATERIALS = 226,
		EDGES = 227,
		WIRES = 228,
		FACES = 229,
		SHELLS = 230,
		SOLIDS = 231,
		LINE = 232,
		ORIGIN = 233,
		DIRECTION = 234,
		CIRCLE = 235,
		ELLIPSE = 236,
		PARABOLA = 237,
		FOCAL = 238,
		HYPERBOLA = 239,
		TORUS = 240,
		PCURVES = 241,
		SIDREF_ARRAY = 242,
		SOURCE = 243,
		ANIMATION = 244,
		LIBRARY_ANIMATIONS = 245,
		SKIN = 246,
		BIND_SHAPE_MATRIX = 247,
		JOINTS = 248,
		VERTEX_WEIGHTS = 249,
		V = 250,
		CONVEX_MESH = 251,
		MESH = 252,
		SPLINE = 253,
		CONTROL_VERTICES = 254,
		NURBS_SURFACE = 255,
		NURBS = 256,
		MORPH = 257,
		TARGETS = 258,
		CONTROLLER = 259,
		LIBRARY_CONTROLLERS = 260,
		ORIENT = 261,
		CURVE = 262,
		SURFACE_CURVES = 263,
		CURVES = 264,
		SWEPT_SURFACE = 265,
		AXIS = 266,
		CONE = 267,
		ANGLE = 268,
		SURFACE = 269,
		SURFACES = 270,
		BREP = 271,
		GEOMETRY = 272,
		LIBRARY_GEOMETRIES = 273,
		LIMITS_SUB = 274,
		INSTANCE_JOINT = 275,
		TARGETABLE_FLOAT4 = 276,
		ROTATE = 277,
		LINK = 278,
		ATTACHMENT_FULL = 279,
		ATTACHMENT_START = 280,
		ATTACHMENT_END = 281,
		RIGID_CONSTRAINT = 282,
		REF_ATTACHMENT = 283,
		ATTACHMENT = 284,
		ENABLED = 285,
		INTERPENETRATE = 286,
		LIMITS = 287,
		SWING_CONE_AND_TWIST = 288,
		LINEAR = 289,
		SPRING = 290,
		ANGULAR = 291,
		NODE = 292,
		LIBRARY_NODES = 293,
		VISUAL_SCENE = 294,
		EVALUATE_SCENE = 295,
		RENDER = 296,
		LAYER = 297,
		TECHNIQUE_OVERRIDE = 298,
		LIBRARY_VISUAL_SCENES = 299,
		RIGID_BODY = 300,
		DYNAMIC = 301,
		MASS_FRAME = 302,
		SHAPE = 303,
		HOLLOW = 304,
		INSTANCE_RIGID_BODY = 305,
		ANGULAR_VELOCITY = 306,
		VELOCITY = 307,
		INSTANCE_PHYSICS_MODEL = 308,
		PHYSICS_MODEL = 309,
		LIBRARY_PHYSICS_MODELS = 310,
		PHYSICS_SCENE = 311,
		LIBRARY_PHYSICS_SCENES = 312,
		KINEMATICS_NEWPARAM = 313,
		SIDREF = 314,
		KINEMATICS_FRAME = 315,
		COMMON_PARAM = 316,
		COMMON_INT_OR_PARAM = 317,
		COMMON_BOOL_OR_PARAM = 318,
		COMMON_FLOAT_OR_PARAM = 319,
		KINEMATICS_LIMITS = 320,
		COMMON_SIDREF_OR_PARAM = 321,
		BIND_JOINT_AXIS = 322,
		BIND_KINEMATICS_MODEL = 323,
		KINEMATICS_CONNECT_PARAM = 324,
		KINEMATICS_SETPARAM = 325,
		INSTANCE_KINEMATICS_SCENE = 326,
		KINEMATICS_INDEX = 327,
		KINEMATICS_PARAM = 328,
		KINEMATICS_BIND = 329,
		MOTION_AXIS_INFO = 330,
		INSTANCE_KINEMATICS_MODEL = 331,
		INSTANCE_ARTICULATED_SYSTEM = 332,
		KINEMATICS_SCENE = 333,
		LIBRARY_KINEMATICS_SCENES = 334,
		COMMON_FLOAT2_OR_PARAM = 335,
		MOTION_EFFECTOR_INFO = 336,
		MOTION_TECHNIQUE = 337,
		MOTION = 338,
		MINMAX = 339,
		JOINT_LIMITS = 340,
		AXIS_CONSTRAINT = 341,
		JOINT = 342,
		LIBRARY_JOINTS = 343,
		FORMULA_TECHNIQUE = 344,
		FORMULA_NEWPARAM = 345,
		FORMULA_SETPARAM = 346,
		INSTANCE_FORMULA = 347,
		ANIMATION_CLIP = 348,
		LIBRARY_ANIMATION_CLIPS = 349,
		FORMULA = 350,
		KINEMATICS_MODEL_TECHNIQUE = 351,
		KINEMATICS_MODEL = 352,
		LIBRARY_KINEMATICS_MODELS = 353,
		KINEMATICS_AXIS_INFO = 354,
		KINEMATICS_TECHNIQUE = 355,
		KINEMATICS = 356,
		ARTICULATED_SYSTEM = 357,
		LIBRARY_ARTICULATED_SYSTEMS = 358,
		LIBRARY_FORMULAS = 359,
		FX_SAMPLER_STATES = 360,
		WRAP_S = 361,
		WRAP_T = 362,
		WRAP_P = 363,
		MINFILTER = 364,
		MAGFILTER = 365,
		MIPFILTER = 366,
		BORDER_COLOR = 367,
		MIP_MAX_LEVEL = 368,
		MIP_MIN_LEVEL = 369,
		MIP_BIAS = 370,
		MAX_ANISOTROPY = 371,
		FX_SETPARAM = 372,
		SAMPLER_STATES = 373,
		GL_PIPELINE_SETTINGS = 374,
		ALPHA_FUNC = 375,
		FUNC = 376,
		VALUE = 377,
		BLEND_FUNC = 378,
		SRC = 379,
		DEST = 380,
		BLEND_FUNC_SEPARATE = 381,
		SRC_RGB = 382,
		DEST_RGB = 383,
		SRC_ALPHA = 384,
		DEST_ALPHA = 385,
		BLEND_EQUATION = 386,
		BLEND_EQUATION_SEPARATE = 387,
		RGB = 388,
		ALPHA = 389,
		COLOR_MATERIAL = 390,
		FACE = 391,
		MODE = 392,
		CULL_FACE = 393,
		DEPTH_FUNC = 394,
		FOG_MODE = 395,
		FOG_COORD_SRC = 396,
		FRONT_FACE = 397,
		LIGHT_MODEL_COLOR_CONTROL = 398,
		LOGIC_OP = 399,
		POLYGON_MODE = 400,
		SHADE_MODEL = 401,
		STENCIL_FUNC = 402,
		MASK = 403,
		STENCIL_OP = 404,
		FAIL = 405,
		ZFAIL = 406,
		ZPASS = 407,
		STENCIL_FUNC_SEPARATE = 408,
		FRONT = 409,
		BACK = 410,
		STENCIL_OP_SEPARATE = 411,
		STENCIL_MASK_SEPARATE = 412,
		LIGHT_ENABLE = 413,
		LIGHT_AMBIENT = 414,
		LIGHT_DIFFUSE = 415,
		LIGHT_SPECULAR = 416,
		LIGHT_POSITION = 417,
		LIGHT_CONSTANT_ATTENUATION = 418,
		LIGHT_LINEAR_ATTENUATION = 419,
		LIGHT_QUADRATIC_ATTENUATION = 420,
		LIGHT_SPOT_CUTOFF = 421,
		LIGHT_SPOT_DIRECTION = 422,
		LIGHT_SPOT_EXPONENT = 423,
		TEXTURE1D = 424,
		TEXTURE2D = 425,
		TEXTURE3D = 426,
		TEXTURECUBE = 427,
		TEXTURERECT = 428,
		TEXTUREDEPTH = 429,
		TEXTURE1D_ENABLE = 430,
		TEXTURE2D_ENABLE = 431,
		TEXTURE3D_ENABLE = 432,
		TEXTURECUBE_ENABLE = 433,
		TEXTURERECT_ENABLE = 434,
		TEXTUREDEPTH_ENABLE = 435,
		TEXTURE_ENV_COLOR = 436,
		TEXTURE_ENV_MODE = 437,
		CLIP_PLANE = 438,
		CLIP_PLANE_ENABLE = 439,
		BLEND_COLOR = 440,
		COLOR_MASK = 441,
		DEPTH_BOUNDS = 442,
		DEPTH_MASK = 443,
		DEPTH_RANGE = 444,
		FOG_DENSITY = 445,
		FOG_START = 446,
		FOG_END = 447,
		FOG_COLOR = 448,
		LIGHT_MODEL_AMBIENT = 449,
		LIGHTING_ENABLE = 450,
		LINE_STIPPLE = 451,
		LINE_WIDTH = 452,
		MATERIAL_AMBIENT = 453,
		MATERIAL_DIFFUSE = 454,
		MATERIAL_EMISSION = 455,
		MATERIAL_SHININESS = 456,
		MATERIAL_SPECULAR = 457,
		MODEL_VIEW_MATRIX = 458,
		POINT_DISTANCE_ATTENUATION = 459,
		POINT_FADE_THRESHOLD_SIZE = 460,
		POINT_SIZE = 461,
		POINT_SIZE_MIN = 462,
		POINT_SIZE_MAX = 463,
		POLYGON_OFFSET = 464,
		PROJECTION_MATRIX = 465,
		SCISSOR = 466,
		STENCIL_MASK = 467,
		ALPHA_TEST_ENABLE = 468,
		BLEND_ENABLE = 469,
		COLOR_LOGIC_OP_ENABLE = 470,
		COLOR_MATERIAL_ENABLE = 471,
		CULL_FACE_ENABLE = 472,
		DEPTH_BOUNDS_ENABLE = 473,
		DEPTH_CLAMP_ENABLE = 474,
		DEPTH_TEST_ENABLE = 475,
		DITHER_ENABLE = 476,
		FOG_ENABLE = 477,
		LIGHT_MODEL_LOCAL_VIEWER_ENABLE = 478,
		LIGHT_MODEL_TWO_SIDE_ENABLE = 479,
		LINE_SMOOTH_ENABLE = 480,
		LINE_STIPPLE_ENABLE = 481,
		LOGIC_OP_ENABLE = 482,
		MULTISAMPLE_ENABLE = 483,
		NORMALIZE_ENABLE = 484,
		POINT_SMOOTH_ENABLE = 485,
		POLYGON_OFFSET_FILL_ENABLE = 486,
		POLYGON_OFFSET_LINE_ENABLE = 487,
		POLYGON_OFFSET_POINT_ENABLE = 488,
		POLYGON_SMOOTH_ENABLE = 489,
		POLYGON_STIPPLE_ENABLE = 490,
		RESCALE_NORMAL_ENABLE = 491,
		SAMPLE_ALPHA_TO_COVERAGE_ENABLE = 492,
		SAMPLE_ALPHA_TO_ONE_ENABLE = 493,
		SAMPLE_COVERAGE_ENABLE = 494,
		SCISSOR_TEST_ENABLE = 495,
		STENCIL_TEST_ENABLE = 496,
		GLES2_PIPELINE_SETTINGS = 497,
		SAMPLE_COVERAGE = 498,
		INVERT = 499,
		POINT_SIZE_ENABLE = 500,
		GLES2_VALUE = 501,
		BVEC2 = 502,
		BVEC3 = 503,
		BVEC4 = 504,
		VEC2 = 505,
		VEC3 = 506,
		VEC4 = 507,
		MAT2 = 508,
		MAT3 = 509,
		MAT4 = 510,
		IVEC2 = 511,
		IVEC3 = 512,
		IVEC4 = 513,
		USERTYPE = 514,
		GLSL_VALUE = 515,
		CG_PARAM = 516,
		BOOL2X1 = 517,
		BOOL2X2 = 518,
		BOOL2X3 = 519,
		BOOL2X4 = 520,
		BOOL3X1 = 521,
		BOOL3X2 = 522,
		BOOL3X3 = 523,
		BOOL3X4 = 524,
		BOOL4X1 = 525,
		BOOL4X2 = 526,
		BOOL4X3 = 527,
		BOOL4X4 = 528,
		INT2X1 = 529,
		INT2X2 = 530,
		INT2X3 = 531,
		INT2X4 = 532,
		INT3X1 = 533,
		INT3X2 = 534,
		INT3X3 = 535,
		INT3X4 = 536,
		INT4X1 = 537,
		INT4X2 = 538,
		INT4X3 = 539,
		INT4X4 = 540,
		HALF = 541,
		HALF2 = 542,
		HALF3 = 543,
		HALF4 = 544,
		HALF2X1 = 545,
		HALF2X2 = 546,
		HALF2X3 = 547,
		HALF2X4 = 548,
		HALF3X1 = 549,
		HALF3X2 = 550,
		HALF3X3 = 551,
		HALF3X4 = 552,
		HALF4X1 = 553,
		HALF4X2 = 554,
		HALF4X3 = 555,
		HALF4X4 = 556,
		FIXED = 557,
		FIXED2 = 558,
		FIXED3 = 559,
		FIXED4 = 560,
		FIXED2X1 = 561,
		FIXED2X2 = 562,
		FIXED2X3 = 563,
		FIXED2X4 = 564,
		FIXED3X1 = 565,
		FIXED3X2 = 566,
		FIXED3X3 = 567,
		FIXED3X4 = 568,
		FIXED4X1 = 569,
		FIXED4X2 = 570,
		FIXED4X3 = 571,
		FIXED4X4 = 572,
		GLES_SAMPLER_STATES = 573,
		GLES_PIPELINE_SETTINGS = 574,
		TEXTURE_PIPELINE = 575,
		GLES_PARAM = 576,
		FLOAT1X1 = 577,
		FLOAT1X2 = 578,
		FLOAT1X3 = 579,
		FLOAT1X4 = 580,
		FX_PROFILE = 581,
		COLLADA = 582,
		SCENE = 583;
}

// Returns the total number of schema types/dom* classes
daeInt DLLSPEC colladaTypeCount();

#endif
