
public final class Main {
    native void helloOGLW();
    native void init(float width, float height);

    static {
        System.loadLibrary("OGLW");
    }

	private Main() { }

    public static void main(String[] args) {

        Main main = new Main();

        // test stdout
        main.helloOGLW();

        // init app
        main.init(800.f, 600.f);

	}
}
