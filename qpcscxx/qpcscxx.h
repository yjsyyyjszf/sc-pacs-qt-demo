#ifndef QPCSCXX_H
#define QPCSCXX_H

#include <QList>
#include <QMetaType>

namespace QPCSCXX {

// classes
class Terminal {
public:
    Terminal();
    Terminal(const Terminal & st);
    Terminal(const QString & name);
    ~Terminal();
    QString name() const;

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

private:
    struct Private;
    Private * p;
    Context();
};
}

Q_DECLARE_METATYPE(QPCSCXX::Terminal);

#endif