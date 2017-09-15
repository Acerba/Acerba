attribute vec4 a_position;														
attribute vec2 a_uv;															
attribute vec4 a_color;														
																		
varying vec4 o_c;															
varying vec2 o_uv;															
																		
uniform vec2 position;															
uniform vec2 scale;									
																		
void main()																
{																		
	o_c = a_color;														
	o_uv = a_uv;														
																		
	vec4 pos = vec4(a_position.xy, 0, 1);
	pos.x *= scale.x;
	pos.y *= scale.y; 
	pos.xy += position.xy;
	gl_Position = pos;													
}																		