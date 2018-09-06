/***************************************************************
 * Name:      EnvelopeTestMain.h
 * Purpose:   Defines Application Frame
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2018-09-04
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#ifndef ENVELOPETESTMAIN_H
#define ENVELOPETESTMAIN_H

//(*Headers(EnvelopeTestFrame)
#include "envelopegraph.h"
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
//*)

class EnvelopeTestFrame: public wxFrame
{
    public:

        EnvelopeTestFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~EnvelopeTestFrame();

    private:

        //(*Handlers(EnvelopeTestFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton(wxCommandEvent& event);
        //*)

        //(*Identifiers(EnvelopeTestFrame)
        static const long ID_BUTTON1;
        static const long ID_GRAPH;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(EnvelopeTestFrame)
        EnvelopeGraph* m_pGraph;
        wxButton* Button1;
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // ENVELOPETESTMAIN_H
