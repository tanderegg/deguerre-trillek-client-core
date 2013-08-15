#ifndef TLKGL_HH_INCLUDED
#define TLKGL_HH_INCLUDED

#ifdef __gl_h_
#error gl.h included before tlkGL.h
#endif

#ifdef __glext_h_
#error glext.h included before tlkGL.h
#endif

#ifdef __wglext_h_
#error wglext.h included before tlkGL.h
#endif

#ifdef __glxext_h_
#error glxext.h included before tlkGL.h
#endif

#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <GL/gl.h>
#elif defined(__APPLE__) || defined(__APPLE_CC__)
# include <OpenGL/gl.h>
#else // GLX
# define __glext_h_  /* prevent glext.h from being included  */
# define __glxext_h_ /* prevent glxext.h from being included */
# include <GL/gl.h>
# include <GL/glx.h>
#endif

#endif // TLKGL_HH_INCLUDED
