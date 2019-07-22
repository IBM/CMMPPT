/*
 * Created on Nov 3, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;
import java.util.Vector;
/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class FullTree {
	private WitObject witObject;
	public FullTree(WitObject wo) {
		witObject = wo;
		FillTree();
	}
	private void FillTree() {
		//first get the raw materials
		Vector rawmaterials = witObject.getRawMaterialsList();
		
		//for each in the list, fill
		for (int i=0; i<rawmaterials.size(); i++) {
			ListInfo li = (ListInfo)rawmaterials.elementAt(i);
		}
	}

}
