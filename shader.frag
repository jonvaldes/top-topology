
void main()
{
	gl_FragColor.rgb = gl_Color.rgb / (pow( gl_FragCoord.z * 300.0, 4.0)/4000000000.0);
	gl_FragColor.a = 1.0;
}


