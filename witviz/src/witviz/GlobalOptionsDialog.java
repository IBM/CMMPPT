/*
 * Created on Jan 7, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;


import java.util.Iterator;
import java.util.Vector;

import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Shell;

import com.ibm.witj.Attribute;

/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class GlobalOptionsDialog extends OptionsDialog {
	Vector listOfGlobalAttributes;

	public GlobalOptionsDialog(Shell parent,SimpleTree p) {
		super(parent,p,"Global");

	}


	public void fillShell() {
		listOfGlobalAttributes= wo.getListOfGlobalAttributes();

		for (Iterator i=listOfGlobalAttributes.iterator();i.hasNext();) {
			Attribute option = (Attribute)i.next();
			addOptionButton(option.toString());
		}	
		setEnabled();
	}


	public void setEnabled() {

	
    	for (Iterator i = listOfGlobalAttributes.iterator(); i.hasNext();) {
	      Attribute attr = (Attribute)i.next();
	      String name = attr.toString();
          Button b = (Button)buttonTable.get(name);
          b.setEnabled(true);
        }
	}
    public void getOptionsString(String s, StyledText t) {
    	//not used;
    }
	public void getOptionsString(StyledText textBox) {
		for (Iterator i=listOfGlobalAttributes.iterator();i.hasNext();) {
			Attribute attribute = (Attribute)i.next();
			if (optionDesired(attribute.toString())) {
				if (!wo.isProblemCompatible(attribute)) {
					String attributeValue = attribute.toString();
					AppendText(textBox,HIGHLIGHT.INVALID,attributeValue+":\n");
				}
				else {
					String subsValue = wo.GetGlobalAttributeDescription(attribute);
					HIGHLIGHT highLight = wo.GetGlobalAttributeInteresting(attribute);
					//just don't add currentScenario as I've already put it in
					if (attribute.toString().compareTo("currentScenario")!=0)
						AppendText(textBox,highLight,subsValue);
				}
		
			}
		}
	}
	

}
