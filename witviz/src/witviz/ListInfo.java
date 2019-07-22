package witviz;

public class ListInfo {
public String type;
public String name;
public int index;
public ListInfo(String t, String n) {
  type =t;
  name =n;
  
}
public void setIndex(int i) {
	index = i;
}
public int getIndex() {
	return index;
}
public String getLabelName() {
  if (type.equals("bomarc")|| type.equals("boparc")) {
	  	String myName = this.name;
	  	int index = myName.indexOf("xyxyxy");
	  	String top = myName.substring(0,index);
	  	String bottom = myName.substring(index+6,myName.length());
	  	return top + "/" + bottom;
  }
  else
  	return this.name;
}
public String toString() {
  return name;
}
}
