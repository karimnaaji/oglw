
public final class Main {
    native void helloOGLW();
    native void init(float width, float height);
    native void run();

    static {
        System.loadLibrary("OGLW");
    }

	private Main() { }

    public static void main(String[] args) {

        Main main = new Main();

        // init app
        main.init(1024, 720.f);
        main.run();

    }
}
