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
public class SubstituteOptionsDialog extends OptionsDialog {
	Vector listOfSubstituteAttributes;

	public SubstituteOptionsDialog(Shell parent,SimpleTree p) {
		super(parent,p,"Substitute");

	}
	public void fillShell() {
		listOfSubstituteAttributes= wo.getListOfSubstituteAttributes();

		for (Iterator i=listOfSubstituteAttributes.iterator();i.hasNext();) {
			Attribute option = (Attribute)i.next();
			addOptionButton(option.toString());
		}		
	}
    public void getOptionsString(String name, StyledText t) {
    	//not used

    }
    public void setEnabled() {
    	//if(parentApp.outputData) {
		
    	//}
 
    }
	public  void getOptionsString(String operationName,   int bomIndex, int subsBomIndex, StyledText textBox) {
	
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

		for (Iterator i=listOfSubstituteAttributes.iterator();i.hasNext();) {
			Attribute attribute = (Attribute)i.next();
			String subsValue = wo.GetSubstituteAttributeDescription(operationName, bomIndex, subsBomIndex, attribute);

			if (optionDesired(attribute.toString())) {
				if (!wo.isSubstituteCompatible(attribute,operationName,bomIndex,subsBomIndex)) {
					String attributeValue = wo.GetPartAttributeDescription(operationName, attribute);
					HIGHLIGHT highLight=HIGHLIGHT.INVALID;
					AppendText(textBox,highLight,attributeValue+": \n");
				}
				else {
					HIGHLIGHT highLight = wo.GetSubstituteAttributeInteresting(operationName, bomIndex, subsBomIndex, attribute);
					AppendText(textBox,highLight,subsValue);
				}
			}
		}
	}
		
	//public void addOptionButton(String name, Shell parent)	{
	//    Button b = new Button(parent,SWT.CHECK);
	//    b.setText(name);
	//    buttonTable.put(name,b);
	//    Button b1 = (Button)buttonTable.get(name);
	//	b.addSelectionListener(new SelectionListener() {
	//		public void widgetSelected(SelectionEvent e) {
				//int foo=1;
				//Button b = (Button)e.widget;
				//boolean isSelected = b.getSelection();
				//String text  = b.getText();
	//		}
	//	    public void widgetDefaultSelected(SelectionEvent e) {
	//		    int foo=1;
	//		}
	//	});

	//}

}
