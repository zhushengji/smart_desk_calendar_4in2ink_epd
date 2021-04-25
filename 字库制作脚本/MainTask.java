package tool;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;

public class MainTask {
	public static String txt2String(File file){
//        StringBuilder result = new StringBuilder();
        String str = "";
        try{
            BufferedReader br = new BufferedReader(new FileReader(file));
            String s,str2 = null;
            
            while((s = br.readLine())!=null){
            	if(s.contains("/*--  ÎÄ×Ö")) {
            		str2=s;
            		s="},\n"+s;
            	}
            	if(s.contains("16x19  --*/")) {
            		s+="\n{\""+str2.charAt(11)+"\",";
            		System.out.println(s);
            		str2=null;
            	}
            	str=str+s+"\n";
            }
            br.close();    
        }catch(Exception e){
            e.printStackTrace();
        }
        return str;//result.toString();
    }
	public static void writeTxt(String txtPath,String content){    
	       FileOutputStream fileOutputStream = null;
	       File file = new File(txtPath);
	       try {
	           if(file.exists()){
	               file.createNewFile();
	           }
	           fileOutputStream = new FileOutputStream(file);
	           fileOutputStream.write(content.getBytes());
	           fileOutputStream.flush();
	           fileOutputStream.close();
	       } catch (Exception e) {
	           e.printStackTrace();
	       }
	    }
	public static void main(String[] args) {
		File file = new File("F:/src.txt");
		writeTxt("F:/src2.txt",txt2String(file));
        System.out.println("Íê³É");
    }
}
