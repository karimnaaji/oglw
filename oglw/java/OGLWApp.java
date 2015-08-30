
public class OGLWApp {
    native void helloOGLW();
    native void init(float width, float height);

    static {
        System.loadLibrary("OGLW");
    }

    static public void main(String argv[]) {
        OGLWApp helloWorld = new OGLWApp();
        helloWorld.helloOGLW();
    }
}
