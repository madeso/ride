#pragma once

#include "core/ints.h"
#include "core/rect.h"

#include "backend/shader.h"
#include "backend/texture.h"

namespace ride::apigl
{

struct Texture;
struct ShaderProgram;
struct Render2;

struct Vertex2
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texturecoord;
};

struct SpriteBatch
{
    static constexpr int max_quads = 100;

    std::vector<float> data;
    int quads = 0;
    Texture* current_texture = nullptr;
    u32 va;
    u32 vb;
    u32 ib;
    Render2* render;
    Texture white_texture;

    SpriteBatch(ShaderProgram* shader, Render2* r);
    ~SpriteBatch();

    SpriteBatch(const SpriteBatch&) = delete;
    void operator=(const SpriteBatch&) = delete;
    SpriteBatch(SpriteBatch&&) = delete;
    void operator=(SpriteBatch&&) = delete;

    void quad(std::optional<Texture*> texture, const Vertex2& v0, const Vertex2& v1, const Vertex2& v2, const Vertex2& v3);
    void quad(std::optional<Texture*> texture, const Rectf& scr, const std::optional<Rectf>& texturecoord, const glm::vec4& tint = glm::vec4(1.0f));
    void quad(std::optional<Texture*> texture, const Rectf& scr, const Recti& texturecoord, const glm::vec4& tint = glm::vec4(1.0f));

    void submit();
};

struct Render2
{
    Render2();

    ShaderVertexAttributes quad_description;
    CompiledShaderVertexAttributes quad_layout;
    ShaderProgram quad_shader;
    Uniform view_projection_uniform;
    Uniform transform_uniform;
    Uniform texture_uniform;

    SpriteBatch batch;
};

}


