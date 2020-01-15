#pragma once

#include <memory>
#include "GSH_VulkanContext.h"
#include "GSH_VulkanFrameCommandBuffer.h"
#include "GSH_VulkanPipelineCache.h"
#include "Convertible.h"
#include "vulkan/ShaderModule.h"
#include "nuanceur/Builder.h"

namespace GSH_Vulkan
{
	class CTransfer
	{
	public:
		typedef uint32 PipelineCapsInt;
		typedef std::vector<uint8> XferBuffer;

		struct PIPELINE_CAPS : public convertible<PipelineCapsInt>
		{
			uint32 dstFormat : 6;
		};

		struct XFERPARAMS
		{
			uint32 bufAddress = 0;
			uint32 bufWidth = 0;
			uint32 rrw = 0;
			uint32 dsax = 0;
			uint32 dsay = 0;
			uint32 pixelCount = 0;
			uint32 padding1 = 0;
			uint32 padding2 = 0;
		};
		static_assert(sizeof(XFERPARAMS) == 0x20, "XFERPARAMS must be 32 bytes large.");

		CTransfer(const ContextPtr&, const FrameCommandBufferPtr&);
		virtual ~CTransfer();

		void SetPipelineCaps(const PIPELINE_CAPS&);

		void DoHostToLocalTransfer(const XferBuffer&);

		XFERPARAMS Params;

	private:
		typedef CPipelineCache<PipelineCapsInt> PipelineCache;

		VkDescriptorSet PrepareDescriptorSet(VkDescriptorSetLayout, VkImageView);

		Framework::Vulkan::CShaderModule CreateXferShader(const PIPELINE_CAPS&);
		PIPELINE CreateXferPipeline(const PIPELINE_CAPS&);

		Nuanceur::CUintRvalue XferStream_Read32(Nuanceur::CShaderBuilder&, Nuanceur::CArrayUintValue, Nuanceur::CIntValue);
		Nuanceur::CUintRvalue XferStream_Read16(Nuanceur::CShaderBuilder&, Nuanceur::CArrayUintValue, Nuanceur::CIntValue);
		Nuanceur::CUintRvalue XferStream_Read8(Nuanceur::CShaderBuilder&, Nuanceur::CArrayUintValue, Nuanceur::CIntValue);

		ContextPtr m_context;
		FrameCommandBufferPtr m_frameCommandBuffer;
		PipelineCache m_pipelineCache;

		Framework::Vulkan::CBuffer m_xferBuffer;
		uint8* m_xferBufferPtr = nullptr;
		uint32 m_xferBufferOffset = 0;

		PIPELINE_CAPS m_pipelineCaps;
	};

	typedef std::shared_ptr<CTransfer> TransferPtr;
}