varying vec3 V; 	//direccion de vision
varying vec3 L;		//posicion de la luz
varying vec3 N;		//normal del vertice

void main()
{
	// eye is at center.
    // P is a vertex position in eye coords.
    vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex); 
    V = -normalize(P);
    L = normalize(gl_LightSource[0].position - P); 
    N = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = ftransform();
} 