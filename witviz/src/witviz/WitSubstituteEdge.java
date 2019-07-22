/*
 * Created on Nov 24, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;


/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class WitSubstituteEdge extends WitEdge {
    private WitNodeInvisible invisible;
	private WitNode part;
	private int subsEntry;
	public WitSubstituteEdge() {
		super();
		edgeType = "substitute";
	}
	public void setSubsEntry(int s){
		subsEntry = s;
	}
	public int getSubsEntry() {
		return subsEntry;
	}
	public void setInvisibleNode(WitNodeInvisible w) {
		invisible = w;
	}
	public WitNodeInvisible getInvisibleNode() {
		return invisible;
	}
	public void setPart(WitNode p) {
		part = p;
	}
	public WitNode getPart() {
		return part;
	}
	public String createKey() {
		String in;
		String key = createKey(invisible.createKey(),subsEntry);
		text =key;
	    return key;
	}
	public static String createKey(String in, int entry) {
		String key = "substitute edge: " + " invisible node: " + in + " subsEntry" + entry;
	    return key;
	}
	public String getSource() {
		return invisible.getText();
	}
	public String getTarget() {
		return part.getText();
	}
	//override getNiceName
	public String getNiceName() {
		String s;
		String mainpart, operation;
		operation = invisible.getOperation().getText();
		mainpart = invisible.getPart().getText();
		s = "Substitute part: " + part.getText() + " for " + mainpart +" in Operation: " + operation + "; subsEntry: " + subsEntry;
	    return s;
	}
	

}
