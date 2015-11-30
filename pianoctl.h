/***********************************************************
    wxPianoControl
    implements a piano control for wxWidgets
    Petru Dimitriu, 2014
************************************************************/

#pragma once

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/dcclient.h>
#include <wx/gdicmn.h>
#include <wx/dcbuffer.h>
#include <wx/kbdstate.h>
#include <wx/notifmsg.h>
#include <wx/textctrl.h>
#include <wx/event.h>
#include "score.h"
#include <set>
#include <map>

#define EVT_PIANO_NOTE_RECEIVED   5001
#define EVT_PIANO_KEY_DOWN  5002
#define EVT_PIANO_KEY_UP    5003

using namespace Music;

class wxPianoControl : public wxPanel
{
public:
    wxPianoControl (wxWindow * parent, const long id, int x, int y, int width, int height, Note First, Note Last, bool names = 1, bool octave = 0,
                    wxColour WhiteKeyColour = wxColour(255,255,255),
                    wxColour BlackKeyColour = wxColour(0,0,0),
                    wxColour WhiteKeyColourPressed = wxColour(120,120,210),
                    wxColour BlackKeyColourPressed = wxColour(30,30,150),
                    wxColour WhiteKeyColourHovered = wxColour(160,160,210),
                    wxColour BlackKeyColourHovered = wxColour(50,50,150));
    void ToggleNamesMenu(wxCommandEvent &event);
    void ToggleOctaveMenu(wxCommandEvent &event);
    void SetNoteNames(wxCommandEvent& event);
    void TogglePlayFromKeyboardMenu(wxCommandEvent &event);

    void ToggleNoteNames();
    void ToggleNoteOctave();
    void TogglePlayFromKeyboard();

    void SetWhiteKeyColour(wxColour);
    void SetBlackKeyColour(wxColour);
    void SetWhiteKeyColourPressed(wxColour);
    void SetBlackKeyColourPressed(wxColour);
    void SetWhiteKeyColourHovered(wxColour);
    void SetBlackKeyColourHovered(wxColour);

    // sets which notation to use for note names
    void SetNoteNames(char Names);
    // sets whether or not to show the octave besides note names
    void SetOctave(bool octave);
    // sets whether or not the piano allows being played using the computer keyboard
    void SetPlayFromKeyboard(bool play);
    // sets whether or not note names should be drawn on the keys
    void SetShowNoteNames(bool);

    // gets the notation used to draw notes
    char GetNoteNames();
    // true if piano shows the octave besides note names
    bool HasOctave();
    // true if piano allows being played using the computer keyboard
    bool HasPlayFromKeyboard();
    // true if piano has note names drawn on the keys
    bool GetShowNoteNames();

    wxColour GetWhiteKeyColour();
    wxColour GetBlackKeyColour();
    wxColour GetWhiteKeyColourPressed();
    wxColour GetBlackKeyColourPressed();
    wxColour GetWhiteKeyColourHovered();
    wxColour GetBlackKeyColourHovered();


private:
    wxMenu* ContextMenu, *NoteNamesMenu;
    wxMenuItem* ShowNoteNamesMenuItem, *ShowNoteOctavesMenuItem, *ClassicMenuItem, *ByzantineMenuItem, *GreekByzantineMenuItem, *WestenMenuItem, *UseKeyboardMenuItem;

    wxColour WhiteKeyColour,
     BlackKeyColour,
     WhiteKeyColourPressed,
     BlackKeyColourPressed,
     WhiteKeyColourHovered,
     BlackKeyColourHovered;

    void paintEvent(wxPaintEvent &evt);
    void eraseEvent(wxEraseEvent &event);
    void mouseMoved(wxMouseEvent &event);
    void leftDown(wxMouseEvent &event);
    void leftUp(wxMouseEvent &event);
    void rightUp(wxMouseEvent &event);
    void mouseLeftWindow(wxMouseEvent &event);
    void sizeEvent(wxSizeEvent &event);
    void scrollEvent(wxScrollEvent &event);
    void KeyDown(wxKeyEvent &event);
    void KeyUp(wxKeyEvent &event);
    void doubleClick(wxMouseEvent &event);

    int KeyHeight, KeyWidth=22;
    long FullWidth, xrel=0, NoteNames=0;
    bool ShowNoteNames, ShowOctave;
    Note FirstDrawnKey, LastDrawnKey, HoveredNote, PlayingNote;
    wxPoint mousePos;
    wxScrollBar *scroll;
    std::set<Note> SelectedNotes;
    std::map<wchar_t,float> keyboard_mapping;
    bool Playing, PlayFromKeyboard=0;

DECLARE_EVENT_TABLE()
};
