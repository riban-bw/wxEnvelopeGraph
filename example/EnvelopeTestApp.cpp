/***************************************************************
 * Name:      EnvelopeTestApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2018-09-04
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#include "EnvelopeTestApp.h"

//(*AppHeaders
#include "EnvelopeTestMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(EnvelopeTestApp);

bool EnvelopeTestApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	EnvelopeTestFrame* Frame = new EnvelopeTestFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;
}
