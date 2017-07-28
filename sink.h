#ifndef SINK_H
#define SINK_H

#include <globalheader.h>
#include <QObject>
#include <cstdlib>
#include <string>
#include <QString>
#include <utility>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/phoenix.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

namespace sinks = boost::log::sinks;

class Sink : public QObject,public sinks::basic_formatted_sink_backend<char,sinks::synchronized_feeding>
{
    Q_OBJECT
public:
    explicit Sink(QObject *parent = 0);
    void consume (const boost::log::record_view& rec, const std::string& str);
signals:
    void logthis(QString s);
public slots:
};

#endif // SINK_H
