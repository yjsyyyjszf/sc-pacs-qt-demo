#include "qpcscxx.h"
#include "utils.h"

int main(int argv, char *args[])
{
    auto terminals = QPCSCXX::terminals();

    qStdout() << "Number of found terminals: " << terminals.size() << Qt::endl;
	return 0;
}
