package com.example.amqpspring;

import jakarta.annotation.PostConstruct;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Component;

import java.util.Random;

//@Component
public class AmqpFullSQL {
    @Autowired
    SaveAmqpMessageRepository saveAmqpMessageRepository;

    private final Logger logger = LoggerFactory.getLogger(AmqpClient.class);

    @PostConstruct
    public void init() throws Exception{
        for(int i = 0 ; i < 100 ; i++){
            AmqpMessage amqpMessage = new AmqpMessage();
            amqpMessage.setTopic("/a62xpdiPmWd/51" + generateRandomString() + "Test/user/update");
            amqpMessage.setMessageId(generateRandomMessageId());
            logger.info("messageid" + amqpMessage.getMessageId());
            amqpMessage.setContent(generateContent());
            logger.info("content" + amqpMessage.getContent());
            saveAmqpMessageRepository.save(amqpMessage);
            try {
                Thread.sleep(1000); // 延迟1s
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static String generateRandomMessageId() {
        Random random = new Random();
        StringBuilder sb = new StringBuilder(19);
        sb.append(random.nextInt(9) + 1);
        for (int i = 1; i < 19; i++) {
            sb.append(random.nextInt(10));
        }
        return sb.toString();
    }

    public static String generateContent() {
        StringBuilder sb = new StringBuilder();
        Random rand = new Random();
        for (int i = 0; i < 7; i++) {
            int n = rand.nextInt(2);
            if (n == 0) {
                sb.append(i);
            } else {
                sb.append("#");
            }
        }
        return sb.toString();
    }

    private static final String[] STRINGS = {"First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eighth", "Ninth", "Tenth", "Eleventh", "Twelfth", "Thirteenth", "Fourteenth", "Fifteenth", "Sixteenth", "Seventeenth", "Eighteenth", "Nineteenth"};

    public static String generateRandomString() {
        Random random = new Random();
        int index = random.nextInt(STRINGS.length);
        return STRINGS[index];
    }
}
