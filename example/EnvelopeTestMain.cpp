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
const long EnvelopeTestFrame::ID_CHECKBOX1 = wxNewId();
const long EnvelopeTestFrame::ID_STATICTEXT1 = wxNewId();
const long EnvelopeTestFrame::ID_SPINCTRL1 = wxNewId();
const long EnvelopeTestFrame::ID_STATICTEXT2 = wxNewId();
const long EnvelopeTestFrame::ID_SPINCTRL2 = wxNewId();
const long EnvelopeTestFrame::ID_BUTTON1 = wxNewId();
const long EnvelopeTestFrame::ID_SLIDER1 = wxNewId();
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
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, id, _("wxEnvelopeGraph Example"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    SetClientSize(wxSize(600,400));
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer1->AddGrowableCol(0);
    FlexGridSizer1->AddGrowableRow(1);
    FlexGridSizer2 = new wxFlexGridSizer(0, 0, 0, 0);
    m_pCmbAllowAdd = new wxCheckBox(this, ID_CHECKBOX1, _("Allow add node"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    m_pCmbAllowAdd->SetValue(true);
    FlexGridSizer2->Add(m_pCmbAllowAdd, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Max nodes"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT1"));
    FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    m_pSpnMaxNodes = new wxSpinCtrl(this, ID_SPINCTRL1, _T("6"), wxDefaultPosition, wxDefaultSize, 0, 0, 10, 6, _T("ID_SPINCTRL1"));
    m_pSpnMaxNodes->SetValue(_T("6"));
    FlexGridSizer2->Add(m_pSpnMaxNodes, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Max height"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT2"));
    FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    m_pSpnMaxHeight = new wxSpinCtrl(this, ID_SPINCTRL2, _T("100"), wxDefaultPosition, wxDefaultSize, 0, 0, 4096, 100, _T("ID_SPINCTRL2"));
    m_pSpnMaxHeight->SetValue(_T("100"));
    FlexGridSizer2->Add(m_pSpnMaxHeight, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button1 = new wxButton(this, ID_BUTTON1, _("Info"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    FlexGridSizer2->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer3 = new wxFlexGridSizer(0, 0, 0, 0);
    FlexGridSizer3->AddGrowableCol(1);
    FlexGridSizer3->AddGrowableRow(0);
    Slider1 = new wxSlider(this, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL, wxDefaultValidator, _T("ID_SLIDER1"));
    FlexGridSizer3->Add(Slider1, 1, wxALL|wxEXPAND, 5);
    m_pGraph = new EnvelopeGraph(this);
    FlexGridSizer3->Add(m_pGraph, 1, wxALL|wxEXPAND, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 5);
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
    SetSizer(FlexGridSizer1);
    Layout();
    Center();

    Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&EnvelopeTestFrame::Onm_pCmbAllowAddClick);
    Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&EnvelopeTestFrame::Onm_pSpnMaxNodesChange);
    Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&EnvelopeTestFrame::Onm_pSpnMaxHeightChange);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EnvelopeTestFrame::OnButton);
    Connect(ID_SLIDER1,wxEVT_SCROLL_CHANGED,(wxObjectEventFunction)&EnvelopeTestFrame::OnSlider1CmdScrollChanged);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EnvelopeTestFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&EnvelopeTestFrame::OnAbout);
    //*)
    m_pSpnMaxHeight->SetValue(m_pGraph->GetMaxHeight());
    m_pGraph->AddNode(wxPoint(200,30));

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
    wxString sMessage = wxString::Format("Node count: %d\nMaximum nodes: %d", m_pGraph->GetNodeCount(),
                                         m_pGraph->GetMaxNodes());
    wxMessageBox(sMessage);

}

void EnvelopeTestFrame::Onm_pCmbAllowAddClick(wxCommandEvent& event)
{
    m_pGraph->AllowAddNodes(event.IsChecked());
}

void EnvelopeTestFrame::Onm_pSpnMaxNodesChange(wxSpinEvent& event)
{
    m_pGraph->SetMaxNodes(event.GetValue() + 1);
}

void EnvelopeTestFrame::Onm_pSpnMaxHeightChange(wxSpinEvent& event)
{
    m_pGraph->SetMaxHeight(event.GetValue());
}

void EnvelopeTestFrame::OnSlider1CmdScrollChanged(wxScrollEvent& event)
{
    m_pGraph->SetOrigin(event.GetPosition() * m_pGraph->GetMaxHeight() / 200);
    m_pGraph->Refresh();
}
