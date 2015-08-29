
public class OGLWApp {
    native void helloOGLW();

    static {
        System.loadLibrary("OGLW");
    }

    static public void main(String argv[]) {
        OGLWApp helloWorld = new OGLWApp();
        helloWorld.helloOGLW();
    }
}
