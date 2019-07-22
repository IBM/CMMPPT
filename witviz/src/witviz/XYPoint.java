package witviz;

/**
 * An XY point pair
 */
public class XYPoint {
  int x;
  int y;
  /**
   * Create an xy point pair using floating point values
   * @param xf x
   * @param yf y
   */
  public XYPoint(int xf, int yf) {
    x=xf;
    y=yf;
  }
  public boolean isEqualTo(XYPoint foo) {
    if ((this.x == foo.x)&&(this.y == foo.y))
       return true;
    else
       return false;
  }
  public int getX() {
    return (int)x;
  }
  public int getY() {
    return (int)y;
  }
}
