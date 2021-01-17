#ifndef QPCSCXX_H
#define QPCSCXX_H

#include <QList>
#include <QMetaType>

namespace QPCSCXX {

enum ProtocolType {ProtocolAuto, ProtocolT0, ProtocolT1, ProtocolRaw};
enum Error {
    NoError = 0,
    UnknownError  // this must be last item in enum
};

struct RawContextHandle;
typedef std::shared_ptr<RawContextHandle> RawContextHandlePtr;

// classes
class Context;

class Terminal {
public:
    Terminal();
    Terminal(const Terminal & st);
    Terminal(const Context & context, const QString & name);
    ~Terminal();
    QString name() const;

    bool connect(ProtocolType protocol = ProtocolAuto);

private:
    struct Private;
    Private * p;
};

class Context {
public:
    static Context * instance();
    ~Context();
    bool isValid();
    QList<Terminal> terminals();
    RawContextHandlePtr handle() const;

private:
    struct Private;
    Private * p;
    Context();
};
}

Q_DECLARE_METATYPE(QPCSCXX::Terminal);

#endif