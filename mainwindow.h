#pragma once

#include <wx/thread.h>
#include <wx/aui/aui.h>

#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/toolbar.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/notebook.h>

class MainWindow: public wxFrame
{
    public:

        MainWindow(wxWindow* parent,wxWindowID id = -1);
        virtual ~MainWindow();

    private:
            void NoteReceived(wxCommandEvent &event);
            void KeyPressed(wxCommandEvent &event);
            void KeyReleased(wxCommandEvent &event);

};

class MessageWindow : public wxFrame
{
public:
    MessageWindow();
    ~MessageWindow();
    wxTextCtrl * Box;

};
