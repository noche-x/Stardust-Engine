#include <Platform/Platform.h>
#include <Utilities/Logger.h>

using namespace Stardust;
int main() {
	Platform::initPlatform();

	Utilities::app_Logger->log("Hello from UNIX BOIS!", Utilities::LOGGER_LEVEL_INFO);

	Platform::exitPlatform();
	return 0;
}
