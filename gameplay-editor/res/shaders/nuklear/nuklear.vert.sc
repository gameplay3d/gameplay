$input a_position, a_texcoord0, a_color0
$output v_texcoord0, v_color0

#include "../common/bgfx_shader.sh"
#include "../common/shaderlib.sh"

void main()
{
    v_texcoord0 = a_texcoord0;
    v_color0 = a_color0;    
	gl_Position = mul(u_proj, vec4(a_position, 0.0, 1.0));

}
