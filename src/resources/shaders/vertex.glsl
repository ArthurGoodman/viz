attribute vec4 vertex;
uniform mat4 mvp;

void main()
{
    gl_PointSize = 4.0;
    gl_Position = mvp * vertex;
}
