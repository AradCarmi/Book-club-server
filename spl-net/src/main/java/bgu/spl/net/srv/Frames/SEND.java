package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.ConnectionsImpl;

public class SEND extends Frame<Frame>
{
    private String msg;
    private String messageId;

    public SEND(int clientId,String messageId,String msg)
    {
        super("SEND",clientId);
        this.msg=msg;
        this.messageId=messageId;
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection) {
        ERROR error =new ERROR(clientId);
        RECEIPT Receipt = new RECEIPT(clientId);
        MESSAGE message = new MESSAGE(clientId);
        String topic = headers.get("destination");
        error.AddHeader("Message-id",messageId);
        error.AddHeader("The message",msg);
        String receipt = headers.get("receipt");
        if(receipt != null)
        {
            error.AddHeader("receipt-id",receipt);
            Receipt.AddHeader("receipt-id",receipt);
            return Receipt.ExecuteFrame(connection);
        }
        if(!connection.alreadyConnect(clientId))
        {
            error.setBody("User is not connected");
            return error.ExecuteFrame(connection);
        }
        if(topic == null)
        {
            error.setBody("Did not contain a Destination header, which is REQUIRED for message");
            return error.ExecuteFrame(connection);
        }
        message.AddHeader("destination",topic);
        message.AddHeader("Message-id",messageId);
        message.setBody(body);
        connection.send(topic,message);
        return null;
    }
}
