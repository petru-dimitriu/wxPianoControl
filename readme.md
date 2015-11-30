# wxPianoControl
A simple Piano widget for wxWidgets. Tested on wxWidgets 2.9.*

## Overview
Widget which shows a piano with notes in any useful range, supports clicking keys as well as using the keyboard to play it, and sends appropriate events accordingly.


Supports showing note names on the keys according to 4 notations: Classic (Do, Re, ...), Byzantine (Ni, Pa, ...), Greek Byzantine (Νη, Πα,...) and Western (C, D, ...).


Includes `score.cpp`, which implements the `Note` and `MultipleNotes` (chord) classes in the `Music` namespace, that the widgets uses to pass and receive note information.


This repository contains a **working example** of the widget. Compile and run `appmain.cpp` to see it run right away.

## Tips
* In order to select multiple notes and build chords: press and hold `Shift`, click the keys on the piano one by one, releasing the `Shift` key before the last one.
* To view an options menu, right click anywhere within the control.

## Getting started & API
After you include `pianoctl.cpp` and `pianoctl.h` in your project, the following line of code will create a piano with notes ranging from C2 to B6:
`wxPianoControl* Piano = new wxPianoControl(this,1234,5,5,1000,250,Note(DO,2),Note(DO,7));`

Note that DO, RE, ... SI are `#define`s which resolve to 0, 1, ... 6.

### Events
The Piano widget sends the following `wxCommandEvent`s:

* `EVT_PIANO_KEY_DOWN`, when the user presses a key, either by clicking it or by pressing its corresponding key on the computer keyboard
* `EVT_PIANO_KEY_UP`, when the user releases a key, either by releasing the left mouse button or by releasing the corresponding key on the computer keyboard
* `EVT_PIANO_NOTE_RECEIVED`, when the user finishes either pressing a key or building a chord.

Note that `EVT_PIANO_KEY_UP` and `EVT_PIANO_NOTE_RECEIVED` are both sent when a user presses and releases a single key.

The events' ClientData contains a `Music::Note` object - for `KEY_DOWN` and `KEY_UP` - and a `Music::MultipleNotes` (chord) object - for `NOTE_RECEIVED`

### Methods

#### Getters
* `char GetNoteNames()` gets the notation used to draw notes
* `bool HasOctave()` is true if piano shows the octave besides note names
* `bool HasPlayFromKeyboard()` is true if piano allows being played using the computer keyboard
* `bool GetShowNoteNames()` is true if piano has note names drawn on the keys
* `wxColour GetWhiteKeyColour()`
* `wxColour GetBlackKeyColour()`
* `wxColour GetWhiteKeyColourPressed()`
* `wxColour GetBlackKeyColourPressed()`
* `wxColour GetWhiteKeyColourHovered()`
* `wxColour GetBlackKeyColourHovered()`

#### Setters
* `void SetNoteNames(char Names)`
sets which notation to use for note names
* `void SetOctave(bool octave)` sets whether or not to show the octave besides note names
* `void SetPlayFromKeyboard(bool play)` sets whether or not the piano allows being played using the computer keyboard
* `void SetShowNoteNames(bool)` sets whether or not note names should be drawn on the keys
* `void SetWhiteKeyColour(wxColour)`
* `void SetBlackKeyColour(wxColour)`
* `void SetWhiteKeyColourPressed(wxColour)`
* `void SetBlackKeyColourPressed(wxColour)`
* `void SetWhiteKeyColourHovered(wxColour)`
* `void SetBlackKeyColourHovered(wxColour)`

#### Togglers
* `void ToggleNoteNames()`
* `void ToggleNoteOctave()`
* `void TogglePlayFromKeyboard()`

### Score.cpp
`Score.cpp` is my own implementation of an abstract musical score, which holds chords (`MultipleNotes`) containing one or more `Note` objects. All functions are wrapped in the `Music` namespace.

* `Note(RE,5)` produces D5
* `Note::Tell()` returns a string with the note's name in a certain notation, optionally including the octave, accidentals (#, b, n) according to a certain `Signature`. 
* `Signature` objects contain signatures in the form of an array of 7 floating points, where `0.5` means the respective note has a sharp, `-0.5` means the note has a flat, and `0` means the note is natural.

## Screenshots
![wxPianoControl](https://raw.githubusercontent.com/petru-dimitriu/wxPianoControl/master/screen1.png)

## License
I hereby release this to the public domain. I don't mind if you credit me, though.
