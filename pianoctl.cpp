/***********************************************************
    wxPianoControl
    implements a piano control for wxWidgets
    Petru Dimitriu, 2014
************************************************************/

#include "pianoctl.h"
#include <utility>

wxPianoControl::wxPianoControl(wxWindow * parent,
                               const long id,
                               int x, int y,
                               int width, int height,
                               Note First, Note Last,
                               bool names, bool octave,
                               wxColour WhiteKeyColour,
                               wxColour BlackKeyColour,
                               wxColour WhiteKeyColourPressed,
                               wxColour BlackKeyColourPressed,
                               wxColour WhiteKeyColourHovered,
                               wxColour BlackKeyColourHovered)
                               : wxPanel(parent, id, wxPoint(x, y), wxSize(width, height), wxNO_BORDER | wxWANTS_CHARS)
{
    this->WhiteKeyColour = WhiteKeyColour;
    this->BlackKeyColour = BlackKeyColour;
    this->WhiteKeyColourHovered = WhiteKeyColourHovered;
    this->BlackKeyColourHovered = BlackKeyColourHovered;
    this->WhiteKeyColourPressed = WhiteKeyColourPressed;
    this->BlackKeyColourPressed = BlackKeyColourPressed;

    FirstDrawnKey=First;
    LastDrawnKey=Last;
    Playing = 0;
    ShowNoteNames = names;
    ShowOctave = octave;
    ContextMenu = new wxMenu();
    ShowNoteNamesMenuItem = new wxMenuItem(ContextMenu,1001,_("Show note names"),_("Show the names of the notes on the keys of the piano"), wxITEM_CHECK);
    ShowNoteOctavesMenuItem = new wxMenuItem(ContextMenu,1002,_("Show note octaves"),_("Show the octave of the notes on the keys along with their names"), wxITEM_CHECK);
    UseKeyboardMenuItem = new wxMenuItem(ContextMenu,1003,_("Use keyboard to play"),_("Use the computer keyboard to play the piano"), wxITEM_CHECK);
    NoteNamesMenu = new wxMenu();
    ClassicMenuItem = new wxMenuItem(ContextMenu,3000,_("Classic"),_("Do, Re, Mi..."), wxITEM_RADIO);
    ByzantineMenuItem = new wxMenuItem(ContextMenu,3001,_("Byzantine"),_("Ni, Pa, Vu..."), wxITEM_RADIO);
    GreekByzantineMenuItem = new wxMenuItem(ContextMenu,3002,_("Greek Byzantine"),_(L"Νη, Πα, Βου..."), wxITEM_RADIO);
    WestenMenuItem = new wxMenuItem(ContextMenu,3003,_("Western"),_("C, D, E..."), wxITEM_RADIO);
    scroll = new wxScrollBar(this,wxID_ANY,wxPoint(0,GetSize().GetHeight()-20),wxSize(GetSize().GetWidth(),15));

    ContextMenu->Append(ShowNoteNamesMenuItem);
    ContextMenu->Append(ShowNoteOctavesMenuItem);
    ContextMenu->Append(UseKeyboardMenuItem);
    ContextMenu->Append(wxID_ANY,_("Note names"),NoteNamesMenu);
    NoteNamesMenu->Append(ClassicMenuItem);
    NoteNamesMenu->Append(ByzantineMenuItem);
    NoteNamesMenu->Append(GreekByzantineMenuItem);
    NoteNamesMenu->Append(WestenMenuItem);

    ShowNoteNamesMenuItem->Check(names);
    ShowNoteOctavesMenuItem->Check(octave);
    ClassicMenuItem->Check(1);

    // Keyboard mapping
    keyboard_mapping.insert(std::make_pair(L'Q',GetTones(DO,4)));
    keyboard_mapping.insert(std::make_pair(L'2',GetTones(DO,4)+0.5));
    keyboard_mapping.insert(std::make_pair(L'W',GetTones(RE,4)));
    keyboard_mapping.insert(std::make_pair(L'3',GetTones(RE,4)+0.5));
    keyboard_mapping.insert(std::make_pair(L'E',GetTones(MI,4)));
    keyboard_mapping.insert(std::make_pair(L'R',GetTones(FA,4)));
    keyboard_mapping.insert(std::make_pair(L'5',GetTones(FA,4)+0.5));
    keyboard_mapping.insert(std::make_pair(L'T',GetTones(SOL,4)));
    keyboard_mapping.insert(std::make_pair(L'6',GetTones(SOL,4)+0.5));
    keyboard_mapping.insert(std::make_pair(L'Y',GetTones(LA,4)));
    keyboard_mapping.insert(std::make_pair(L'7',GetTones(LA,4)+0.5));
    keyboard_mapping.insert(std::make_pair(L'U',GetTones(SI,4)));
    keyboard_mapping.insert(std::make_pair(L'I',GetTones(DO,5)));
    keyboard_mapping.insert(std::make_pair(L'9',GetTones(DO,5)+0.5));
    keyboard_mapping.insert(std::make_pair(L'O',GetTones(RE,5)));
    keyboard_mapping.insert(std::make_pair(L'0',GetTones(RE,5)+0.5));
    keyboard_mapping.insert(std::make_pair(L'P',GetTones(MI,5)));
    keyboard_mapping.insert(std::make_pair(L'[',GetTones(FA,5)));
    keyboard_mapping.insert(std::make_pair(L'+',GetTones(FA,5)+0.5));
    keyboard_mapping.insert(std::make_pair(L']',GetTones(SOL,5)));
}

void wxPianoControl::paintEvent(wxPaintEvent &evt)
{
    wxBufferedPaintDC dc(this);
    dc.SetBackground(wxBrush(wxColour(255,255,255)));
    dc.Clear();
    int x=10, y=10;
    HoveredNote = -1;
    Note i;
    dc.SetBrush(wxBrush(WhiteKeyColour));
    dc.SetPen(wxPen(BlackKeyColour));
    dc.SetFont(*wxNORMAL_FONT);
    wxString NoteName;
    for (i = FirstDrawnKey; i!=LastDrawnKey; i+=0.5)
    {
        if (!(i.HasAccidental()))
        {
            if (SelectedNotes.count(i)) // draw selected keys
                    dc.SetBrush(wxBrush(WhiteKeyColourPressed));
            else
            {
                // draw hovered keys
                if ( (wxRect(xrel+x+1,y+1,KeyWidth-1,GetSize().GetHeight()-30-1).Contains(mousePos))
                    && !(wxRect(xrel+x-(KeyWidth/4+1)+1,y+1,2*(KeyWidth/4+1)-1,(GetSize().GetHeight()-30)/5 * 3 - 2).Contains(mousePos))
                    && !(wxRect(xrel+x-(KeyWidth/4+1)+(KeyWidth-1)+1,y+1,2*(KeyWidth/4+1)-1,(GetSize().GetHeight()-30)/5 * 3 - 2).Contains(mousePos)))
                    {
                        dc.SetBrush(wxBrush(WhiteKeyColourHovered));
                        HoveredNote = i;
                    }
                    else
                        dc.SetBrush(wxBrush(WhiteKeyColour));
            }
            dc.DrawRectangle(xrel+x,y,KeyWidth,GetSize().GetHeight()-30);
            if ( (wxRect(xrel+x,y,KeyWidth,GetSize().GetHeight()-30).Contains(mousePos))
                && !(wxRect(xrel+x-(KeyWidth/4+1)+1,y+1,(KeyWidth/4+1)*2-1,(GetSize().GetHeight()-30)/5 * 3 - 2).Contains(mousePos))
                && !(wxRect(xrel+x-(KeyWidth/4+1)+(KeyWidth-1)+1,y+1,(KeyWidth/4+1)*2-1,(GetSize().GetHeight()-30)/5 * 3 - 2).Contains(mousePos)))
                dc.SetBrush(wxBrush(WhiteKeyColour));

            if (ShowNoteNames)
                NoteName = i.Tell(NullSignature,NoteNames,ShowOctave);
            dc.DrawText(NoteName,xrel+x+(KeyWidth-GetTextExtent(NoteName).GetWidth())/2,y+GetSize().GetHeight()-30-dc.GetCharHeight()-1);
            x+=KeyWidth-1;
        }
    }

    FullWidth = x; // save the full width for scrolling
    scroll->SetScrollbar(-xrel,GetSize().GetWidth(),FullWidth,GetSize().GetWidth());
    if (GetSize().GetWidth()>FullWidth)
        scroll->Hide();
    else
        scroll->Show();

    x = 10;
    dc.SetBrush(wxBrush(BlackKeyColour));
    for (i = FirstDrawnKey; i!=LastDrawnKey; i+=0.5)
    {
        if (i.HasAccidental())
        {
            if (SelectedNotes.count(i)) // draw selected keys
                    dc.SetBrush(wxBrush(BlackKeyColourPressed));
            else // draw hovered keys
            {
                if (wxRect(xrel+x-(KeyWidth/4+1)+1,y+1,2*(KeyWidth/4+1)-1,(GetSize().GetHeight()-30)/5 * 3 - 2).Contains(mousePos))
                {
                    dc.SetBrush(wxBrush(BlackKeyColourHovered));
                    HoveredNote = i;
                }
                else // draw normal keys
                    dc.SetBrush(wxBrush(BlackKeyColour));
            }
            dc.DrawRectangle(xrel+x-(KeyWidth/4+1),y,2*(KeyWidth/4+1),(GetSize().GetHeight()-30)/5 * 3);
            if (wxRect(xrel+x-(KeyWidth/4+1)+1,y+1,2*(KeyWidth/4+1)-1,(GetSize().GetHeight()-30)/5 * 3 - 2).Contains(mousePos))
                dc.SetBrush(wxBrush(BlackKeyColour));
        }
        else
            x+=KeyWidth-1;
    }
}

void wxPianoControl::mouseMoved(wxMouseEvent &event)
{
    mousePos = event.GetPosition();
    Note OldHoveredNote = HoveredNote;
    SetFocusIgnoringChildren();
    Refresh();
    Update();
    if (HoveredNote != OldHoveredNote)
    {
        if (HoveredNote != -1)
        {
            wxCommandEvent e(EVT_PIANO_KEY_HOVERON,GetId());
            e.SetClientData(&HoveredNote);
            e.SetEventObject(this);
            GetEventHandler()->ProcessEvent(e);
        }
        else
        {
            wxCommandEvent e(EVT_PIANO_KEY_HOVEROFF,GetId());
            //e.SetClientData(&HoveredNote);
            e.SetEventObject(this);
            GetEventHandler()->ProcessEvent(e);
        }
    }
}

void wxPianoControl::leftDown(wxMouseEvent &event)
{
    if (SelectedNotes.count(HoveredNote))
        SelectedNotes.erase(HoveredNote);
    else
    {
        Playing = true;
        PlayingNote = HoveredNote;
        // Send pressed event
        wxCommandEvent f(EVT_PIANO_KEY_DOWN,GetId());
        f.SetClientData(&PlayingNote);
        f.SetEventObject(this);
        GetEventHandler()->ProcessEvent(f);

        SelectedNotes.insert(HoveredNote);
    }
    Refresh(); Update();
}

void wxPianoControl::leftUp(wxMouseEvent &event)
{
    Playing = false;
    Refresh(); Update();
    if (SelectedNotes.size()==0)
       return;

    wxCommandEvent f(EVT_PIANO_KEY_UP,GetId());
    f.SetClientData(&PlayingNote);
    f.SetEventObject(this);
    GetEventHandler()->ProcessEvent(f);

    if (! (wxGetKeyState(WXK_SHIFT)) )
    {
        MultipleNotes * c = new MultipleNotes();
        for (std::set<Note>::iterator i=SelectedNotes.begin();i!=SelectedNotes.end();i++)
            c->notes.push_back(*i);
        SelectedNotes.clear();


        wxCommandEvent e(EVT_PIANO_NOTE_RECEIVED,GetId());
        e.SetClientData(c);
        e.SetEventObject(this);
        GetEventHandler()->ProcessEvent(e);
    }

    Refresh();
    Update();

}

void wxPianoControl::mouseLeftWindow(wxMouseEvent &event)
{
    leftUp(event);
}

void wxPianoControl::ToggleNoteNames()
{
    ShowNoteNames = !ShowNoteNames;
    Refresh(); Update();
}

void wxPianoControl::ToggleNoteOctave()
{
    ShowOctave = !ShowOctave;
    Refresh(); Update();
}

void wxPianoControl::TogglePlayFromKeyboard()
{
    PlayFromKeyboard = !PlayFromKeyboard;
}

void wxPianoControl::ToggleNamesMenu(wxCommandEvent& event)
{
    ToggleNoteNames();
}

void wxPianoControl::ToggleOctaveMenu(wxCommandEvent& event)
{
    ToggleNoteOctave();
}

void wxPianoControl::TogglePlayFromKeyboardMenu(wxCommandEvent &event)
{
    TogglePlayFromKeyboard();
}

void wxPianoControl::sizeEvent(wxSizeEvent& event)
{
    long width = this->GetClientSize().GetWidth(), height = GetSize().GetHeight();
    if (-xrel + width>= FullWidth)
    {
        xrel = -FullWidth+width < 0 ? -FullWidth+width : 0;
    }
    scroll->SetSize(0,height-20,width,15);
    Refresh(); Update();
}

void wxPianoControl::scrollEvent(wxScrollEvent& event)
{
    xrel = -scroll->GetThumbPosition();
    Refresh(); Update();
}

void wxPianoControl::SetNoteNames(wxCommandEvent& event)
{
    NoteNames = event.GetId() % 3000;
    Refresh();
}

void wxPianoControl::eraseEvent(wxEraseEvent& event){ }

void wxPianoControl::doubleClick(wxMouseEvent &event) { }

void wxPianoControl::rightUp(wxMouseEvent &event)
{
    PopupMenu(ContextMenu);
}

void wxPianoControl::KeyDown(wxKeyEvent &event)
{
    if (!PlayFromKeyboard)
        return;
    wxChar c = event.GetUnicodeKey();
    Note n;
    n.flags=0;
    float t;
    t = keyboard_mapping[c];
    if (t!=0)
        n.tones = t;
    else
        return;
    if (SelectedNotes.count(n))
        return;

    wxCommandEvent f(EVT_PIANO_KEY_DOWN,GetId());
    f.SetClientData(&n);
    f.SetEventObject(this);
    GetEventHandler()->ProcessEvent(f);
    SelectedNotes.insert(n);
    Refresh();

}

void wxPianoControl::KeyUp(wxKeyEvent &event)
{
    if (!PlayFromKeyboard)
        return;
    wxChar c = event.GetUnicodeKey();
    Note n;
    n.flags=0;
    float t;
    t = keyboard_mapping[c];
    if (t!=0)
        n.tones = t;
    else
        return;

    wxCommandEvent f(EVT_PIANO_KEY_UP,GetId());
    f.SetClientData(&n);
    f.SetEventObject(this);
    GetEventHandler()->ProcessEvent(f);

    MultipleNotes *C = new MultipleNotes(n);
    wxCommandEvent e(EVT_PIANO_NOTE_RECEIVED,GetId());
    e.SetClientData(C);
    e.SetEventObject(this);
    GetEventHandler()->ProcessEvent(e);

    SelectedNotes.erase(n);
    Refresh();
}

void wxPianoControl::SetWhiteKeyColour(wxColour colour)
{
    WhiteKeyColour = colour;
}

void wxPianoControl::SetBlackKeyColour(wxColour colour)
{
    BlackKeyColour = colour;
}

void wxPianoControl::SetWhiteKeyColourPressed(wxColour colour)
{
    WhiteKeyColourPressed = colour;
}

void wxPianoControl::SetBlackKeyColourPressed(wxColour colour)
{
    BlackKeyColourPressed = colour;
}

void wxPianoControl::SetWhiteKeyColourHovered(wxColour colour)
{
    WhiteKeyColourHovered = colour;
}

void wxPianoControl::SetBlackKeyColourHovered(wxColour colour)
{
    BlackKeyColourHovered = colour;
}

void wxPianoControl::SetNoteNames(char Names)
{
    NoteNames = Names;
    Refresh(); Update();
}

void wxPianoControl::SetOctave(bool octave)
{
    ShowOctave = octave;
    Refresh(); Update();
}

void wxPianoControl::SetPlayFromKeyboard(bool play)
{
    PlayFromKeyboard = play;
}

void wxPianoControl::SetShowNoteNames(bool show)
{
    ShowNoteNames = show;
}

bool wxPianoControl::GetShowNoteNames()
{
    return ShowNoteNames;
}

char wxPianoControl::GetNoteNames()
{
    return NoteNames;
}

bool wxPianoControl::HasOctave()
{
    return ShowOctave;
}

bool wxPianoControl::HasPlayFromKeyboard()
{
    return PlayFromKeyboard;
}

BEGIN_EVENT_TABLE(wxPianoControl,wxPanel)

    EVT_PAINT(wxPianoControl::paintEvent)
    EVT_ERASE_BACKGROUND(wxPianoControl::eraseEvent)
    EVT_MOTION(wxPianoControl::mouseMoved)
    EVT_LEFT_DOWN(wxPianoControl::leftDown)
    EVT_LEFT_UP(wxPianoControl::leftUp)
    EVT_LEFT_DCLICK(wxPianoControl::doubleClick)
    EVT_RIGHT_UP(wxPianoControl::rightUp)
    EVT_LEAVE_WINDOW(wxPianoControl::mouseLeftWindow)
    EVT_SIZE(wxPianoControl::sizeEvent)
    EVT_SCROLL(wxPianoControl::scrollEvent)
    EVT_MENU(1001,wxPianoControl::ToggleNamesMenu)
    EVT_MENU(1002,wxPianoControl::ToggleOctaveMenu)
    EVT_MENU(1003,wxPianoControl::TogglePlayFromKeyboardMenu)
    EVT_MENU(3000,wxPianoControl::SetNoteNames)
    EVT_MENU(3001,wxPianoControl::SetNoteNames)
    EVT_MENU(3002,wxPianoControl::SetNoteNames)
    EVT_MENU(3003,wxPianoControl::SetNoteNames)
    EVT_KEY_DOWN(wxPianoControl::KeyDown)
    EVT_KEY_UP(wxPianoControl::KeyUp)


END_EVENT_TABLE()
