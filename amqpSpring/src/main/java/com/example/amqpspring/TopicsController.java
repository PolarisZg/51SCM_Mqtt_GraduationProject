package com.example.amqpspring;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@CrossOrigin(origins = "*")
@RestController
@RequestMapping("/topics")
public class TopicsController {
    @Autowired
    private TopicAmqpMessageRepository topicAmqpMessageRepository;

    @GetMapping("")
    public List<String> getAllTopics(){
        return topicAmqpMessageRepository.findAllTopics();
    }
}
