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
#include <QUrl>

#include "parser.h"
#include "eventer.h"
#include "retriever.h"

namespace slurp {

    Parser::Parser(
        Eventer * owner, 
        QString raw_url, 
        QString raw_data) {
  	    this->owner = owner;
	    url = QUrl( raw_url );
	    data = raw_data;
    }
	
    void Parser::run() {
	QWebElement document;
        QWebElementCollection allLinkTags;
        QString currentRawUrl;
        QUrl currentUrl;

	qDebug() << "debug: in parse thread " 
	         << QThread::currentThreadId()
		 << " beginning...";

	page = new QWebPage();
	page->mainFrame()->setHtml(data, url);
	document = page->mainFrame()->documentElement();

	allLinkTags = document.findAll("a");

	foreach (QWebElement currentElement, allLinkTags) {
             currentRawUrl = currentElement.attribute("href");

	     if( currentRawUrl != "" ) {
                 owner -> addUrl( QUrl( currentRawUrl ) );
	     }
        }

	qDebug() << "debug: " 
		 << data.size() << " bytes in data string and "
		 << page ->totalBytes() << " bytes in page, and found " 
		 << allLinkTags.count() << " link tags";

	qDebug() << "debug: parse complete on thread " 
		 << QThread::currentThreadId();
    }

    Parser::~Parser() {
        delete page;
	owner -> dispatchRetrievers();
    }
  }				/* namespace slurp */