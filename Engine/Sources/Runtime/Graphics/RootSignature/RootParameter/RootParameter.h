#ifndef _SE_ROOTPARAMETER_H_
#define _SE_ROOTPARAMETER_H_

namespace SE
{
	class GRootParameter
	{
	public:
		enum Type
		{
			SE_PARAMETER_CBV,
			SE_PARAMETER_SRV,
		};

	public:
		GRootParameter();
		GRootParameter(Type type, unsigned int index, unsigned int descriptorCount = 1);
		GRootParameter(const GRootParameter& other);
		~GRootParameter() = default;

		bool operator==(const GRootParameter& other) const noexcept;

		Type ParameterType;
		unsigned int ShaderRegisterIndex;
		unsigned int DescriptorCount;
	};
}

#endif