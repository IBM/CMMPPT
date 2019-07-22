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
public class WitNodeInvisible extends WitNode {
	private  WitNode operation;
	private  WitNode part;
	private int bomEntry;
	public WitNodeInvisible() {
		super();
	}
    public String createKey() {
		String key;
		String op,pa;
		op = operation.getText();
		pa = part.getText();
		key = createKey(op,pa,bomEntry);
        return key;
	}
	public static String createKey(String op, String pa, int entry) {
		String key;
		key = "InvisibleNode: operation: "+op + " part: " + pa +" BomEntry: "+ entry;
		return key;
	}
	public void setOperation(WitNode o) {
		operation = o;
	}
	public WitNode getOperation() {
		return operation;
	}
	public void setPart(WitNode p) {
		part = p;
	}
	public WitNode getPart() {
		return part;
	}
	public void setBomEntry(int i){
		bomEntry = i;
	}
	public int getBomEntry() {
		return bomEntry;
	}
	public String getNiceName() {
		return createKey();
	}
}
