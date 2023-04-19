package com.example.amqpspring;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

@CrossOrigin(origins = "*")
@RestController
public class MessageOfTopicController {
    @Autowired
    private TopicAmqpMessageRepository topicAmqpMessageRepository;

    @GetMapping("/message")
    public List<AmqpMessageDTO> getMessages(@RequestParam("topic") String topic,
                                    @RequestParam("page_num") int pageNum,
                                    @RequestParam("page_length") int pageLength){
        Pageable pageable = PageRequest.of(pageNum,pageLength, Sort.by("id").descending());
        List<AmqpMessage> amqpMessages = topicAmqpMessageRepository.findLatestByTopic(topic,pageable).getContent();

        return amqpMessages.stream().map(AmqpMessageDTO::new).collect(Collectors.toList());
    }

}
