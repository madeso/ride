#include "spritebatch.h"

#include "assert/assert.h"

#include "core/c.h"

#include "backend/dependency_opengl.h"
#include "backend/shader.h"
#include "backend/texture.h"


using namespace std::literals;

namespace ride::backends::gl
{

SpriteBatch::SpriteBatch(ShaderProgram* quad_shader, Render2* r)
    : render(r)
    , white_texture
    (
        load_image_from_color
        (
            0xffffffff,
            TextureEdge::clamp,
            TextureRenderStyle::pixel,
            Transparency::include
        )
    )
{
    quad_shader->use();

    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    constexpr auto vertex_size = 9 * sizeof(float);
    constexpr auto max_vertices = 4 * max_quads;
    constexpr auto max_indices = 6 * max_quads;

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, vertex_size * max_vertices, nullptr, GL_DYNAMIC_DRAW);

    auto relative_offset = [](unsigned int i)
    {
        return reinterpret_cast<void*>(i * sizeof(float));
    };

    unsigned int offset = 0;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, relative_offset(offset));
    offset += 3;

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertex_size, relative_offset(offset));
    offset += 4;

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, relative_offset(offset));
    offset += 2;


    std::vector<u32> indices;
    indices.reserve(max_indices);

    for(auto quad_index=0; quad_index<max_quads; quad_index+=1)
    {
        const auto base = quad_index * 4;
        indices.emplace_back(base + 0);
        indices.emplace_back(base + 1);
        indices.emplace_back(base + 2);

        indices.emplace_back(base + 2);
        indices.emplace_back(base + 3);
        indices.emplace_back(base + 0);
    }

    ASSERT(max_indices == indices.size());

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_indices * sizeof(u32), indices.data(), GL_STATIC_DRAW);
}


SpriteBatch::~SpriteBatch()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ib);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vb);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &va);
}


void add_vertex(SpriteBatch* batch, const Vertex2& v)
{
    batch->data.push_back(v.position.x);
    batch->data.push_back(v.position.y);
    batch->data.push_back(v.position.z);

    batch->data.push_back(v.color.x);
    batch->data.push_back(v.color.y);
    batch->data.push_back(v.color.z);
    batch->data.push_back(v.color.w);

    batch->data.push_back(v.texturecoord.x);
    batch->data.push_back(v.texturecoord.y);
}

Rectf get_sprite(const Texture& texture, const Recti& ri)
{
    const auto r = Cint_to_float(ri);
    const auto w = 1.0f/static_cast<float>(texture.width);
    const auto h = 1.0f/static_cast<float>(texture.height);
    return {r.get_left() * w, 1-r.get_top() * h, r.get_right() * w, 1-r.get_bottom() * h};
}

void SpriteBatch::quad(std::optional<Texture*> texture_argument, const Vertex2& v0, const Vertex2& v1, const Vertex2& v2, const Vertex2& v3)
{
    Texture* texture = texture_argument.value_or(&white_texture);
    assert(texture);

    if(quads == max_quads)
    {
        submit();
    }

    if(current_texture == nullptr)
    {
        current_texture = texture;
    }
    else if (current_texture != texture)
    {
        submit();
        current_texture = texture;
    }

    quads += 1;

    add_vertex(this, v0);
    add_vertex(this, v1);
    add_vertex(this, v2);
    add_vertex(this, v3);
}

void SpriteBatch::quad(std::optional<Texture*> texture, const Rectf& scr, const std::optional<Rectf>& texturecoord, const glm::vec4& tint)
{
    const auto tc = texturecoord.value_or(Rectf{0.0f, 0.0f, 1.0f, 1.0f});
    quad
    (
        texture,
        {{scr.get_left(), scr.get_bottom(), 0.0f}, tint, {tc.get_left(), tc.get_bottom()}},
        {{scr.get_right(), scr.get_bottom(), 0.0f}, tint, {tc.get_right(), tc.get_bottom()}},
        {{scr.get_right(), scr.get_top(), 0.0f}, tint, {tc.get_right(), tc.get_top()}},
        {{scr.get_left(), scr.get_top(), 0.0f}, tint, {tc.get_left(), tc.get_top()}}
    );
}

void SpriteBatch::quad(std::optional<Texture*> texture_argument, const Rectf& scr, const Recti& texturecoord, const glm::vec4& tint)
{
    Texture* texture = texture_argument.value_or(&white_texture);
    quad(texture, scr, get_sprite(*texture, texturecoord), tint);
}




Render2::Render2()
    // todo(Gustav): move quad_description and quad_layout to a seperate setup
    : quad_description
    (
        {
            {VertexType::position2, "position"},
            {VertexType::color4, "color"},
            {VertexType::texture2, "uv"}
        }
    )
    , quad_layout
    (
        compile_shader_layout(compile_attribute_layouts({quad_description}), quad_description)
    )
    , quad_shader
    (
        R"glsl(
            #version 450 core
            in vec3 position;
            in vec4 color;
            in vec2 uv;

            uniform mat4 view_projection;
            uniform mat4 transform;

            out vec4 varying_color;
            out vec2 varying_uv;

            void main()
            {
                varying_color = color;
                varying_uv = uv;
                gl_Position = view_projection * transform * vec4(position, 1.0);
            }
        )glsl"sv,
        R"glsl(
            #version 450 core

            in vec4 varying_color;
            in vec2 varying_uv;

            uniform sampler2D uniform_texture;

            out vec4 color;

            void main()
            {
                color = texture(uniform_texture, varying_uv) * varying_color;
            }
        )glsl"sv,
        quad_layout
    )
    , view_projection_uniform(quad_shader.get_uniform("view_projection"))
    , transform_uniform(quad_shader.get_uniform("transform"))
    , texture_uniform(quad_shader.get_uniform("uniform_texture"))
    , batch(&quad_shader, this)
{
    setup_textures(&quad_shader, {&texture_uniform});
}


void SpriteBatch::submit()
{
    if(quads == 0)
    {
        return;
    }

    bind_texture(render->texture_uniform, *current_texture);
    glBindVertexArray(va);

    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferSubData
    (
        GL_ARRAY_BUFFER,
        0,
        static_cast<GLsizeiptr>(sizeof(float) * data.size()),
        static_cast<const void*>(data.data())
    );
    glDrawElements(GL_TRIANGLES, 6 * quads, GL_UNSIGNED_INT, nullptr);

    data.resize(0);
    quads = 0;
    current_texture = nullptr;
}

}

