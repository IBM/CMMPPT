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
import org.eclipse.swt.widgets.Shell;

import com.ibm.witj.Attribute;
/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class BopOptionsDialog extends OptionsDialog {
	Vector listOfBopAttributes;

	/**
	 * @param parent
	 */
	public BopOptionsDialog(Shell parent, SimpleTree p) {
		super(parent,p,"BOP");
	}
	public void setEnabled() {
		//don't need to do anything
	}


	public void fillShell() {
		listOfBopAttributes= wo.getListOfBopAttributes();

		for (Iterator i=listOfBopAttributes.iterator();i.hasNext();) {
			Attribute option = (Attribute)i.next();
			addOptionButton(option.toString());
		}	    
	

 	}
	public void getOptionsString(String opName, StyledText t) {
		//not used
	}

	public void getOptionsString(String opName,  int bopIndex, StyledText textBox) {

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
		
		for (Iterator i=listOfBopAttributes.iterator();i.hasNext();) {
			Attribute attribute = (Attribute)i.next();
			String bopValue = wo.GetBopAttributeDescription(opName, bopIndex, attribute);

			if (optionDesired(attribute.toString())) {
				if (!wo.isBopCompatible(attribute,opName,bopIndex)) {
					HIGHLIGHT highLight=HIGHLIGHT.INVALID;
					AppendText(textBox,highLight,bopValue+": \n");
				}
				else {
					HIGHLIGHT highLight = wo.GetBopAttributeInteresting(opName, bopIndex, attribute);
					AppendText(textBox,highLight,bopValue);
				}
			}
		}
	}

}
