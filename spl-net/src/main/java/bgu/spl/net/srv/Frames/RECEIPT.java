package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class RECEIPT extends Frame<Frame>
{

    public RECEIPT(int clientId)
    {
        super("RECEIPT",clientId);
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection)
    {
        return this;
    }
}
