#include "apigl/vertex_layout.h"

#include "assert/assert.h"


std::optional<VertexType> parse_vertex_type(const std::string& name)
{
    #define NAME(x) if(name == #x) { return VertexType::x; }

    NAME(position2)
    else NAME(position3)
    else NAME(normal3)
    else NAME(color4)
    else NAME(texture2)
    else return {};
    #undef NAME
}


/** A list of things we need to extract from the Geom when compiling */
struct VertexTypeList
{
    VertexTypeList(const std::vector<VertexType>& a_base_layout)
        : base_layout(a_base_layout)
    {
    }

    void
    add(const ShaderVertexAttributes& elements)
    {
        for(const auto& e: elements)
        {
            indices.insert(e.type);
        }
    }

    std::vector<VertexType> base_layout;
    std::set<VertexType> indices;
};



CompiledShaderVertexAttributes
compile_shader_layout(const CompiledVertexTypeList& l, const ShaderVertexAttributes& elements)
{
    std::vector<CompiledVertexElement> list;

    for(const auto& e: elements)
    {
        const auto found = l.indices.find(e.type);
        ASSERT(found != l.indices.end() && "layout wasn't added to the compilation list");
        if(found != l.indices.end())
        {
            list.push_back({e.type, e.name, found->second});
        }
    }

    return {list, l.debug_types};
}


[[nodiscard]] CompiledGeomVertexAttributes
get_mesh_layout(const CompiledVertexTypeList& l)
{
    std::vector<CompiledVertexElementNoName> list;

    for(const auto& e: l.indices)
    {
        list.push_back({e.first, e.second});
    }

    return {list, l.debug_types};
}


int
ShaderAttributeSize(const VertexType&)
{
    return 1;
}


CompiledVertexTypeList
compile_vertex_type_list(const VertexTypeList& list)
{
    std::map<VertexType, int> indices;

    int next_index = 0;

    for(const auto type: list.base_layout)
    {
        indices.insert({type, next_index});
        next_index += ShaderAttributeSize(type);
    }

    for(const auto type: list.indices)
    {
        // already in base layout, don't add again
        if(indices.find(type) != indices.end())
        {
            continue;
        }

        indices.insert({type, next_index});
        next_index += ShaderAttributeSize(type);
    }

    return {indices, {list.indices.begin(), list.indices.end()} };
}


CompiledVertexTypeList
compile_attribute_layouts(const std::vector<VertexType>& base_layout, const std::vector<ShaderVertexAttributes>& descriptions)
{
    auto list = VertexTypeList{base_layout};

    for(const auto& d: descriptions)
    {
        list.add(d);
    }

    return compile_vertex_type_list(list);
}


CompiledVertexTypeList
compile_attribute_layouts(const std::vector<ShaderVertexAttributes>& descriptions)
{
    return compile_attribute_layouts({}, descriptions);
}
