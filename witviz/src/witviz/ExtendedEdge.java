/*
 * Created on Dec 8, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;

import java.util.Vector;

import org.eclipse.draw2d.graph.Edge;
import org.eclipse.draw2d.graph.Node;

/**
 * @author gresh
 * 
 * TODO To change the template for this generated type comment go to Window -
 * Preferences - Java - Code Style - Code Templates
 */
public class ExtendedEdge extends Edge {
	private Vector bomindexlist = new Vector();

	public WitEdge data;

	String name;

	int Index;

	// int subsIndex;
	String type;

	public ExtendedEdge(Node source, Node target, int index) {
		super(source, target);
		Index = index; // bomIndex for normal; unnecessary for subs
		// name = getKey();
		// String foo=name;
	}

	public int getIndex() {
		return Index;
	}

	public void setData(WitEdge p) {
		data = p;
		name = p.createKey();
	}

	// public String getKey() {
	// WitNode wnS = (WitNode) this.source.data;
	// WitNode wnT = (WitNode) this.target.data;
	// String type = this.getType();

	// String edgeName = getEdgeName(wnS, wnT,Index);
	// return edgeName;
	// }
	// private static String getEdgeName(WitNode source, WitNode target, int
	// index) {

	// return edgeKeyFromSourceTargetIndex(source.getInternalName(), source
	// .getNodeType(), target.getInternalName(), target.getNodeType(), index);

	// }
	// static String edgeKeyFromSourceTargetIndex(String source, String stype,
	// String target, String ttype, int index) {
	// return "edge:" + "source: " + source + " target: " + target + "type1: "
	// + stype + "type2: " + ttype + "INDEX: " + index;
	//	

	// }

	// public int getBomIndex() {
	// return bomIndex;
	// }
	// public void setBomIndex(int i) {
	// bomIndex =i;
	// }
	// public int getSubsIndex() {
	// return subsIndex;
	// }
	// public void setSubsIndex(int i) {
	// subsIndex =i;
	// }

	public String getType() {
		return type;
	}

	public void setType(String t) {
		type = t;
	}

	public void addToEdge(int i) {
		Integer iObj = new Integer(i);
		bomindexlist.add(iObj);
	}

	public Vector getBomList() {
		return bomindexlist;
	}

	public String toString() {
		WitNode source = (WitNode) super.source.data;
		WitNode target = (WitNode) super.target.data;
		return "\"" + source.getText() + "-" + source.getNodeType()
				+ "\"=>\"" + target.getText() + "-" + target.getNodeType()+"\"";
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}
