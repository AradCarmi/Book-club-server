package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;


public class ERROR extends Frame<Frame>
{
    public ERROR(int clientId)
    {
        super("ERROR",clientId);
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection)
    {
        AddHeader("message","malformed frame received");
        if(connection.alreadyConnect(clientId))
            connection.disconnect(clientId);
        return this;
    }
}
