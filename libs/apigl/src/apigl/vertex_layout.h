#pragma once

#include <map>
#include <set>

namespace ride::apigl
{

/** Vertex source type, position, normal etc. */
enum class VertexType
{
    position2, position3, normal3, color4, texture2
    // change to include other textcoords and custom types that are created from scripts
};


/** A not-yet-realised binding to a shader variable like 'vec3 position' */
struct VertexElementDescription
{
    VertexType type;
    std::string name;
};


/** Describes all vertex inputs a shader requires like `[vec3 position, vec3 normal, vec2 uv]`*/
using ShaderVertexAttributes = std::vector<VertexElementDescription>;


/** A realized VertexElementDescription like `vec3 position` at gl shader position 3 */
struct CompiledVertexElement
{
    VertexType type;
    std::string name;
    int index;
};


/** A realized VertexElementDescription without name like `vec3` at gl shader position 3 */
struct CompiledVertexElementNoName
{
    VertexType type;
    int index;
};


using VertexTypes = std::vector<VertexType>;


/** A list of CompiledVertexElement (for shader) */
struct CompiledShaderVertexAttributes
{
    std::vector<CompiledVertexElement> elements;
    VertexTypes debug_types;
};


/** A list of CompiledVertexLayoutNoNameList (for mesh) */
struct CompiledGeomVertexAttributes
{
    std::vector<CompiledVertexElementNoName> elements;
    VertexTypes debug_types;
};


/** A mapping of the vertex type (position...) to the actual shader id (for more than one shader) */
struct CompiledVertexTypeList
{
    std::map<VertexType, int> indices;
    VertexTypes debug_types;
};


std::optional<VertexType>
parse_vertex_type(const std::string& name);

[[nodiscard]] CompiledShaderVertexAttributes
compile_shader_layout
(
    const CompiledVertexTypeList& l,
    const ShaderVertexAttributes& elements
);

[[nodiscard]] CompiledGeomVertexAttributes
get_mesh_layout(const CompiledVertexTypeList& l);

CompiledVertexTypeList
compile_attribute_layouts
(
    const std::vector<VertexType>& base_layout,
    const std::vector<ShaderVertexAttributes>& descriptions
);

CompiledVertexTypeList
compile_attribute_layouts(const std::vector<ShaderVertexAttributes>& descriptions);

}


