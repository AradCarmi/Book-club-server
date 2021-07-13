package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.ConnectionsImpl;

public class CONNECT extends Frame<Frame> {

    private String msg;
    private String messageId;

    public CONNECT(int clientId,String messageId,String msg)
    {
        super("CONNECT",clientId);
        this.msg = msg;
        this.messageId=messageId;
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connections)
    {
        String username;
        String password;
        String receipt;
        receipt = headers.get("receipt");
        String version;
        ERROR error =new ERROR(clientId);
        error.AddHeader("message-id",messageId);
        error.AddHeader("The message",msg);
        if(receipt != null)
            error.AddHeader("receipt-id",receipt);
        username = headers.get("login");
        password = headers.get("passcode");
        version = headers.get("accept-version");
        if(username != null && password != null)
        {
            if(connections.alreadyConnect(username)) // account already connected
            {
                error.setBody("User already logged in");
                return error.ExecuteFrame(connections);
            }
            if (connections.checkClient(username, password,clientId)) // login info correct
            {
                if(version == null)
                {
                    error.setBody("Did not contain a version header, which is REQUIRED for message");
                    return error.ExecuteFrame(connections);
                }
                CONNECTED connected= new CONNECTED(clientId);
                if(receipt != null)
                connected.AddHeader("receipt-id",receipt);
                connected.AddHeader("version",version);
                connected.setBody(body);
                return connected.ExecuteFrame(connections);
            }
            else
                {
                error.setBody("Wrong password");
                return error.ExecuteFrame(connections);
                }
        }
        else {
            error.setBody("Did not contain a username/passcode header, which is REQUIRED for message");
            return error.ExecuteFrame(connections);
        }

    }
}
