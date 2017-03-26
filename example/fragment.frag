varying lowp vec4 o_c;												
varying lowp vec2 o_uv;												
																
uniform sampler2D Texture;
uniform int UseColor;
									
//out vec4 color;													
void main()														
{																
	//color = vec4(o_uv.x, o_uv.y, 0, 1.0);						
	gl_FragColor = texture2D(Texture, o_uv);		

	//if(color.r < 0.975)											
	//	color.a = 0.0;
}																