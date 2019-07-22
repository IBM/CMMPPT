package witviz;



import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Shell;

public class ChooseScenarioDialog extends ChooseFromMultiple {

	public ChooseScenarioDialog(Shell parent, SimpleTree p) {
		super(parent, p, "Current Scenario");
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
				       String which = str[0];
					   int whichint=Integer.parseInt(which);
					   app.UpdateScenario(whichint);
					   persistent=whichint;
				     }
				  }
				 );
		
	}
	public void updateShell() {
		WitObject wo = app.wo;
		int numScenarios = wo.getNumScenarios();
		
		list.removeAll();
		for (int i=0; i<numScenarios; i++) {
			String ii=i+"";
			list.add(ii);
		}
				
	}

}
