package com.example.amqpspring;

import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface TopicAmqpMessageRepository extends JpaRepository<AmqpMessage, Long> {
    @Query("SELECT DISTINCT am.topic from AmqpMessage am")
    List<String> findAllTopics();

    @Query("select am from AmqpMessage am where am.topic = :topic ORDER BY am.id desc ")
    Page<AmqpMessage> findLatestByTopic(@Param("topic") String topic, Pageable pageable);

    @Query("select am from AmqpMessage am where am.topic = :topic")
    List<AmqpMessage> findByTopic(@Param("topic") String topic);
}
