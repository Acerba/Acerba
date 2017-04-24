#version 100													
precision lowp float;											
varying lowp vec4 o_c;											
varying lowp vec2 o_uv;										
																
uniform sampler2D Diffuse;										
uniform lowp vec4 Color;										
																
																
void main()													
{																
	gl_FragColor = texture2D(Diffuse, o_uv);					
}																