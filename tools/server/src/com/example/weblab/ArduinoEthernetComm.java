package com.example.weblab;

import java.io.DataOutputStream;
import java.net.Socket;


public class ArduinoEthernetComm
{
	
	public ArduinoEthernetComm(){
		
	}
	
	public void CommEthArduino(String serverIP, int serverPort, String msgToServer) throws Exception
	 {  

		  Socket clientSocket = new Socket(serverIP, serverPort);//making the socket connection
		  System.out.println("Connected to:"+serverIP+" on port:"+serverPort);//debug
		  
		  //OutputStream to Arduino-Server
		  DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
		  
		  //BufferedReader from Arduino-Server
		  //BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));//

		  outToServer.writeBytes(msgToServer+'\n');//sending the message
		  
		  System.out.println("sending to Arduino-Server: "+msgToServer);//debug
		  		  	      
	      //if((msgFromServer = inFromServer.readLine()) != null){
		  //    System.out.println("received from Arduino-Server: " + msgFromServer);//print the answer
	      //}
	      
		  clientSocket.close();
		  System.out.println("Disconnected from:"+serverIP);//debug
		  
	 }

}