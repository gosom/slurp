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

#include <QDebug>
#include <QObject>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QPainter>
#include <QWidget>
#include <QUrl>

#include "parser.h"
#include "eventer.h"
#include "retriever.h"
#include "util.h"
#include "scanner.h"

namespace slurp {

    /* NOTE: parser does not delete it's page instance */

    Parser::Parser(Eventer * owner, QString raw_url, QString raw_data) {
        this->owner = owner;
        url = QUrl(raw_url);
        data = raw_data;
    } 
    
    void Parser::run() {

        setAutoDelete(false);

        qDebug() << "debug: in parse thread " << QThread::currentThreadId();

        qDebug() << "debug: constructing web page instance";
        page = QSharedPointer< QWebPage > ( new QWebPage() );

        qDebug() << "debug: page instance running in thread: " << page->thread();

        QObject::connect(
            page.data(),
            SIGNAL(destroyed()),
            owner -> getParserPool(),
            SLOT(destroyedCallback()));

        QObject::connect( 
            page.data(), 
            SIGNAL(loadStarted()),
            owner -> getParserPool(),
            SLOT(loadStartedCallback()));

         QObject::connect( 
           page.data(), 
           SIGNAL(loadProgress(int)),
           owner -> getParserPool(),
           SLOT(loadProgressCallback(int)));
         
        QObject::connect(
            page.data(),
            SIGNAL(loadFinished(bool)),
            owner -> getParserPool(),
            SLOT(loadFinishedCallback(bool)));

        QObject::connect(
            page.data(),
            SIGNAL(frameCreated(QWebFrame*)),
            owner -> getParserPool(),
            SLOT(frameCreationCallback(QWebFrame*)));

        QObject::connect(
            page.data(),
            SIGNAL(contentsChanged()),
            owner -> getParserPool(),
            SLOT(contentsChangedCallback()));

        qDebug() << "debug: setting html with "
                 << data.size() << " bytes of data..";

        page->mainFrame()->setHtml(data, url);
        page->mainFrame()->load(url);

        qDebug() << "debug: post setHtml/load calls";
    }
}   /* namespace slurp */
