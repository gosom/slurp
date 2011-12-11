/*
 * Slurp - a web crawler
 * Copyright (C) 2011 Joseph Max DeLiso

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SLURP_EVENTER_H
#define SLURP_EVENTER_H

#include <QString>
#include <QQueue>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QApplication>

#include <event2/event.h>
#include <curl/curl.h>

#include "retriever.h"

namespace slurp {
    class Retriever;	    
    class Eventer : public QThread {
	CURLM *multi;
	int quota, flags, running;
	event_base *eventBasePtr;
	struct event *timerEventPtr;
	QQueue < QString > initialURIs;
	QThreadPool parserPool;

    public:

	Eventer(
	    QApplication* thisApp,
	    const QQueue < QString > &initialURIs, 
	    int quota, int flags);
	struct event *registerSocket(curl_socket_t sockfd, int kind);
	void addHandle(CURL * handle);
	void processSocketEvent(int fd, short kind);
	void checkTimer();
	void updateTimer();
	void addTimer(long timeout_ms);
	void setSocket(Retriever * retriever,
		       curl_socket_t s, CURL * e, int act);

	void addSocket(curl_socket_t s, CURL * easy, int action);
	void scanMultiInfo();
	void processEvent(int fd, short kind);
	void run();
	void addUrl(const QString & url);
	void stop();
    };
}
#endif				/* SLURP_EVENTER_H */
