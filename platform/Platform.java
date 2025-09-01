package platform;

public class Platform{
    static {
        System.loadLibrary("native");
    }

    public native String nativeGetString();

    public static void main(String [] args){
        System.out.println(new Platform().nativeGetString());
    }
}


