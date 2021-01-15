
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


struct QPCSCXX::Terminal::Private
{
    QString name;
};

QPCSCXX::Terminal::Terminal()
{
    p = new Private;
}

QPCSCXX::Terminal::Terminal(const Terminal & st)
{
    p = new Private;
    p->name = st.p->name;
}

QPCSCXX::Terminal::Terminal(const QString & name)
{
    p = new Private;
    p->name = name;
}

QPCSCXX::Terminal::~Terminal()
{
    delete p;
}

QString QPCSCXX::Terminal::name() const
{
    return p->name;
}

struct QPCSCXX::Context::Private
{
    bool valid;
    SCARDCONTEXT sc_context;
};

QPCSCXX::Context::Context()
{
    p = new Private;
    p->valid = false;
    LONG result;
    result = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &(p->sc_context));
    if (result != SCARD_S_SUCCESS) {
        qWarning() << "SCardEstablishContext failed: " << pcsc_stringify_error(result);
    } else {
        p->valid = true;
    }
}

QPCSCXX::Context::~Context()
{
    delete p;
}

static QPCSCXX::Context *contextInstance = 0;

QPCSCXX::Context* QPCSCXX::Context::instance()
{
    if (contextInstance == 0) {
        contextInstance = new QPCSCXX::Context();
    }
    return contextInstance;
}

bool QPCSCXX::Context::isValid()
{
    return p->valid;
}

QList<QPCSCXX::Terminal> QPCSCXX::Context::terminals()
{
    QList<QPCSCXX::Terminal> ts;
    LONG result;
    // calculate required memory size for a list of readers
    DWORD readers_size;

    result = SCardListReaders(p->sc_context, NULL, 0, &readers_size);

    if (result != SCARD_S_SUCCESS && result != SCARD_E_NO_READERS_AVAILABLE) {
        qWarning() << "SCardListReaders failed: " << pcsc_stringify_error(result);;
        return ts;
    }

    // allocate memory and fetch readers list
    QByteArray readers(readers_size, 0);
    result = SCardListReaders(p->sc_context, NULL, readers.data(), &readers_size);

    if (result != SCARD_S_SUCCESS && result != SCARD_E_NO_READERS_AVAILABLE) {
        qWarning() << "failed to call SCardListReaders: " << pcsc_stringify_error(result);
        return ts;
    }
    // qDebug() << "readers_size: " << readers_size;

    const auto rd = readers.data();
    for (int i = 0; i < readers_size - 1; ++i) {
        QPCSCXX::Terminal t(QString::fromUtf8(&rd[i]));
        while (rd[++i] != 0);
        ts << t;
    }
    return ts;
}
