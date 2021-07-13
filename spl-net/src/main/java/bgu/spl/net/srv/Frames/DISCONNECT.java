package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.ConnectionsImpl;

public class DISCONNECT extends Frame<Frame>
{
    private String msg;
    private String messageId;

    public DISCONNECT(int clientId,String messageId,String msg)
    {
        super("DISCONNECT",clientId);
        this.msg=msg;
        this.messageId=messageId;
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection)
    {

        String receipt = headers.get("receipt");
        ERROR error =new ERROR(clientId);
        error.AddHeader("Message-id",messageId);
        error.AddHeader("The message",msg);
        error.setBody("Did not contain a receipt header, which is REQUIRED for message");
        if(!connection.alreadyConnect(clientId))
        {
            error.setBody("User is not connected");
            return error.ExecuteFrame(connection);
        }
            connection.disconnect(clientId);
        if(receipt == null)
            return null;
        RECEIPT Receipt = new RECEIPT(clientId);
        Receipt.AddHeader("receipt-id",receipt);
        return Receipt.ExecuteFrame(connection);
    }
}
