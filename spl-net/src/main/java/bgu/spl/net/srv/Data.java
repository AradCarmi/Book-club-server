package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Data {
    private Map<String, List<Integer>> topicMap; //<topic,list of subscribers>
    private Map<String,String> usernamePassword; // <username,password>
    private Map<Integer,Subscription> clientSubscriptionTopic; //<idConnection,Map<idSubscribtion,topic>>
    private Map<String,Integer> usernameConnectionId; // <username,idConnection>

    Data()
    {
        topicMap = new ConcurrentHashMap<>();
        usernamePassword = new ConcurrentHashMap<>();
        clientSubscriptionTopic=new ConcurrentHashMap<>();
        usernameConnectionId = new ConcurrentHashMap<>();
    }
    public boolean subscribeToTopic(int connectionId,String topic, int idSubscriber)
    {
            if(!topicMap.containsKey(topic)) // if new topic add it
                topicMap.put(topic,new LinkedList<Integer>());
            if (!topicMap.get(topic).contains(connectionId)) // if the client is not already registered to the topic
                topicMap.get(topic).add(connectionId);
            if (clientSubscriptionTopic.containsKey(connectionId)) //if its exist client
                if(!clientSubscriptionTopic.get(connectionId).contain(topic,idSubscriber)) // if there is no any topic with the idSubscriber add it the the subscriber map
                    clientSubscriptionTopic.get(connectionId).setTopicId(topic,idSubscriber);
                else
                    {
                    return false;
                    }
            else
                {
                        clientSubscriptionTopic.put(connectionId,new Subscription(topic,idSubscriber));
                }
                    return true;
    }
    public boolean unsubscribeToTopic(Integer connectionId ,int idSubscriber)
    {
        String topic = clientSubscriptionTopic.get(connectionId).getTopic(idSubscriber);
         clientSubscriptionTopic.get(connectionId).remove(idSubscriber);
        if(topic != null && topicMap.get(topic).contains(connectionId))
        {
            if(topicMap.get(topic).contains(connectionId))
                topicMap.get(topic).remove(connectionId);
            return true;
        }
        return false;
    }
    public List<Integer> registeredClients(String topic)
    {
        List<Integer> clients = new LinkedList<>();
        if(!topicMap.containsKey(topic))
            return null;
        for (int id : topicMap.get(topic))
        {
            clients.add(id);
        }
        return clients;
    }
    public void remove(Integer connectionId)
    {
        for(String topic : topicMap.keySet())
        {
            if(topicMap.get(topic).contains(connectionId))
                topicMap.get(topic).remove(connectionId);
        }
        usernameConnectionId.remove(getUsernameByConnectionid(connectionId));
    }
    public String getUsernameByConnectionid(int connectionId)
    {
        for(String username : usernameConnectionId.keySet())
        {
            if (usernameConnectionId.get(username) == connectionId)
                return username;
        }
        return null;
    }
    public void addUserPasswordId(String username,String password,int connectionid)
    {
        usernameConnectionId.put(username,connectionid);
        usernamePassword.put(username,password);
    }
    public boolean checkClientLoginInfo(String userName,String password)
    {
        return usernamePassword.get(userName).equals(password);
    }
    public boolean checkAlreadyLogin(String username)
    {
        if(username == null)
            return false;
        return usernameConnectionId.containsKey(username);
    }
    public int getSubscriptionId(int connectionId,String topic)
    {
       return clientSubscriptionTopic.get(connectionId).getId(topic);
    }

}
