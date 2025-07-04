#include <Flux.h>

class ATventure : public Flux::Application
{
public:
	ATventure()
	{

	}
	~ATventure()
	{

	}
};

Flux::Application* Flux::CreateApplication()
{
	return new ATventure();
}
