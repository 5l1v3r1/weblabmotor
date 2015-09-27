package com.example.weblab;

import java.io.File;

import es.deusto.weblab.experimentservers.ExperimentServer;
import es.deusto.weblab.experimentservers.exceptions.ExperimentServerInstantiationException;
import es.deusto.weblab.experimentservers.exceptions.WebLabException;
import sun.org.mozilla.javascript.regexp.SubString;

public class DummyExperimentServer extends ExperimentServer {

	public DummyExperimentServer()
			throws ExperimentServerInstantiationException {
		super();
	}

	public void startExperiment() throws WebLabException {
		System.out.println("I'm at startExperiment");
	}

	public String sendFile(File file, String fileInfo)  throws WebLabException {
		System.out.println("I'm at send_program: " + file.getAbsolutePath() + "; fileInfo: " + fileInfo);
		return "ok";
	}
	
	public String sendCommand(String command) {
					
		ArduinoEthernetComm aec = new ArduinoEthernetComm();
		
		if(command.startsWith("usr")){
			String comando = command.substring(4);
			System.out.println(comando);
			
			try {
				//Unidade de Sensoriamento Remoto - GALILEO
				aec.CommEthArduino("192.168.0.101", 10000, comando);		
			} catch (Exception e) {
				e.printStackTrace();
			}
			
		}else if(command.startsWith("ucr")){
			System.out.println("Controlar ARDUINO");
			String comando = command.substring(4);
			System.out.println(comando);
	
			try {
				//Unidade de Controle Remoto -  ARDUINO
				aec.CommEthArduino("192.168.0.102", 10000, comando);		
			} catch (Exception e) {
				e.printStackTrace();
			}
	
		}	
			return "ok";
	}
	
	public void dispose() {
		System.out.println("I'm at dispose");

	}
	
	/* Optional methods (Override, Java 1.4 compatible...) */
	
	public boolean isUpAndRunning() {
		return false;
	}
}
