package witviz;





import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Text;

abstract public class ChooseFromMultiple extends Dialog {
    final Shell shell;
 
    Shell parent;
    SimpleTree app;
    List list;
    int persistent;
	public ChooseFromMultiple(Shell parent, SimpleTree p, String label) {
		super(parent);
		app=p;
		
	    //shell= new Shell(parent, SWT.DIALOG_TRIM|SWT.RESIZE);
	    shell= new Shell(parent, SWT.DIALOG_TRIM  );

	    initializeShell(label);

	}
	public void initializeShell(String label) {
		persistent=0;
		parent = getParent();
		//shell.setSize(300,400);
		
	    GridLayout gridLayout1 = new GridLayout();
	    gridLayout1.numColumns = 1;
	    shell.setLayout(gridLayout1);
	    
	    Text textlabel = new Text(shell,0);
	    textlabel.setText(label);
	    
	    Composite listComposite = new Composite(shell,0);
	    listComposite.setLayout(new RowLayout());
		
	    list = new List(listComposite,SWT.SINGLE|SWT.V_SCROLL|SWT.CENTER);
		//list.setSize(400,400);
		fillShell();
		//Rectangle clientArea = shell.getClientArea ();
		//list.setBounds (clientArea.x, clientArea.y, 100, 100);

		Composite composite = new Composite(shell,0);
		composite.setLayout (new RowLayout ());

		final Button ok = new Button (composite, SWT.PUSH);
		ok.setText ("Ok");
		Button cancel = new Button (composite, SWT.PUSH);
		cancel.setText ("Cancel");
		Listener listener =new Listener () {
			public void handleEvent (Event event) {
				//result [0] = event.widget == ok;
				shell.setVisible (false);
				
			}
		};
		ok.addListener (SWT.Selection, listener);
		cancel.addListener (SWT.Selection, listener);

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
	abstract public void updateShell();
	

	public void open () {
		//get the relevant info
		WitObject wo = app.wo;
		shell.setVisible(true);
		//shell.setSize(400,200);
		list.setSelection(persistent);
		Display display = parent.getDisplay();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) display.sleep();
		}

	}
	public void close() {
		//just make invisible
		shell.setVisible(false);
	}
	abstract public void fillShell();
	
}
