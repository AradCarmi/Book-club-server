package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class Subscription
{
    private Map<String,Integer> topicId;

    public Subscription(String topic, Integer id)
    {
        topicId = new HashMap<>();
        topicId.put(topic,id);
    }
    public Integer getId(String topic)
    {
        return topicId.get(topic);
    }
    public String getTopic(Integer id)
    {
        for(Map.Entry<String,Integer> entry : topicId.entrySet())
        {
            if(entry.getValue() == id)
                return entry.getKey();
        }
        return null;
    }
    public void setTopicId(String topic,Integer id)
    {
        topicId.put(topic,id);
    }
    public boolean contain(String topic, Integer id)
    {
         if(topicId.containsKey(topic) && topicId.containsValue(id))
             return true;
         return false;
    }
    public boolean remove(Integer id)
    {
        for(Map.Entry<String,Integer> entry : topicId.entrySet())
        {
            if(entry.getValue() == id)
            {
                topicId.remove(entry.getKey());
                return true;
            }
        }
        return false;
    }
}
