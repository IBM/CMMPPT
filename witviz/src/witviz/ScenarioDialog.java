package witviz;


import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Scale;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

public class ScenarioDialog extends Dialog {
    final Shell shell;
    Composite buttonC;
    Shell parent;
    SimpleTree app;
    Text leftText, rightText, valueText;
    Scale scale;
	public ScenarioDialog(Shell parent, SimpleTree p) {
		super(parent);
		app=p;
		// TODO Auto-generated constructor stub
	    shell= new Shell(parent, SWT.DIALOG_TRIM );
	    initializeShell();

	}
	public void initializeShell() {
		

		parent = getParent();
		RowLayout rowLayout = new RowLayout();
		
		shell.setLayout(new FillLayout(SWT.VERTICAL));
		
		
		Composite sliderRow = new Composite(shell,SWT.BORDER); 
		sliderRow.setLayout(rowLayout);
		    
		    
		    
		leftText = new Text(sliderRow, SWT.CENTER);	    
	    Button leftBut = new Button(sliderRow, SWT.ARROW | SWT.LEFT);
	    scale = new Scale (sliderRow, SWT.HORIZONTAL);
	    scale.setPageIncrement(1);
	    Button rightBut = new Button(sliderRow, SWT.ARROW | SWT.RIGHT);	    
		rightText = new Text(sliderRow, SWT.CENTER);
		scale.setIncrement(1);
		valueText = new Text(shell, SWT.CENTER);
		
		scale.addListener (SWT.Selection, new Listener () {
			public void handleEvent (Event event) {
				int selection = scale.getSelection();
				int min = scale.getMinimum();
				int max = scale.getMaximum();
				int scenario = selection;
				String scenarioString = "scenario "+scenario;
				valueText.setText(scenarioString);
				app.UpdateScenario(scenario);
			}
		});
		
		Listener leftlistener =new Listener () {
			public void handleEvent (Event event) {
				int currentValue = scale.getSelection();
				int min = scale.getMinimum();
				if (currentValue>min) {
					scale.setSelection(currentValue-1);
					int newScenario=currentValue-1;
					String scenarioString = "scenario "+newScenario;
					valueText.setText(scenarioString);
					app.UpdateScenario(newScenario);
				}
			}
		};
		leftBut.addListener (SWT.Selection, leftlistener);
	
		Listener rightlistener =new Listener () {
			public void handleEvent (Event event) {
				int currentValue = scale.getSelection();
				int max = scale.getMaximum();
				if (currentValue<max) {
					int newScenario= currentValue+1;
					scale.setSelection(newScenario);
					String scenarioString = "scenario "+newScenario;
					valueText.setText(scenarioString);
					app.UpdateScenario(newScenario);
				}
			}
		};
		rightBut.addListener (SWT.Selection, rightlistener);

		



		
		shell.addListener(SWT.Close, new Listener(){
			   public void handleEvent(Event event) {
			       event.doit=false;
			       close();
			       //your code here

			    }
			   });	   

 
		shell.pack ();
		shell.setVisible(false);
	}
	public void updateShell() {
		//get the min and max from the witobject
		WitObject wo = app.wo;
		int numScenarios = wo.getNumScenarios();
		
		int intmin= 0;
		int intmax= numScenarios-1;
		
		String min=""+intmin;
		String max=""+intmax;
		leftText.setText(min);
	    
	    scale.setMaximum(intmax);
	    scale.setMinimum(intmin);
	    scale.setPageIncrement(1);
	    scale.setSelection(intmin);
		rightText.setText(max);
		
		int scenario = intmin;
		valueText.setText("scenario "+ min);
		app.UpdateScenario(scenario);
		
		
	}

	public void open () {
		//get the relevant info
		WitObject wo = app.wo;
		int numScenarios = wo.getNumScenarios();
		shell.setVisible(true);

		Display display = parent.getDisplay();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) display.sleep();
		}

	}
	public void close() {
		//just make invisible
		shell.setVisible(false);
	}
}
