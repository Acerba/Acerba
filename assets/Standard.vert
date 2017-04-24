#version 100													
attribute vec4 a_position;										
attribute vec2 a_uv;											
attribute vec4 a_color;										
																
varying vec4 o_c;												
varying vec2 o_uv;												
																
uniform vec2 Position;											
uniform vec2 Scale;											
uniform mat2 Rotation; // 2D									
uniform mat4 Model;											
																
void main()													
{																
	o_c = a_color;												
	o_uv = a_uv;												
																
	vec4 pos = vec4(a_position.xyz, 1);							
	pos.xy = Rotation * pos.xy;									
	pos.x *= Scale.x;											
   pos.y *= Scale.y;											
	pos.xy += Position.xy;										
																
																
	gl_Position = Model * pos;									
}																