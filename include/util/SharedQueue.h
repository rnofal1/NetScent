/* This file contains declarations and definitions for the template SharedQueue class
 * https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl
 * https://forum.qt.io/topic/124166/producer-consumer-multithread-qqueue-qsemaphore/4
 * Ramsey Nofal, 11/2023
 */


#ifndef SHAREDQUEUE_H
#define SHAREDQUEUE_H


//Standard Qt
#include <QQueue>
#include <QMutexLocker>
#include <QSemaphore>
#include <QDebug>

//Defines
#define DEFAULT_CAP 9999

template <typename T>
class SharedQueue {
public:
    SharedQueue();
    SharedQueue(int capacity);
    void push(const T& val);
    T pop();
    bool is_empty();
    bool is_full();

    void clear();
private:
    QQueue<T> queue;
    int capacity;

    QMutex mutex;
    QSemaphore availability;
    QSemaphore input_needs;
};

template <typename T>
SharedQueue<T>::SharedQueue() : capacity(DEFAULT_CAP), input_needs(DEFAULT_CAP){
    qDebug() << "SharedQueue initialized with default capacity: " << DEFAULT_CAP;
}

template <typename T>
SharedQueue<T>::SharedQueue(int capacity) : capacity(capacity), input_needs(capacity) {
    qDebug() << "SharedQueue initialized with input capacity: " << capacity;
}

template <typename T>
void SharedQueue<T>::push(const T& val) {
    input_needs.acquire(); //Prevents overfilling the queue
    QMutexLocker lock(&mutex); //Prevents concurrent access/modification
    queue.enqueue(val);
    availability.release();
}

template <typename T>
T SharedQueue<T>::pop() {
    availability.acquire(); //Prevents concurrent modification
    QMutexLocker lock(&mutex); //Prevents concurrent access/modification
    if(queue.empty()) {
        return NULL;
    }
    T val = queue.dequeue();
    input_needs.release();
    return val;
}

template <typename T>
bool SharedQueue<T>::is_empty() {
    QMutexLocker lock(&mutex); //Prevents concurrent access/modification
    return queue.empty();
}

template <typename T>
bool SharedQueue<T>::is_full() {
    QMutexLocker lock(&mutex); //Prevents concurrent access/modification
    return queue.size() == capacity;
}

template <typename T>
void SharedQueue<T>::clear() {
    QMutexLocker lock(&mutex); //Prevents concurrent access/modification
    queue.clear();
    input_needs.release(capacity);
}

#endif // SHAREDQUEUE_H
