package witviz;






import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Shell;

public class MyMessageDialog extends Dialog {
	Object result;
	String message;
	Shell myshell;

	public MyMessageDialog (Shell parent, int style) {
	super (parent, style);
	}
	public MyMessageDialog (Shell parent, String text) {
	this (parent, SWT.APPLICATION_MODAL | SWT.RESIZE); // your default style bits go here (not the Shell's style bits)
	message=text;
	
	}
	public void open () {
	Shell parent = getParent();
	//myshell = new Shell(parent, SWT.DIALOG_TRIM | SWT.APPLICATION_MODAL);
	myshell = new Shell(parent, SWT.DIALOG_TRIM );
	//myshell.setSize(350,20);


	myshell.setText("Attention");
	
	

    GridLayout gridLayout1 = new GridLayout();
    gridLayout1.numColumns = 1;
    myshell.setLayout(new FormLayout());
    FormData fd1 = new FormData();
    fd1.top = new FormAttachment(0,5);
    fd1.left = new FormAttachment(0,5);
    fd1.right = new FormAttachment(100,-5);
    fd1.bottom = new FormAttachment(100,-5);
    
	StyledText textBox = new StyledText (myshell, SWT.LEAD|SWT.MULTI|SWT.WRAP|SWT.H_SCROLL|SWT.V_SCROLL|SWT.BORDER);
	textBox.setText (message);
	

	textBox.setLayoutData(fd1);
	myshell.pack ();
	myshell.setSize(350,100);
	// Your code goes here (widget creation, set result, etc).
	myshell.open();
	//Display display = parent.getDisplay();
	//while (!myshell.isDisposed()) {
	//	if (!display.readAndDispatch()) display.sleep();
	//}
	//return result;
	}
	public void close () {
		myshell.close();
	}
	}