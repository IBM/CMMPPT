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
public class PartOptionsDialog extends OptionsDialog {
	Vector listOfPartOptions;

	public PartOptionsDialog(Shell parent,SimpleTree p) {
		super(parent,p,"Part");

	}


	public void fillShell() {
		
		listOfPartOptions= wo.getListOfPartAttributes();

		for (Iterator i=listOfPartOptions.iterator();i.hasNext();) {
			Attribute option = (Attribute)i.next();
			addOptionButton(option.toString());
		}
		setEnabled();
    

	}


	public void setEnabled() {

		
    	//boolean isStochMode = wo.getStochMode();
    	
    	for (Iterator i = listOfPartOptions.iterator(); i.hasNext();) {
	      Attribute attr = (Attribute)i.next();
	      
	      String name = attr.toString();
          Button b = (Button)buttonTable.get(name);
          //if (!wo.isCompatible(attr))
        	  //b.setEnabled(false);
          //else 
        	  b.setEnabled(true);
        }
    	
    	//boolean stageByObject = wo.GetStageByObject();

     	//Button bO = (Button)buttonTable.get("objectStage");
    	//if (stageByObject) {
    	//	bO.setEnabled(true);
    	//}
    	//else {
    	//	bO.setEnabled(false);
    	//}
	}

	public void getOptionsString(String partName, StyledText textBox) {
		if (wo.getStochMode()) {
			int currentScenario=wo.getCurrentScenario();
			String currentScenarioString = "Current Scenario: "+currentScenario+"\n";
			AppendText(textBox,HIGHLIGHT.HIGHLIGHT,currentScenarioString);
		}
		if (wo.getMultObjectiveMode()) {
			String currentObjective=wo.getCurrentObjective();
			String currentObjectiveString = "Current Objective: "+currentObjective+"\n";
			textBox.append(currentObjectiveString);
		}
	
		//put the category at the top
		String category = wo.getPartCategory(partName);
		AppendText(textBox,HIGHLIGHT.HIGHLIGHT,category+"\n");
		for (Iterator i=listOfPartOptions.iterator();i.hasNext();) {
			Attribute attribute = (Attribute)i.next();
			
			if (optionDesired(attribute.toString())) {
				if (!wo.isPartCompatible(attribute,partName)) {
					String attributeName = attribute.toString();
					HIGHLIGHT highLight=HIGHLIGHT.INVALID;
					AppendText(textBox,highLight,attributeName+":\n");
				}
				else {
					String attributeValue = wo.GetPartAttributeDescription(partName, attribute);
					HIGHLIGHT highLight = wo.GetPartAttributeInteresting(partName, attribute);
					AppendText(textBox,highLight,attributeValue);
				}
			}
		}
	}
	
    

}
