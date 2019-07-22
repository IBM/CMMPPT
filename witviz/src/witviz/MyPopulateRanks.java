/*
 * Created on Nov 19, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;

import org.eclipse.draw2d.graph.DirectedGraph;
import org.eclipse.draw2d.graph.Edge;
import org.eclipse.draw2d.graph.Node;
import org.eclipse.draw2d.graph.NodeList;
import org.eclipse.draw2d.graph.RankList;
import org.eclipse.draw2d.graph.VirtualNode;
import org.eclipse.draw2d.internal.graph.PopulateRanks;



/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MyPopulateRanks extends PopulateRanks {
	private DirectedGraph g;
	public void visit(DirectedGraph g) {
		this.g = g;
		g.ranks = new RankList();
		for (int i = 0; i < g.nodes.size(); i++) {
			Node node = g.nodes.getNode(i);
			g.ranks.getRank(node.rank).add(node);
		}
		for (int i = 0; i < g.nodes.size(); i++) {
			Node node = g.nodes.getNode(i);
			for (int j = 0; j < node.outgoing.size(); j++) {
				Edge e = node.outgoing.getEdge(j);
				if (e.getLength() > 1)
					addVirtualNodes(e);
				//if (e.isFeedback)
				//	addVirtualNodesFeedback(e);
			}
		}
	}


	void addVirtualNodesFeedback(Edge e) {
		int start = e.source.rank-1;
		int end = e.target.rank+1;
		VirtualNode previous = null;
		VirtualNode current = null;
		e.vNodes = new NodeList();
		for (int i = start; i < end; i++) {
			current = createVirtualNode(e, i);
			if (previous == null)
				current.prev = e.source;
			else {
				previous.next = current;
				current.prev = previous;
			}
			g.ranks.getRank(i).add(current);
			e.vNodes.add(current);
			previous = current;
		}
		current.next = e.target;
	}
	void addVirtualNodes(Edge e) {
		int start = e.source.rank + 1;
		int end = e.target.rank;
		VirtualNode previous = null;
		VirtualNode current = null;
		e.vNodes = new NodeList();
		for (int i = start; i < end; i++) {
			current = createVirtualNode(e, i);
			if (previous == null)
				current.prev = e.source;
			else {
				previous.next = current;
				current.prev = previous;
			}
			g.ranks.getRank(i).add(current);
			e.vNodes.add(current);
			previous = current;
		}
		current.next = e.target;
	}
	VirtualNode createVirtualNode(Edge e, int i) {
		return new VirtualNode(e, i);
	}

}
