
#include <QtDebug>

#include "pcsc.h"

QPCSCXX::Error convert_scard_error(LONG scard_error);

// This struct implements PIMPL pattern and hides SCARDCONTEXT from this library user
struct QPCSCXX::ContextHandleImpl
{
    SCARDCONTEXT sc_context;
};

// This struct implements PIMPL pattern and hides SCARDHANDLE from this library user
struct QPCSCXX::CardHandleImpl
{
    SCARDHANDLE sc_card;
};

struct QPCSCXX::Terminal::Private
{
    bool valid = false;
    QPCSCXX::Error error;
    QString errorString;
    QString name;
    ContextHandleImplPtr context;
};

struct QPCSCXX::Context::Private
{
    bool valid = false;
    QPCSCXX::Error error;
    QString errorString;
    SCARDCONTEXT sc_context;
};

struct QPCSCXX::Card::Private
{
    bool valid = false;
    QPCSCXX::CardHandleImplPtr pc;
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

QPCSCXX::Terminal::Terminal(const QPCSCXX::ContextHandleImplPtr & context, const QString & name)
{
    p = new Private;
    p->name = name;
    p->context = context;
    p->valid = true;
    qDebug() << "context handle" << p->context->sc_context;
}

QPCSCXX::Terminal::~Terminal()
{
    delete p;
}

QString QPCSCXX::Terminal::name() const
{
    return p->name;
}

QPCSCXX::Card QPCSCXX::Terminal::connect(QPCSCXX::ProtocolType protocol)
{
    if (!p->valid) {
        return QPCSCXX::Card();
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
    SCARDHANDLE sc_card;

    result = SCardConnect(p->context->sc_context, p->name.toStdString().c_str(), SCARD_SHARE_SHARED,
        initial_protocol, &(sc_card), &active_protocol);
    if (result != SCARD_S_SUCCESS) {
        p->valid = false;
        p->error = convert_scard_error(result);
        p->errorString = pcsc_stringify_error(result);
        qWarning() << "SCardConnect failed: " << pcsc_stringify_error(result);
        return QPCSCXX::Card();
    }
    QPCSCXX::CardHandleImplPtr h(new QPCSCXX::CardHandleImpl);
    h->sc_card = sc_card;
    return QPCSCXX::Card(h);
}

bool QPCSCXX::Terminal::isValid()
{
    return p->valid;
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
        qWarning() << "SCardListReaders failed: " << pcsc_stringify_error(result);
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
        QPCSCXX::Terminal t(handle(), QString::fromUtf8(&rd[i]));
        while (rd[++i] != 0);
        ts << t;
    }
    return ts;
}

QPCSCXX::ContextHandleImplPtr QPCSCXX::Context::handle() const
{
    auto h = new ContextHandleImpl;
    h->sc_context = p->sc_context;
    return QPCSCXX::ContextHandleImplPtr(h);
}

QPCSCXX::Card::Card()
{
    p = new Private;
}

QPCSCXX::Card::Card(const Card & card)
{
    p = new Private;
    *p = *(card.p);
}

QPCSCXX::Card::Card(const QPCSCXX::CardHandleImplPtr & c)
{
    p = new Private;
    p->pc = c;
    p->valid = true;
}

QPCSCXX::Card::~Card()
{
    delete p;
}

bool QPCSCXX::Card::isValid()
{
    return p->valid;
}



// local functions
QPCSCXX::Error convert_scard_error(LONG scard_error)
{
    return QPCSCXX::NoError;
}
