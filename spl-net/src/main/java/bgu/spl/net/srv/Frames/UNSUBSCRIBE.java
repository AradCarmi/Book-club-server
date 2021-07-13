package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class UNSUBSCRIBE extends Frame<Frame>
{
    private String msg;
    private String messageId;

    public UNSUBSCRIBE(int clientId,String messageId,String msg)
    {
        super("UNSUBSCRIBE",clientId);
        this.msg=msg;
        this.messageId=messageId;
    }
    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection)
    {

        ERROR error =new ERROR(clientId);
        error.AddHeader("Message-id",messageId);
        error.AddHeader("The message",msg);
        String topic = headers.get("destination");
        if(!connection.alreadyConnect(clientId))
        {
            error.setBody("User is not connected");
            return error.ExecuteFrame(connection);
        }
        if ( headers.get("id") == null)
        {
            error.setBody("Did not contain a Id header, which is REQUIRED for message");
            return error.ExecuteFrame(connection);
        }
        int id = Integer.parseInt(headers.get("id"));
        if(!connection.unsubscribeToTopic(clientId,id))
        {
            error.setBody("Already unSubscribed to this topic");
            return error.ExecuteFrame(connection);
        }
        if(headers.get("receipt") == null)
            return null;
        RECEIPT receipt = new RECEIPT(clientId);
        receipt.AddHeader("receipt-id",headers.get("receipt"));
        return receipt.ExecuteFrame(connection);

    }

}
