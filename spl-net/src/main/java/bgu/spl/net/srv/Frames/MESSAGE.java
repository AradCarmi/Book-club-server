package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

import java.sql.Connection;

public class MESSAGE extends Frame<Frame>
{
    public MESSAGE(int clientId)
    {
        super("MESSAGE",clientId);
    }

    @Override
    public Frame ExecuteFrame(ConnectionsImpl connection)
    {
        return this;
    }
}
