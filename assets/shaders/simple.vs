//global info about the light
uniform vec3 camera_pos;

uniform mat4 model;
uniform mat4 modelt;
uniform mat4 mvp;

//this will store the color for the pixel shader
varying vec3 pixel_pos;
varying vec4 pixel_color;
varying vec3 pixel_normal;

void main()
{	
	//calcule the normal in camera space (the NormalMatrix is like ViewMatrix but without traslation)
	pixel_normal = (modelt * vec4(gl_Normal,1.0) ).xyz;
	
	//calcule the vertex in object space
	pixel_pos = (model * gl_Vertex ).xyz;
	
	//store the color in the varying var to use it from the pixel shader
	pixel_color = gl_Color;

	//calcule the position of the vertex using the matrices
	gl_Position = mvp * gl_Vertex;
}