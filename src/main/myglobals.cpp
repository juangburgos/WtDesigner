/*
 * icons.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: uli
 */

#include "myglobals.h"

#include <QApplication>

QMap<QString, QIcon> &
Icons::GetCache()
{
    static QMap<QString, QIcon> cache;
    static auto conn = QObject::connect(qApp,
                                        &QCoreApplication::aboutToQuit,
                                        [&]{
        cache.clear();
    });
    return cache;
}

