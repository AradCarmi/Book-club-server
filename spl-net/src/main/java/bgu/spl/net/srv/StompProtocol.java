package bgu.spl.net.srv;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Frames.*;

import java.io.Serializable;
import java.nio.charset.StandardCharsets;

public class StompProtocol implements StompMessagingProtocol<String>
{
    private boolean shouldTerminate = false;
    private ConnectionHandler clientConnectionHandler;
    private ConnectionsImpl connections;
    private int clientId;
    private int messageId;


    public StompProtocol()
    {
        messageId = 0;
    }

    @Override
    public String process(String msg)
    {
        Frame currentFrame= StringToFrameConverter(msg);
        Frame answer = currentFrame.ExecuteFrame(connections);
        if ((answer != null ) && (currentFrame.getCommandName().equals("DISCONNECT") | answer.equals("ERROR")))
            shouldTerminate=true;
        if(answer == null)
            return null;
        return answer.toString();
    }

    private Frame StringToFrameConverter(String msg)
    {
        Frame frame=null;
        String[] lines = msg.split(System.getProperty("line.separator"));
        messageId++;
        String stringMessageId = Integer.toString(messageId);
        switch (lines[0]) //recognize the Stomp command type
        {
            case "CONNECT":
                frame=new CONNECT(clientId,stringMessageId,msg);
                break;
            case "DISCONNECT":
                frame=new DISCONNECT(clientId,stringMessageId,msg);
                break;
            case "SEND":
                frame=new SEND(clientId,stringMessageId,msg);
                break;
            case "SUBSCRIBE":
                frame=new SUBSCRIBE(clientId,stringMessageId,msg);
                break;
            case "UNSUBSCRIBE":
                frame=new UNSUBSCRIBE(clientId,stringMessageId,msg);
                break;
            default:
                frame = new ERROR(clientId);
                //TODO not valid stomp command
        }
        if(frame.getCommandName().equals("ERROR"))
        {
            frame.AddHeader("message","COMMAND");
            frame.AddHeader("receipt-id",stringMessageId);
            frame.AddHeader("The message",msg);
            frame.setBody("Illegal command");
            return frame;
        }
        for (int i=1;i<lines.length;i++)
        {
            if (lines[i].equals("")) // if it is blank line the rest of the lines are the body
            {
                String body="";
                for (int j=i+1;j<lines.length;j++)
                    body=body+lines[j];
                frame.setBody(body);
                break;
            }
            else if (lines[i].contains(":")) //if it is header line add the header to the frame
            {
                String key=lines[i].substring(0,lines[i].indexOf(':'));
                String value=lines[i].substring(lines[i].indexOf(':')+1);
                frame.AddHeader(key,value);
            }
        }
        return frame;
    }

    @Override
    public void start(int connectionId, Connections<String> connections)
    {
        this.connections=(ConnectionsImpl)connections;
        clientId=connectionId;
    }

    @Override
    public boolean shouldTerminate()
    {
        return shouldTerminate;
    }
}
