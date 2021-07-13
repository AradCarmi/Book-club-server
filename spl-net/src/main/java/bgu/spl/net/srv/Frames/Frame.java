package bgu.spl.net.srv.Frames;

import bgu.spl.net.srv.ConnectionsImpl;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public abstract class Frame<T>
{
    private String CommandName;
    protected Map<String,String> headers;
    protected String body;
    protected int clientId;

    Frame(String commandName,int clientId)
    {
        this.CommandName=commandName;
        headers=new HashMap<>();
        this.clientId=clientId;
        body="";
    }

    public void AddHeader(String key,String value)
    {
        headers.put(key,value);
    }
    public void changeHeader(String key,String value)
    {
        headers.replace(key,value);
    }

    public void setBody(String body)
    {
        if(body == null)
            this.body="";
        else
            {
            this.body = body;
            }
    }

    public String getCommandName()
    {
        return this.CommandName;
    }

    public abstract Frame ExecuteFrame(ConnectionsImpl connection);

    @Override
    public String toString()
    {
        String returnString="";
        returnString=CommandName+'\n'; //Stomp command name
        for (String key : headers.keySet()) //headers lines
        {
            returnString=returnString+key+":"+headers.get(key)+'\n';
        }
        returnString=returnString+'\n'+ body;
        return returnString;
    }
}
