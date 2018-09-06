#include "envelopegraph.h"

//wxWidgets Event table
BEGIN_EVENT_TABLE(EnvelopeGraph, wxScrolledWindow)
    EVT_PAINT           (EnvelopeGraph::OnPaint)
    EVT_LEFT_DOWN       (EnvelopeGraph::OnMouseLeftDown)
    EVT_LEFT_UP         (EnvelopeGraph::OnMouseLeftUp)
    EVT_LEFT_DCLICK     (EnvelopeGraph::OnMouseLeftDClick)
    EVT_MOTION          (EnvelopeGraph::OnMotion)
    EVT_ENTER_WINDOW    (EnvelopeGraph::OnEnterWindow)
    EVT_SIZE            (EnvelopeGraph::OnSize)
    EVT_RIGHT_DOWN      (EnvelopeGraph::OnRightDown)
    EVT_RIGHT_UP        (EnvelopeGraph::OnRightUp)
    EVT_RIGHT_DCLICK    (EnvelopeGraph::OnRightDClick)
END_EVENT_TABLE()

EnvelopeGraph::EnvelopeGraph(wxWindow *parent)
: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                   wxHSCROLL | wxVSCROLL | wxNO_FULL_REPAINT_ON_RESIZE)
{
    m_nScaleX = 1;
    m_nScaleY = 1;
    m_nNodeRadius = 5;
    m_nMaxNodes = 6;
    m_nDragNode = -1;
    m_pParent = parent;
    m_colourLine = *wxGREEN;
    m_colourNode = *wxBLACK;
    SetScrollRate(SCROLL_RATE, SCROLL_RATE);
    GetScrollPixelsPerUnit(&m_nPxScrollH, &m_nPxScrollV);
    m_pLabel = new wxStaticText(this, wxID_ANY, _(""), wxPoint(100,100));
    m_nMinimumY = 0;
    m_nMaximumY = 100;
    m_ptOrigin = wxPoint(0,0); //!@todo convert to node value rather than coord
    Clear();
}

EnvelopeGraph::~EnvelopeGraph()
{
}

bool EnvelopeGraph::AddNode(wxPoint node, bool refresh)
{
    //Limit quantity and size of nodes
    if(m_vNodes.size() >= m_nMaxNodes)// || node.x < m_nMinimumY || node.x > m_nMaximumY)
        return false;
    //Insert node at corresponding X position
    for(vector <wxPoint>::iterator it = m_vNodes.begin(); it != m_vNodes.end(); ++it)
    {
        if((*it).x < node.x) //!@todo This seems wrong way round
            continue;
        m_vNodes.insert(it, node);
        if(refresh)
            Refresh();
        return true;
    }
    //Not inserted so add to end
    m_vNodes.push_back(node);
    if(refresh)
        Refresh();
    return true;
}

bool EnvelopeGraph::RemoveNode(unsigned int index, bool refresh)
{
    //Validate index (retain first node)
    if(index == 0 || index >= m_vNodes.size())
        return false;
    m_vNodes.erase(m_vNodes.begin() + index);
    if(refresh)
        Refresh();
    return true;
}

void EnvelopeGraph::Clear(bool refresh)
{
    m_vNodes.clear();
    m_vNodes.push_back(m_ptOrigin);
    if(refresh)
        Refresh();
}

void EnvelopeGraph::DrawGraph(wxDC& dc)
{
    for(unsigned int nNode = 1; nNode < m_vNodes.size(); nNode++)
    {
        //Draw node
        dc.SetBrush(m_colourNode);
        dc.DrawCircle(GetNodeCentre(m_vNodes[nNode]), m_nNodeRadius);
        //Draw lines
        wxPen penGraph(m_colourLine, 1);
        dc.SetPen(penGraph);
        dc.DrawLine(GetNodeCentre(m_vNodes[nNode - 1]), GetNodeCentre(m_vNodes[nNode]));
    }
}

void EnvelopeGraph::OnPaint(wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc(this);
//    dc.SetAxisOrientation(true, true);
//    dc.SetDeviceOrigin( 0, dc.GetSize().GetHeight()-1 );
    PrepareDC(dc);
    dc.Clear();
    DrawGraph(dc);
}

bool EnvelopeGraph::IsPointInRegion(wxPoint point, wxPoint centre, unsigned int radius)
{
    wxRegion region(centre + wxPoint(-radius, radius), centre + wxPoint(radius, -radius));
    return region.Contains(point);
}

void EnvelopeGraph::OnMouseLeftDown(wxMouseEvent &event)
{
    int nX, nY;
    GetViewStart(&nX, &nY); //Scroll units
    nX *= m_nPxScrollH; //Pixels
    nY *= m_nPxScrollV; //Pixels
    wxPoint pointViewStart(nX, nY);
    for(unsigned int nNode = 0; nNode < m_vNodes.size(); ++nNode)
    {
        wxPoint ptNodeCentre(GetNodeCentre(m_vNodes[nNode]));
        if(IsPointInRegion(event.GetPosition() + pointViewStart, ptNodeCentre, m_nNodeRadius))
        {
            if(nNode == 0)
                return; //Don't select first or last node
            m_nDragNode = nNode;
            m_pointClickOffset = ptNodeCentre - event.GetPosition(); //Handle click offset from center of node
            CaptureMouse(); //Handle mouse movement outside window
            break;
        }
    }
}

void EnvelopeGraph::OnMouseLeftUp(wxMouseEvent &event)
{
    if(m_nDragNode == -1)
        return;
    m_nDragNode = -1;
    ReleaseMouse();
    Refresh();
}

wxPoint EnvelopeGraph::GetNodeCentre(wxPoint ptNode)
{
    wxPoint ptCentre(ptNode.x * m_nScaleX, ptNode.y * m_nScaleY);
    return ptCentre;
}

wxPoint EnvelopeGraph::GetNodeFromCentre(wxPoint ptPos)
{
    wxPoint ptNode(ptPos.x / m_nScaleX, ptPos.y / m_nScaleY);
    return ptNode;
}

void EnvelopeGraph::OnMotion(wxMouseEvent &event)
{
    //!@todo Reduce flicker on drag
    if(m_nDragNode == -1)
        return;

    //event.GetPosition returns the mouse position within the viewable area, not within the virtual area
    int nX, nY, nXsu, nYsu;
    GetViewStart(&nXsu, &nYsu); //Scroll units
    nX = m_nPxScrollH * nXsu; //Pixels
    nY = m_nPxScrollV * nYsu; //Pixels
    wxPoint pointViewStart(nX, nY);

    //Limit horizontal position to between previous and next nodes
    if(event.GetPosition().x + nX >= GetNodeCentre(m_vNodes[m_nDragNode - 1]).x && (m_nDragNode == (int)m_vNodes.size() - 1 || event.GetPosition().x + nX <= GetNodeCentre(m_vNodes[m_nDragNode + 1]).x))
    {
        wxSize sizeClient = GetClientSize();//Pixels
        if(event.GetPosition().x > sizeClient.x - SCROLL_RATE * 2)
        {
            if(m_nDragNode == (int)m_vNodes.size() - 1)
                SetVirtualSize(GetVirtualSize() + wxSize(SCROLL_RATE, 0));
            Scroll(nXsu + 1, -1);
            m_pointClickOffset.x += m_nPxScrollH;
        }
        else if(event.GetPosition().x < SCROLL_RATE * 2)
        {
            Scroll(nXsu - 1, -1);
            m_pointClickOffset.x -= m_nPxScrollH;
        }
        //!@todo constrain x value
        m_vNodes[m_nDragNode] = GetNodeFromCentre(event.GetPosition()  + m_pointClickOffset);
    }
    Refresh();
}

void EnvelopeGraph::OnMouseLeftDClick(wxMouseEvent &event)
{
//    m_pLabel->SetLabel(wxString::Format(_("DClick @ %d,%d"), event.GetPosition().x, event.GetPosition().y));
    //Try to delete existing node
    int nX, nY;
    GetViewStart(&nX, &nY);
    wxPoint pointViewStart(nX * m_nPxScrollH, nY * m_nPxScrollV);
    for(unsigned int nNode = 0; nNode < m_vNodes.size(); ++nNode)
    {
        if(IsPointInRegion(event.GetPosition() + pointViewStart, GetNodeCentre(m_vNodes[nNode]), m_nNodeRadius))
        {
            RemoveNode(nNode);
            return;
        }
    }
    //Got here so add a node
    if(m_nMaxNodes > m_vNodes.size())
        AddNode(GetNodeFromCentre(event.GetPosition() + pointViewStart));
}

void EnvelopeGraph::OnEnterWindow(wxMouseEvent &event)
{
    if(!event.LeftIsDown())
        m_nDragNode = -1;
}

void EnvelopeGraph::OnSize(wxSizeEvent &event)
{
    int nX(0), nY(0);
    for(unsigned int nNode = 0; nNode < m_vNodes.size(); ++nNode)
    {
        if(GetNodeCentre(m_vNodes[nNode]).x > nX)
            nX = m_vNodes[nNode].x;
        if(GetNodeCentre(m_vNodes[nNode]).y > nY)
            nY = m_vNodes[nNode].y;
    }
    SetVirtualSize(wxSize(nX, nY));
    Refresh();
}

void EnvelopeGraph::OnRightDown(wxMouseEvent &event)
{

}

void EnvelopeGraph::OnRightUp(wxMouseEvent &event)
{
    int nX, nY;
    GetViewStart(&nX, &nY);
    wxPoint pointViewStart(nX * m_nPxScrollH, nY * m_nPxScrollV);
    for(unsigned int nNode = 0; nNode < m_vNodes.size(); ++nNode)
    {
        if(IsPointInRegion(event.GetPosition() + pointViewStart, GetNodeCentre(m_vNodes[nNode]), m_nNodeRadius))
        {
            wxMessageBox(wxString::Format(_("Node %d\nX: %d\nY: %d"), nNode, m_vNodes[nNode].x, m_vNodes[nNode].y));
            return;
        }
    }
}

void EnvelopeGraph::OnRightDClick(wxMouseEvent &event)
{
    Clear();
}
