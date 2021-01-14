
#ifdef __APPLE__
#include <PCSC/pcsclite.h>
#include <PCSC/winscard.h>
#include <PCSC/wintypes.h>
#else
#include <pcsclite.h>
#include <winscard.h>
#include <wintypes.h>
#endif

#include <QtDebug>

#include "qpcscxx.h"

QList<QPCSCXX::Terminal> QPCSCXX::terminals()
{
    LONG result;
    SCARDCONTEXT sc_context;
    QList<QPCSCXX::Terminal> t;

    // calculate required memory size for a list of readers
    DWORD readers_size;

    result = SCardListReaders(sc_context, NULL, 0, &readers_size);

    if (result != SCARD_S_SUCCESS) {
        qWarning() << "SCardListReaders failed: " << pcsc_stringify_error(result);;
        return t;
    }

    // allocate memory and fetch readers list
    QByteArray readers(readers_size, 0);
    result = SCardListReaders(sc_context, NULL, readers.data(), &readers_size);

    if (result != SCARD_S_SUCCESS) {
        qWarning() << "failed to call SCardListReaders: " << pcsc_stringify_error(result);
        return t;
    }

    qDebug() << "readers_size: " << readers_size;

    for (int i = 0; i < readers_size - 1; ++i) {

    }

    return t;
}