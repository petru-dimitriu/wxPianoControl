/***************************************************************
 * Name:      ByzApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Petru (petrimetri@gmail.com)
 * Created:   2013-08-10
 * Copyright: Petru ()
 * License:
 **************************************************************/

#include "mainwindow.h"
#include <wx/image.h>

#include "appmain.h"
#include <wx/notifmsg.h>

IMPLEMENT_APP(ByzApp);

bool ByzApp::OnInit()
{
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MainWindow* Frame = new MainWindow(0);
    	MessageViewer = new MessageWindow();
    	Frame->Show();
    	MessageViewer->Show();
    	SetTopWindow(Frame);
    }

    return wxsOK;

}
