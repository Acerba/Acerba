#version 130													
in lowp vec4 o_c;												
in lowp vec2 o_uv;												
																
uniform sampler2D Texture;										
out vec4 color;													
void main()														
{																
	//color = vec4(o_uv.x, o_uv.y, 0, 1.0);						
	color = texture(Texture, o_uv);								
	if(color.b > 0.975)											
		color.a = 0.0;											
}																