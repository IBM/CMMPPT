package witviz;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import witviz.OptionsDialog.HIGHLIGHT;

import com.ibm.witj.Attribute;
import com.ibm.witj.BadCharacterException;
import com.ibm.witj.BomEntry;
import com.ibm.witj.BopEntry;
import com.ibm.witj.BoundSet;
import com.ibm.witj.Component;
import com.ibm.witj.Demand;
import com.ibm.witj.MessageGroup;
import com.ibm.witj.Operation;
import com.ibm.witj.Part;
import com.ibm.witj.Part.Category;
import com.ibm.witj.Problem;
import com.ibm.witj.ReadDataException;
import com.ibm.witj.Substitute;





//import com.ibm.mathsci.witinterface.witAttr;
/**
 * <p>
 * Title:
 * </p>
 * <p>
 * Description:
 * </p>
 * <p>
 * Copyright: Copyright (c) 2004
 * </p>
 * <p>
 * Company:
 * </p>
 * 
 * @author not attributable
 * @version 1.0
 */

public class WitObject  {
	//Log logger = LogFactory.getLog(WitObject.class);

	enum STATUS { OK, DEAD, RETRY }


	STATUS allOK=STATUS.OK;

	String message="";
	
	private Vector listOfPartAttributes=null;
	private Vector listOfOperationAttributes=null;
	private Vector listOfBomAttributes=null;
	private Vector listOfBopAttributes=null;
	private Vector listOfSubstituteAttributes=null;
	private Vector listOfDemandEdgeAttributes=null;
	private Vector listOfGlobalAttributes=null;
	private Vector currentChildren = new Vector();
	private Vector currentParents = new Vector();
	private Vector selectedChildren = new Vector();
	private Vector selectedParents = new Vector();
	private Vector selectedSubstitutes = new Vector();
	private ListInfo currentFocus;
	private Vector rawMaterialsList = new Vector();
	private Vector rawOperationsList = new Vector();

	private ListInfo currentSelected; // (not the same as focus, necessarily)
	Problem theProblem;
	int nPeriodsVal;

	public WitObject(String filename)
			throws Exception {
	

		
		theProblem = Problem.newInstance();
		
		theProblem.getMessageMgr().setMesgTimesPrint(MessageGroup.INFORMATIONAL,0);
		
	    theProblem.getMessageMgr().setMesgFileAccessMode("w");
	    theProblem.getMessageMgr().setMesgFileName("WitVizErrorLog.txt");
		
		
		//check if file exists
		File file = new File(filename);
		if (!file.exists()) {
			message = "File "+filename+" does not exist";
			allOK=STATUS.RETRY;
			return;
		}
		try {
			theProblem.readData(filename);
		}
		catch (ReadDataException e) {
			message = "WIT issued a severe error message while attempting to read the data file \n                       \""+filename+"\". \nThis indicates an error in the data file. As a result of this severe error,\nWitViz will need to terminate when this window is closed.  \n\nThe severe error message follows:\n\n";
			allOK=STATUS.DEAD;
			
			
			File errorfile = new File("WitVizErrorLog.txt");
	        BufferedReader in = new BufferedReader(new FileReader(errorfile));
	        String line;
	        while ((line = in.readLine()) != null) {
	        	message+=line+"\n";
	        }
	        in.close();

			return;
		}
		catch (BadCharacterException e) {
			String badstring = e.getOffendingString();
			message = "WitViz has read a string containing a bad character in \n"+filename+". The first offending string is '"+badstring+"'";
			allOK=STATUS.DEAD;
			return;

		}
		
		nPeriodsVal = theProblem.get (Attribute.nPeriods);
		
		
		CreateRawMaterialsList();
		CreateRawOperationsList();
		
		

	}

public WitObject()
	throws Exception {

	//just used for setup purposes
	theProblem = Problem.newInstance();
	
	theProblem.getMessageMgr().setMesgTimesPrint(MessageGroup.INFORMATIONAL,0);
	
	
	CreateRawMaterialsList();
	CreateRawOperationsList();

}


	public void RecurseUpOperation(int currentLevel) {
		currentLevel--;
		if (currentLevel == 0)
			return;
		else {
			// do the stuff at the next level

			// RecurseUpPart(currentLevel, partName);
		}
	}
	public Vector getRawMaterialsList() {
		return rawMaterialsList;
	}

	public Vector getRawOperationsList() {
		return rawOperationsList;
	}
	public boolean getStochMode() {
		boolean stochMode = theProblem.get(Attribute.stochMode);
		return stochMode;
	}	
	public boolean getMultObjectiveMode() {
		boolean stochMode = theProblem.get(Attribute.multiObjMode);
		return stochMode;
	}


	public void setFocusObject(ListInfo li) {
		currentFocus = li;
	}

	public ListInfo getFocus() {
		return currentFocus;
	}
	public int getNumScenarios() {
		return theProblem.get(Attribute.nScenarios);
	}
	public Vector<String> getObjectiveList() {
		Vector<String> retVec=new Vector<String>();
		ArrayList<String> stringlist = theProblem.getObjectiveList();
		for (Iterator i=stringlist.iterator();i.hasNext();) {
			retVec.add((String)i.next());
		}
		
		return retVec;
	}
	
	public Vector getListOfGlobalAttributes() {

		if (listOfGlobalAttributes==null) {
			listOfGlobalAttributes = new Vector();
		
			
			Vector list = new Vector();
			List<Attribute<?>> allAtts = Attribute.getAttributes();
			
			for (int i=0; i<allAtts.size();i++) {
				Attribute<?> attr = allAtts.get(i);
				if (attr.appliesTo(Problem.class))
					listOfGlobalAttributes.add(attr);
			}
		}
		return listOfGlobalAttributes;

	}
	
	public void setScenario(int scenario) {
		theProblem.set(Attribute.currentScenario,scenario);
	}
	public int getCurrentScenario() {
		int scenario = theProblem.get(Attribute.currentScenario);
		return scenario;
	}
	public void setObjective(String objective) {
		theProblem.set(Attribute.currentObjective,objective);
	}
	public String getCurrentObjective() {
		String scenario = theProblem.get(Attribute.currentObjective);
		return scenario;
	}

	
	public Vector getListOfPartAttributes() {

		if (listOfPartAttributes==null) {
			listOfPartAttributes = new Vector();
					
			Vector list = new Vector();
			List<Attribute<?>> allAtts = Attribute.getAttributes();
			
			for (int i=0; i<allAtts.size();i++) {
				Attribute<?> attr = allAtts.get(i);
				if (attr.appliesTo(Part.class)) {
					if (attr.toString().compareTo("focusShortageVol")!=0)
						listOfPartAttributes.add(attr);
				}
			}
		}
		return listOfPartAttributes;

	}
	public Vector getPartDemands(String partName) {
		Vector retVec = new Vector();
		Part part = theProblem.getPart(partName);
		List<Demand> demands = part.getDemands();
		String category = "demand";
		for (int i=0; i<demands.size(); i++) {
			Demand demand = demands.get(i);
			String name = demand.get(Attribute.demandName);
			ListInfo li = new ListInfo(category, name);
			retVec.add(li);
		}
		return retVec;
	}
	
	public Vector getListOfDemandEdgeAttributes() {
		
		
		if (listOfDemandEdgeAttributes==null) {
			listOfDemandEdgeAttributes = new Vector();
		
			Vector list = new Vector();
			List<Attribute<?>> allAtts = Attribute.getAttributes();
			
			for (int i=0; i<allAtts.size();i++) {
				Attribute<?> attr = allAtts.get(i);
				if (attr.appliesTo(Demand.class))
					listOfDemandEdgeAttributes.add(attr);
			}
		}
		return listOfDemandEdgeAttributes;

	}

	public Vector getListOfOperationAttributes() {
		if (listOfOperationAttributes==null) {
			listOfOperationAttributes = new Vector();
			List<Attribute<?>> allAtts = Attribute.getAttributes();
			
			for (int i=0; i<allAtts.size();i++) {
				Attribute<?> attr = allAtts.get(i);
				if (attr.appliesTo(Operation.class))
					if (attr.toString().compareTo("fssExecVol")!=0)
						listOfOperationAttributes.add(attr);
			}

		}
		return listOfOperationAttributes;

	}
	
	public Vector getListOfSubstituteAttributes() {
		if (listOfSubstituteAttributes==null) {
			listOfSubstituteAttributes = new Vector();
					
			Vector list = new Vector();
			List<Attribute<?>> allAtts = Attribute.getAttributes();
			
			for (int i=0; i<allAtts.size();i++) {
				Attribute<?> attr = allAtts.get(i);
				if (attr.appliesTo(Substitute.class))
					if (attr.toString().compareTo("fssSubVol")!=0)
						listOfSubstituteAttributes.add(attr);
			}

		}
		return listOfSubstituteAttributes;

	}

	
	public Vector getListOfBomAttributes() {
		if (listOfBomAttributes==null) {
			listOfBomAttributes = new Vector();
					
			Vector list = new Vector();
			List<Attribute<?>> allAtts = Attribute.getAttributes();
			
			for (int i=0; i<allAtts.size();i++) {
				Attribute<?> attr = allAtts.get(i);
				if (attr.appliesTo(BomEntry.class))
					listOfBomAttributes.add(attr);
			}
		}
		return listOfBomAttributes;

	}

	public Vector getListOfBopAttributes() {
		if (listOfBopAttributes==null) {
			listOfBopAttributes = new Vector();
			Vector list = new Vector();
			List<Attribute<?>> allAtts = Attribute.getAttributes();
			
			for (int i=0; i<allAtts.size();i++) {
				Attribute<?> attr = allAtts.get(i);
				if (attr.appliesTo(BopEntry.class))
					listOfBopAttributes.add(attr);
			}

		}
		return listOfBopAttributes;

	}
	

	public Vector getBomArcSubstitutes(String name) {
		Vector vector = new Vector();
		return vector;
	}
	
	public void setSelectedObject(ListInfo li) {
		currentSelected = li;
		selectedChildren.clear();
		selectedParents.clear();
		selectedSubstitutes.clear();
		if (li.type.equals("operation")) {
			selectedChildren = getOperationChildren(li.name);
			selectedParents = getOperationParents(li.name);
		} else if (li.type.equals("material") || (li.type.equals("capacity"))) {
			selectedChildren = getPartChildren(li.name);
			selectedParents = getPartParents(li.name);
		} else if (li.type.equals("bomarc")) {
			selectedSubstitutes = getBomArcSubstitutes(li.name);
			selectedChildren = getSubstituteChild(li.name, li.type);
			selectedParents = getSubstituteParent(li.name, "operation");
		} else if (li.type.equals("boparc")) {
			// selectedSubstitutes = getBopArcSubstitutes(li.name);
			// there are no substitutes on a bop arc
			selectedParents = getSubstituteParent(li.name, li.type);
			selectedChildren = getSubstituteChild(li.name, "operation");
		}
	}

	public Vector getSelectedSubstitutes() {
		return selectedSubstitutes;
	}

	






	public Vector getBopArcSubstitutes(String arcName) {
		int separator = arcName.indexOf("xyxyxy");
		String part = arcName.substring(0, separator);
		String operation = arcName.substring(separator + 6, arcName.length());
		return null;
	}

	public Vector getSubstituteChild(String arcName, String type) {
		Vector c = new Vector();
		int separator = arcName.indexOf("xyxyxy");
		String child = arcName.substring(separator + 6, arcName.length());
		ListInfo li = new ListInfo(type, child);
		c.add(li);
		return c;
	}

	public Vector getSubstituteParent(String arcName, String type) {
		Vector p = new Vector();
		int separator = arcName.indexOf("xyxyxy");
		String parent = arcName.substring(0, separator);
		ListInfo li = new ListInfo(type, parent);
		p.add(li);
		return p;
	}


	public void finalize() {
		if (theProblem.isActive())
			theProblem.deactivate();
	}
	
	String getPartCategory(String partName) {
		Part thePart = theProblem.getPart(partName);
		Category category = thePart.getCategory();
		
		return category.toString();
	}
	
	String getElementAttributeDescription(Component element, Attribute attr) {
		
		String retString=attr.toString()+": ";
		
		Class attributeClass = attr.getValueType();
		if (attributeClass.equals(double[].class)) {
			Attribute<double[]> doubleAttr;
			doubleAttr = (Attribute<double[]>)attr;
			double[] val = element.get(doubleAttr);
			String str="{";
			for (int i =0; i<val.length-1;i++) {
				double j = val[i];
				str += j+",";
			}
			str+=val[val.length-1]+"}";
			return retString+ str+"\n";
			//print it out
		}
		else if (attributeClass.equals(Boolean.class)) {
			Attribute<Boolean> booleanAttr;
			booleanAttr = (Attribute<Boolean>)attr;
			Boolean val = element.get(booleanAttr);
			return retString + val.toString()+"\n";
			//print it out
		}
		else if (attributeClass.equals(boolean[].class)) {
			Attribute<boolean[]> booleanAttr;
			booleanAttr = (Attribute<boolean[]>)attr;
			boolean[] val = element.get(booleanAttr);
			String str="{";
			for (int i =0; i<val.length-1;i++) {
				boolean j = val[i];
				str += j + ",";
				//if (j)
				//	str += "true,";
				//else
				//	str += "false,";
			}
			str+=val[val.length-1]+"}";
			return retString+ str+"\n";
			
		}
		else if (attributeClass.equals(Integer.class)) {
			Attribute<Integer> integerAttr;
			integerAttr = (Attribute<Integer>)attr;
			Integer val = element.get(integerAttr);
			return retString+ val.toString()+"\n";
			//print it out
		}
		else if (attributeClass.equals(int[].class)) {
			Attribute<int[]> integerAttr;
			integerAttr = (Attribute<int[]>)attr;
			int[] val = element.get(integerAttr);
			String str="{";
			for (int i =0; i<val.length-1;i++) {
				int j = val[i];
				str += j+",";
			}
			str+=val[val.length-1]+"}";
			return retString+ str+"\n";
			//print it out
		}
		else if (attributeClass.equals(BoundSet.class)) {
			Attribute<BoundSet> boundsetAttr;
			int numPeriods = theProblem.get(Attribute.nPeriods);
			boundsetAttr = (Attribute<BoundSet>)attr;
			double[] hlb = new double[numPeriods];
			double[] slb = new double[numPeriods];
			double[] hub = new double[numPeriods];
			element.get(boundsetAttr, hlb, slb, hub);
			
			String str1="{";
			for (int i =0; i<hlb.length-1;i++) {
				double j = hlb[i];
				str1 += j+",";
			}
			str1+=hlb[hlb.length-1]+"}";
			
			String str2="{";
			for (int i =0; i<slb.length-1;i++) {
				double j = slb[i];
				str2 += j+",";
			}
			str2+=slb[slb.length-1]+"}";
			
			String str3="{";
			for (int i =0; i<hub.length-1;i++) {
				double j = hub[i];
				str3 += j+",";
			}
			str3+=hub[hub.length-1]+"}";
			
			
			
			return retString+ "\n"+"  HLB: "+str1+"\n"+"  SLB: "+str2+"\n"+"  HUB: "+str3+"\n";
			//print it out
		}
		else if (attributeClass.equals(String.class)) {
			Attribute<String> stringAttr;
			stringAttr = (Attribute<String>)attr;
			String val = element.get(stringAttr);
			return retString+val.toString()+"\n";
			//print it out
		}
		else if (attributeClass.equals(Double.class)) {
			Attribute<Double> doubleAttr;
			doubleAttr = (Attribute<Double>)attr;
			Double val = element.get(doubleAttr);
			return retString+val.toString()+"\n";
			//print it out
		}
		else {
			System.out.println("unknown attribute class: "+attributeClass.toString());
			System.exit(0);
		}
		
		
		return retString;
	}
	HIGHLIGHT getElementAttributeInteresting(Component element, Attribute attr) {
		
		HIGHLIGHT interesting=HIGHLIGHT.DEFAULT;
		
		
		
		Class attributeClass = attr.getValueType();
		if (attributeClass.equals(double[].class)) {
			Attribute<double[]> doubleAttr;
			doubleAttr = (Attribute<double[]>)attr;
			
			if (doubleAttr.hasDefaultValue()) {
				double[] val = element.get(doubleAttr);
				double[] defaultValue = new double[nPeriodsVal];

				theProblem.getDefaultValue(doubleAttr, defaultValue);
				
				for (int i=0; i<nPeriodsVal; i++) {
					if (val[i] != defaultValue[i])
						interesting=HIGHLIGHT.HIGHLIGHT;
				}
			}
		}
		else if (attributeClass.equals(Boolean.class)) {
			Attribute<Boolean> booleanAttr;
			booleanAttr = (Attribute<Boolean>)attr;
			if (booleanAttr.hasDefaultValue()) {
				boolean defaultVal = theProblem.getDefaultValue(booleanAttr);
				Boolean val = element.get(booleanAttr);
				//it is interesting if they are different
				if (defaultVal != val.booleanValue())
					interesting = HIGHLIGHT.HIGHLIGHT;
			}
		}
		else if (attributeClass.equals(boolean[].class)) {
			Attribute<boolean[]> booleanAttr;
			booleanAttr = (Attribute<boolean[]>)attr;
			if (booleanAttr.hasDefaultValue()) {
				boolean[] val = element.get(booleanAttr);
				boolean[] defaultValue = new boolean[nPeriodsVal];
				theProblem.getDefaultValue(booleanAttr, defaultValue);
				for (int i=0; i<nPeriodsVal; i++) {
					if (val[i]!=defaultValue[i])
						interesting= HIGHLIGHT.HIGHLIGHT;
				}
			}
		}
		else if (attributeClass.equals(Integer.class)) {
			Attribute<Integer> integerAttr;
			integerAttr = (Attribute<Integer>)attr;
			if (integerAttr.hasDefaultValue()) {
				Integer val = element.get(integerAttr);
				int defaultValue = theProblem.getDefaultValue(integerAttr);
				if (defaultValue != val.intValue())
					interesting= HIGHLIGHT.HIGHLIGHT;
			}
		}
		else if (attributeClass.equals(int[].class)) {
			Attribute<int[]> integerAttr;
			integerAttr = (Attribute<int[]>)attr;
			if (integerAttr.hasDefaultValue()) {
				int[] val = element.get(integerAttr);
				int[] defaultValue = new int[nPeriodsVal];
				theProblem.getDefaultValue(integerAttr, defaultValue);
				for (int i=0; i<nPeriodsVal; i++) 
					if (defaultValue[i]!=val[i]) 
						interesting= HIGHLIGHT.HIGHLIGHT;
			}
		}
		else if (attributeClass.equals(BoundSet.class)) {
			Attribute<BoundSet> boundsetAttr;
			int numPeriods = theProblem.get(Attribute.nPeriods);
			boundsetAttr = (Attribute<BoundSet>)attr;
			if (boundsetAttr.hasDefaultValue()) {
				double[] hlb = new double[numPeriods];
				double[] slb = new double[numPeriods];
				double[] hub = new double[numPeriods];
				element.get(boundsetAttr, hlb, slb, hub);
						
				double[] hlbDefault = new double[numPeriods];
				double[] slbDefault = new double[numPeriods];
				double[] hubDefault = new double[numPeriods];
				
				theProblem.getDefaultValue(boundsetAttr, hlbDefault, slbDefault, hubDefault);
		
				for (int i=0; i<numPeriods; i++) {
					if (hlb[i]!=hlbDefault[i])
						interesting= HIGHLIGHT.HIGHLIGHT;
					if (slb[i]!=slbDefault[i])
						interesting= HIGHLIGHT.HIGHLIGHT;
					if (hub[i]!=hubDefault[i])
						interesting= HIGHLIGHT.HIGHLIGHT;
				}
			}
		}
		else if (attributeClass.equals(String.class)) {
			Attribute<String> stringAttr;
			stringAttr = (Attribute<String>)attr;
			if (stringAttr.hasDefaultValue()) {
				String val = element.get(stringAttr);
				String defaultValue = theProblem.getDefaultValue(stringAttr);
				if (val.compareTo(defaultValue)!=0) 
					interesting= HIGHLIGHT.HIGHLIGHT;
			}
		}
		else if (attributeClass.equals(Double.class)) {
			Attribute<Double> doubleAttr;
			doubleAttr = (Attribute<Double>)attr;
			if (doubleAttr.hasDefaultValue()) {
				Double val = element.get(doubleAttr);
				double defaultValue = theProblem.getDefaultValue(doubleAttr);
				if (val.doubleValue()!=defaultValue)
					interesting= HIGHLIGHT.HIGHLIGHT;
			}
		}
		else if (attributeClass.equals(Part.Category.class)) {
			interesting= HIGHLIGHT.HIGHLIGHT;
		}
		else {
			System.out.println("Unknown attribute class: "+attributeClass.toString());
			System.exit(0);
		}
		return interesting;
	}
	
	boolean GetStageByObject() {
	    boolean val = theProblem.get(Attribute.stageByObject);
		return val;
	}
	String GetGlobalAttributeDescription(Attribute attr) {
		return getElementAttributeDescription((Component)theProblem, attr);
	}
	HIGHLIGHT GetGlobalAttributeInteresting(Attribute attr) {
		return getElementAttributeInteresting((Component)theProblem, attr);
	}
	
	String GetPartAttributeDescription(String partName, Attribute attr) {
		Part thePart = theProblem.getPart(partName);
		return getElementAttributeDescription((Component)thePart, attr);
	}
	HIGHLIGHT GetPartAttributeInteresting(String partName, Attribute attr) {
		Part thePart = theProblem.getPart(partName);
		return getElementAttributeInteresting((Component)thePart, attr);
	}
	String GetDemandEdgeAttributeDescription(String demandName, String partName, Attribute attr) {
		Part thePart = theProblem.getPart(partName);
		Demand demand = thePart.getDemand(demandName);
		return getElementAttributeDescription((Component)demand, attr);
	}
	boolean isDemandEdgeCompatible(Attribute attribute, String demandName, String partName){
		Part thePart = theProblem.getPart(partName);
		Demand demand = thePart.getDemand(demandName);
		return attribute.isValidFor(demand);
	}
	HIGHLIGHT GetDemandEdgeAttributeInteresting(String demandName, String partName, Attribute attr) {
		Part thePart = theProblem.getPart(partName);
		Demand demand = thePart.getDemand(demandName);
		return getElementAttributeInteresting((Component)demand, attr);
	}


	String GetOperationAttributeDescription(String operationName, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		return getElementAttributeDescription((Component)theOperation, attr);
	}
	HIGHLIGHT GetOperationAttributeInteresting(String operationName, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		return getElementAttributeInteresting((Component)theOperation, attr);
	}

	String GetBomAttributeDescription(String operationName, int bomIndex, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		List<BomEntry> bomEntries = theOperation.getBomEntries();
		BomEntry bomEntry = bomEntries.get(bomIndex);
		return getElementAttributeDescription((Component)bomEntry, attr);
	}
	boolean isBomCompatible(Attribute attribute, String operationName, int bomIndex){
		Operation theOperation = theProblem.getOperation(operationName);
		List<BomEntry> bomEntries = theOperation.getBomEntries();
		BomEntry bomEntry = bomEntries.get(bomIndex);
		return attribute.isValidFor(bomEntry);
	}
	HIGHLIGHT GetBomAttributeInteresting(String operationName, int bomIndex, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		List<BomEntry> bomEntries = theOperation.getBomEntries();
		BomEntry bomEntry = bomEntries.get(bomIndex);
		return getElementAttributeInteresting((Component)bomEntry, attr);
	}

	String GetSubstituteAttributeDescription(String operationName, int bomIndex, int subsBomIndex, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		List<BomEntry> bomEntries = theOperation.getBomEntries();
		BomEntry bomEntry = bomEntries.get(bomIndex);
		Substitute subs = bomEntry.getSubstitutes().get(subsBomIndex);
		return getElementAttributeDescription((Component)subs, attr);
	}
	boolean isSubstituteCompatible(Attribute attribute,String operationName,int bomIndex,int subsBomIndex) {
		Operation theOperation = theProblem.getOperation(operationName);
		List<BomEntry> bomEntries = theOperation.getBomEntries();
		BomEntry bomEntry = bomEntries.get(bomIndex);
		Substitute subs = bomEntry.getSubstitutes().get(subsBomIndex);
		return attribute.isValidFor(subs);
	}
		
	HIGHLIGHT GetSubstituteAttributeInteresting(String operationName, int bomIndex, int subsBomIndex, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		List<BomEntry> bomEntries = theOperation.getBomEntries();
		BomEntry bomEntry = bomEntries.get(bomIndex);
		Substitute subs = bomEntry.getSubstitutes().get(subsBomIndex);
		return getElementAttributeInteresting((Component)subs, attr);
	}
	String GetBopAttributeDescription(String operationName, int bopIndex, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		List<BopEntry> bopEntries = theOperation.getBopEntries();
		BopEntry bopEntry = bopEntries.get(bopIndex);
		return getElementAttributeDescription((Component)bopEntry, attr);
	}
	boolean isBopCompatible(Attribute attribute, String operationName, int bopIndex){
		Operation theOperation = theProblem.getOperation(operationName);
		List<BopEntry> bopEntries = theOperation.getBopEntries();
		BopEntry bopEntry = bopEntries.get(bopIndex);
		return attribute.isValidFor(bopEntry);
	}
	HIGHLIGHT GetBopAttributeInteresting(String operationName, int bopIndex, Attribute attr) {
		Operation theOperation = theProblem.getOperation(operationName);
		List<BopEntry> bopEntries = theOperation.getBopEntries();
		BopEntry bopEntry = bopEntries.get(bopIndex);
		return getElementAttributeInteresting((Component)bopEntry, attr);
	}
	
	boolean isLegalAttribute(String partName, Attribute attr) {
		String category = getPartCategory(partName);
		if (category.equals("CAPACITY")) {
			if (attr.toString().equals("asapPipOrder")) 
				return false;
			else if (attr.toString().equals("buildAheadUB")) 
				return false;
			else if (attr.toString().equals("buildAsap")) 
				return false;
			else if (attr.toString().equals("buildNstn"))
				return false;
			else if (attr.toString().equals("stockBounds"))
				return false;
			else if (attr.toString().equals("stockCost"))
				return false;
		}
		return true;
		
			
		

	}
	//boolean getPostProcessed() {
		
	//	boolean postProc = theProblem.getGlobalAspect().get(postprocessed());
	//	return postProc;
	//}


	public int postprocess() throws Exception {
		//
		try {
			theProblem.postprocess();
			return 0;
		}
		catch (Exception e) {
			File errorfile = new File("WitVizErrorLog.txt");
	        BufferedReader in = new BufferedReader(new FileReader(errorfile));
	        String line;
	        while ((line = in.readLine()) != null) {
	        	message+=line+"\n";
	        }
	        in.close();
			
			this.allOK = STATUS.DEAD;
			return 1;
		}
	}
	public int heuristicimplosion() throws Exception {
		//
		try {
			theProblem.heurImplode();
			return 0;
		}
		catch (Exception e) {
			File errorfile = new File("WitVizErrorLog.txt");
	        BufferedReader in = new BufferedReader(new FileReader(errorfile));
	        String line;
	        while ((line = in.readLine()) != null) {
	        	message+=line+"\n";
	        }
	        in.close();
			this.allOK = STATUS.DEAD;
			return 1;
		
		}
	}
	public int optimizingimplosion() throws Exception {
		//
		try {
			theProblem.optImplode();
			return 0;
		}
		catch (Exception e) {
			File errorfile = new File("WitVizErrorLog.txt");
	        BufferedReader in = new BufferedReader(new FileReader(errorfile));
	        String line;
	        while ((line = in.readLine()) != null) {
	        	message+=line+"\n";
	        }
	        in.close();
			this.allOK = STATUS.DEAD;
			return 1;
		
		}
	}
	public int stochasticimplosion() throws Exception {
		//
		try {
			theProblem.stochImplode();
			return 0;
		}
		catch (Exception e) {
			File errorfile = new File("WitVizErrorLog.txt");
	        BufferedReader in = new BufferedReader(new FileReader(errorfile));
	        String line;
	        while ((line = in.readLine()) != null) {
	        	message+=line+"\n";
	        }
	        in.close();
			this.allOK = STATUS.DEAD;
			return 1;
		}
	}
	public int mrp() throws Exception {
		//
		try {
			theProblem.mrp();
			return 0;
		}
		catch (Exception e) {
			File errorfile = new File("WitVizErrorLog.txt");
	        BufferedReader in = new BufferedReader(new FileReader(errorfile));
	        String line;
	        while ((line = in.readLine()) != null) {
	        	message+=line+"\n";
	        }
	        in.close();
			this.allOK = STATUS.DEAD;
			return 1;
		}
	}
	public Vector getOperationChildren(String operationName) {

		Vector kids = new Vector();
		
		try {
			// how many children does this operation have?
			Operation operation = theProblem.getOperation(operationName);
			List<BomEntry> bomEntries = operation.getBomEntries();
			int numBOMentries = bomEntries.size();
			for (int i = 0; i < numBOMentries; i++) {
				BomEntry bo = bomEntries.get(i);
				Part part = bo.getPart();
				String category = getPartCategory(part.get(Attribute.partName));
				ListInfo li;
				li = new ListInfo(category, part.get(Attribute.partName));
				li.setIndex(i);
				li.index = i; //XXX check this
				kids.add(li);

			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return kids;
	}

	public Vector getOperationParents(String operationName) {

		Vector rents = new Vector();
		
		try {
			// now how many parents does it have?
			
			Operation operation = theProblem.getOperation(operationName);
			List<BopEntry> bopEntries = operation.getBopEntries();
			int numBopEntries = bopEntries.size();
			for (int i=0; i<numBopEntries; i++) {
				BopEntry bo = bopEntries.get(i);
				Part part = bo.getPart();
				String category = getPartCategory(part.get(Attribute.partName));
				ListInfo li;
				li = new ListInfo(category, part.get(Attribute.partName));
				li.index = i;
				rents.add(li);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return rents;
	}

	public Vector getBopEntryIndex(String opName, String partName) {
		Vector retVal = new Vector();

		try {
			Operation op = theProblem.getOperation(opName);
			List<BopEntry> bopEntries = op.getBopEntries();
			int nBops = bopEntries.size();

			int bei;
			for (bei = 0; bei < nBops; bei++) {
				BopEntry bo = bopEntries.get(bei);
				Part consPart = bo.getPart();
				if (partName.equals(consPart.get(Attribute.partName)))
					retVal.add(new Integer(bei));
				// checkWitRC(witFree(consPartFromWit));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return retVal;
	}

	
	public Vector getBomEntryIndex(String opName, String partName) {
		Vector retVal = new Vector();

		// Get current number of bops connected to operation
		
		try {
			Operation operation = theProblem.getOperation(opName);
			List<BomEntry> bomEntries = operation.getBomEntries();
			int nBoms = bomEntries.size();
			
			for (int bei = 0;bei<nBoms;bei++) {
				String consPartFromWit;
				BomEntry bo = bomEntries.get(bei);
				Part consPart = bo.getPart();
				if (partName.compareTo(consPart.get(Attribute.partName))==0)
					retVal.add(new Integer(bei));
				// checkWitRC(witFree(consPartFromWit));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return retVal;
	}

	public ListInfo getSelected() {
		return currentSelected;
	}

	public void createTreeObject(String name, String type) {
		// basically just creates center, parents, and children
		ListInfo center = new ListInfo(type, name);
		setFocusObject(center);
		if (type.equals("operation"))
			createTreeOperationObject(name);
		else
			createTreePartObject(name);
	}

	public void createTreeOperationObject(String operationName) {
		currentParents.clear();
		currentChildren.clear();
		currentParents = getOperationParents(operationName);
		currentChildren = getOperationChildren(operationName);
	}

	public Vector getPartChildren(String partName) {
		Vector kids = new Vector();
		int numBOPentries;
		String operationName;
		
		try {
			// how many children does this part have?
			Part part = theProblem.getPart(partName);
	
			List<BopEntry> bopEntries = part.getBopEntries();
			numBOPentries = bopEntries.size();
			for (int i = 0; i < numBOPentries; i++) {
				BopEntry bo = bopEntries.get(i);
				Operation operation = bo.getOperation();
				operationName = operation.get(Attribute.operationName);
				int bopEntryIndex = bo.get(Attribute.bopIndex);
				ListInfo li = new ListInfo("operation", operationName);
				li.index = bopEntryIndex;
				kids.add(li);
			}

		}

		catch (Exception e) {
			e.printStackTrace();
		}
		return kids;
	}

	public Vector getPartParents(String partName) {
		Vector rents = new Vector();
		try {
			// now how many parents does it have?
			Part part = theProblem.getPart(partName);
			List<BomEntry> bomEntries = part.getBomEntries();
			int numBomEntries = bomEntries.size();
			
			for (int i = 0; i < numBomEntries; i++) {
				BomEntry bo = bomEntries.get(i);
				Operation operation = bo.getOperation();
				int bomEntryIndex = bo.get(Attribute.bomIndex);
				String operationName = operation.get(Attribute.operationName);
				ListInfo li = new ListInfo("operation", operationName);
				li.index = bomEntryIndex;
				rents.add(li);
			}
		}

		catch (Exception e) {
			e.printStackTrace();
		}
		return rents;
	}
	
	

	//public String getRealPartFromSubsParentIndex(String SubsPartName,
	//		int BOMindex) {
	//	Vector rents = new Vector();
	//	Part substitutePart = theProblem.getPart(SubsPartName);
	//	Substitute[] substitutes = substitutePart.getSubstitutes();
	//	Substitute subEdge = substitutes[BOMindex];
	//	BomEntry be = subEdge.getBomEntry();
	//	Part part = be.getPart();
	//	return part.get(partName());
	//}
	

	public Vector getPartSubstitutes(String pName) {
		Vector rents = new Vector();

		//starting with a part figure out all the relevant info about things
		// that substitute for it
		Part part = theProblem.getPart(pName);
		List<BomEntry> bomEntries = part.getBomEntries();
		for (int i=0; i<bomEntries.size();i++) {
			//for all bom entries that this part is part of
			BomEntry be = bomEntries.get(i);
			Operation op = be.getOperation();
			String opName = op.get(Attribute.operationName);
			//get any substitutes
			List<Substitute> subs = be.getSubstitutes();
			for (int j=0; j<subs.size(); j++) {
				//get all substitutes
				Substitute substitute = subs.get(j);
				Part subsPart = substitute.getPart();
				String subsPartName = subsPart.get(Attribute.partName);
				SubsInfo li = new SubsInfo(opName,be.get(Attribute.bomIndex), j, subsPartName, pName);
				rents.add(li);

			}
		}
		return rents;
	}

	public void createTreePartObject(String partName) {

		currentParents.clear();
		currentChildren.clear();
		currentParents = getPartParents(partName);
		currentChildren = getPartChildren(partName);
	}

	public int getPlanningHorizon() {
		int nPeriods;
		nPeriods = theProblem.get(Attribute.nPeriods);
		return nPeriods;
	}


	public Vector getCurrentChildren() {
		return currentChildren;
	}

	public Vector getCurrentParents() {
		return currentParents;
	}

	public Vector getSelectedChildren() {
		return selectedChildren;
	}

	public Vector getSelectedParents() {
		return selectedParents;
	}

	public Vector getAllOperations() {
		Vector retvec = new Vector();
		try {
			for (Component theElement: theProblem.getComponents()) {
				if (theElement instanceof Operation) {
					String opName = theElement.get(Attribute.operationName);
					retvec.add(opName);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return retvec;
	}

	public int getNumOperations() {
		Vector allOps = getAllOperations();
		return allOps.size();
	}

	public Vector getAllParts() {
		Vector retvec = new Vector();
		try {
			for (Component theElement: theProblem.getComponents()) {
				if (theElement instanceof Part) {
					String partName = theElement.get(Attribute.partName);
					retvec.add(partName);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return retvec;
	}

	public Vector getAllCapacities() {
		Vector capacityList = new Vector();
		Vector allParts = getAllParts();
		Iterator i;
		for (i=allParts.iterator();i.hasNext();) {
			String partName = (String)i.next();
			String category = getPartCategory(partName);
			if (category.equals("CAPACITY")) {
				capacityList.add(partName);
			}
		}
		return capacityList;
	}
	
	public Vector getDemandedParts(String demandName) {
		Vector retVec = new Vector();
		Vector	allparts = getAllParts();
		for (Iterator i=allparts.iterator();i.hasNext();) {
			String partName = (String)i.next();
			Part p = theProblem.getPart(partName);
			List<Demand> demands = p.getDemands();
			for (int j=0; j<demands.size();j++) {
				Demand demand = demands.get(j);
				String dName = demand.get(Attribute.demandName);
				if (dName.compareTo(demandName)==0)
					retVec.add(partName);
			}
		}
		return retVec;
	}

	public Vector getAllDemandedParts() {
		Vector allparts = getAllParts();
		Vector demandedParts = new Vector();
		
		for (int i = 0; i < allparts.size(); i++) {
			String partName = (String)allparts.elementAt(i);
			boolean demanded = isPartDemanded(partName);
			if (demanded)
				demandedParts.add(partName);
		}
		return demandedParts;

	}

	public int getNumParts() {

		Vector allParts = getAllParts();
		return allParts.size();
		
	}
	
	boolean isPartDemanded(String partName) {
		Part part = theProblem.getPart(partName);
		List<Demand> demand = part.getDemands();
		if (demand.size()==0)
			return false;
		else
			return true;
	}



	

	

	public int getPartNConsumingSubsBomEntries(String name) {
		Part part = theProblem.getPart(name);
		List<Substitute> subs = part.getSubstitutes();
		return subs.size();
	}



	public SubsInfo GetPartSubsInfo(String subName, int subsBomIndex) {
		Part part = theProblem.getPart(subName);
		List<Substitute> subs = part.getSubstitutes();
		Substitute sub = subs.get(subsBomIndex);
		//get the rest of the info
		BomEntry be = sub.getBomEntry();
		Operation op = be.getOperation();
		Part p = be.getPart();
		SubsInfo si = new SubsInfo(op.get(Attribute.operationName),be.get(Attribute.bomIndex), subsBomIndex, subName, p.get(Attribute.partName));
		
		return si;
	}



	public Vector getBomArcSubstitutes(String operationName, int bomIndex) {
		Operation op = theProblem.getOperation(operationName);
		List<BomEntry> bomEntries = op.getBomEntries();
		BomEntry be = bomEntries.get(bomIndex);
		List<Substitute> subs = be.getSubstitutes();
		Vector subsVec = new Vector();
		ListInfo li;
		
		for (int i=0; i<subs.size(); i++) {
			Substitute sub = subs.get(i);
			Part p = sub.getPart();
			String category = getPartCategory(p.get(Attribute.partName));
			String subsPartName = p.get(Attribute.partName);
			li = new ListInfo(category, subsPartName);
			li.index = bomIndex;
			subsVec.add(li);
		}
		return subsVec;
	}
	
	public void CreateRawMaterialsList() {
		try {
			Vector parts = getAllParts();
			int numparts = parts.size();
			for (int j = 0; j < numparts; j++) {
				// find the parts that have no producing BOP entries and
				// no consuming substitute BOM entries
				String partName = (String)parts.elementAt(j);
				Part part = theProblem.getPart(partName);
				List<BopEntry> bopEntries = part.getBopEntries();
				int numbops = bopEntries.size();
				if (numbops == 0) {
					// which type of part is it?
					String cat = getPartCategory(partName);
					ListInfo li;
					li = new ListInfo(cat, partName);
					rawMaterialsList.add(li);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	public void CreateRawOperationsList() {
		try {

			Vector allOperations = getAllOperations();
			int numops = allOperations.size();
			for (int j = 0; j < numops; j++) {
				// find the operations that have no BOM entries
				String operationName = (String)allOperations.elementAt(j);
				Operation operation = theProblem.getOperation(operationName);
				List<BomEntry> bomEntries = operation.getBomEntries();
				int numboms = bomEntries.size();
					if (numboms == 0) {
					// which type of part is it?
					ListInfo li;
					li = new ListInfo("operation",operationName);
					rawOperationsList.add(li);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	public boolean isPartCompatible(Attribute attr, String partName) {		
		return(attr.isValidFor(theProblem.getPart(partName)));
	}
	public boolean isOperationCompatible(Attribute attr, String operationName) {		
		return(attr.isValidFor(theProblem.getOperation(operationName)));
	}
	public boolean isProblemCompatible(Attribute attr) {
		return(attr.isValidFor(theProblem));
	}

}