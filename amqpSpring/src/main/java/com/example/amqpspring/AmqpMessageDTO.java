package com.example.amqpspring;

import java.sql.Timestamp;

public class AmqpMessageDTO {

    private String topic;

    private String content;

    /*
     * UTF-8
     * 该属性仅能存储至2038年, 超过这个时间不保证程序可以稳定运行
     * This attribute can only be stored until 2038, beyond this time, the program cannot be guaranteed to run stably
     * */
    private Timestamp timestamp;

    public AmqpMessageDTO(AmqpMessage amqpMessage){
        this.content = amqpMessage.getContent();
        this.timestamp = amqpMessage.getTimestamp();
        this.topic = amqpMessage.getTopic();
    }
    public String getTopic(){
        return topic;
    }

    public void setTopic(String topic){
        this.topic = topic;
    }

    public String getContent(){
        return content;
    }

    public void setContent(String content){
        this.content = content;
    }

    public void setTimestamp(Timestamp timestamp){
        this.timestamp = timestamp;
    }

    public Timestamp getTimestamp(){
        return timestamp;
    }
}
