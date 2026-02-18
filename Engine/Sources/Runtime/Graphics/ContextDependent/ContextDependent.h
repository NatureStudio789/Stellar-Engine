#ifndef _SE_CONTEXTDEPENDENT_H_
#define _SE_CONTEXTDEPENDENT_H_

namespace SE
{
	class GGraphicsContext;

	class GContextDependent
	{
	public:
		GContextDependent() = default;
		GContextDependent(const GContextDependent& other) = default;
		virtual ~GContextDependent() = default;

		void SetDependentContext(const std::string& name);

		std::shared_ptr<GGraphicsContext> GetContext();

	private:
		std::string DependentContextName = "Main";
	};
}

#endif