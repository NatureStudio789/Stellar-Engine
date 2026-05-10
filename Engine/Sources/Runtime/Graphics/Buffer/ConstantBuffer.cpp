#include <Core.h>
#include "../Renderer/Renderable/Renderable.h"
#include "ConstantBuffer.h"

namespace SE
{
    GTransformCBuffer::GTransformCBuffer() : GConstantBuffer<GTransformCBData>()
    {
        this->Parent = null;
        this->BufferData = {};
    }

    GTransformCBuffer::GTransformCBuffer(const std::string& renderGroupName, unsigned int shaderRegisterIndex) : 
        GConstantBuffer<GTransformCBData>(renderGroupName, shaderRegisterIndex)
    {
        
    }

    GTransformCBuffer::GTransformCBuffer(const GTransformCBuffer & other) : GConstantBuffer<GTransformCBData>(other)
    {

    }

    GTransformCBuffer::~GTransformCBuffer()
    {
        this->Parent = null;
    }

    void GTransformCBuffer::SetParent(const GRenderable & parent)
    {
        this->Parent = &parent;
    }

    void GTransformCBuffer::Apply()
    {
        GConstantBuffer<GTransformCBData>::Apply();

        glm::mat4x4 WorldMatrix;
        WorldMatrix = this->Parent->GetTransform().GetMatrix() * this->Parent->GetAccumulatedMatrix();
        WorldMatrix = glm::transpose(WorldMatrix);

        this->UpdateData({ WorldMatrix });
    }
}
