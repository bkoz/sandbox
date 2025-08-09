#include <iostream>
#include <thread>
#include <semaphore> // For std::counting_semaphore

const int MAX_THREADS = 3;
std::counting_semaphore<MAX_THREADS> processing_slots(MAX_THREADS);

void process_task(int id) {
    processing_slots.acquire(); // Acquire a slot
    std::cout << "Thread " << id << " is processing..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    std::cout << "Thread " << id << " finished processing. Releasing slot." << std::endl;
    processing_slots.release(); // Release the slot
}

int main() {
    std::thread t1(process_task, 1);
    std::thread t2(process_task, 2);
    std::thread t3(process_task, 3);
    std::thread t4(process_task, 4); // This thread will block until a slot is free

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
