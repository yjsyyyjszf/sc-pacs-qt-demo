#include "qpcscxx.h"
#include "utils.h"

int main(int argv, char *args[])
{
    auto context = QPCSCXX::Context::instance();
    if (!context->isValid()) {
        qStdout() << "Failed to initialized SC context";
        return 1;
    }
    auto terminals = context->terminals();
    qStdout() << "Number of found terminals: " << terminals.size() << Qt::endl;
    for (const auto & s : terminals) {
        qStdout() << "  Terminal: " << s.name() << Qt::endl;
    }
	return 0;
}
