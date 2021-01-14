#include "utils.h"

QTextStream& qStdout()
{
    static QTextStream ts(stdout);
    return ts;
}