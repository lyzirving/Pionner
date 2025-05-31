#include "TransformComponent.h"

#include "scene/node/Node.h"

namespace pio
{
	void TransformComponent::OnAttach()
	{
		OnPositionChange();
		OnScaleChange();
		OnRotationChange();
	}

	void TransformComponent::OnTick()
	{
		if(HaveParentHost())
		{
			auto parentComp = m_Host.lock()->GetParent()->GetComponent<TransformComponent>();
			auto transMat = m_TranslateMat * parentComp->m_TranslateMat;
			auto rotMat = (m_Rotation * parentComp->m_Rotation).Mat();
			auto scaleMat = m_ScaleMat * parentComp->m_ScaleMat;
			m_TransformMat = transMat * rotMat * scaleMat;
		}
		else
		{
			m_TransformMat = m_TranslateMat * m_Rotation.Mat() * m_ScaleMat;
		}
	}

	void TransformComponent::OnPositionChange()
	{
		m_TranslateMat = glm::translate(glm::mat4(1.f), m_Position);
		Invalidate();
	}

	void TransformComponent::OnScaleChange()
	{
		m_ScaleMat = glm::scale(glm::mat4(1.f), m_Scale);
		Invalidate();
	}

	void TransformComponent::OnRotationChange()
	{		
		m_Rotation.Mat();
		Invalidate();
	}

	void TransformComponent::OnMobilityChange()
	{
	}
}