/*
 * Created on Dec 21, 2004
 * NEED TO ADD DEMANDS AND DEAL WITH SUBSTITUTE CORRECTLY IN THE DETAIL
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

package witviz;

import org.eclipse.draw2d.PolylineConnection;
import org.eclipse.draw2d.ChopboxAnchor;
import org.eclipse.draw2d.AutomaticRouter;
import org.eclipse.draw2d.FanRouter;


import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Stack;
import java.util.Vector;
import org.eclipse.swt.custom.*;
import org.eclipse.draw2d.AbsoluteBendpoint;
import org.eclipse.draw2d.BendpointConnectionRouter;
import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.Graphics;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.Label;
import org.eclipse.draw2d.LineBorder;
import org.eclipse.draw2d.MouseEvent;
import org.eclipse.draw2d.graph.*;
import org.eclipse.draw2d.MouseListener;
import org.eclipse.draw2d.MouseMotionListener;
import org.eclipse.draw2d.Panel;
import org.eclipse.draw2d.PolygonDecoration;
import org.eclipse.draw2d.XYLayout;
import org.eclipse.draw2d.geometry.Dimension;
import org.eclipse.draw2d.geometry.Rectangle;
import org.eclipse.draw2d.graph.DirectedGraph;
import org.eclipse.draw2d.graph.Edge;
import org.eclipse.draw2d.graph.EdgeList;
import org.eclipse.draw2d.graph.Node;
import org.eclipse.draw2d.graph.NodeList;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.ScrollBar;
import org.eclipse.swt.SWT;

import witviz.SimpleTree.BottomAnchor;
// import witviz.SimpleTree.Selector;
import witviz.SimpleTree.TopAnchor;
import org.eclipse.draw2d.FigureCanvas;



/**
 * @author gresh
 * 
 * TODO To change the template for this generated type comment go to Window -
 * Preferences - Java - Code Style - Code Templates
 */
public class CreateTreeFigure extends Panel {
	Hashtable hNodes = new Hashtable();

	Hashtable hEdges = new Hashtable();

	StyledText TextBox;

	EdgeList el;

	NodeList nl;

	WitObject wo;

	FigureCanvas myCanvas;

	Vector alldemands;

	NodeList allNodes;

	EdgeList allEdges;

	boolean isMaster;

	SimpleTree parent;

	// String SelectedType, SelectedName;
	Object SelectedObject;

	Color childColor = new Color(Display.getDefault(), 220, 220, 250);

	Color parentColor = new Color(Display.getDefault(), 160, 160, 220);

	Color selfColor = new Color(Display.getDefault(), 250, 200, 200);

	Color arcSelfColor = new Color(Display.getDefault(), 250, 100, 100);

	Color arcChildColor = new Color(Display.getDefault(), 140, 140, 220);

	Color arcParentColor = new Color(Display.getDefault(), 60, 60, 220);

	Color subsParentColor = new Color(Display.getDefault(), 220, 220, 100);

	Color subsChildColor = new Color(Display.getDefault(), 250, 250, 200);

	Color arcDemandColor = new Color(Display.getDefault(), 50, 180, 50);

	Color demandColor = new Color(Display.getDefault(), 200, 250, 200);

	Color expandedColor = new Color(Display.getDefault(), 0, 0, 100);

	int nodewidth = 90;

	int nodeheight = 40;

	AutomaticRouter router;

	// XYLayout xylayout;
	DirectedGraph dg;

	static Image partIcon=null, opIcon=null, capacityIcon=null, demandIcon=null;

	public CreateTreeFigure() {
		// empty object
	}

	public CreateTreeFigure(WitObject witobject, boolean im, SimpleTree p,
			FigureCanvas c, int nw, int nh) {
		init();		
		setNodeWidth(nw);
		setNodeHeight(nh);
		isMaster = im;
		myCanvas = c;
		parent = p;
		wo = witobject;
		router = new FanRouter();
		((FanRouter) router).setSeparation(10);
		router.setNextRouter(new BendpointConnectionRouter());

		if (wo == null)
			return;
		dg = CreateWitTree();
		// dg = CreateTestTree();
		buildGraph(dg);

	}

	

	public void init() {
		if (partIcon == null) {
			partIcon = new Image(null, "images/part.gif");
			opIcon = new Image(null, "images/operation.gif");
			capacityIcon = new Image(null, "images/capacity.gif");
			demandIcon = new Image(null, "images/demand.gif");
			//SVGGraphics.registerImage(partIcon,"images/part.gif");
			//SVGGraphics.registerImage(opIcon,"images/operation.gif");
			//SVGGraphics.registerImage(capacityIcon,"images/capacity.gif");
			//SVGGraphics.registerImage(demandIcon,"images/demand.gif");
		}
	}



	public void AddTestNode(Node node) {
		Label label = (Label) node.data;
		label.setOpaque(true);
		label.setBorder(new LineBorder());

		this.add(label, new Rectangle(node.x, node.y, node.width, node.height));
	}

	public void AddTestEdge(Edge edge) {
		PolylineConnection conn = new PolylineConnection();
		conn.setForegroundColor(ColorConstants.gray);
		PolygonDecoration dec = new PolygonDecoration();
		conn.setSourceDecoration(dec);
		Node s = edge.source;
		Node t = edge.target;
		conn.setSourceAnchor(new ChopboxAnchor((Figure) s.data));
		conn.setTargetAnchor(new ChopboxAnchor((Figure) t.data));
		conn.setConnectionRouter(router);
		NodeList nodes = edge.vNodes;
		List bends = new ArrayList();
		if (nodes != null) {
			for (int ii = 0; ii < nodes.size(); ii++) {
				Node vn = nodes.getNode(ii);
				int x = vn.x;
				int y = vn.y;
				if (edge.isFeedback) {
					bends.add(new AbsoluteBendpoint(x, y + vn.height));
					bends.add(new AbsoluteBendpoint(x, y));
				} else {
					bends.add(new AbsoluteBendpoint(x, y));
					bends.add(new AbsoluteBendpoint(x, y + vn.height));
				}

			}
		}
		conn.setRoutingConstraint(bends);

		this.add(conn);

	}

	public CreateTreeFigure(WitObject witobject, Object focusObject,
			SimpleTree p, FigureCanvas c, int nw, int nh) {
		init();		
		setNodeWidth(nw);
		setNodeHeight(nh);

		isMaster = false;
		parent = p;
		myCanvas = c;
		wo = witobject;
		if (wo == null)
			return;
		router = new FanRouter();
		((FanRouter) router).setSeparation(10);
		router.setNextRouter(new BendpointConnectionRouter());

		dg = CreateWitDetailTree(wo, focusObject);
		buildGraph(dg);

	}

	public CreateTreeFigure(WitObject witobject, Object focusObject,
			SimpleTree p, FigureCanvas c) {
		init();		
		isMaster = false;
		parent = p;
		myCanvas = c;
		wo = witobject;
		if (wo == null)
			return;
		router = new FanRouter();
		((FanRouter) router).setSeparation(10);
		router.setNextRouter(new BendpointConnectionRouter());

		dg = CreateWitDetailTree(wo, focusObject);
		buildGraph(dg);

	}

	public CreateTreeFigure getExpandedDetailContents(Figure f) {
		// check the class of the object; either a WitNode or a WitEdge
		if ((f.getClass().toString().equals("class witviz.WitNode"))
				|| (f.getClass().toString()
						.equals("class witviz.WitNodeInvisible"))
				|| (f.getClass().toString()
						.equals("class witviz.WitDemandNode"))) {
			// SelectedName = ((WitNode) f).getText();
			// SelectedType = ((WitNode) f).getNodeType();
			SelectedObject = ((WitNode) f);
		} else {
			// electedName = ((WitEdge) f).getText();
			// SelectedType = ((WitEdge) f).getEdgeType();
			SelectedObject = (WitEdge) f;
		}
		return getExpandedDetailContents(SelectedObject);
	}

	public CreateTreeFigure getExpandedDetailContents(Object focusObject) {
		String type, name;
		if (focusObject.getClass().toString().equals("class witviz.WitNode")
				|| focusObject.getClass().toString().equals(
						"class witviz.WitDemandNode")) {
			type = ((WitNode) focusObject).getNodeType();
			name = ((WitNode) focusObject).getText();
			Expand(type, name);

			buildGraph(dg);
			this.layout();
			parent.unsetWait();
			return this;
		}

		return this;

	}

	public void setTextBox(StyledText tb) {
		TextBox = tb;
		tb.setText("");
	}

	public void buildGraph(DirectedGraph graph) {
		// Figure contents = new Panel();

		// contents.setFont(new Font(null, "Tahoma", 10, 0));
		this.setBackgroundColor(ColorConstants.white);
		this.setLayoutManager(new XYLayout());
		// this.setLayoutManager(xylayout);
		for (int i = 0; i < graph.nodes.size(); i++) {
			Node node = graph.nodes.getNode(i);
			buildNodeFigure(this, node);
		}

		for (int i = 0; i < graph.edges.size(); i++) {
			ExtendedEdge edge = (ExtendedEdge) graph.edges.getEdge(i);
			buildEdgesFigure(this, edge);
		}

		// if (buildPrime)
		// / buildPrimeGraph(graph.gPrime, contents);

	}

	public void buildTestGraph(DirectedGraph graph) {

		this.setBackgroundColor(ColorConstants.white);
		this.setLayoutManager(new XYLayout());

		for (int i = 0; i < graph.nodes.size(); i++) {
			Node node = graph.nodes.getNode(i);
			Label label = (Label) node.data;
			label.setOpaque(true);
			label.setBorder(new LineBorder());

			this.add(label, new Rectangle(node.x, node.y, node.width,
					node.height));
		}

		for (int i = 0; i < graph.edges.size(); i++) {
			Edge edge = (Edge) graph.edges.getEdge(i);
			PolylineConnection conn = new PolylineConnection();
			conn.setForegroundColor(ColorConstants.gray);
			PolygonDecoration dec = new PolygonDecoration();
			conn.setSourceDecoration(dec);
			Node s = edge.source;
			Node t = edge.target;
			// conn.setSourceAnchor(new BottomAnchor((Figure) s.data));
			conn.setSourceAnchor(new ChopboxAnchor((Figure) s.data));
			// conn.setTargetAnchor(new TopAnchor((Figure) t.data));
			conn.setTargetAnchor(new ChopboxAnchor((Figure) t.data));
			conn.setConnectionRouter(router);
			NodeList nodes = edge.vNodes;
			List bends = new ArrayList();
			if (nodes != null) {
				for (int ii = 0; ii < nodes.size(); ii++) {
					Node vn = nodes.getNode(ii);
					int x = vn.x;
					int y = vn.y;
					if (edge.isFeedback) {
						bends.add(new AbsoluteBendpoint(x, y + vn.height));
						bends.add(new AbsoluteBendpoint(x, y));
					} else {
						bends.add(new AbsoluteBendpoint(x, y));
						bends.add(new AbsoluteBendpoint(x, y + vn.height));
					}

				}
			}
			conn.setRoutingConstraint(bends);

			this.add(conn);
		}
		// this.layout();

	}

	void buildNodeFigure(Figure contents, Node node) {
		WitNode label = null;
		if (node.data instanceof WitNode) {
			label = (WitNode) node.data;
		} else {
			label = new WitNode();
			node.data = label;
			label.setNodeType("operation");
		}
		label.setBackgroundColor(ColorConstants.white);
		label.bottomAnchor = new BottomAnchor(label);
		label.topAnchor = new TopAnchor(label);
		label.setOpaque(true);
		// label.setBorder(new LineBorder());

		String text = node.data.toString();// + "(" + node.index
		// +","+node.sortValue+ ")";

		if (label.getNodeType().equals("operation"))
			label.setIcon(opIcon);
		else if (label.getNodeType().equals("MATERIAL"))
			label.setIcon(partIcon);
		else if (label.getNodeType().equals("CAPACITY"))
			label.setIcon(capacityIcon);
		else
			label.setIcon(demandIcon);

		// new Selector((Figure) node.data);

		contents.add(label, new Rectangle(node.x, node.y, node.width,
				node.height));
	}

	WitEdge connection(ExtendedEdge e) {
		// String key;
		// key = ((WitEdge)e.data).createKey();
		// get the edge out of the hashtable
		// ExtendedEdge ee = (ExtendedEdge) hEdges.get(key);
		WitEdge conn = (WitEdge) e.data;
		return conn;
	}

	private void buildEdgesFigure(Figure contents, ExtendedEdge edge) {
		WitEdge conn = connection(edge);
		conn.setForegroundColor(ColorConstants.gray);
		PolygonDecoration dec = new PolygonDecoration();
		conn.setSourceDecoration(dec);
		Node s = edge.source;
		Node t = edge.target;

		conn.setSourceAnchor(((WitNode) s.data).bottomAnchor);
		conn.setTargetAnchor(((WitNode) t.data).topAnchor);

		// add a lot more little bends if it is a feedback edge
		List bends = new ArrayList();
		NodeList nodes = edge.vNodes;
		conn.setConnectionRouter(router);

		if (edge.isFeedback) {
			// conn.setConnectionRouter(new BendpointConnectionRouter());
			// for this connection we go bottom to top
			Node feedbacksource = edge.source;
			int x = feedbacksource.x + feedbacksource.width / 2;
			int y = feedbacksource.y + feedbacksource.height;
			bends.add(new AbsoluteBendpoint(x, y + 10)); // down
			bends.add(new AbsoluteBendpoint(x + feedbacksource.width / 2 + 5,
					y + 10)); // over half the width
			bends.add(new AbsoluteBendpoint(x + feedbacksource.width / 2 + 5, y
					- feedbacksource.height - 5)); // up
			if (nodes != null)
				bends.add(new AbsoluteBendpoint(x, y - feedbacksource.height
						- 5)); // back over only if vnodes exists
		}
		if (nodes != null) {
			// conn.setConnectionRouter(new ManhattanConnectionRouter());
			for (int i = 0; i < nodes.size(); i++) {
				Node vn = nodes.getNode(i);
				int x = vn.x;
				int y = vn.y;
				if (edge.isFeedback) {
					bends.add(new AbsoluteBendpoint(x, y + vn.height));
					bends.add(new AbsoluteBendpoint(x, y));
				} else {
					bends.add(new AbsoluteBendpoint(x, y));
					bends.add(new AbsoluteBendpoint(x, y + vn.height));
				}
			}

		}
		if (edge.isFeedback) {
			// for this connection we go bottom to top
			Node feedbacktarget = edge.target;
			int x = feedbacktarget.x + feedbacktarget.width / 2;
			int y = feedbacktarget.y + feedbacktarget.height;
			if (nodes != null)
				bends.add(new AbsoluteBendpoint(x, y + 5)); // vicinity of
			// target
			bends.add(new AbsoluteBendpoint(x + feedbacktarget.width / 2 + 5,
					y + 5)); // over half the width
			bends.add(new AbsoluteBendpoint(x + feedbacktarget.width / 2 + 5, y
					- feedbacktarget.height - 5)); // up
			bends.add(new AbsoluteBendpoint(x, y - feedbacktarget.height - 5)); // back
			// over

		}
		conn.setRoutingConstraint(bends);
		// new Selector((Figure) conn);

		contents.add(conn);
	}

	void ConnectThemAll() {
		// connect all rawmaterials to a fictitious part
		// wni.setPreferredSize(new Dimension(1, 1));
		// wni.setMaximumSize(new Dimension(1, 1));
		// String nodekey = "dummypart";
		// hNodes.put(nodekey, n1);
		Vector rawmaterials = wo.getRawMaterialsList();
		int num1 = rawmaterials.size();
		Node n, n1 = null, n2 = null;
		if (num1 > 0) {
			n1 = addNode("dummypart", "dummy");
			n1.width = 10;
			n1.height = 10;
		}
		// nl.add(n1);

		for (int i = 0; i < num1; i++) {
			WitBomEdge e = new WitBomEdge();
			e.setVisible(false);
			ListInfo li = (ListInfo) rawmaterials.elementAt(i);
			WitNode wn = new WitNode();

			String key = WitNode.createKey(li.type, li.name);
			n = (Node) hNodes.get(key);
			ExtendedEdge trueedge = new ExtendedEdge(n1, n, -1);
			e.setOperation((WitNode) n1.data);
			e.setPart((WitNode) n.data);
			e.setBomEntry(0);
			trueedge.setData(e);
			trueedge.setType("dummy");
			// String edgekey = trueedge.name;
			key = e.createKey();
			e.setText(key);
			hEdges.put(key, trueedge);
			// new Selector((Figure) e);

			// trueedge.data = e;
			// new Selector((Figure) e);

			el.add(trueedge);

		}
		// connect all rawoperations to a fictitious part

		Vector rawoperations = wo.getRawOperationsList();
		int num2 = rawoperations.size();

		if (num2 > 0) {
			n2 = addNode("dummyoperation", "dummy");
			n2.width = 10;
			n2.height = 10;
		}

		for (int i = 0; i < num2; i++) {
			WitBomEdge e = new WitBomEdge();
			e.setVisible(false);
			e.setOperation((WitNode) n2.data);

			ListInfo li = (ListInfo) rawoperations.elementAt(i);
			String key = WitNode.createKey("operation", li.name);
			n = (Node) hNodes.get(key);
			e.setPart((WitNode) n.data);
			ExtendedEdge trueedge = new ExtendedEdge(n2, n, -1);
			trueedge.setData(e);

			new Selector((Figure) e);
			String edgekey = trueedge.name;
			hEdges.put(edgekey, trueedge);

			trueedge.setData(e);

			el.add(trueedge);
		}
		// now connect the dummy part to the dummy operation
		if (num1 > 0 && num2 > 0) {
			WitBomEdge e = new WitBomEdge();
			e.setVisible(false);
			ExtendedEdge trueedge = new ExtendedEdge(n2, n1, -1);
			e.setOperation((WitNode) n2.data);
			e.setPart((WitNode) n1.data);
			e.setBomEntry(-1);
			trueedge.setData(e);
			String edgekey = trueedge.name;
			new Selector((Figure) e);
			hEdges.put(edgekey, trueedge);

			trueedge.setData(e);

			el.add(trueedge);
		}
		// connect all demands to a fictitious part

		int num3 = alldemands.size();
		Node n3 = null;
		if (num3 > 0) {
			n3 = addNode("dummydemand", "dummy");
			n3.width = 10;
			n3.height = 10;
		}

		for (int i = 0; i < num3; i++) {
			WitBomEdge e = new WitBomEdge();
			e.setVisible(false);
			String demandname = (String) alldemands.elementAt(i);
			// String key = "demand" + demandname; //foo1
			String key = WitDemandNode.createKey("demand", demandname);
			n = (Node) hNodes.get(key);
			ExtendedEdge trueedge = new ExtendedEdge(n3, n, -1);

			e.setOperation((WitNode) n3.data);
			e.setPart((WitNode) n.data);
			e.setBomEntry(-1);
			trueedge.setData(e);

			trueedge.setType("dummy");
			String edgekey = trueedge.name;
			new Selector((Figure) e);
			hEdges.put(edgekey, trueedge);

			trueedge.setData(e);

			el.add(trueedge);
		}

	}

	public void addPartParentEdge(String partName, String partType, ListInfo p,
			int index) {
		String operationName = p.name;
		String operationType = p.type;

		Vector substitutes = wo.getBomArcSubstitutes(operationName, index);

		// get the nodes for part and operation. We'll need them
		String key2 = WitNode.createKey(partType, partName);
		String key1 = WitNode.createKey(operationType, operationName);
		Node nOperation = (Node) hNodes.get(key1);
		Node nPart = (Node) hNodes.get(key2);

		// if there are no substitutes
		if (substitutes.size() == 0) {
			// we just make a regular edge
			String key = WitBomEdge.createKey(operationName, partName, index);
			if (hEdges.get(key) == null) {
				ExtendedEdge trueedge = new ExtendedEdge(nOperation, nPart,
						index);
				WitBomEdge wbe = new WitBomEdge();

				wbe.setPart((WitNode) nPart.data);
				wbe.setOperation((WitNode) nOperation.data);
				wbe.setBomEntry(index);
				new Selector((Figure) wbe);

				trueedge.setData(wbe);
				hEdges.put(key, trueedge);
				el.add(trueedge);
			}
		}

		else {
			// there are substitutes
			Node nInvis;
			for (int jj = 0; jj < substitutes.size(); jj++) {
				ListInfo li = (ListInfo) substitutes.elementAt(jj);
				int bomIndexSub = li.index;
				WitNodeInvisible wni = new WitNodeInvisible();

				wni.setPart((WitNode) nPart.data);
				wni.setOperation((WitNode) nOperation.data);
				wni.setBomEntry(index);

				String invisibleNodeName = wni.createKey();
				//if (!hNodes.containsKey(invisibleNodeName)) {
					nInvis = addInvisibleNode(invisibleNodeName, wni);
					nInvis.width = 1;
					nInvis.height = 1;
	
					if (jj == 0) {
						// create an invisible node between the source and
						// target
						// make two edges instead of one for the primary
						// edge
						ExtendedEdge trueedge1 = new ExtendedEdge(nOperation,
								nInvis, index);
						trueedge1.setType("bom");
						WitBomUpperFragmentEdge wbu = new WitBomUpperFragmentEdge();
						WitBomLowerFragmentEdge wbl = new WitBomLowerFragmentEdge();
						wbu.setOperation((WitNode) nOperation.data);
						wbu.setInvisible((WitNodeInvisible) nInvis.data);
						wbu.setBomEntry(index);
						wbl.setInvisible((WitNodeInvisible) nInvis.data);
						wbl.setPart((WitNode) nPart.data);
						wbl.setBomEntry(index);
	
						ExtendedEdge trueedge2 = new ExtendedEdge(nInvis, nPart,
								index);
						trueedge2.setType("bom");
	
						trueedge1.setData(wbu);
						trueedge2.setData(wbl);
	
						new Selector((Figure) wbu);
						new Selector((Figure) wbl);
						if (hEdges.get(wbu.createKey()) == null) {
							hEdges.put(wbu.createKey(), trueedge1);
							hEdges.put(wbl.createKey(), trueedge2);
	
							el.add(trueedge1);
							el.add(trueedge2);
						}
					}
					// add the edge from the invisible node to the substitute
					// part
					int subsIndex = jj;
					String subsKey = WitNode.createKey(li.type, li.name);
					Node nSubs = (Node) hNodes.get(subsKey);
					if (nSubs == null) {
						// we need to add it; this is necessary for the detail view
						// for example
						nSubs = addNode(li.name, li.type);
					}
					WitNode wn = ((WitNode) nSubs.data);
	
					ExtendedEdge trueedgeSubs = new ExtendedEdge(nInvis, nSubs,
							subsIndex);
					trueedgeSubs.setType("substitute");
	
					WitSubstituteEdge wse = new WitSubstituteEdge();
					wse.setLineStyle(Graphics.LINE_DOT);
					wse.setDefaultWidth(1); // these are thinner
					wse.setInvisibleNode((WitNodeInvisible) nInvis.data);
					wse.setPart((WitNode) nSubs.data);
					wse.setSubsEntry(subsIndex);
					trueedgeSubs.setData(wse);
					if (hEdges.get(wse.createKey()) == null) {
						el.add(trueedgeSubs);
						new Selector((Figure) wse);
						hEdges.put(wse.createKey(), trueedgeSubs);
					}
				//}
			}
		} // end of substitutes
	}

	public void addPartChildEdge(String partName, String partType, ListInfo k,
			int index) {
		String kidname = k.name;
		String kidtype = k.type;

		// see if the child is already in the table
		String key = WitBopEdge.createKey(k.name, partName, index);
		ExtendedEdge ee = (ExtendedEdge) hEdges.get(key);
		if (ee == null) {
			// put it in the hashtable
			String key1, key2;

			key1 = WitNode.createKey(partType, partName);
			key2 = WitNode.createKey(k.type, kidname);
			Node n1 = (Node) hNodes.get(key1);
			Node n2 = (Node) hNodes.get(key2);
			ExtendedEdge trueedge = new ExtendedEdge(n1, n2, index);
			WitBopEdge wbe = new WitBopEdge();
			wbe.setPart((WitNode) n1.data);
			wbe.setOperation((WitNode) n2.data);
			wbe.setBopEntry(index);
			new Selector((Figure) wbe);
			key = wbe.createKey();
			trueedge.setData(wbe);
			hEdges.put(key, trueedge);
			el.add(trueedge);
		}
	}

	public void setNodeWidth(int width) {
		nodewidth = width;
	}

	public void setNodeHeight(int height) {
		nodeheight = height;
	}

	public Node addNode(String thisName, String thisType) {
		WitNode wni = new WitNode();
		String key;

		wni.setText(thisName);
		wni.setNodeType(thisType);
		key = wni.createKey();

		wni.setNodeType(thisType);

		Node n;
		n = (Node) hNodes.get(key);
		if (n == null) {
			n = new Node(wni);

			n.width = nodewidth;
			n.height = nodeheight;
			nl.add(n);
			new Selector((Figure) wni);
			hNodes.put(key, n);
		}
		return n;

	}

	public Node addDemandNode(String demand, String part) {
		WitDemandNode wdn = new WitDemandNode();
		String key;

		wdn.setNodeType("demand");
		wdn.setPartName(part);
		wdn.setDemandName(demand);

		key = wdn.createKey();
		wdn.setText(key);

		Node n;
		n = (Node) hNodes.get(key);
		if (n == null) {
			n = new Node(wdn);

			n.width = nodewidth;
			n.height = nodeheight;
			nl.add(n);
			new Selector((Figure) wdn);
			hNodes.put(key, n);
			return n;
		}
		else {
			return null;
		}

	}

	public Node addInvisibleNode(String thisName, WitNodeInvisible wni) {

		String key = wni.createKey();
		wni.setNodeType("invisible");

		Node n;
		n = (Node) hNodes.get(key);
		if (n == null) {
			n = new Node(wni);

			n.width = nodewidth;
			n.height = nodeheight;
			nl.add(n);
			new Selector((Figure) wni);
			hNodes.put(key, n);
		}
		return n;

	}

	// public void addParentNode(String thisName, String thisType, ListInfo p,
	// int index) {
	// String parentname = p.name;
	// / String parenttype = p.type;
	// addNode(parentname,parenttype);

	// now the edge for this new parent node
	// addParentEdge(thisName, thisType, p, index);
	// }

	public DirectedGraph CreateWitDetailTree(WitObject witobject,
			Object focusObject) {

		if (focusObject == null)
			return new DirectedGraph();

		String type, name;
		if (focusObject.getClass().toString().equals("class witviz.WitNode")
				|| focusObject.getClass().toString().equals(
						"class witviz.WitDemandNode")
				|| focusObject.getClass().toString().equals(
						"class witviz.WitNodeInvisible")) {
			type = ((WitNode) focusObject).getNodeType();
			name = ((WitNode) focusObject).getText();
		} else {
			type = ((WitEdge) focusObject).getEdgeType();
			name = ((WitEdge) focusObject).getText();
		}

		// do all parents and children of the focus node. Then stop.
		if ((type.equals("MATERIAL")) || (type.equals("CAPACITY"))) {
			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			addNode(name, type);

			Vector kids = wo.getPartChildren(name);
			Vector parents = wo.getPartParents(name);
			for (int i = 0; i < kids.size(); i++) {
				int index = ((ListInfo) kids.elementAt(i)).index;
				ListInfo k = (ListInfo) kids.elementAt(i);
				addNode(k.name, k.type);
				addPartChildEdge(name, type, (ListInfo) kids.elementAt(i),
						index);
			}
			Vector parentList = new Vector();
			for (int i = 0; i < parents.size(); i++) {
				ListInfo p = (ListInfo) parents.elementAt(i);
				int index = p.index;
				String pname = p.name;
				parentList.add(pname);
				String ptype = p.type;
				addNode(pname, ptype);
				addPartParentEdge(name, type, (ListInfo) parents.elementAt(i),
						index);
			}

			// now handle substitutes
			int numsubsbomentries = wo.getPartNConsumingSubsBomEntries(name);
			// //go through them, determine the original part and add it
			String[] opName = new String[1];
			int[] bomEntryIndex = new int[1];
			int[] subsBomEntryIndex = new int[1];
			for (int i = 0; i < numsubsbomentries; i++) {
				SubsInfo si = wo.GetPartSubsInfo(name, i);
				if (!parentList.contains(si.consumingOperation)) {
					addNode(si.consumingOperation, "operation");
					ListInfo parent = new ListInfo("operation",
							si.consumingOperation);
					// figure out the real part
					String consumedPart = si.consumedPart;
					String mainPart = si.mainPart;
					String partType = wo.getPartCategory(mainPart);
					addNode(mainPart, partType);
					addPartParentEdge(mainPart, partType, parent, si.bomEntryIndex); // this
				}
				// should
				// pull
				// in
				// the
				// substitute
				// arcs
			}

			// add the demands too
			addDemands(name);
			setAsExpanded(name, type);

		} else if (type.equals("operation")) {
			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			addNode(name, type);

			Vector kids = wo.getOperationChildren(name);
			Vector parents = wo.getOperationParents(name);
			for (int i = 0; i < parents.size(); i++) {
				ListInfo parentpart = (ListInfo) parents.elementAt(i);
				String key = WitNode
						.createKey(parentpart.type, parentpart.name);
				addNode(parentpart.name, parentpart.type);
				ListInfo me = new ListInfo(type, name);
				int index = parentpart.index;
				addPartChildEdge(parentpart.name, parentpart.type, me, index);
			}
			for (int i = 0; i < kids.size(); i++) {
				int index = ((ListInfo) kids.elementAt(i)).index;
				ListInfo childpart = (ListInfo) kids.elementAt(i);
				addNode(childpart.name, childpart.type);
				ListInfo me = new ListInfo(type, name);
				addPartParentEdge(childpart.name, childpart.type, me, index);
			}
			setAsExpanded(name, type);
		} else if (type.equals("demand")) {
			WitDemandNode wdn = (WitDemandNode) focusObject;

			String d = wdn.getDemandName();
			String p = wdn.getPartName();

			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			Node n = addDemandNode(d, p);
			String partType = wo.getPartCategory(p);
			ListInfo li = new ListInfo(partType, p);
			Node pn = addNode(li.name, li.type);
			WitNode wp = (WitNode) pn.data;
			ExtendedEdge e = new ExtendedEdge(n, pn, -1);
			WitDemandEdge wde = new WitDemandEdge();
			wde.setDemand(wdn);
			wde.setPart(wp);
			String key = WitDemandEdge.createKey(d, p);
			e.setType("demandedge");
			e.data = wde;
			e.name = key;
			hEdges.put(key, e);
			el.add(e);
			wde.setText(key);
			wde.setEdgeType("demandedge");
			new Selector((Figure) wde);
			setAsExpanded(name, type);
		} else if (type.equals("bomupper")) {
			// add the input part
			WitBomUpperFragmentEdge wbe = (WitBomUpperFragmentEdge) focusObject;

			WitNodeInvisible wni = (WitNodeInvisible) wbe.getInvisible();
			String partName, partType;
			String operationName, operationType;

			partName = wni.getPart().getText();
			partType = wni.getPart().getNodeType();
			operationName = wbe.getOperation().getText();
			operationType = wbe.getOperation().getNodeType();
			int operationIndex = wbe.getBomEntry();

			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			addNode(partName, partType);
			addNode(operationName, operationType);
			ListInfo li = new ListInfo(operationType, operationName);
			// add the edge + any substitutes that come along with it
			addPartParentEdge(partName, partType, li, operationIndex);

		} else if (type.equals("bomlower")) {
			// add the input part
			WitBomLowerFragmentEdge wbe = (WitBomLowerFragmentEdge) focusObject;

			WitNodeInvisible wni = (WitNodeInvisible) wbe.getInvisible();
			String partName, partType;
			String operationName, operationType;

			operationName = wni.getOperation().getText();
			operationType = wni.getOperation().getNodeType();
			partName = wbe.getPart().getText();
			partType = wbe.getPart().getNodeType();
			int operationIndex = wbe.getBomEntry();

			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			addNode(partName, partType);
			addNode(operationName, operationType);
			ListInfo li = new ListInfo(operationType, operationName);
			// add the edge + any substitutes that come along with it
			addPartParentEdge(partName, partType, li, operationIndex);

		} else if (type.equals("substitute")) {
			// add the input part
			WitSubstituteEdge wbe = (WitSubstituteEdge) focusObject;

			WitNodeInvisible wni = (WitNodeInvisible) wbe.getInvisibleNode();
			String partName, partType;
			String operationName, operationType;

			operationName = wni.getOperation().getText();
			operationType = wni.getOperation().getNodeType();
			partName = wni.getPart().getText();
			partType = wni.getPart().getNodeType();
			int operationIndex = wni.getBomEntry();

			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			addNode(partName, partType);
			addNode(operationName, operationType);
			ListInfo li = new ListInfo(operationType, operationName);
			// add the edge + any substitutes that come along with it
			addPartParentEdge(partName, partType, li, operationIndex);

		} else if (type.equals("bom")) {
			// add the input part
			WitBomEdge wbe = (WitBomEdge) focusObject;
			String partName, partType;
			String operationName, operationType;

			partName = wbe.getPart().getText();
			partType = wbe.getPart().getNodeType();
			operationName = wbe.getOperation().getText();
			operationType = wbe.getOperation().getNodeType();
			int operationIndex = wbe.getBomEntry();

			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			addNode(partName, partType);
			addNode(operationName, operationType);
			ListInfo li = new ListInfo(operationType, operationName);
			// add the edge + any substitutes that come along with it
			addPartParentEdge(partName, partType, li, operationIndex);
		} else if (type.equals("bop")) {
			// add the input operation
			String operationName, operationType;
			WitBopEdge wbe = (WitBopEdge) focusObject;
			operationName = wbe.getOperation().getText();
			operationType = wbe.getOperation().getNodeType();
			int index = wbe.getBopEntry();

			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			addNode(operationName, operationType);

			// add the part
			String partName, partType;
			partName = wbe.getPart().getText();
			partType = wbe.getPart().getNodeType();

			addNode(partName, partType);
			ListInfo li = new ListInfo(operationType, operationName);
			// //add the edge + any substitutes that come along with it
			addPartChildEdge(partName, partType, li, index);

		} else if (type.equals("demandedge")) {

			String partName, partType;
			WitDemandEdge wde = (WitDemandEdge) focusObject;
			partName = wde.getPart().getText();
			partType = wde.getPart().getNodeType();

			hNodes.clear();
			hEdges.clear();
			nl = new NodeList();
			el = new EdgeList();
			alldemands = new Vector();
			allNodes = null;
			allEdges = null;
			Node pn = addNode(partName, partType);

			String demandName, demandType;
			demandName = wde.getDemand().getText();
			Node dn = addDemandNode(demandName, partName);

			// //add the edge
			addPartDemandEdge(pn, dn);

		}

		allNodes = nl;
		DirectedGraph graph = new DirectedGraph();
		graph.nodes = nl;
		graph.edges = el;
		allEdges = el;

		new DirectedGraphLayout().visit(graph);

		// readjust height of invisible nodes
		for (int i = 0; i < nl.size(); i++) {
			Node n = allNodes.getNode(i);
			WitNode wn = (WitNode) n.data;
			type = wn.getNodeType();
			// if ((type.equals("dummy")) || (type.equals("invisible")))
			if (type.equals("dummy"))
				n.height = 0;
		}

		DirectedGraph dg = graph;
		SelectedObject = focusObject;

		return dg;

	}

	public void setAsExpanded(String name, String type) {
		if ((type.equals("operation")) || (type.equals("MATERIAL"))
				|| (type.equals("CAPACITY"))) {
			String key = WitNode.createKey(type, name);
			Node n = (Node) hNodes.get(key);
			WitNode wn = (WitNode) n.data;
			wn.setBorder(new LineBorder(expandedColor, 2));
		} else { // demand node

			Node n = (Node) hNodes.get(name);
			WitDemandNode wdn = (WitDemandNode) n.data;
			wdn.setBorder(new LineBorder(expandedColor, 2));
		}
	}

	public void Collapse(String name, String type) {

	}

	public void Expand(String type, String name) {
		// clear the canvas
		
		
		
		this.removeAll();

		//need to be careful to only add nodes and edges that aren't
		//already there



		if (type.equals("operation")) {
			Vector parents = wo.getOperationParents(name);
			for (int i = 0; i < parents.size(); i++) {
				ListInfo li = (ListInfo) parents.elementAt(i);
				
				String key = WitNode.createKey(li.type, li.name);
				if (!hNodes.containsKey(key)) {
					addNode(li.name, li.type);
					String part = li.name;
					String ty = li.type;
					// do it "upside down"
					ListInfo me = new ListInfo(type, name);
					addPartChildEdge(part, ty, me, li.index);
				}
			}
			Vector kids = wo.getOperationChildren(name);
			for (int i = 0; i < kids.size(); i++) {
				// kids are different to deal with substitutes correctly.
				// only "add parent edge" looks for subs
				ListInfo li = (ListInfo) kids.elementAt(i);
				String key = WitNode.createKey(li.type, li.name);
				if (!hNodes.containsKey(key)) {
					addNode(li.name, li.type);
					ListInfo pli = new ListInfo(type, name);
					addPartParentEdge(li.name, li.type, pli, li.index);
				}
			}
		} else if ((type.equals("MATERIAL") || (type.equals("CAPACITY")))) {

			Vector parents = wo.getPartParents(name);
			for (int i = 0; i < parents.size(); i++) {
				ListInfo li = (ListInfo) parents.elementAt(i);
				String key = WitNode.createKey(li.type, li.name);
				if (!hNodes.containsKey(key)) {
					addNode(li.name, li.type);
					addPartParentEdge(name, type, li, li.index);
				}
			}
			Vector kids = wo.getPartChildren(name);
			for (int i = 0; i < kids.size(); i++) {
				ListInfo li = (ListInfo) kids.elementAt(i);
				String key = WitNode.createKey(li.type, li.name);
				if (!hNodes.containsKey(key)) {
					addNode(li.name, li.type);
					addPartChildEdge(name, type, li, li.index);
				}
			}
			// add demands
			addDemands(name);
		}
		setAsExpanded(name, type);
		DirectedGraph graph = new DirectedGraph();
		graph.nodes = nl;
		graph.edges = el;
		
		new DirectedGraphLayout().visit(graph);

	}

	public void addDemands(String partname) {
		Vector demandList = wo.getPartDemands(partname);
		String parttype = wo.getPartCategory(partname);
		
		// add the demand nodes
		Node n;
		for (int j = 0; j < demandList.size(); j++) {
			ListInfo demand = (ListInfo) demandList.elementAt(j);
			n = addDemandNode(demand.name, partname);
			if (n!=null) {
				// add the arc
				int index = -1;
				String demandname = ((WitDemandNode) n.data).getText();
	
				String partkey = WitNode.createKey(parttype, partname);
				Node partNode = (Node) hNodes.get(partkey);
				ExtendedEdge e = new ExtendedEdge(n, partNode, -1);
				e.setType("demandedge");
				WitDemandEdge wde = new WitDemandEdge();
				// WitDemandNode wdn = (WitDemandNode)n.data;
	
				wde.setDemand((WitDemandNode) n.data);
				wde.setPart((WitNode) partNode.data);
	
				wde.setEdgeType("demandedge");
				e.data = (WitEdge) wde;
	
				String key = ((WitDemandEdge) e.data).createKey();
				e.name = key;
				wde.setText(key);
				if (hEdges.get(key) == null) {
					hEdges.put(key, e);
					el.add(e);
				}
	
				new Selector((Figure) e.data);
			}
		}
	}

	public void addPartDemandEdge(Node pn, Node dn) {
		// add the arc
		int index = -1;

		Node partNode = pn;
		Node demandNode = dn;
		ExtendedEdge e = new ExtendedEdge(demandNode, partNode, -1);
		e.setType("demandedge");
		WitDemandEdge wde = new WitDemandEdge();
		wde.setDemand((WitDemandNode) demandNode.data);
		wde.setPart((WitNode) partNode.data);

		wde.setEdgeType("demandedge");
		e.data = (WitEdge) wde;

		String key = ((WitDemandEdge) e.data).createKey();
		e.name = key;
		wde.setText(key);
		hEdges.put(key, e);
		el.add(e);

		new Selector((Figure) e.data);

	}

	// String SourceTypeFromKey(String key) {
	// int i1 = key.indexOf("type1: ") + 7;
	// int i2 = key.indexOf("type2: ");
	// String s = key.substring(i1, i2);
	// return s;
	// }
	// String DemandNameFromName(String s) {
	// String d;
	// int i1 = s.indexOf("demand:")+7;
	// int i2 = s.indexOf("part:");
	// d = s.substring(i1,i2);
	// return d;
	// }
	// String PartNameFromName(String s) {
	// String d;
	// int i1 = s.indexOf("part:")+5;
	// int i2 = s.length();
	// d = s.substring(i1,i2);
	// return d;
	// }

	// String TargetTypeFromKey(String key) {
	// int i1 = key.indexOf("type2: ") + 7;
	// int i2 = key.indexOf("INDEX:");
	// String s = key.substring(i1, i2);
	// return s;
	// }
	public DirectedGraph CreateWitTree() {
		hNodes.clear();
		hEdges.clear();

		Vector allparts = wo.getAllParts();
		int numparts = wo.getNumParts();
		nl = new NodeList();
		el = new EdgeList();
		alldemands = null;
		allNodes = null;
		allEdges = null;

		alldemands = new Vector();
		for (int i = 0; i < numparts; i++) {
			String partname = (String)allparts.elementAt(i);
			String category = wo.getPartCategory(partname);
			addNode(partname, category);
			// do demands
			addDemands(partname); // foo1
		}

		Vector allops = wo.getAllOperations();
		int numops = wo.getNumOperations();
		for (int i = 0; i < numops; i++) {
			// check whether it's connected to anything else first
			// Vector kids = wo.getOperationChildren(allops[0][i]);
			// Vector parents = wo.getOperationParents(allops[0][i]);
			// if ((kids.size() != 0) || (parents.size() != 0)) {
			String operationname = (String)allops.elementAt(i);
			addNode(operationname, "operation");
			// }
		}

		// for all parts and operations do their inputs and outputs
		for (int i = 0; i < numparts; i++) {
			String partname = (String)allparts.elementAt(i);
			String parttype = wo.getPartCategory(partname);
			Vector kids = wo.getPartChildren(partname);
			for (int j = 0; j < kids.size(); j++) {
				ListInfo liChild = ((ListInfo) kids.elementAt(j));
				int index = liChild.index;
				addPartChildEdge(partname, parttype, liChild, index);
			}
			Vector parents = wo.getPartParents((String)allparts.elementAt(i));
			for (int j = 0; j < parents.size(); j++) {
				ListInfo liParent = ((ListInfo) parents.elementAt(j));
				int index = liParent.index;
				addPartParentEdge(partname, parttype, liParent, index);

			}

		}

		allNodes = nl;
		ConnectThemAll();
		DirectedGraph graph = new DirectedGraph();
		graph.nodes = nl;
		graph.edges = el;
		Vector nodes = new Vector();
		for (Iterator i = nl.listIterator(); i.hasNext();) {
			nodes.add(i.next());
		}
		Stack stack = new Stack();
		Vector added = new Vector();
		stack.push(nl.get(0));
		//
		// Check everything is connected.
		//
		while (stack.size() > 0) {
			Node n = (Node) stack.pop();
			if (added.contains(n)) {
				continue;
			}
			added.add(n);
			nodes.remove(n);
			EdgeList outgoing = n.outgoing;
			for (Iterator j = outgoing.listIterator(); j.hasNext();) {
				Edge e = (Edge) j.next();
				if (!e.source.equals(n)) {
					continue;
				}
				if (!added.contains(e.target) && !stack.contains(e.target)) {
					stack.push(e.target);
				}
			}
			EdgeList incoming = n.incoming;
			for (Iterator j = incoming.listIterator(); j.hasNext();) {
				Edge e = (Edge) j.next();
				if (!e.target.equals(n)) {
					continue;
				}
				if (!added.contains(e.source) && !stack.contains(e.source)) {
					stack.push(e.source);
				}
			}
		}
		//System.out.println(">>>>>>>>>>>>>>>>>>>>");
		for (Iterator i = nl.listIterator(); i.hasNext();) {
			Node nn;
			WitNode n = (WitNode) (nn = (Node) i.next()).data;
			//System.out.println("Node: " + n.getText() + ":" + n.getNodeType());
			EdgeList outgoing = nn.outgoing;
			if (outgoing.size() > 0) {
				for (Iterator j = outgoing.listIterator(); j.hasNext();) {
					ExtendedEdge e = (ExtendedEdge) j.next();
					//System.out.print("\t");
					WitNode source = (WitNode) e.source.data;
					WitNode target = (WitNode) e.target.data;
					int isource = nl.indexOf(e.source);
					int itarget = nl.indexOf(e.target);
					//System.out.print("\"" + source.getText() + "-"
					//		+ source.getNodeType() + "(" + isource + ")"
					//		+ "\"=>\"" + target.getText() + "-"
					//		+ target.getNodeType() + "(" + itarget + ")"
					//		+ "\": ");
					//System.out.println(e.name);
				}
			}
		}
		//System.out.println(">>>>>>>>>>>>>>>>>>>>");
		for (Iterator i = el.listIterator(); i.hasNext();) {
			ExtendedEdge e = (ExtendedEdge) i.next();
			//System.out.println(e.name);
		}
		allNodes = nl;
		allEdges = el;
		
		DirectedGraphLayout mdgl = new DirectedGraphLayout();
		mdgl.visit(graph);
		// readjust height of invisible nodes
		for (int i = 0; i < nl.size(); i++) {
			Node n = allNodes.getNode(i);
			WitNode wn = (WitNode) n.data;
			String type = wn.getNodeType();
			// if ((type.equals("dummy")) || (type.equals("invisible"))) {
			if (type.equals("dummy")) {
				n.y = n.y + n.height;
				n.height = 0;
			}
		}

		DirectedGraph dg = graph;
		return dg;

	}

	public void ResetToWhite() {
		NodeList nl = allNodes;
		for (int i = 0; i < nl.size(); i++) {
			Label l = (Label) nl.getNode(i).data;
			l.setBackgroundColor(ColorConstants.white);
		}
		EdgeList el = allEdges;
		for (int i = 0; i < el.size(); i++) {
			ExtendedEdge ee = (ExtendedEdge) el.getEdge(i);
			String key = ee.name;
			ExtendedEdge ee1 = (ExtendedEdge) hEdges.get(key);
			WitEdge e = (WitEdge) ee1.data;

			e.setForegroundColor(ColorConstants.gray);
			e.setLineWidth(e.getDefaultWidth());

		}

	}

	// String SourceFromKey(String key) {
	// int i1 = key.indexOf("edge:source: ") + 13;
	// int i2 = key.indexOf(" target: ");
	// String s = key.substring(i1, i2);
	// return s;
	// }

	// String TargetFromKey(String key) {
	// int i1 = key.indexOf(" target: ") + 9;
	// int i2 = key.indexOf("type1:");
	// String s = key.substring(i1, i2);
	// return s;
	// }
	// int BomIndexFromKey(String key) {
	// int i1 = key.indexOf("INDEX: ") + 7;
	// int i2 = key.length();
	// String s = key.substring(i1, i2);
	// int i = (Integer.decode(s)).intValue();
	// return i;
	// }
	// int SubsIndexFromKey(String key) {
	// int i1 = key.indexOf("index: ") + 7;
	// int i2 = key.indexOf("index:");
	// String s = key.substring(i1, i2);
	// int i = (Integer.decode(s)).intValue();
	// return i;
	// }
	public void ColorSelf(Object w) {
		if (w.getClass().toString().equals("class witviz.WitNode")
				|| (w.getClass().toString()
						.equals("class witviz.WitDemandNode")))
			ColorSelfNode((WitNode) w);
		else
			ColorSelfEdge((WitEdge) w);
	}

	public void ColorPart(String partname, Color color) {
		String type = wo.getPartCategory(partname);
		String key = WitNode.createKey(type, partname);
		Node n = (Node) hNodes.get(key);
		WitNode wn = (WitNode) n.data;
		wn.setBackgroundColor(color);

	}

	public void ColorSelfNode(WitNode w) {
		Label l = (Label) w;
		// l.setBackgroundColor(ColorConstants.red);
		l.setBackgroundColor(selfColor);
	}

	public void ColorSelfEdge(WitEdge e) {
		e.setForegroundColor(arcSelfColor);
		if (e.getLineWidth() < 3)
			e.setLineWidth(3);
	}

	private void ColorPartFamily(WitNode w) {

		String name = w.getText();
		String type = w.getNodeType();

		Vector kids = wo.getPartChildren(name);
		for (int i = 0; i < kids.size(); i++) {
			String key = WitNode.createKey("operation", kids.elementAt(i)
					.toString());
			Node n = (Node) hNodes.get(key);
			Label l = (Label) n.data;
			l.setBackgroundColor(childColor);
			int index = ((ListInfo) kids.elementAt(i)).index;
			key = WitBopEdge.createKey(kids.elementAt(i).toString(), name,
					index);
			ExtendedEdge ee = (ExtendedEdge) hEdges.get(key);
			WitEdge e = (WitEdge) ee.data;
			e.setForegroundColor(arcChildColor);
			if (e.getLineWidth() < 3)
				e.setLineWidth(3);
		}
		Vector parents = wo.getPartParents(name);
		for (int i = 0; i < parents.size(); i++) {
			String key = WitNode.createKey("operation", parents.elementAt(i)
					.toString());
			Node n = (Node) hNodes.get(key);
			Label l = (Label) n.data;
			l.setBackgroundColor(parentColor);
			int index = ((ListInfo) parents.elementAt(i)).index;
			key = WitBomEdge.createKey(parents.elementAt(i).toString(), name,
					index);
			ExtendedEdge ee = (ExtendedEdge) hEdges.get(key);
			if (ee == null) {
				// assume it's an invisible node situation
				String ni = WitNodeInvisible.createKey(parents.elementAt(i)
						.toString(), name, index);
				// get the lower and upper portions of the arc
				Node ninv = (Node) hNodes.get(ni);
				WitNodeInvisible wni = (WitNodeInvisible) ninv.data;
				String lowerkey = WitBomLowerFragmentEdge.createKey(wni
						.createKey(), name, index);
				ee = (ExtendedEdge) hEdges.get(lowerkey);
				WitBomLowerFragmentEdge le = (WitBomLowerFragmentEdge) ee.data;
				le.setForegroundColor(arcParentColor);
				if (le.getLineWidth() < 3)
					le.setLineWidth(3);
				String upperkey = WitBomUpperFragmentEdge.createKey(parents
						.elementAt(i).toString(), wni.createKey(), index);
				ee = (ExtendedEdge) hEdges.get(upperkey);
				WitBomUpperFragmentEdge ue = (WitBomUpperFragmentEdge) ee.data;
				ue.setForegroundColor(arcParentColor);
				if (ue.getLineWidth() < 3)
					ue.setLineWidth(3);

			} else {
				WitEdge e = ee.data;
				e.setForegroundColor(arcParentColor);
				if (e.getLineWidth() < 3)
					e.setLineWidth(3);
			}

		}
		//find the things that sub for the part
		Vector subsparents = wo.getPartSubstitutes(name);
		for (int i = 0; i < subsparents.size(); i++) {
			SubsInfo si = (SubsInfo) subsparents.elementAt(i);
			String key = WitNode.createKey("operation", si.consumingOperation);
			Node n = (Node) hNodes.get(key);
			Label l = (Label) n.data;
			l.setBackgroundColor(subsParentColor);
			int ii = si.bomEntryIndex;
			String op = si.consumingOperation;

			// assume it's an invisible node situation
			// the invisible node will have the name of the real
			// part (not the substitute part)
			//String realPart = wo.getRealPartFromSubsParentIndex(name, i);d
			String realPart=name;
			int bomIndex = si.bomEntryIndex;
			int subsBomIndex = si.subsBomEntryIndex;
			//String realPartType = wo.getPartCategory(name);

			String k = WitNodeInvisible.createKey(op, realPart, bomIndex);
			Node ni = (Node) hNodes.get(k);
			WitNodeInvisible wni = (WitNodeInvisible) ni.data;

			String subskey = WitSubstituteEdge.createKey(wni.createKey(),
					subsBomIndex);

			ExtendedEdge ee = (ExtendedEdge) hEdges.get(subskey);
			WitEdge e = (WitEdge) ee.data;
			e.setForegroundColor(arcParentColor);
			if (e.getLineWidth() < 3)
				e.setLineWidth(3);
			String upperk = WitBomUpperFragmentEdge.createKey(op, wni
					.createKey(), bomIndex);
			ee = (ExtendedEdge) hEdges.get(upperk);
			e = (WitEdge) ee.data;
			e.setForegroundColor(arcParentColor);
			if (e.getLineWidth() < 3)
				e.setLineWidth(3);
		}
		// now do demands
		Vector demands = wo.getPartDemands(name);
		for (int i = 0; i < demands.size(); i++) {
			ListInfo li = (ListInfo) demands.elementAt(i);
			// String fullDemandName = WitDemandNode.createKey(li.name,name);
			String key = WitDemandNode.createKey(li.name, name);
			Node n = (Node) hNodes.get(key);
			Label l = (Label) n.data;
			l.setBackgroundColor(demandColor);
			key = WitDemandEdge.createKey(li.name, name);
			ExtendedEdge ee = (ExtendedEdge) hEdges.get(key);
			WitEdge e = (WitEdge) ee.data;
			e.setForegroundColor(arcDemandColor);
			if (e.getLineWidth() < 3)
				e.setLineWidth(3);
		}

	}

	private void ColorOperationFamily(WitNode w) {
		String name = w.getText();
		String type;
		Vector kids = wo.getOperationChildren(name);
		for (int i = 0; i < kids.size(); i++) {
			ListInfo k = (ListInfo) kids.elementAt(i);
			int index = k.index;
			type = k.type;
			String key = WitNode.createKey(type, k.name);
			Node n = (Node) hNodes.get(key);
			Label l = (Label) n.data;
			l.setBackgroundColor(childColor);
			key = WitBomEdge.createKey(name, k.name, index);
			ExtendedEdge ee = (ExtendedEdge) hEdges.get(key);
			if (ee == null) {
				// assume it's an invisible node situation
				key = WitNodeInvisible.createKey(name, k.name, index);
				Node in = (Node) hNodes.get(key);
				WitNodeInvisible wni = (WitNodeInvisible) in.data;
				String part = wni.getPart().getText();
				String truetype = wni.getPart().getNodeType();
				String k1 = WitBomUpperFragmentEdge.createKey(name, wni
						.createKey(), index);
				ee = (ExtendedEdge) hEdges.get(k1);
				WitBomUpperFragmentEdge ue = (WitBomUpperFragmentEdge) ee.data;
				ue.setForegroundColor(arcChildColor);
				if (ue.getLineWidth() < 3)
					ue.setLineWidth(3);
				String k2 = WitBomLowerFragmentEdge.createKey(wni.createKey(),
						k.name, index);
				ee = (ExtendedEdge) hEdges.get(k2);
				WitBomLowerFragmentEdge le = (WitBomLowerFragmentEdge) ee.data;
				le.setForegroundColor(arcChildColor);
				if (le.getLineWidth() < 3)
					le.setLineWidth(3);

			} else {
				WitEdge e = ee.data;
				e.setForegroundColor(arcChildColor);
				if (e.getLineWidth() < 3)
					e.setLineWidth(3);
			}

		}

		Vector parents = wo.getOperationParents(name);
		for (int i = 0; i < parents.size(); i++) {
			type = ((ListInfo) parents.elementAt(i)).type;
			int index = ((ListInfo) parents.elementAt(i)).index;
			String partname = ((ListInfo) parents.elementAt(i)).name;
			String key = WitNode.createKey(type, partname);
			Node n = (Node) hNodes.get(key);
			Label l = (Label) n.data;
			l.setBackgroundColor(parentColor);
			key = WitBopEdge.createKey(name, partname, index);
			ExtendedEdge ee = (ExtendedEdge) hEdges.get(key);
			WitBopEdge e = (WitBopEdge) ee.data;
			e.setForegroundColor(arcParentColor);
			if (e.getLineWidth() < 3)
				e.setLineWidth(3);

		}

	}

	public void ColorFamilyNode(WitNode w) {
		String type = w.getNodeType();
		String name = w.getText();
		if ((type.equals("MATERIAL")) || (type.equals("CAPACITY"))) {
			ColorPartFamily(w);
		} else if (type.equals("operation")) {
			ColorOperationFamily(w);
		} else if (type.equals("demand")) {
			// //get parts demanded
			Vector partsDemanded = wo.getDemandedParts(name);
			for (int i = 0; i < partsDemanded.size(); i++) {
				type = ((ListInfo) partsDemanded.elementAt(i)).type;
				String key = WitNode.createKey(type, partsDemanded.elementAt(i)
						.toString());
				Node n = (Node) hNodes.get(key);
				Label l = (Label) n.data;
				l.setBackgroundColor(childColor);
			}
		}
	}

	public void ColorFamilyEdge(WitEdge e) {
		String type = e.getEdgeType();

		// if (type.equals("bom")) {
		// //need to color arc's parents, children, and substitutes
		// ExtendedEdge ee = (ExtendedEdge)hEdges.get(name);
		// WitBomEdge wbe = (WitBomEdge)ee.data;
		// String source = wbe.getSource();
		// String target = wbe.getTarget();
		// int index = wbe.getBomEntry();
		// String stype = wbe.getOperation().getNodeType();
		// String ttype = wbe.getPart().getNodeType();
		// ExtendedEdge Subsee = ((ExtendedEdge) hEdges
		// .get(ExtendedEdge.edgeKeyFromSourceTargetIndex(source, stype, target,
		// ttype,index)));
		// String truesource;
		// if (stype.equals("invisible")) {
		// //move on to the upper arc, then to the "real" source
		// truesource = operationFromInvisibleName(source);
		// index = indexFromInvisibleName(source);
		// String k1 = ExtendedEdge.edgeKeyFromSourceTargetIndex(truesource,
		// "operation", source, "invisible",index);
		// ee = (ExtendedEdge) hEdges.get(k1);
		// WitEdge we = (WitEdge) ee.data;
		// we.setForegroundColor(arcSelfColor);
		// if (we.getLineWidth()<3) we.setLineWidth(3);
		// String k2 = "operation" + truesource;
		// Node n = (Node) hNodes.get(k2);
		// Label l = (Label) n.data;
		// l.setBackgroundColor(parentColor);
		// } else if (ttype.equals("invisible")) {
		// //this is the upper part of an arc to an invisible node
		// String truetarget = partFromInvisibleName(target);
		// String truetype = partTypeFromInvisibleName(target);
		// index = indexFromInvisibleName(target);
		// truesource = source;
		// String k1 = ExtendedEdge.edgeKeyFromSourceTargetIndex(target,
		// "invisible", truetarget, truetype,index);
		// ee = (ExtendedEdge) hEdges.get(k1);
		// WitEdge we = (WitEdge) ee.data;
		// we.setForegroundColor(arcSelfColor);
		// if (we.getLineWidth()<3) we.setLineWidth(3);

		// now do the actual parent
		// String k2 = "operation" + source;
		// Node n = (Node) hNodes.get(k2);
		// Label l = (Label) n.data;
		// l.setBackgroundColor(parentColor);
		// } else {
		// truesource = source;
		// String k2 = "operation" + truesource;
		// Node n = (Node) hNodes.get(k2);
		// Label l = (Label) n.data;
		// l.setBackgroundColor(parentColor);

		// }

		// child
		// if this is an invisible node we need to do some other work
		// String truetarget;
		// if (target.indexOf("invisible") >= 0) {
		// //move on to the lower arc, then to the "real" target
		// truetarget = partFromInvisibleName(target);
		// String truetype = wo.getPartCategory(truetarget);
		// index = indexFromInvisibleName(target);
		// String k1 = ExtendedEdge.edgeKeyFromSourceTargetIndex(target,
		// "invisible",
		// truetarget, truetype,index);
		// ee = (ExtendedEdge) hEdges.get(k1);
		// WitEdge we = (WitEdge) ee.data;
		// we.setForegroundColor(arcSelfColor);
		// if (we.getLineWidth()<3) we.setLineWidth(3);
		// String k2 = truetype + truetarget;
		// Node wn = (Node) hNodes.get(k2);
		// Label l = (Label) wn.data;
		// l.setBackgroundColor(childColor);
		// } else {
		// String key = WitNode.createKey(ttype, target);
		// truetarget = target;
		// Node n = (Node) hNodes.get(key);
		// Label l = (Label) n.data;
		// l.setBackgroundColor(childColor);
		// }
		// now substitutes
		// index = Subsee.getIndex();

		// Vector substitutes = wo.getBomArcSubstitutes(truesource,
		// index);
		// for (int j = 0; j < substitutes.size(); j++) {
		// String subspart = ((ListInfo)substitutes.elementAt(j)).type
		// + ((ListInfo) substitutes.elementAt(j)).name;
		// Node n = (Node) hNodes.get(subspart);
		// Label l = (Label) n.data;
		// l.setBackgroundColor(subsChildColor);

		// }

		// } else if (type.equals("bop")) {
		// //need to color arc's parents, children, and substitutes
		// ExtendedEdge ee = (ExtendedEdge)hEdges.get(name);
		// WitBopEdge wbe = (WitBopEdge)ee.data;
		//			
		// WitNode par = wbe.getPart();
		// par.setBackgroundColor(childColor);

		// WitNode op = wbe.getOperation();
		// op.setBackgroundColor(parentColor);
		// }
	}

	public void FillInfoBox(String type, String name) {
		if (type.equals("operation")) {
			FillInfoBoxOperation(name);
		} else if (type.equals("MATERIAL") || (type.equals("CAPACITY"))) {
			FillInfoBoxPart(name, type);
		} else if (type.equals("demand")) {
			FillInfoBoxDemand(name);
		} else if (type.equals("bom")) {
			FillInfoBoxBom(name);
		} else if (type.equals("bop")) {
			FillInfoBoxBop(name);
		} else if (type.equals("demandedge")) {
			FillInfoBoxDemandEdge(name);
		} else if (type.equals("bomupper")) {
			FillInfoBoxBomUpper(name);
		} else if (type.equals("bomlower")) {
			FillInfoBoxBomLower(name);
		} else if (type.equals("substitute")) {
			FillInfoBoxSubstitute(name);
		} else {
			TextBox.setText("Type: " + type + "\nName:" + name);
		}
	}

	public void FillInfoBoxBomUpper(String bomName) {

		TextBox.setText("");
		{

			ExtendedEdge ee = (ExtendedEdge) hEdges.get(bomName);
			WitBomUpperFragmentEdge e = (WitBomUpperFragmentEdge) ee.data;
			int index = ee.getIndex();
			String label = e.getNiceName();
			String operation = e.getOperation().getText();
			String info = label + "\n";
			TextBox.append(info);
			info = "Bom Index: " + index + "\n";
			TextBox.append(info);
			parent.bomOptionDialog.getOptionsString(operation, index, TextBox);
		}

	}

	public void FillInfoBoxSubstitute(String bomName) {

		TextBox.setText("");
		{

			ExtendedEdge ee = (ExtendedEdge) hEdges.get(bomName);
			WitSubstituteEdge e = (WitSubstituteEdge) ee.data;

			String label = e.getNiceName();
			WitNodeInvisible wni = e.getInvisibleNode();
			String operation = wni.getOperation().getText();
			int bomIndex = wni.getBomEntry();
			int subsBomIndex = e.getSubsEntry();
			String info = label + "\n" + "subs Bom Index: " + subsBomIndex
					+ "\n";
			TextBox.append(info);
			parent.substituteOptionDialog.getOptionsString(operation, bomIndex,
					subsBomIndex, TextBox);
		}

	}

	public void FillInfoBoxBomLower(String bomName) {

		TextBox.setText("");
		{

			ExtendedEdge ee = (ExtendedEdge) hEdges.get(bomName);
			WitBomLowerFragmentEdge e = (WitBomLowerFragmentEdge) ee.data;
			int index = ee.getIndex();
			String label = e.getNiceName();
			WitNodeInvisible wni = e.getInvisible();
			String operation = wni.getOperation().getText();
			String info = label + "\n";
			TextBox.append(info);
			info = "Bom Index: " + index + "\n";
			TextBox.append(info);
			parent.bomOptionDialog.getOptionsString(operation, index, TextBox);
		}

	}

	public void FillInfoBoxBom(String bomName) {

		TextBox.setText("");
		{

			ExtendedEdge ee = (ExtendedEdge) hEdges.get(bomName);
			WitBomEdge e = (WitBomEdge) ee.data;
			int index = ee.getIndex();
			String label = e.getNiceName();
			String operation = e.getOperation().getText();
			String info = label + "\n";
			TextBox.append(info);
			info = "Bom Index: " + index + "\n";
			TextBox.append(info);
			parent.bomOptionDialog.getOptionsString(operation, index, TextBox);
		}

	}

	public void FillInfoBoxBop(String bopName) {
		String info;
		ExtendedEdge ee = (ExtendedEdge) hEdges.get(bopName);
		WitBopEdge wbe = (WitBopEdge) ee.data;

		String label = wbe.getNiceName();
		String operation, part;
		operation = wbe.getOperation().getText();
		part = wbe.getPart().getText();
		info = label + "\n" + "\n";
		TextBox.setText(info);
		Vector indices = wo.getBopEntryIndex(operation, part);
		for (int i = 0; i < indices.size(); i++) {
			int bopindex = ((Integer) indices.elementAt(i)).intValue();
			String header = "Bop Entry Index: " + bopindex + "\n";
			TextBox.append(header);
			parent.bopOptionDialog.getOptionsString(operation, bopindex,
					TextBox);
			TextBox.append("\n\n");
		}

	}

	public void FillInfoBoxDemandEdge(String d) {
		ExtendedEdge e = (ExtendedEdge) hEdges.get(d);
		WitDemandEdge wde = (WitDemandEdge) e.data;
		WitDemandNode demand = wde.getDemand();
		FillInfoBoxDemand(demand.createKey());

	}

	public void FillInfoBoxDemand(String d) {
		Node n = (Node) hNodes.get(d);
		WitDemandNode wdn = (WitDemandNode) n.data;
		String demand = wdn.getDemandName();
		String part = wdn.getPartName();

		String label = wdn.getNiceName();

		String info = label + "\n";

		TextBox.setText(info);
		parent.demandEdgeOptionDialog.getOptionsString(demand, part, TextBox);
	}

	public void FillInfoBoxOperation(String operationName) {
		String info;
		// String key = WitNode.createKey("operation",operationName);
		// Node n = (Node)hNodes.get(key);
		// WitNode wn = (WitNode)n.data;
		// String label = wn.getNiceName();
		// info = label + "\n";
		info = "Operation: " + operationName + "\n";
		TextBox.setText(info);
		parent.operationOptionDialog.getOptionsString(operationName, TextBox);

	}

	public void FillInfoBoxPart(String partName, String type) {
		String info;
		// String key = WitNode.createKey(type,partName);
		// Node n = (Node)parent.mainContents.hNodes.get(key);
		// WitNode wn = (WitNode)n.data;
		// String label = wn.getNiceName();
		// info = label + "\n";
		//ScrollBar sb = TextBox.getVerticalBar();
		//int pos = sb.getSelection();
		info = "Part: " + partName + "\n";
		TextBox.setText(info);
		parent.partOptionDialog.getOptionsString(partName, TextBox);
		//sb = TextBox.getVerticalBar();
		//sb.setSelection(pos);
	}

	public void recenter(Object o) {
		String sType, sName;
		if (o.getClass().toString().equals("class witviz.WitNode")
				|| o.getClass().toString().equals(
						"class witviz.WitNodeInvisible")
				|| o.getClass().toString().equals("class witviz.WitDemandNode")) {
			sType = ((WitNode) o).getNodeType();
			sName = ((WitNode) o).getText();
		} else {
			sType = ((WitEdge) o).getEdgeType();
			sName = ((WitEdge) o).getText();
		}

		// also want to recenter the main canvas on the focused object
		String key = "";
		float x, y;
		x = 0;
		y = 0;
		if (sType.equals("operation")
				|| (sType.equals("CAPACITY") || (sType.equals("MATERIAL")))) {
			key = WitNode.createKey(sType, sName);
			Node n = (Node) hNodes.get(key);
			x = n.x;
			y = n.y;
		} else if (sType.equals("demand")) {
			String d = ((WitDemandNode) o).getDemandName();
			String p = ((WitDemandNode) o).getPartName();
			key = WitDemandNode.createKey(d, p);
			Node n = (Node) hNodes.get(key);
			x = n.x;
			y = n.y;
		} else if (sType.equals("bop")
				|| (sType.equals("bom") || (sType.equals("demandedge")))) {
			key = sName;
			Edge e = (Edge) hEdges.get(key);
			x = (float) (e.start.x + e.end.x) / (float) 2.;
			y = (float) (e.start.y + e.end.y) / (float) 2.;
		}

		ScrollBar v = myCanvas.getVerticalBar(); // .setSelection(ycenter);
		ScrollBar h = myCanvas.getHorizontalBar(); // .setSelection(xcenter);

		int vmin = v.getMinimum();
		int vmax = v.getMaximum();
		int hmin = h.getMinimum();
		int hmax = h.getMaximum();

		int vsize = v.getThumb();
		int hsize = h.getThumb();

		int vpos = (int) y - vsize / 2;
		int hpos = (int) x - hsize / 2;

		if (vpos < 0)
			vpos = 0;
		if (hpos < 0)
			hpos = 0;

		v.setSelection(vpos);
		h.setSelection(hpos);

		Event e = new Event();
		e.type = SWT.Selection;

		v.notifyListeners(SWT.Selection, e);
		h.notifyListeners(SWT.Selection, e);

	}

	public void recenter(String sType, String sName) {

		// also want to recenter the main canvas on the focused object
		String key = "";
		float x, y;
		x = 0;
		y = 0;
		if (sType.equals("operation")
				|| (sType.equals("CAPACITY") || (sType.equals("MATERIAL")))) {
			key = WitNode.createKey(sType, sName);
			Node n = (Node) hNodes.get(key);
			x = n.x;
			y = n.y;
		}

		else if (sType.equals("bop")
				|| (sType.equals("bom") || (sType.equals("demandedge")))) {
			key = sName;
			Edge e = (Edge) hEdges.get(key);
			x = (float) (e.start.x + e.end.x) / (float) 2.;
			y = (float) (e.start.y + e.end.y) / (float) 2.;
		}

		ScrollBar v = myCanvas.getVerticalBar(); // .setSelection(ycenter);
		ScrollBar h = myCanvas.getHorizontalBar(); // .setSelection(xcenter);

		int vmin = v.getMinimum();
		int vmax = v.getMaximum();
		int hmin = h.getMinimum();
		int hmax = h.getMaximum();

		int vsize = v.getThumb();
		int hsize = h.getThumb();

		int vpos = (int) y - vsize / 2;
		int hpos = (int) x - hsize / 2;

		if (vpos < 0)
			vpos = 0;
		if (hpos < 0)
			hpos = 0;

		v.setSelection(vpos);
		h.setSelection(hpos);

		Event e = new Event();
		e.type = SWT.Selection;

		v.notifyListeners(SWT.Selection, e);
		h.notifyListeners(SWT.Selection, e);

	}

	public void setSelected(Object w) {
		if (w.getClass().toString().equals("class witviz.WitNode")
				|| (w.getClass().toString()
						.equals("class witviz.WitDemandNode")))
			setSelectedWitNode((WitNode) w);
		else
			setSelectedWitEdge((WitEdge) w);
	}

	public void setSelectedWitNode(WitNode w) {
		ResetToWhite();
		// SelectedName = w.getText();
		// SelectedType = w.getNodeType();
		String sName = w.getText();
		String sType = w.getNodeType();

		if (isMaster) {
			parent.detailLast=false;
			// make sure we are getting the right one
			
			String key="";
			if (sType == "demand")
				key=((WitDemandNode) w).createKey();
			else if (sType == "invisible")
				key=((WitNodeInvisible) w).createKey();
			else
				key= WitNode.createKey(sType, sName);
			
			Node n= (Node)hNodes.get(key);

			WitNode nw = (WitNode) n.data;
			parent.focusObject = nw;
			parent.focusChanged();
			if (sType == "MATERIAL" || sType == "part" || sType == "demand"
					|| sType == "operation") {
				ColorSelfNode(nw);
				ColorFamilyNode(nw);
			}
			FillInfoBox(sType, sName);
		} else {
			parent.detailLast=true;

			if (parent.fulltree) {
				parent.mainContents.ResetToWhite();
				if (sType == "MATERIAL" || sType == "CAPACITY"
						|| sType == "demand" || sType == "operation") {
					// need to get the appropriate node from the parent object
					String key = w.createKey();
					Node pn = (Node) parent.mainContents.hNodes.get(key);
					parent.mainContents.ColorSelfNode((WitNode) pn.data);
					parent.mainContents.ColorFamilyNode((WitNode) pn.data);
				}
			}
			ColorSelfNode(w);
			FillInfoBox(sType, sName);
			this.SelectedObject = w; //DLG
		}
	}

	public void setSelectedWitEdge(WitEdge w) {
		ResetToWhite();
		// SelectedName = w.getText();
		// SelectedType = w.getEdgeType();
		String sName = w.getText();
		String sType = w.getEdgeType();

		if (isMaster) {
			parent.focusObject = w;
			parent.focusChanged();
			ColorSelfEdge(w);
			ColorFamilyEdge(w);
			FillInfoBox(sType, sName);
		} else {
			if (parent.fulltree) {
				parent.mainContents.ResetToWhite();
				parent.mainContents.ColorSelfEdge(w);
				parent.mainContents.ColorFamilyEdge(w);
			}
			ColorSelfEdge(w);
			FillInfoBox(sType, sName);
		}
	}

	public void doToolTip(Figure f, Dimension pt) {

		String name = "";
		if (f.getClass().toString().equals("class witviz.WitNode")) {
			name = ((WitNode) f).getNiceName();
		} else if (f.getClass().toString().equals("class witviz.WitBomEdge")) {
			name = ((WitBomEdge) f).getNiceName();
		} else if (f.getClass().toString().equals(
				"class witviz.WitBomLowerFragmentEdge")) {
			name = ((WitBomLowerFragmentEdge) f).getNiceName();
		} else if (f.getClass().toString().equals(
				"class witviz.WitBomUpperFragmentEdge")) {
			name = ((WitBomUpperFragmentEdge) f).getNiceName();
		} else if (f.getClass().toString().equals("class witviz.WitDemandEdge")) {
			name = ((WitDemandEdge) f).getNiceName();
		} else if (f.getClass().toString().equals("class witviz.WitDemandNode")) {
			name = ((WitDemandNode) f).getNiceName();
		} else if (f.getClass().toString().equals("class witviz.WitBopEdge")) {
			name = ((WitBopEdge) f).getNiceName();
		} else if (f.getClass().toString().equals(
				"class witviz.WitSubstituteEdge")) {
			name = ((WitSubstituteEdge) f).getNiceName();
		}
		Label l = new Label();
		l.setText(name);
		f.setToolTip(l);

	}

	public String GetFigureLabel(String t, String n) {
		String val = "Type: " + t + " Name: " + n;
		return val;
	}

	public void refocus(Figure f) {

		// check the class of the object; either a WitNode or a WitEdge
		if (f.getClass().toString().equals("class witviz.WitNode")) {
			// SelectedName = ((WitNode) f).getText();
			// SelectedType = ((WitNode) f).getNodeType();
			setSelectedWitNode((WitNode) f);
		} else if (f.getClass().toString().equals("class witviz.WitDemandNode")) {
			// SelectedName = ((WitDemandNode) f).getText();
			// SelectedType = ((WitDemandNode) f).getNodeType();
			setSelectedWitNode((WitNode) f);
		} else if (f.getClass().toString().equals("class witviz.WitDemandEdge")) {
			// SelectedName = ((WitDemandEdge) f).getText();
			// SelectedType = ((WitDemandEdge) f).getEdgeType();
			setSelectedWitEdge((WitEdge) f);
		} else if (f.getClass().toString().equals("class witviz.WitBomEdge")) {
			// SelectedName = ((WitBomEdge) f).getText();
			// SelectedType = ((WitBomEdge) f).getEdgeType();
			setSelectedWitEdge((WitEdge) f);
			// setSelectedWitBomEdge((WitBomEdge)f);
		} else if (f.getClass().toString().equals("class witviz.WitBopEdge")) {
			// SelectedName = ((WitBopEdge) f).getText();
			// SelectedType = ((WitBopEdge) f).getEdgeType();
			setSelectedWitEdge((WitEdge) f);
			// setSelectedWitBopEdge((WitBopEdge)f);
		} else if (f.getClass().toString().equals(
				"class witviz.WitBomLowerFragmentEdge")) {
			// SelectedName = ((WitBomLowerFragmentEdge) f).getText();
			// SelectedType = ((WitBomLowerFragmentEdge) f).getEdgeType();
			setSelectedWitEdge((WitEdge) f);
			// setSelectedWitBomLowerFragmentEdge((WitBomLowerFragmentEdge)f);
		} else if (f.getClass().toString().equals(
				"class witviz.WitBomUpperFragmentEdge")) {
			// SelectedName = ((WitBomUpperFragmentEdge) f).getText();
			// SelectedType = ((WitBomUpperFragmentEdge) f).getEdgeType();
			setSelectedWitEdge((WitEdge) f);
			// setSelectedWitBomUpperFragmentEdge((WitBomUpperFragmentEdge)f);
		} else if (f.getClass().toString().equals(
				"class witviz.WitSubstituteEdge")) {
			// SelectedName = ((WitSubstituteEdge) f).getText();
			// SelectedType = ((WitSubstituteEdge) f).getEdgeType();
			setSelectedWitEdge((WitEdge) f);
			// setSelectedWitSubstituteEdge((WitSubstituteEdge)f);
		}

		else {
			// SelectedType = "";
			// SelectedName = "";
			SelectedObject = null;
		}
		// if (SelectedType.equals("invisible")) {
		// get the bom above it FOOBAR
		// String operation = operationFromInvisibleName(SelectedName);
		// String part = partFromInvisibleName(SelectedName);
		// String parttype = partTypeFromInvisibleName(SelectedName);
		// int index = indexFromInvisibleName(SelectedName);
		// String BomName =
		// ExtendedEdge.edgeKeyFromSourceTargetIndex(operation,"operation",SelectedName,"invisible",index);
		// SelectedName = BomName;
		// SelectedType = "bom";
		// }

	}

	class Selector extends MouseMotionListener.Stub implements MouseListener,
			MouseMotionListener {
		// Color familyColor = ColorConstants.cyan;
		public Selector(IFigure figure) {
			figure.addMouseListener(this);
			figure.addMouseMotionListener(this);
		}

		public void mouseReleased(MouseEvent e) {
			// which button?
			parent.setWait();

			if (e.button == 3) {
				if (!isMaster) {
					Figure f = (Figure) e.getSource();
					e.consume();
					getExpandedDetailContents(f);
				}
			} else if (e.button == 1) {
				Figure f = (Figure) e.getSource();
				e.consume();
				refocus(f);
			}
			parent.unsetWait();

		}

		public void mouseHover(MouseEvent event) {
			Dimension pt = new Dimension(event.x, event.y);
			Figure f = (Figure) event.getSource();
			doToolTip(f, pt);

		}

		public void mouseExit(MouseEvent e) {
			// if (tipShell.isVisible()) tipShell.setVisible(false);
			// tipWidget = null;
		}

		public void mouseEnter(MouseEvent e) {

		}

		public void mouseClicked(MouseEvent e) {
		}

		public void mouseDoubleClicked(MouseEvent e) {
			if (!isMaster && e.button == 1) {
				Figure f = (Figure) e.getSource();
				e.consume();
				parent.RefocusButtonPushed(f);
			}
		}

		public void mousePressed(MouseEvent e) {

		}

	}

	public static Image getCapacityIcon() {
		return capacityIcon;
	}

	public static void setCapacityIcon(Image capacityIcon) {
		CreateTreeFigure.capacityIcon = capacityIcon;
	}

	public static Image getDemandIcon() {
		return demandIcon;
	}

	public static void setDemandIcon(Image demandIcon) {
		CreateTreeFigure.demandIcon = demandIcon;
	}

	public static Image getOpIcon() {
		return opIcon;
	}

	public static void setOpIcon(Image opIcon) {
		CreateTreeFigure.opIcon = opIcon;
	}

	public static Image getPartIcon() {
		return partIcon;
	}

	public static void setPartIcon(Image partIcon) {
		CreateTreeFigure.partIcon = partIcon;
	};

}
