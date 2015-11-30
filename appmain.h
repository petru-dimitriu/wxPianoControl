/***************************************************************
 * Name:      ByzApp.h
 * Purpose:   Defines Application Class
 * Author:    Petru (petrimetri@gmail.com)
 * Created:   2013-08-10
 * Copyright: Petru ()
 * License:
 **************************************************************/

#ifndef BYZAPP_H
#define BYZAPP_H

#include <wx/app.h>
#include "mainwindow.h"

class ByzApp : public wxApp
{
    public:
        virtual bool OnInit();
};

MessageWindow* MessageViewer;

#endif // BYZAPP_H
