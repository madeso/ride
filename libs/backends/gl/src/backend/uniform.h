#pragma once


namespace ride::backends::gl
{

/** Represents a found shader uniform and created via ShaderProgram::GetUniform()
 */
struct Uniform
{
    std::string name;
    int location;
    unsigned int debug_shader_program;
    int texture = -1; // >=0 if this is uniform maps to a texture

    /// Creates a invalid uniform
    Uniform();

    Uniform(const std::string& n, int l, unsigned int sp);

    operator bool() const;

    bool
    is_valid() const;
};

}

