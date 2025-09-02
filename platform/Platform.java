package platform;

import java.util.List;
import java.util.ArrayList;

public class Platform{
    static {
        System.loadLibrary("native");
    }

    static public class PlatformDelegate{
        private String value;
        public PlatformDelegate(String value){
            this.value = value;
        }
        public void handler(){
            System.out.println("PlatformDelegate:" + this.value);
        }
    }

    public Platform(){
        delegates = new ArrayList<PlatformDelegate>();
    }

    private List<PlatformDelegate> delegates;

    public void registerDelegate(PlatformDelegate delegate){
        this.delegates.add(delegate);
    }

    private String label = "Java String";

    public void showLabel(){
        System.out.println(this.label);
    }

    public native String nativeGetString();

    public native void nativeUpdate();

    public static void main(String [] args){
        Platform plt = new Platform();
        String result = plt.nativeGetString();
        System.out.println("\n------------------\n"+result);
        plt.showLabel();
        plt.nativeUpdate();
        plt.showLabel();
        for (int i = 0; i < plt.delegates.size(); i++) {
            PlatformDelegate delegate = plt.delegates.get(i);
            delegate.handler();
        }
    }
}


