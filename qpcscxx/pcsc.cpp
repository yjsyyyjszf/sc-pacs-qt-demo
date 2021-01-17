
#include <QtDebug>

#include "pcsc.h"

QPCSCXX::Error convert_scard_error(LONG scard_error);

struct QPCSCXX::Terminal::Private
{
    bool valid = false;
    QPCSCXX::Error error;
    QString errorString;
    QString name;
    SCARDHANDLE reader;
};

struct QPCSCXX::Context::Private
{
    bool valid = false;
    QPCSCXX::Error error;
    QString errorString;
    SCARDCONTEXT sc_context;
};

struct QPCSCXX::RawContextHandle
{
    SCARDCONTEXT sc_context;
};

QPCSCXX::Terminal::Terminal()
{
    p = new Private;
}

QPCSCXX::Terminal::Terminal(const Terminal & st)
{
    p = new Private;
    *p = *(st.p);
}

QPCSCXX::Terminal::Terminal(const Context & context, const QString & name)
{
    p = new Private;
    p->name = name;
    auto h = context.handle();
    qDebug() << "context handle" << h->sc_context;
}

QPCSCXX::Terminal::~Terminal()
{
    delete p;
}

QString QPCSCXX::Terminal::name() const
{
    return p->name;
}

bool QPCSCXX::Terminal::connect(QPCSCXX::ProtocolType protocol)
{
    if (!p->valid) {
        return false;
    }
    DWORD initial_protocol;
    switch (protocol) {
    case QPCSCXX::ProtocolAuto:
        initial_protocol = SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1;
        break;
    case QPCSCXX::ProtocolT1:
        initial_protocol = SCARD_PROTOCOL_T1;
        break;
    case QPCSCXX::ProtocolT0:
        initial_protocol = SCARD_PROTOCOL_T0;
        break;
    case QPCSCXX::ProtocolRaw:
        initial_protocol = SCARD_PROTOCOL_RAW;
        break;
    }
    DWORD active_protocol;
    LONG result;

    // result = SCardConnect(p->sc_context, p->name.constData(), SCARD_SHARE_SHARED,
    //     initial_protocol, &(p->reader), &active_protocol);
    // if (result != SCARD_S_SUCCESS) {
    //     p->valid = false;
    //     p->error = convert_scard_error(result);
    //     p->errorString = pcsc_stringify_error(result);
    //     qWarning() << "SCardConnect failed: " << pcsc_stringify_error(result);
    //     return false;
    // }
}

QPCSCXX::Context::Context()
{
    p = new Private;
    LONG result;
    result = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &(p->sc_context));
    if (result != SCARD_S_SUCCESS) {
        p->error = convert_scard_error(result);
        p->errorString = pcsc_stringify_error(result);
        qWarning() << "SCardEstablishContext failed: " << pcsc_stringify_error(result);
    } else {
        p->valid = true;
    }
}

QPCSCXX::Context::~Context()
{
    SCardReleaseContext(p->sc_context);
    delete p;
}

static QPCSCXX::Context * contextInstance = 0;

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
        p->error = convert_scard_error(result);
        p->errorString = pcsc_stringify_error(result);
        qWarning() << "SCardListReaders failed: " << pcsc_stringify_error(result);;
        return ts;
    }

    // allocate memory and fetch readers list
    QByteArray readers(readers_size, 0);
    result = SCardListReaders(p->sc_context, NULL, readers.data(), &readers_size);

    if (result != SCARD_S_SUCCESS && result != SCARD_E_NO_READERS_AVAILABLE) {
        p->error = convert_scard_error(result);
        p->errorString = pcsc_stringify_error(result);
        qWarning() << "failed to call SCardListReaders: " << pcsc_stringify_error(result);
        return ts;
    }

    const auto rd = readers.data();
    for (int i = 0; i < readers_size - 1; ++i) {
        QPCSCXX::Terminal t(*this, QString::fromUtf8(&rd[i]));
        while (rd[++i] != 0);
        ts << t;
    }
    return ts;
}

QPCSCXX::RawContextHandlePtr QPCSCXX::Context::handle() const
{
    auto h = new RawContextHandle;
    h->sc_context = p->sc_context;
    return QPCSCXX::RawContextHandlePtr(h);
}

// local functions
QPCSCXX::Error convert_scard_error(LONG scard_error)
{
    return QPCSCXX::NoError;
}