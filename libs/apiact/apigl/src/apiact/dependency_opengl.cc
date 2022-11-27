#include "apiact/dependency_opengl.h"


namespace ride::apigl
{

GLenum
Cint_to_glenum(int i)
{
    return static_cast<GLenum>(i);
}


GLuint
Cint_to_gluint(int i)
{
    return static_cast<GLuint>(i);
}


GLsizeiptr
Csizet_to_glsizeiptr(std::size_t t)
{
    return static_cast<GLsizeiptr>(t);
}


GLsizei
Csizet_to_glsizei(std::size_t t)
{
    return static_cast<GLsizei>(t);
}

}

