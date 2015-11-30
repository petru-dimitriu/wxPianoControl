#include "mainwindow.h"
#include "pianoctl.h"

extern MessageWindow* MessageViewer;

MainWindow::MainWindow(wxWindow* parent,wxWindowID id) : wxFrame(NULL,wxID_ANY,"Main window",wxDefaultPosition,wxSize(500,200))
{
    wxPianoControl* Piano = new wxPianoControl(this,1234,5,5,1000,250,Note(DO,2),Note(DO,7));
    //Piano->SetWhiteKeyColour(wxColour(255,255,0));
    Connect(1234,EVT_PIANO_NOTE_RECEIVED,(wxObjectEventFunction)&MainWindow::NoteReceived);
    Connect(1234,EVT_PIANO_KEY_DOWN,(wxObjectEventFunction)&MainWindow::KeyPressed);
    Connect(1234,EVT_PIANO_KEY_UP,(wxObjectEventFunction)&MainWindow::KeyReleased);
}

MainWindow::~MainWindow()
{
    Destroy();
}

void MainWindow::NoteReceived(wxCommandEvent &event)
{
    MultipleNotes* n = (MultipleNotes*)(event.GetClientData());
    *(MessageViewer->Box) << wxString("NOTE RECEIVED: ") << n->Tell(NullSignature,CLASSIC_NOTE_NAMES,1,1) << "\n";
}

void MainWindow::KeyPressed(wxCommandEvent &event)
{
    Note* n = (Note*)(event.GetClientData());
    *(MessageViewer->Box) << wxString("KEY PRESSED: ") << n->Tell(NullSignature,CLASSIC_NOTE_NAMES,1,1) << "\n";
}

void MainWindow::KeyReleased(wxCommandEvent &event)
{
    Note* n = (Note*)(event.GetClientData());
    *(MessageViewer->Box) << wxString("KEY RELEASED: ") << n->Tell(NullSignature,CLASSIC_NOTE_NAMES,1,1) << "\n";
}

MessageWindow::MessageWindow() : wxFrame(NULL,wxID_ANY,"Messages",wxDefaultPosition,wxSize(200,200))
{
    Box = new wxTextCtrl(this,2000,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_WORDWRAP);
}

MessageWindow::~MessageWindow()
{
    Destroy();
}
