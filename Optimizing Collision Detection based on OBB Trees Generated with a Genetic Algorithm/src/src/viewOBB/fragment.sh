varying vec3 V; 	//direccion de vision
varying vec3 L;		//posicion de la luz
varying vec3 N;		//normal del vertice

void main()
{
	//gl_FragColor = vec4(1,0,0,0);
	vec3 R = -normalize(reflect(L, N));
    vec4 amb = gl_FrontLightProduct[0].ambient;
    vec4 diff = gl_FrontLightProduct[0].diffuse * max(dot(N, L), 0.0);
    vec4 spec = gl_FrontLightProduct[0].specular * pow(max(abs(dot(R, V)), 0.0), gl_FrontMaterial.shininess);
    gl_FragColor = amb + diff + spec;
}