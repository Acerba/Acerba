#version 150															
in vec4 a_position;														
in vec2 a_uv;															
in vec4 a_color;														
																		
out vec4 o_c;															
out vec2 o_uv;															
																		
uniform mat4 P;															
uniform mat4 V;															
uniform mat4 M[10];														
																		
void main()																
{																		
	o_c = a_color;														
	o_uv = a_uv;														
																		
	vec4 pos = P* V * M[int(a_position.w)] * vec4(a_position.xy, 0, 1);	
	gl_Position = pos;													
}																		