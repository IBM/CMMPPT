/*
 * Created on Nov 24, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;

import org.eclipse.draw2d.PolylineConnection;


/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
abstract public class WitEdge extends PolylineConnection {
	protected String edgeType;
	protected String text;
	protected String source;
	protected String target;
	private int defaultWidth;
	public WitEdge() {
		super();
		setDefaultWidth(2);
		setLineWidth(getDefaultWidth());
		
	}


	public void setDefaultWidth(int w) {
		defaultWidth=w;
		setLineWidth(w);
	}
	public int getDefaultWidth() {
		return defaultWidth;
	}

	public String getText() {
		return text;	
	}
	public void setText(String t) {
		text = t;
	}
	public String getEdgeType() {
		return edgeType;		
	}
	public void setEdgeType(String t) {
		edgeType = t;
	}
	abstract public String createKey();
	abstract public String getSource();
	abstract public String getTarget();
	public String getNiceName() {
		return createKey();
	}

}
