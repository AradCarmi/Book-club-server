package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

import java.sql.Connection;

public class SUBSCRIBE extends Frame<String>
{
    private String msg;
    private String messageId;

    public SUBSCRIBE(int clientId,String messageId,String msg)
    {
        super("SUBSCRIBE",clientId);
        this.msg=msg;
        this.messageId=messageId;
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection)
    {
        ERROR error =new ERROR(clientId);
        RECEIPT receipt = new RECEIPT(clientId);
        error.AddHeader("Message-id",messageId);
        error.AddHeader("The message",msg);
        String topic = headers.get("destination");
        int id;
        if(headers.get("id") == null)
        {
            error.setBody("Did not contain a Id header, which is REQUIRED for message");
            return error.ExecuteFrame(connection);
        }
         id = Integer.parseInt(headers.get("id"));
        if(!connection.alreadyConnect(clientId))
        {
            error.setBody("User is not connected");
            return error.ExecuteFrame(connection);
        }
        if (topic == null)
        {
            error.setBody("Did not contain a Destination header, which is REQUIRED for message");
            return error.ExecuteFrame(connection);
        }
        if(!connection.subscribeToTopic(clientId,topic,id))
        {
            error.setBody("Already Subscribed to this topic");
            return error.ExecuteFrame(connection);
        }
        if(headers.get("receipt") == null)
            return null;
        receipt.AddHeader("receipt-id",headers.get("receipt"));
        return receipt.ExecuteFrame(connection);
    }
}
