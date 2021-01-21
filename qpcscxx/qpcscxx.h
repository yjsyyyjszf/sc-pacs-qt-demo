#ifndef QPCSCXX_H
#define QPCSCXX_H

#include <memory>
#include <QList>
#include <QMetaType>

namespace QPCSCXX {

enum ProtocolType {ProtocolAuto, ProtocolT0, ProtocolT1, ProtocolRaw};
enum Error {
    NoError = 0,
    UnknownError  // this must be last item in enum
};

struct ContextHandleImpl;
typedef std::shared_ptr<ContextHandleImpl> ContextHandleImplPtr;

struct CardHandleImpl;
typedef std::shared_ptr<CardHandleImpl> CardHandleImplPtr;

// classes
class Context;

class Card {
public:
    Card();
    Card(const Card & card);
    Card(const CardHandleImplPtr & c);
    ~Card();
    bool isValid();
private:
    struct Private;
    Private * p;
};

class Terminal {
public:
    Terminal();
    Terminal(const Terminal & st);
    // Terminal(const Context & context, const QString & name);
    Terminal(const ContextHandleImplPtr & context, const QString & name);
    ~Terminal();
    QString name() const;
    bool isValid();
    Card connect(ProtocolType protocol = ProtocolAuto);

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
    ContextHandleImplPtr handle() const;

private:
    struct Private;
    Private * p;
    Context();
};

}

Q_DECLARE_METATYPE(QPCSCXX::Terminal);
Q_DECLARE_METATYPE(QPCSCXX::Card);

#endif