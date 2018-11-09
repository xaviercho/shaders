#version 150






in   vec4 vPosition;
in   vec3 vNormal;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL, fLL, fLLL;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform vec4 LightPosition, LightPosition2, LightPosition3;


void main()
{
       fLLL = LightPosition3.xyz;
	   fLL = LightPosition2.xyz;
       fL = LightPosition.xyz;
    
    
    fN = vNormal;
    fE = vPosition.xyz;
    
	
    if( LightPosition3.w != 0.0 ){
        fLLL = LightPosition3.xyz - vPosition.xyz;
    }
	
    if( LightPosition2.w != 0.0 ){
        fLL = LightPosition2.xyz - vPosition.xyz;
    }
	
	if( LightPosition.w != 0.0 ){
        fL = LightPosition.xyz - vPosition.xyz;
    }
	

    gl_Position = Projection*ModelView*vPosition;
}





