#include "ModuleDispatcher.hpp"
#include "example/modules/PrinterModule.hpp"
#include "example/modules/TimerModule.hpp"
#include <unistd.h>

int main() {

	ModuleDispatcher dispatcher;
	auto printer_id = dispatcher.instanceModule<PrinterModule>("Printer");
	auto timer_id = dispatcher.instanceModule<TimerModule>("Timer");

	sleep(1);

	dispatcher.destroyModule(timer_id);

	sleep(2);

	dispatcher.destroyModule(printer_id);
	return 0;
}