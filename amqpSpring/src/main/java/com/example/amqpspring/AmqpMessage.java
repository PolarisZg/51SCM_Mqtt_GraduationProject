package com.example.amqpspring;

import jakarta.persistence.*;

import java.sql.Timestamp;


@Entity
@Table(name = "amqpmessage")
public class AmqpMessage {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id")
    private Long id;

    @Column(name = "topic")
    private String topic;

    @Column(name = "messageid")
    private String messageId;

    @Column(name = "content")
    private String content;

    /*
    * UTF-8
    * 该属性仅能存储至2038年, 超过这个时间不保证程序可以稳定运行
    * This attribute can only be stored until 2038, beyond this time, the program cannot be guaranteed to run stably
    * */
    @Column(name = "timestamp", columnDefinition = "TIMESTAMP DEFAULT CURRENT_TIMESTAMP")
    private Timestamp timestamp;

    public Long getId(){
        return id;
    }

    public void setId(Long id){
        this.id = id;
    }

    public String getTopic(){
        return topic;
    }

    public void setTopic(String topic){
        this.topic = topic;
    }

    public String getMessageId(){
        return messageId;
    }

    public void setMessageId(String messageId){
        this.messageId = messageId;
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

// 界面设计更加专业一些
// 可以放日志
