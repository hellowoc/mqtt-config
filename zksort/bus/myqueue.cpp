#include "myqueue.h"

MySerialQueue myQueue;
MySerialQueue2 myQueue2;

MySerialQueue::MySerialQueue()
{
	m_queue = (SqQueue *)malloc(sizeof(SqQueue));	
	m_queue->rear = 0;
    m_queue->front = 0;

    QMutexLocker lock(&m_queueLock);
}


MySerialQueue::~MySerialQueue()
{
	if (m_queue)
	{
		free(m_queue);
		m_queue = NULL;
	}	
}

bool MySerialQueue::_isEmpty()
{
	if (m_queue == NULL)
	{
		return true;
	}
	return (m_queue->front == m_queue->rear ? true : false);
}
bool MySerialQueue::_push(char* data)
{
	if (m_queue == NULL)
	{
		return false;
	}
	if ((m_queue->rear + 1) % QUEUE_SIZE == m_queue->front) 
	{
		return false;
	}	
    memcpy(m_queue->data[m_queue->rear], data, SEND_PACKET_SIZE);
    m_queue->rear = (m_queue->rear + 1) % QUEUE_SIZE;
	return true;
}
bool MySerialQueue::_pop(char* data)
{
	if (m_queue == NULL)
	{
		return false;
	}
	if (m_queue->rear == m_queue->front)
	{
		return false;
	}
    memcpy(data, m_queue->data[m_queue->front], SEND_PACKET_SIZE);
	m_queue->front = (m_queue->front + 1) % QUEUE_SIZE;
	return true;
}
bool MySerialQueue::isEmpty()
{
    QMutexLocker lock(&m_queueLock);
    bool ret = _isEmpty();
	return ret;
}
bool MySerialQueue::push(char* data)
{
    QMutexLocker lock(&m_queueLock);
    bool ret = _push(data);
	return ret;
}
bool MySerialQueue::pop(char* data)
{
    QMutexLocker lock(&m_queueLock);
    bool ret = _pop(data);
	return ret;
}



/*    Squeue2    */
MySerialQueue2::MySerialQueue2()
{
    m_queue2 = (SqQueue2 *)malloc(sizeof(SqQueue2));
    m_queue2->rear = 0;
    m_queue2->front = 0;

    QMutexLocker lock(&m_queue2Lock);
}


MySerialQueue2::~MySerialQueue2()
{
    if (m_queue2)
    {
        free(m_queue2);
        m_queue2 = NULL;
    }
}

bool MySerialQueue2::_isEmpty()
{
    if (m_queue2 == NULL)
    {
        return true;
    }
    return (m_queue2->front == m_queue2->rear ? true : false);
}
bool MySerialQueue2::_push(char* data)
{
    if (m_queue2 == NULL)
    {
        return false;
    }
    if ((m_queue2->rear + 1) % QUEUE_SIZE == m_queue2->front)
    {
        return false;
    }
    memcpy(m_queue2->data[m_queue2->rear], data, SEND_PACKET_RECTRL_BOARD_SIZE);
    m_queue2->rear = (m_queue2->rear + 1) % QUEUE_SIZE;
    return true;
}
bool MySerialQueue2::_pop(char* data)
{
    if (m_queue2 == NULL)
    {
        return false;
    }
    if (m_queue2->rear == m_queue2->front)
    {
        return false;
    }
    memcpy(data, m_queue2->data[m_queue2->front], SEND_PACKET_RECTRL_BOARD_SIZE);
    m_queue2->front = (m_queue2->front + 1) % QUEUE_SIZE;
    return true;
}
bool MySerialQueue2::isEmpty()
{
    QMutexLocker lock(&m_queue2Lock);
    bool ret = _isEmpty();
    return ret;
}
bool MySerialQueue2::push(char* data)
{
    QMutexLocker lock(&m_queue2Lock);
    bool ret = _push(data);
    return ret;
}
bool MySerialQueue2::pop(char* data)
{
    QMutexLocker lock(&m_queue2Lock);
    bool ret = _pop(data);
    return ret;
}

