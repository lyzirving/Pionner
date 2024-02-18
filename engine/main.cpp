#include "Application.h"

int main()
{
	auto app = pio::CreateScope<pio::Application>();
	app->run();
	app.reset(nullptr);
	return 0;
}
