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

#include <iostream>

#include "eventer.h"
#include "slurpscanner.h"
#include "scanner.h"
#include "uri.h"

using namespace slurp;

static int checkArgs( int argc, char** argv );
void doUnitTests();

int main(int argc, char** argv) {
    int i, ret;
    const static int DEFAULT_URLS = 1024;
    Eventer* eventer = NULL;

    doUnitTests(); 

    if( checkArgs( argc, argv ) ) {
        eventer = new Eventer();

        for( i = 1; i <= argc-1; i++ ) {
            eventer -> queueURI( URI( argv[i] ) );
        }

        eventer -> run( DEFAULT_URLS );
    } else {
        std::cerr << "insufficient or malformed arguments\n";
    }
    
    ret = eventer ? 0 : 1;
    delete eventer;
    return  ret; 
}

static int checkArgs( int argc, char** argv ) {
    int i, ret = 1;
    
    if( argc < 2 ) {
      ret = 0;
    } else {


    }

    return ret;
}

void doUnitTests() {
    const char testBlock[] = "<html><head><title>test</title></head><body><a href=\"http://www.balls.com/index.html\">l1</a><a href='http://www.sack.com/test.php'>l2</a></body></html>";
    std::vector<URI>* uriVector;
    unsigned int i;

    std::cout << "initiating lexing\n**FLEX OUTPUT BEGINS:\n";
    uriVector = scanHTML(testBlock);
    std::cout << "\n**FLEX OUTPUT ENDS\ndone lexing with " << uriVector->size() << " URIs\n";

    for( i = 0; i < uriVector->size(); i++ ) {
        std::cout << i << " : " << (*uriVector)[i].getRawData() << std::endl;

    }

    delete uriVector; 
}
