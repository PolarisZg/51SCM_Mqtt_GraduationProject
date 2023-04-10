package com.example.amqpspring;

import org.springframework.data.repository.PagingAndSortingRepository;

public interface PageAmqpMessageRepository extends PagingAndSortingRepository<AmqpMessage,Long> {
}
