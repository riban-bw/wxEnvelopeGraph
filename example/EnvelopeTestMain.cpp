/***************************************************************
 * Name:      EnvelopeTestMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2018-09-04
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#include "EnvelopeTestMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(EnvelopeTestFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(EnvelopeTestFrame)
const long EnvelopeTestFrame::ID_BUTTON1 = wxNewId();
const long EnvelopeTestFrame::ID_GRAPH = wxNewId();
const long EnvelopeTestFrame::idMenuQuit = wxNewId();
const long EnvelopeTestFrame::idMenuAbout = wxNewId();
const long EnvelopeTestFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(EnvelopeTestFrame,wxFrame)
    //(*EventTable(EnvelopeTestFrame)
    //*)
END_EVENT_TABLE()

EnvelopeTestFrame::EnvelopeTestFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(EnvelopeTestFrame)
    wxFlexGridSizer* FlexGridSizer1;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer1->AddGrowableCol(0);
    FlexGridSizer1->AddGrowableRow(1);
    Button1 = new wxButton(this, ID_BUTTON1, _("Label"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer1->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_pGraph = new EnvelopeGraph(this);
    FlexGridSizer1->Add(m_pGraph, 1, wxALL|wxEXPAND, 5);
    SetSizer(FlexGridSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    FlexGridSizer1->Fit(this);
    FlexGridSizer1->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EnvelopeTestFrame::OnButton);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EnvelopeTestFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EnvelopeTestFrame::OnAbout);
    //*)
//    m_pGraph->AddNode(wxPoint(0,0));
//    m_pGraph->AddNode(wxPoint(100,0));
//    m_pGraph->AddNode(wxPoint(50,50));
    m_pGraph->AddNode(wxPoint(200,30));
//    m_pGraph->AddNode(wxPoint(120,70));
//    m_pGraph->AddNode(wxPoint(10,90));
//    m_pGraph->AddNode(wxPoint(50,30));
    Maximize();
}

EnvelopeTestFrame::~EnvelopeTestFrame()
{
    //(*Destroy(EnvelopeTestFrame)
    //*)
}

void EnvelopeTestFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void EnvelopeTestFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void EnvelopeTestFrame::OnButton(wxCommandEvent& event)
{
    wxMessageBox(_("Hello"));
}
