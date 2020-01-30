#include "envelopegraph.h"

//wxWidgets Event table
BEGIN_EVENT_TABLE(EnvelopeGraph, wxScrolledWindow)
    EVT_PAINT           (EnvelopeGraph::OnPaint)
    EVT_LEFT_DOWN       (EnvelopeGraph::OnMouseLeftDown)
    EVT_LEFT_UP         (EnvelopeGraph::OnMouseLeftUp)
    EVT_LEFT_DCLICK     (EnvelopeGraph::OnMouseLeftDClick)
    EVT_MOTION          (EnvelopeGraph::OnMotion)
    EVT_ENTER_WINDOW    (EnvelopeGraph::OnEnterWindow)
    EVT_LEAVE_WINDOW    (EnvelopeGraph::OnExitWindow)
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
    m_colourLine = *wxGREEN;
    m_colourNode = *wxBLACK;
    SetScrollRate(SCROLL_RATE, SCROLL_RATE);
    GetScrollPixelsPerUnit(&m_nPxScrollX, &m_nPxScrollY);
    m_pLabel = new wxStaticText(this, wxID_ANY, _(""), wxPoint(100,100));
    m_nMinimumY = 0;
    m_nMaximumY = 1000;
    m_ptOrigin = wxPoint(0,0); //!@todo convert to node value rather than coord
    Clear();
}

EnvelopeGraph::~EnvelopeGraph()
{
}

void EnvelopeGraph::FitGraph()
{
    //Assumes vector always has at least one node
    SetVirtualSize(GetNodeCentre(m_vNodes.back()).x, GetNodeCentre(m_vNodes.back()).y);
    Refresh();
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
    if(index == m_vNodes.size())
        FitGraph();
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

unsigned int EnvelopeGraph::GetNodeCount()
{
    return m_vNodes.size();
}

void EnvelopeGraph::SetMaxNodes(unsigned int maxNodes)
{
    if(maxNodes < 1)
        maxNodes = 1;
    if(maxNodes < m_vNodes.size())
        for(unsigned int nNode = m_vNodes.size(); nNode >= maxNodes; --nNode)
            RemoveNode(nNode, false);
    m_nMaxNodes = maxNodes;
    Refresh();
}

unsigned int EnvelopeGraph::GetMaxNodes()
{
    return m_nMaxNodes;
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

void EnvelopeGraph::ScrollToNode(unsigned int nNode)
{
    if(nNode >= m_vNodes.size())
        return;
    int nViewStartX, nViewStartY, bViewWidth, nViewHeight;
    wxPoint ptNode = m_vNodes[nNode];
    GetViewStart(&nViewStartX, &nViewStartY); //scroll units
    GetClientSize(&bViewWidth, &nViewHeight); //pixels
    bViewWidth /= m_nPxScrollX; //scroll units
    nViewHeight /= m_nPxScrollY; //scroll units
    int nNodeX = ptNode.x * m_nScaleX / m_nPxScrollX;  // x positino of node in scroll units
    int nNodeY = ptNode.y * m_nScaleY / m_nPxScrollY; //Y positino of node in scroll units
    if(nNodeX > nViewStartX + bViewWidth)
        nViewStartX = nNodeX - bViewWidth; //node is beyond RHS of view so set to RHS of view
    else if(nNodeX < nViewStartX)
        nViewStartX = nNodeX;
    if(nNodeY > nViewStartY + nViewHeight)
        nViewStartY = nNodeY - nViewHeight;
    else if(nNodeY < nViewStartY)
        nViewStartY = nNodeY;
    else
        return;
    Scroll(nViewStartX, nViewStartY);
    Refresh();
}

void EnvelopeGraph::OnMouseLeftDown(wxMouseEvent &event)
{
    int nX, nY;
    GetViewStart(&nX, &nY); //Scroll units
    nX *= m_nPxScrollX; //Pixels
    nY *= m_nPxScrollY; //Pixels
    wxPoint pointViewStart(nX, nY);
    m_nLastXPos = event.GetPosition().x;
    m_nLastYPos = event.GetPosition().y;
    for(unsigned int nNode = 0; nNode < m_vNodes.size(); ++nNode)
    {
        wxPoint ptNodeCentre(GetNodeCentre(m_vNodes[nNode]));
        if(IsPointInRegion(event.GetPosition() + pointViewStart, ptNodeCentre, m_nNodeRadius))
        {
            if(nNode == 0)
                return; //Don't select first or last node
            m_nDragNode = nNode;
            m_ptClickOffset = ptNodeCentre - event.GetPosition(); //Handle click offset from center of node
            CaptureMouse(); //Handle mouse movement outside window
            break;
        }
    }
}

void EnvelopeGraph::OnMouseLeftUp(wxMouseEvent &event)
{
    if(m_nDragNode == -1)
        return;
    ReleaseMouse();
    FitGraph();
    int nViewStartX, nViewStartY, nViewWidth, nViewHeight;
    GetViewStart(&nViewStartX, &nViewStartY);
    GetClientSize(&nViewWidth, &nViewHeight);

    if(event.GetPosition().x > nViewWidth)
        nViewStartX = (GetNodeCentre(m_vNodes[m_nDragNode]).x - nViewWidth) / m_nPxScrollX + m_nPxScrollX;
    else if(event.GetPosition().x < 0)
        nViewStartX = (GetNodeCentre(m_vNodes[m_nDragNode]).x - nViewWidth) / m_nPxScrollX + m_nPxScrollX;
    if(event.GetPosition().y > nViewHeight)
        nViewStartY = (GetNodeCentre(m_vNodes[m_nDragNode]).y - nViewHeight) / m_nPxScrollY + m_nPxScrollY;
    else if(event.GetPosition().y < 0)
        nViewStartY = (GetNodeCentre(m_vNodes[m_nDragNode]).y - nViewHeight) / m_nPxScrollY + m_nPxScrollY;
//    Scroll(nViewStartX, nViewStartY);
    m_nDragNode = -1;
    Refresh();
}

void EnvelopeGraph::OnMotion(wxMouseEvent &event)
{
    //!@todo Dragging node beyond left hand neigbour when that neigbour is out of view breaks drag offset
    //!@todo Dragging beyond Y coord does not add scrollbars
    //!@todo Set limits of window / Y max
    if(m_nDragNode == -1)
        return;

    //event.GetPosition returns the mouse position within the viewable area, not within the virtual area
    int nViewStartX, nViewStartY;
    GetViewStart(&nViewStartX, &nViewStartY); //Scroll units
    nViewStartX *= m_nPxScrollX; //Pixels
    nViewStartY *= m_nPxScrollY; //Pixels

    //Limit horizontal position to between previous and next nodes
    if(event.GetPosition().x + nViewStartX < GetNodeCentre(m_vNodes[m_nDragNode - 1]).x)
    {
        m_vNodes[m_nDragNode].x = m_vNodes[m_nDragNode - 1].x;
        m_ptClickOffset = wxPoint(0,0);
    }
    else if(m_nDragNode < (int)m_vNodes.size() - 1 && event.GetPosition().x + nViewStartX > GetNodeCentre(m_vNodes[m_nDragNode + 1]).x)
    {
        m_vNodes[m_nDragNode].x = m_vNodes[m_nDragNode + 1].x;
        m_ptClickOffset = wxPoint(0,0);
    }
    else
    {
        m_vNodes[m_nDragNode].x = (GetNodeFromCentre(event.GetPosition() + m_ptClickOffset)).x;
    }
    if(event.ShiftDown())
        m_vNodes[m_nDragNode].y = m_vNodes[m_nDragNode - 1].y;
    else
    {
        if(event.GetPosition().y / m_nScaleY < m_nMinimumY)
            m_vNodes[m_nDragNode].y = m_nMinimumY;
        else if(event.GetPosition().y / m_nScaleY > m_nMaximumY)
            m_vNodes[m_nDragNode].y = m_nMaximumY;
        else
            m_vNodes[m_nDragNode].y = (GetNodeFromCentre(event.GetPosition() + m_ptClickOffset)).y;
    }
    if(event.GetPosition().x > GetClientSize().x + nViewStartX)
    {
        FitGraph();
    }
    else if(event.GetPosition().x < 0)
    {
        FitGraph();
    }
    if(event.GetPosition().y > GetClientSize().y + nViewStartY)
        FitGraph();
    else if(event.GetPosition().y < 0)
        FitGraph();

    m_nLastXPos = event.GetPosition().x;
    m_nLastYPos = event.GetPosition().y;
    Refresh();
}

void EnvelopeGraph::OnMouseLeftDClick(wxMouseEvent &event)
{
//    m_pLabel->SetLabel(wxString::Format(_("DClick @ %d,%d"), event.GetPosition().x, event.GetPosition().y));
    //Try to delete existing node
    int nViewStartX, nViewStartY;
    GetViewStart(&nViewStartX, &nViewStartY);
    wxPoint pointViewStart(nViewStartX * m_nPxScrollX, nViewStartY * m_nPxScrollY);
    for(unsigned int nNode = 0; nNode < m_vNodes.size(); ++nNode)
    {
        if(IsPointInRegion(event.GetPosition() + pointViewStart, GetNodeCentre(m_vNodes[nNode]), m_nNodeRadius))
        {
            RemoveNode(nNode);
            return;
        }
    }
    //Got here so add a node
    if(m_bAllowAddNodes && m_nMaxNodes > m_vNodes.size())
        AddNode(GetNodeFromCentre(event.GetPosition() + pointViewStart));
}

void EnvelopeGraph::OnEnterWindow(wxMouseEvent &event)
{
    if(!event.LeftIsDown())
        m_nDragNode = -1;
    m_ptExtOffset = wxPoint(0, 0);
//    if(m_nDragNode == (int)m_vNodes.size() - 1)
//        FitGraph();//!@todo This causes jump in position which may be undesitable
}

void EnvelopeGraph::OnExitWindow(wxMouseEvent &event)
{
        m_ptExtOffset = wxPoint(0, 0);
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
    wxPoint pointViewStart(nX * m_nPxScrollX, nY * m_nPxScrollY);
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

void EnvelopeGraph::AllowAddNodes(bool enable)
{
    m_bAllowAddNodes = enable;
}

void EnvelopeGraph::AllowRemoveNode(bool enable, wxPoint Node)
{

}

int EnvelopeGraph::GetMaxHeight()
{
    return m_nMaximumY;
}

void EnvelopeGraph::SetMaxHeight(int maxHeight)
{
    m_nMaximumY = maxHeight;
    Refresh();
}

void EnvelopeGraph::SetOrigin(int y)
{
    m_vNodes[0].y = y;
    Refresh();
}
