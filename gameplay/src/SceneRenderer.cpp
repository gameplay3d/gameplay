#include "Base.h"
#include "SceneRenderer.h"

namespace gameplay
{

SceneRenderer::SceneRenderer()
    : _wireframe(false)
{
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::setWireframe(bool wireframe)
{
    _wireframe = wireframe;
}

bool SceneRenderer::isWireframe() const
{
    return _wireframe;
}

}