varying vec4 vert_pos;

uniform sampler2D texture;
uniform vec3 ambientLight;
uniform vec2 lightPos;
uniform bool hasLight;

void main()
{
	//Ambient light
	vec4 ambient = vec4(ambientLight.xyz, 1.0);

	//Convert light to view coords
	vec2 newLightPos = (gl_ModelViewProjectionMatrix * vec4(lightPos, 0, 1)).xy;
	
	//Calculate the vector from light to pixel (Make circular)
	vec2 lightToFrag = vert_pos.xy - newLightPos;
	lightToFrag.y = lightToFrag.y / 1.7;

	//Length of the vector (distance)
	float vecLength = clamp(length(lightToFrag) * 4, 0, 1);

	vec4 lampLight;

	if (hasLight)
	{
		lampLight = vec4(1-vecLength, 1-vecLength, 1-vecLength, 1) * vec4(0.8, 0.4, 0.2, 1.0);
	}

	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
    // multiply it by the color and lighting
	gl_FragColor = gl_Color * pixel * (clamp(ambient + lampLight, 0, 1));
}



	//vec2 position = (gl_ModelViewProjectionMatrix * vec4(gl_FragCoord.xy, 0, 1)).xy;

	//Convert light to view coords
	//vec2 newLightPos = (gl_ModelViewProjectionMatrix * vec4(lightPos, 0, 1)).xy;

	//float dist = distance(position.xy, lightPos.xy);

	// lookup the pixel in the texture
    //vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	//if(0.2 > dist) //Check if this pixel is without the range
	//{
	//	gl_FragColor = pixel * gl_Color * clamp(ambient + (1.0 - dist/0.2), 0.0, 1.0);
    //} else
	//{
    //    gl_FragColor = ambient * pixel;
    //}

	//float  pixelDistFromLight = distance(lightPos, vec2(gl_FragCoord.x,  gl_FragCoord.y)) / 20.0;

	//vec4 lampLight;

	//if (hasLight)
	//{
	//	lampLight = vec4(1-pixelDistFromLight, 1-pixelDistFromLight, 1-pixelDistFromLight, 1) * vec4(0.8, 0.4, 0.2, 1.0);
	//}

	
	//vec4 modColor = vec4(ambient.r + -pixelDistFromLight/100.0, ambient.g + -pixelDistFromLight/100.0, ambient.b + -pixelDistFromLight/100.0, ambient.a );
	//gl_FragColor = texture2D(texture, gl_TexCoord[0].xy) * modColor ;