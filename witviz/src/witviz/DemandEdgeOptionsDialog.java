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
public class DemandEdgeOptionsDialog extends OptionsDialog {
	Vector listOfDemandEdgeOptions;

	public DemandEdgeOptionsDialog(Shell parent, SimpleTree p) {
		super(parent,p,"Demand");
	}


	public void fillShell() {
		listOfDemandEdgeOptions= wo.getListOfDemandEdgeAttributes();

		for (Iterator i=listOfDemandEdgeOptions.iterator();i.hasNext();) {
			Attribute option = (Attribute)i.next();
			addOptionButton(option.toString());
		}
    
	}
    public void getOptionsString(String demandName, StyledText t) {
    	//not used
    }
    public void setEnabled() {
    	//don't need to do anything;
    	
    		Button b = (Button)buttonTable.get("cumShipReward");
    		b.setEnabled(true);
    		b = (Button)buttonTable.get("shipReward");
    		b.setEnabled(true);

    	
    }
	public void getOptionsString(String demandName, String partName,StyledText textBox) {
		
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
		for (Iterator i=listOfDemandEdgeOptions.iterator();i.hasNext();) {
			Attribute attribute = (Attribute)i.next();

			if (optionDesired(attribute.toString())) {
				if (!wo.isDemandEdgeCompatible(attribute,demandName,partName)) {
					HIGHLIGHT highLight=HIGHLIGHT.INVALID;
					String attrName=attribute.toString();
					AppendText(textBox,highLight,attrName+": \n");
				}
				else {
					String demandEdgeValue = wo.GetDemandEdgeAttributeDescription(demandName, partName, attribute);
					HIGHLIGHT highLight = wo.GetDemandEdgeAttributeInteresting(demandName, partName, attribute);
					AppendText(textBox,highLight,demandEdgeValue);
				}
			}
		}
	}
	

}
