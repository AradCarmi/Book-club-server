package bgu.spl.net.srv;

import bgu.spl.net.srv.Frames.Frame;
import bgu.spl.net.srv.Frames.MESSAGE;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T>  implements Connections<Frame>
{
    private Map<Integer,ConnectionHandler<String>> connectionHandlerMap;
    private Data data;

    public ConnectionsImpl()
    {
        connectionHandlerMap = new ConcurrentHashMap<>();;
        data=new Data();
    }

    @Override
    public boolean send(int connectionId, Frame msg)
    {
        try
        {
            connectionHandlerMap.get(connectionId).send(msg.toString());
            return true;
        }
        catch(Exception E)
        {
            return false;
        }
    }
    @Override
    public void send(String channel, Frame msg)
    {
        Integer subscription;
        Integer clientId;
        msg.AddHeader("subscription","");
        List<Integer> clientList = data.registeredClients(channel);
        if(clientList != null)
        {
            Iterator clientIterator = clientList.iterator();

            while (clientIterator.hasNext()) {
                clientId = (Integer) clientIterator.next();
                subscription = getSubscriptionId(clientId, channel);
                msg.changeHeader("subscription", Integer.toString(subscription));
                send(clientId.intValue(), msg);
            }
        }
    }
    @Override
    public void disconnect(int connectionId) {
        connectionHandlerMap.remove(connectionId);
        data.remove(connectionId);

    }
    public void addNewClient(ConnectionHandler connectionHandler,int clientId)
    {
        connectionHandlerMap.put(clientId,connectionHandler);
    }
    public boolean alreadyConnect(int connectionId)
    {
        return data.checkAlreadyLogin(data.getUsernameByConnectionid(connectionId));
    }
    public boolean alreadyConnect(String username)
    {
        return data.checkAlreadyLogin(username);
    }
    public boolean checkClient(String username,String password,int connectionid)
    {
        if(data.checkAlreadyLogin(username))
            return data.checkClientLoginInfo(username,password);
        data.addUserPasswordId(username,password,connectionid);
        return true;
    }
    public boolean subscribeToTopic(int connectionId,String topic, int idSubscriber)
    {
        return data.subscribeToTopic(connectionId,topic,idSubscriber);
    }
    public boolean unsubscribeToTopic(int connectionId, int idSubscriber)
    {
      return  data.unsubscribeToTopic(connectionId,idSubscriber);
    }
    public int getSubscriptionId(int connectionId,String topic)
    {
       return data.getSubscriptionId(connectionId,topic);
    }
}
