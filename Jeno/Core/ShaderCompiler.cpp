#include "ShaderCompiler.h"

#include <fstream>

namespace Jeno::Core
{
	ShaderSet ShaderCompiler::Compile(const char* vsPath, const char* psPath)
	{
        auto vs = LoadShader(vsPath);
        auto ps = LoadShader(psPath);

        ShaderSet shader{};
        shader.vs = vs;
        shader.ps = ps;

        return shader;
	}

	std::vector<char> ShaderCompiler::LoadShader(const char* path)
	{
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file");
        }

        const std::streamsize fileSize = file.tellg();
        if (fileSize <= 0)
        {
            throw std::runtime_error("shader file is empty");
        }

        std::vector<char> buffer(static_cast<size_t>(fileSize));

        file.seekg(0, std::ios::beg);

        if (!file.read(buffer.data(), fileSize))
        {
            throw std::runtime_error("failed to read shader file");
        }

        return buffer;
	}
}