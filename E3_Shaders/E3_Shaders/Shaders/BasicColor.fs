#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform bool ComplementaryColor = false;

out vec4 Fragment;

void main()
{
	Fragment = vec4(Color,1);
	
	if(ComplementaryColor) {
		vec3 complementColor = Color - vec3(1,1,1);
		complementColor *= -1;
		Fragment = vec4(complementColor,1);
	}
	
	if(SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0){
		Fragment = vec4(SolidColor, 1);
		if(ComplementaryColor) {
			vec3 complementColor = SolidColor - vec3(1,1,1);
			complementColor *= -1;
			Fragment = vec4(complementColor,1);
		}
	}
	

	return;
}