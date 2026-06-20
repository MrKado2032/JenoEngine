#include <Jeno.h>
#include <DirectXMath.h>

class SandboxApp : public Jeno::Core::Application
{
public:
    SandboxApp()
    {

    }

    ~SandboxApp()
    {

    }

private:
    void Start() override
    {
        CreateQuad(100.0f, 100.0f, { .position = {100, 100, 0} });
    }

    void Update(float dt) override
    {

    }
};

int main()
{
    Jeno::Core::Application* app = new SandboxApp();
    app->Run();
    delete app;
}