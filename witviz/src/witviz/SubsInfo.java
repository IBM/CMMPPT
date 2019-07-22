package witviz;

public class SubsInfo {
public String consumingOperation;
public int bomEntryIndex;
public int subsBomEntryIndex;
public String consumedPart;
public String mainPart;
public SubsInfo(String c, int b, int s, String p, String m) {
  consumingOperation=c;
  bomEntryIndex=b;
  subsBomEntryIndex=s;
  consumedPart = p;
  mainPart = m;
}

}
