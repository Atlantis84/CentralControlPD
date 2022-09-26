#ifndef HTTPPDMSG_H
#define HTTPPDMSG_H

#include "httpmsgprocess.h"
class HttpPdMsg : public HttpMsgProcess
{
    Q_OBJECT
public:
    HttpPdMsg();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);

signals:

public slots:
};

#endif // HTTPPDMSG_H
