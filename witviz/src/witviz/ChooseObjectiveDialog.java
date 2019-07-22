package witviz;

import java.util.Iterator;
import java.util.Vector;

import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Shell;

public class ChooseObjectiveDialog extends ChooseFromMultiple {

	public ChooseObjectiveDialog(Shell parent, SimpleTree p) {
		super(parent, p, "Current Objective");
		// TODO Auto-generated constructor stub
	}

	@Override
	public void fillShell() {
		// TODO Auto-generated method stub
		updateShell();
		list.addSelectionListener(
				   new SelectionAdapter()
				   {
				     public void widgetSelected(SelectionEvent e)
				     {
				       List list = (List) e.getSource();
				       String[] str = list.getSelection();
				       int whichint = list.getSelectionIndex();
				       String which = str[0];
					   
					   app.UpdateObjective(which);
					   persistent=whichint;
				     }
				  }
				 );
		
	}
	public void updateShell() {
		WitObject wo = app.wo;
		Vector<String> objectives = wo.getObjectiveList();
		
		list.removeAll();
		for (Iterator i=objectives.iterator(); i.hasNext();) {
			String objective = (String)i.next();
			list.add(objective);
		}
				
	}

}
