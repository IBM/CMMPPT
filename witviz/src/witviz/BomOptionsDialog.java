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
public class BomOptionsDialog extends OptionsDialog {
	Vector listOfBomAttributes;

     public BomOptionsDialog(Shell shell, SimpleTree p) {
    	super(shell,p, "BOM");
    }

	public void fillShell() {
		listOfBomAttributes= wo.getListOfBomAttributes();

		for (Iterator i=listOfBomAttributes.iterator();i.hasNext();) {
			Attribute option = (Attribute)i.next();
			addOptionButton(option.toString());
		}	    
	}
	public void setEnabled() {
		//nothing to do here
	}
	public void getOptionsString(String operationName, StyledText t) {
		//not used
	}

	public void getOptionsString(String operationName, int bomIndex, StyledText textBox) {
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
		
		for (Iterator<Attribute<String>> i=listOfBomAttributes.iterator();i.hasNext();) {
			Attribute<String> attribute = (Attribute<String>)i.next();
			String bomValue = wo.GetBomAttributeDescription(operationName, bomIndex, attribute);

			if (optionDesired(attribute.toString())) {
				if (!wo.isBomCompatible(attribute,operationName,bomIndex)) {
					HIGHLIGHT highLight=HIGHLIGHT.INVALID;
					AppendText(textBox,highLight,bomValue+": \n");
				}
				else {
					HIGHLIGHT highLight = wo.GetBomAttributeInteresting(operationName, bomIndex, attribute);
					AppendText(textBox,highLight,bomValue);
				}
			}
		}
	}
		

	
	




}
