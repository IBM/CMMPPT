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
public class WitBomLowerFragmentEdge extends WitEdge {
    private int bomEntry;
	private WitNode part;
	private WitNodeInvisible invisible;
	public WitBomLowerFragmentEdge() {
		super();
		edgeType = "bomlower";
    }
	public int getBomEntry(){
		return bomEntry;
	}
	public void setBomEntry(int b){
		bomEntry = b;
	}
	public void setPart(WitNode o){
		part = o;
	}
	public WitNode getPart() {
		return part;
	}
	public void setInvisible(WitNodeInvisible p) {
		invisible = p;
	}
	public WitNodeInvisible getInvisible() {
		return invisible;
	}
	public String createKey() {
		String key;
		String pa,in;
		pa = part.getText();
		in = invisible.createKey();
		key = createKey(in,pa,bomEntry);
		text = key;
	    return key;
	}
	public String getNiceName() {
		String key = "BOM: Operation: "+invisible.getOperation().getText()+ " Part: " + part.getText() + " BomEntry: "+ bomEntry;
        return key;
	}

	public static String createKey(String in, String pa, int entry) {
		String key;
		key = "Invisible: " + in + "Part: "+ pa + " bomEntry: " + entry;
	    return key;
	}

	public String getSource() {
		return invisible.getText();
	}
	public String getTarget() {
		return part.getText();
	}
	
	

}
