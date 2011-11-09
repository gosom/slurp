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

#ifndef RETRIEVER_H
#define RETRIEVER_H

#include <QString>
#include <QRunnable>

#include <curl/curl.h>

#include "globals.h"
#include "eventer.h"
#include "scanner.h"

namespace slurp {
  class Retriever : public QRunnable {
     QString uri;
     CURL* conn;
     char errorBuffer[CURL_ERROR_SIZE];

     public:

     Retriever( const Eventer* eventer, QString uri, int flags );
     ~Retriever();
     void run();
     bool isValid() const;
  };
}
#endif 

