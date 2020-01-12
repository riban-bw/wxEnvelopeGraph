# wxEnvelopeGraph
A wxWidget control that displays draggable nodes connected by straight lines on a scrollable window

This wxWidget control extends wxScrolledWindow to provide a window that may be scrolled, resized, etc. A user may double click to add or remove nodes upto a defined maximum and drag nodes to reposition them. Each node is constrained by its neigbours, i.e. it may not be dragged horizonrally beyond its neigbouring nodes. Nodes may be added or removed programatically.

# Initial Specification
This list is the initial specification. Not all is (will be) implemented.

* A panel that displays a set of straight lines joined by a dot.
* The panel will scale in the X & Y axis as it is resized.
* The maximum quantity of lines / joins (nodes) is programatically configured for the class.
* The minimum and maximum X and Y ranges is programatically configured for the class.
* The graph will scale to fill the panel, i.e. the first implicit node will be at LHS and last (implicit?) node will at RHS.
* Scrollbars may be shown to access wider range. (Must be able to set display limits / scale to calculate how scrolling will manifest.)
* A grid may be displayed.
* The units may be set (used for grid display).
* The divisions of the grid may be set.
* Each node (x,y) may be set programatically.
* A node may be added to the end or inserted at any point (except too close to an existing node) subject to the maximum quantity of nodes.
* Inserting a node moves all subsequent nodes to the right.
* A node may be removed.
* Removing a node moves all subsequent nodes to the left.
* Double clicking on a line will insert a node at that position. (Do we want to allow inserting nodes at any x,y position or just on lines?)
* Double clicking on a node will remove the node.
* Click and drag a node will move it in x & y plane.
* Attempts to drag node beyond the bounds of the panel will limit node position to edge of panel but allow continued movement, i.e. user may drag outside panel area and continue to control the node.
* No part of a node cannot extend beyond the bounds of the panel, i.e. the dot must be completely within the panel - the outer edge of the dot may touch the edge of the panel. This means the line end is constrained to half the diameter of the dot inside the edge of the panel.
* Attempts to drag a node horizontally (x-axis) beyond the preceding or proceeding node will limit node position to after / before the adjacent node, i.e. a user may not drag a node beyond its neighbour.
* An undo history will be maintained which may be invoked programatically.
* An redo history will be maintained which may be invoked programatically.
