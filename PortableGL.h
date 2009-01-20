#ifndef PORTABLE_GL_H
#define PORTABLE_GL_H

#if defined(__APPLE__)
    #include <OpenGL/gl.h> 
    #include <OpenGL/glu.h> 
#else
    #include <GL/gl.h>  
    #include <GL/glu.h> 
#endif

#endif
