#version 150





// per-fragment interpolated values from the vertex shader
in  vec3 fN;
in  vec3 fL, fLL, fLLL;
in  vec3 fE;

out vec4 fColor;

uniform vec4 LightPosition, LightPosition2, LightPosition3;
uniform float Shininess;
uniform vec4 AmbientProduct, AmbientProduct2, AmbientProduct3;
uniform vec4 DiffuseProduct, DiffuseProduct2, DiffuseProduct3;
uniform vec4 SpecularProduct, SpecularProduct2, SpecularProduct3;
uniform mat4 ModelView;


void main() 
{ 
   // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
	vec3 LLL = normalize(fLLL);
    vec3 LL = normalize(fLL);
	vec3 L = normalize(fL);
    float Ks;
    vec4 spec;
    vec3 H;
    vec4 amb;
    float Kd;
    vec4 diff;
   
	
	  vec4 speculars[3];
    speculars[0] =SpecularProduct;
    speculars[1] =SpecularProduct2;
    speculars[2] =SpecularProduct3;
	
    
    vec3 let_there_be_light[3];
    let_there_be_light[0]=L;
    let_there_be_light[1]=LL;
    let_there_be_light[2]=LLL;
    
    
    vec4 diffuses[3];
    diffuses[0]= DiffuseProduct;
    diffuses[1]= DiffuseProduct2;
    diffuses[2]= DiffuseProduct3;
    
	vec4 ambients[3];
    ambients[0]= AmbientProduct;
    ambients[1]= AmbientProduct2;
    ambients[2]= AmbientProduct3;
    

    for(int i=0; i<=2; i++)
    {
    
        H = normalize( let_there_be_light[i] + E );
            
        amb += ambients[i];

        Kd = max(dot(let_there_be_light[i], N), 0.0);
        diff += Kd*diffuses[i]; //diff += Kd*diffuses[i]
        
        Ks = pow(max(dot(N, H), 0.0), Shininess);
        spec  +=  Ks*speculars[i];  //check

        // discard the spec highlight if the light's behind the vertex
        if( dot(let_there_be_light[i], N) < 0.0 )
        {
            spec = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }

    gl_FragColor = amb + diff + spec;
    gl_FragColor.a = 1.0;
} 





