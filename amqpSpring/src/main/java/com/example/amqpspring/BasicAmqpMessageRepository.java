package com.example.amqpspring;

import org.springframework.data.jpa.repository.JpaRepository;

public interface BasicAmqpMessageRepository extends JpaRepository<AmqpMessage,Long> {
    AmqpMessage save(AmqpMessage amqpMessage);
}
