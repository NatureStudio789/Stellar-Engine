#ifndef _SE_RENDERER_H_
#define _SE_RENDERER_H_
#include "../../Core/Addressable/Addressable.h"
#include "../../Core/Registry/Registry.h"
#include "RenderPass/RenderPass.h"

namespace SE
{
	class GRenderer : public SAddressable, public GContextDependent
	{
	public:
		GRenderer();
		GRenderer(const std::string& name);
		virtual ~GRenderer();

		void Compile();

		virtual void Execute();

	protected:
		void SetInflowTarget(const std::string& inflowName, const std::string& target);
		void AddGlobalInflow(std::shared_ptr<GInflow> inflow);
		void AddGlobalOutflow(std::shared_ptr<GOutflow> outflow);
		void AppendRenderPass(std::shared_ptr<GRenderPass> renderPass);

		std::shared_ptr<GRenderPass> GetRenderPass(const std::string& name);

		std::vector<std::shared_ptr<GRenderPass>> RenderPassList;

		friend class GRenderStage;

	private:
		//void LinkPassInflows(std::shared_ptr<GRenderPass> renderPass);
		//void LinkGlobalInflows();

		void BuildFlowChain(std::shared_ptr<GFlowChain> flowChain, std::shared_ptr<GOutflow> outflow, 
			std::map<std::string, std::vector<std::shared_ptr<GInflow>>> targetPassOrderedInflowList);

		std::shared_ptr<GCommandList> RendererCommandList;

		std::vector<std::shared_ptr<GInflow>> GlobalInflowList;
		std::vector<std::shared_ptr<GOutflow>> GlobalOutflowList;

		std::vector<std::shared_ptr<GFlowChain>> StaticFlowChainList;
		std::vector<std::shared_ptr<GFlowChain>> DynamicFlowChainList;
		bool IsCompiled = false;
	};

	class SRendererRegistry : public SRegistry<GRenderer> 
	{
	public: 
		static void CompileAllRenderer();

		static std::shared_ptr<GRenderer> GetMainInstance() 
		{
			return GetInstance(MainInstanceName);
		}
	};
}

#endif