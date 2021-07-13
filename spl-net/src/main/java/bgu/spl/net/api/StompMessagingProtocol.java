package bgu.spl.net.api;

import bgu.spl.net.srv.Connections;

import java.io.Serializable;

public interface StompMessagingProtocol<T> extends MessagingProtocol<T>
{
	/**
	 * Used to initisate the current client protocol with it's personal connection ID and the connections implementation
	**/
   	 void start(int connectionId, Connections<String> connections);
    
    T process(T message);
	
	/**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
}
