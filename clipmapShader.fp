uniform vec3 lightDir;
uniform sampler2D normalMapParent;
uniform sampler2D normalMap;
uniform sampler2D sandTexture;
uniform sampler2D grasTexture;
uniform sampler2D rockTexture;
uniform sampler2D snowTexture;

varying vec4 out_color;
varying float out_fog;
varying float out_blend;
varying vec2 out_coordParent;
varying vec2 out_coord;
varying vec2 out_pos;
varying float out_height;
varying float out_blendRegions;

void main(){
	vec3 n  = texture2D( normalMap, out_coord.rg ).rgb;    
	vec3 nP = texture2D( normalMapParent, out_coordParent ).rgb;
	vec3 normal = mix( nP, n, out_blend);
	
	vec3 tex_color; 
	
	float step1 = 0.5;
	float step2 = 2.0;
	float step3 = 3.0;
	
	float blend = 0.4;
	float blend2 = 0.8;
	
	//Sand
	if(out_height <= step1 - blend)
		tex_color = texture2D ( sandTexture, out_pos.xy).rgb;
	
	//Blend Sand --> Gras
	if(out_height > step1-blend && out_height <= step1+blend)
	{
		float factorA = (out_height - (step1-blend)) / blend2;
		tex_color = texture2D(sandTexture, out_pos.xy).rgb * (1.0 - factorA) + texture2D(grasTexture, out_pos.xy).rgb * factorA;
	}
	
	//Gras
	if(out_height > step1+blend && out_height <= step2-blend)
		tex_color = texture2D ( grasTexture, out_pos.xy).rgb;
		
	//Blend Gras --> Rock
	if(out_height > step2-blend && out_height <= step2+blend)
	{
		float factorA = (out_height - (step2-blend)) / blend2;
		tex_color = texture2D(grasTexture, out_pos.xy).rgb * (1.0 - factorA) + texture2D(rockTexture, out_pos.xy).rgb * factorA;
	}
		
	//Rock
	if(out_height > step2+blend && out_height <= step3-blend)
		tex_color = texture2D ( rockTexture, out_pos.xy).rgb;
		
	//Blend Rock --> Snow
	if(out_height > step3-blend && out_height <= step3+blend)
	{
		float factorA = (out_height - (step3-blend)) / blend2;
		tex_color = texture2D(rockTexture, out_pos.xy).rgb * (1.0 - factorA) + texture2D(snowTexture, out_pos.xy).rgb * factorA;
	}
		
	//Snow
	if(out_height > step3+blend)
		tex_color = texture2D ( snowTexture, out_pos.xy).rgb;

	//blend regions
	if(out_blendRegions>0.5)
		tex_color += out_color.xyz;

	//clamp fog
	float f = out_fog;
	if(f<0.0)
		f=0.0;
	if(f>1.0)
		f=1.0;

	gl_FragColor = vec4( mix( tex_color.xyz * dot( normal, lightDir ), vec3( 0.3, 0.4, 0.8), f), 1.0 );
}
