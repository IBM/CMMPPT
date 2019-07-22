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
public class OperationOptionsDialog extends OptionsDialog {
	Vector listOfOperationAttributes;


	public OperationOptionsDialog(Shell parent,SimpleTree p) {
		super(parent,p,"Operation");

	}


	public void setEnabled() {
		//if (parentApp.outputData) {
		
		
		//boolean isStochMode = wo.getStochMode();
    	for (Iterator i = listOfOperationAttributes.iterator(); i.hasNext();) {
	      Attribute attr = (Attribute)i.next();
	      String name = attr.toString();
          Button b = (Button)buttonTable.get(name);
          //if (!wo.isCompatible(attr))
        	//  b.setEnabled(false);
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
	public void fillShell() {
		listOfOperationAttributes= wo.getListOfOperationAttributes();

		for (Iterator i=listOfOperationAttributes.iterator();i.hasNext();) {
			Attribute option = (Attribute)i.next();
			addOptionButton(option.toString());
		}
		setEnabled();
	}

	public void getOptionsString(String operationName, StyledText textBox) {
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

		
		for (Iterator i=listOfOperationAttributes.iterator();i.hasNext();) {
			Attribute attribute = (Attribute)i.next();

			if (optionDesired(attribute.toString())) {
				if (!wo.isOperationCompatible(attribute,operationName)) {
					HIGHLIGHT highLight=HIGHLIGHT.INVALID;
					String attributeName = attribute.toString();
					AppendText(textBox,highLight,attributeName+": \n");
				}
				else {
					String attributeValue = wo.GetOperationAttributeDescription(operationName, attribute);
					HIGHLIGHT highLight = wo.GetOperationAttributeInteresting(operationName, attribute);
					AppendText(textBox,highLight,attributeValue);
				}
			}
		}
	}

		

}
