#include <Jeno.h>

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