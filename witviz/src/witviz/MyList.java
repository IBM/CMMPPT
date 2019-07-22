package witviz;

import java.awt.Component;
import java.awt.Dimension;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.ListCellRenderer;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004</p>
 * <p>Company: </p>
 * @author not attributable
 * @version 1.0
 */

public class MyList extends JList {
  public MyList() {
     setCellRenderer(new MyCellRenderer());
  }
  public MyList(DefaultListModel model) {
    super(model);
    setCellRenderer(new MyCellRenderer());
  }
  public Dimension getPreferredSize() {
  	Dimension size = new Dimension(100,100);
  	return size;
  }
  class MyCellRenderer extends JLabel implements ListCellRenderer {
      //final static ImageIcon longIcon = new ImageIcon("long.gif");
      //final static ImageIcon shortIcon = new ImageIcon("short.gif");
      ImageIcon operationIcon = createImageIcon("operation.gif");
      ImageIcon partIcon = createImageIcon("part.gif");
      ImageIcon capacityIcon = createImageIcon("capacity.gif");
      ImageIcon bomIcon = createImageIcon("bom.gif");
      ImageIcon bopIcon = createImageIcon("bop.gif");
      ImageIcon demandIcon = createImageIcon("demand.gif");
      ImageIcon substituteIcon = createImageIcon("subs.gif");
      ImageIcon noneIcon = createImageIcon("none.gif");

      // This is the only method defined by ListCellRenderer.
      // We just reconfigure the JLabel each time we're called.

      public Component getListCellRendererComponent(
        JList list,
        Object value,            // value to display
        int index,               // cell index
        boolean isSelected,      // is the cell selected
        boolean cellHasFocus)    // the list and the cell have the focus
      {
          String s = value.toString();
          setText(s);
          String t = ((ListInfo)value).type;
          if (t.equals("part")) 
          	setIcon(partIcon);
          else if (t.equals("none"))
          	setIcon(noneIcon);
          else if (t.equals("capacity"))
          	setIcon(capacityIcon);
          else if (t.equals("demand"))
          	setIcon(demandIcon);
          else if (t.equals("substitute"))
          	setIcon(substituteIcon);
          else setIcon(operationIcon);
          if (isSelected) {
                setBackground(list.getSelectionBackground());
                setForeground(list.getSelectionForeground());
          }
          else {
           setBackground(list.getBackground());
           setForeground(list.getForeground());
          }

          setEnabled(list.isEnabled());
          setFont(list.getFont());
          setOpaque(true);
          return this;
      }
  }


  protected ImageIcon createImageIcon(String path) {
    java.net.URL imgURL =this.getClass().getResource(path);
    if (imgURL != null) {
      return new ImageIcon(imgURL);
    } else {
      System.err.println("couldn't find file: " + path);
      return null;
    }
  }

}