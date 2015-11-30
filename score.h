/***********************************************************
/// score.h
/// Implements a musical score
/// Petru Dimitriu, 2013
************************************************************/

#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <wx/msgdlg.h>

#define DO 0
#define RE 1
#define MI 2
#define FA 3
#define SOL 4
#define LA 5
#define SI 6

#define SHARP 10
#define FLAT 20
#define NORMAL 30

#define ID_NOTE 0
#define ID_REST 1
#define ID_SIGNATURE 2
#define ID_LINE 3

#define BOTH_CLEFS 0
#define SOL_CLEF 1
#define FA_CLEF 2

#define CLASSIC_NOTE_NAMES         0
#define BYZANTINE_NOTE_NAMES       1
#define GREEK_BYZANTINE_NOTE_NAMES 2
#define WESTERN_NOTE_NAMES         3

namespace Music
{
    typedef float Signature[7];
    const Signature CMajor = {1, 1, 0.5, 1, 1, 1, 0.5},
              NullSignature = {0, 0, 0, 0, 0, 0, 0};
    const std::wstring NoteNames[4][7] =
    {
        { L"Do", L"Re", L"Mi", L"Fa", L"Sol", L"La", L"Si" },
        { L"Ni", L"Pa", L"Vu", L"Ga", L"Di", L"Ke", L"Zo" },
        { L"Νη", L"Πα", L"Βου", L"Γα", L"Δι", L"Κε", L"Ζο"},
        { L"C", L"D", L"E", L"F", L"G", L"A", L"B" },
    };

    float AwayFromC(int,const Signature);
    float GetTones(int note, int octave = 4, const Signature sig = NullSignature);

    class Note
    {
        public:
            float tones, duration;
            long flags;
            void operator+= (float);

            Note(Note*);
            Note(char note, char octave, float d = 0.25,const Signature = NullSignature, const long f = 0);
            Note(float t);
            Note();
            int GetOctave();
            int GetNote(const Signature s = NullSignature);
            int GetStep(const Signature s = NullSignature);
            bool HasAccidental(const Signature s = NullSignature);
            bool operator< (Note) const;
            bool operator> (Note) const;
            bool operator== (Note) const;
            bool operator!= (Note) const;
            std::wstring Tell(const Signature sig = NullSignature, char Name = 0, bool withOctave = 0, bool withAccidental = 0);
    };

    Note NoteFromTones(float t, float d = 0.25);

    class MultipleNotes
    {
        public:
            std::vector<Note> notes;
            std::wstring lyric;
            short flags=0;
            MultipleNotes();
            MultipleNotes(Note);
            MultipleNotes(MultipleNotes*);
            MultipleNotes(Signature);
            void ChangeDurationDelta(int n, float exponent);
            void ChangeDurationDelta(float exponent);
            void ChangeToneDelta(int n, float tones);
            void ChangeToneDelta(float tones);
            void ChangeDuration(float duration);
            void SetSignature(Signature s);
            bool IsRest();
            bool IsNote();
            bool IsSignature();
            void MakeSignature(Signature&);
            std::wstring Tell(const Signature sig, int Name, bool withOctave, bool withAccidental);
    };

    class Score
    {
        public:
            int NumberOfThings, instrument=0;
            bool Saved = 1;
            float tempo = 80;
            Signature sig;
            std::vector<MultipleNotes*> clusters;
            std::wstring title, TitleFont;
            char Name = 0, clefs = 0;

            Score();
            Score(const Signature);
            std::wstring TellNote(int, bool, bool, const Signature);

            double CalculateSeconds(int n,float tempo);
            void AddNote(MultipleNotes*);
            void InsertNote(int x, MultipleNotes* note);
            void DeleteNote(int x, bool destroy = 1);
            void Clear();
            void GetSignatureAtNote(int n, Signature& s);
            void SetInstrument(int instrument);
            int GetInstrument();

            double GetBeats(int x=-1);
            double GetTime(int x=-1);
    };

    MultipleNotes* CreateRest(float d = 0.25);
    MultipleNotes* CreateLine();
    MultipleNotes* MakeSignatureMultipleNotesFromString(std::wstring s);
    void CopySignature(Signature&, const Signature);

    wxString Interval(float);
}
