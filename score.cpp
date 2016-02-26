/***********************************************************
/// Score.cpp
/// Implements a musical score
/// Petru Dimitriu, 2013
************************************************************/

#include "score.h"

namespace Music
{
    float AwayFromC(int note, const Signature s)
    {
        float r=0;
        for (int i=0;i<note;i++)
            r += s[i] + CMajor[i];
        return r;
    }

    float GetTones(int note, int octave, const Signature sig)
    {
        return octave * (sig[0] + CMajor[0] + sig[1] + CMajor[1] + sig[2] + CMajor[2] + sig[3] + CMajor[3] +
                          sig[4] + CMajor[4] + sig[5] + CMajor[5] + CMajor[6]+ sig[6]) + AwayFromC(note,sig);
    }

    Note::Note(char note, char octave, float d, const Signature sig, long f)
    {
        tones = octave * (sig[0] + CMajor[0] + sig[1] + CMajor[1] + sig[2] + CMajor[2] + sig[3] + CMajor[3] +
                          sig[4] + CMajor[4] + sig[5] + CMajor[5] + CMajor[6]+ sig[6]) + AwayFromC(note,sig);
        duration = d;
        flags = f;
    }

    Note::Note(float t)
    {
        tones = t;
        duration = 0.25;
        flags = 0;
    }

    Note::Note(Note* n)
    {
        tones = n->tones;
        duration = n->duration;
        flags = n->flags;
    }

    Note::Note()
    {
        flags = 0;
    }

    Note NoteFromTones(float t, float d)
    {
        Note n;
        n.tones = t;
        n.duration = d;
        n.flags=0;
        return n;
    }

    MultipleNotes* CreateRest(float d)
    {
        Note n;
        MultipleNotes* c=new MultipleNotes();
        c->flags=ID_REST;
        n.flags = ID_REST;
        n.duration = d;
        c->notes.push_back(n);
        return c;
    }

    MultipleNotes* CreateLine()
    {
        Note n;
        MultipleNotes* c=new MultipleNotes();
        c->flags=ID_LINE;
        n.flags =ID_LINE;
        c->notes.push_back(n);
        return c;
    }

    void Note::operator=(float t)
    {
        tones = t;
    }

    void Note::operator+=(float t)
    {
        tones += t;
    }

    bool Note::operator==(Note t) const
    {
        return (duration == t.duration &&
                tones == t.tones);
    }

    bool Note::operator!=(Note t) const
    {
        return (duration != t.duration ||
                tones != t.tones);
    }

    int Note::GetOctave()
    {
        return int(tones/6);
    }

    int Note::GetNote(const Signature s)
    {
        float t = tones - int(tones/6)*6, aux = 0;
        int i=0;

        if (t==0)           i = 00; // DO
        if (t==0.5)         i = 01; // DO#
        if (t==1)           i = 10; // RE
        if (t==1.5)         i = 12; // REb
        if (t==2)           i = 20; // MI
        if (t==2.5)         i = 30; // FA
        if (t==3)           i = 31; // FA#
        if (t==3.5)         i = 40; // SOL
        if (t==4)           i = 41; // SOL#
        if (t==4.5)         i = 50; // LA
        if (t==5)           i = 62; // SIb
        if (t==5.5)         i = 60; // SI

        // bemol + diez = becar
        if ((i%10==0 && s[i/10]==-0.5) ||
            (i%10==0 && s[i/10]==0.5))
            i = i/10 * 10 + 3;
        if ((i%10==1 && s[i/10]==0.5) ||
            (i%10==2 && s[i/10]==-0.5))
            i = i/10 * 10 + 0;
        if ((i%10==1 && s[i/10]==-0.5) ||
            (i%10==2 && s[i/10]==0.5))
            i = i/10 * 10 + 0;

        return i;
    }

    int Note::GetStep(const Signature s)
    {
        return GetOctave()*7+(GetNote(s)/10);
    }

    bool Note::operator<(Note a) const
    {
        return (tones<a.tones);
    }

    bool Note::operator>(Note a) const
    {
        return (tones>a.tones);
    }

    bool Note::HasAccidental(const Signature s)
    {
        return ((GetNote(s)%10) != 0);
    }

    Score::Score()
    {
        Saved = 1;
        Score(NullSignature);
    }

    Score::Score(const Signature s)
    {
        NumberOfThings = 0;
        for (int i=0;i<7;i++)
            sig[i]=s[i];
    }

    void Score::AddNote(MultipleNotes* note)
    {
        MultipleNotess.push_back(note);
        NumberOfThings++;
        Saved = 0;
    }

    void Score::InsertNote(int x, MultipleNotes* note)
    {
        std::vector<MultipleNotes*>::iterator i = MultipleNotess.begin();
        MultipleNotess.insert(i+x,note);
        NumberOfThings++;
        Saved = 0;
    }

    void Score::DeleteNote(int x, bool destroy)
    {
        if (destroy) delete MultipleNotess[x];
        std::vector<MultipleNotes*>::iterator i = MultipleNotess.begin();
        MultipleNotess.erase(i+x);
        NumberOfThings--;
        Saved = 0;
    }

    void Score::Clear()
    {
        MultipleNotess.clear();
        NumberOfThings=0;
        Saved = 0;
    }

    void Score::SetInstrument(int instrument)
    {
        this->instrument = instrument;
    }

    int Score::GetInstrument()
    {
        return this->instrument;
    }

    MultipleNotes::MultipleNotes()
    {
        flags = 0;
    }

    MultipleNotes::MultipleNotes(Note x)
    {
        notes.push_back(x);
        flags = 0;
    }

    MultipleNotes::MultipleNotes(MultipleNotes* x)
    {
        notes = x->notes;
        lyric = x->lyric;
        flags = 0;
    }

    MultipleNotes::MultipleNotes(Signature s)
    {
        flags = 2;
        notes.push_back(Note());
        std::wstringstream st;
        for (int i=0; i<=6; i++)
            st << s[i] << " ";
        lyric = st.str();
    }

    MultipleNotes* MakeSignatureMultipleNotesFromString(std::wstring s)
    {
        MultipleNotes * C = new MultipleNotes();
        C->flags = ID_SIGNATURE;
        C->notes.push_back(Note());
        C->lyric=s;
        return C;
    }

    void MultipleNotes::MakeSignature(Signature& sig)
    {
        int i;
        std::wstringstream st(lyric);
        for (i=0;i<=6;i++)
            st >> sig[i];
    }

    void MultipleNotes::SetSignature(Signature s)
    {
        std::wstringstream st;
        for (int i=0; i<=6; i++)
            st << s[i] << " ";
        lyric = st.str();
    }

    void MultipleNotes::ChangeDurationDelta(float exponent)
    {
        for (int i=0;i<notes.size();i++)
            notes[i].duration *= pow(2,exponent);
    }

    void MultipleNotes::ChangeDurationDelta(int n,float exponent)
    {
        notes[n].duration *= pow(2,exponent);
    }

    void MultipleNotes::ChangeToneDelta(int n,float tones)
    {
        notes[n].tones += tones;
    }

    void MultipleNotes::ChangeToneDelta(float tones)
    {
        for (int i=0;i<notes.size();i++)
            notes[i].tones += tones;
    }

    bool MultipleNotes::IsRest()
    {
        return (flags==ID_REST);
    }

    bool MultipleNotes::IsNote()
    {
        return (flags==ID_NOTE);
    }

    bool MultipleNotes::IsSignature()
    {
        return (flags==ID_SIGNATURE);
    }

    void MultipleNotes::ChangeDuration(float duration)
    {
        for (int i=0;i<notes.size();i++)
        {
            notes[i].duration = duration;
        }
    }

    std::wstring MultipleNotes::Tell(const Signature sig, int Name, bool withOctave, bool withAccidental)
    {
        std::wstringstream s;
        int NoteGot, x = notes.size();

        for (int i=0;i<x;i++)
        {
            NoteGot = notes[i].GetNote(sig);
            s << NoteNames[Name][NoteGot/10];
            if (withAccidental)
                switch (NoteGot%10)
                {
                    case 1: s << "#"; break;
                    case 2: s << "b"; break;
                    case 3: s << "n"; break;
                }
            if (withOctave)
                s << notes[i].GetOctave();
            if (i<x-1)
                s << "/";
        }
        return s.str();
    }

    std::wstring Score::TellNote(int n, bool withOctave, bool withAccidental, const Signature sig)
    {
        return MultipleNotess[n]->Tell(sig,Name,withOctave,withAccidental);
    }

    double Score::GetTime(int x)
    {
        double time=0;
        if (x==-1)
            x = NumberOfThings;
        for (int i=0;i<x;i++)
            time += CalculateSeconds(i,tempo);
        return time;
    }

    double Score::GetBeats(int x)
    {
        wxString s;
        double beats=0;
        if (x==-1)
            x = NumberOfThings;
        for (int i=0;i<x;i++)
            beats += MultipleNotess[i]->notes[0].duration*4;
        return beats;
    }

    double Score::CalculateSeconds(int n,float tempo)
    {
        double a = 60/tempo*4*MultipleNotess[n]->notes[n].duration;
        return a;
    }

    void Score::GetSignatureAtNote(int n, Signature& s)
    {
        int gasit = -1, i;
        CopySignature(s,NullSignature);
        for (i=0;i<n;i++)
        {
            if (MultipleNotess[i]->IsSignature())
                gasit = i;
        }
        if (gasit!=-1)
            MultipleNotess[gasit]->MakeSignature(s);
    }

    std::wstring Note::Tell(const Signature sig, char Name, bool withOctave, bool withAccidental)
    {
        std::wstringstream s;
        int NoteGot = GetNote(sig);
        if (withAccidental)
            switch (NoteGot%10)
            {
                case 1: s << "#"; break;
                case 2: s << "b"; break;
                case 3: s << "n"; break;
            }
        s << NoteNames[Name][NoteGot/10];
        if (withOctave)
            s << GetOctave();
        return s.str();
    }

    void CopySignature(Signature& dest, const Signature source)
    {
        int i;
        for (i=0;i<7;i++)
            dest[i] = source[i];
    }

    wxString Interval(float f)
    {
        if (f<0) f=-f;
        wxString s;
        s << int(f) << "t";
        if (f-float(int(f))==0.5)
            s << " + 1st";
        return s;
    }
}
