#pragma once

namespace Jeno::Core
{
	struct ShaderSet
	{
		std::vector<char> vs;
		std::vector<char> ps;
	};

	class ShaderCompiler
	{
	public:
		static ShaderSet Compile(const char* vsPath, const char* psPath);

	private:
		static std::vector<char> LoadShader(const char* path);

	};
}