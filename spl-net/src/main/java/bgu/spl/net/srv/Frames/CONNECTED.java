package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class CONNECTED extends Frame<Frame>
{
    public CONNECTED(int clientId)
    {
        super("CONNECTED",clientId);
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection)
    {
        return this;
    }
}
