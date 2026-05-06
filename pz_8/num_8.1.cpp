// Завдання 1. Реалізувати алгоритм використання динамічного приорітету. 
// Процесу призначено початковий приорітет. Пріорітет процесу збільшується, якщо при 
// наступному зверненні до черги він не отримав процесорного часу. Після отримання 
// процесорного часу, його приорітет зменшується до початкового. 


#include <iostream> 
#include <vector>   

using namespace std; 

class Process { 
public: 
    int pid;
    int base_priority; 
    int current_priority;
    int remaining_time; 
    int times_skipped; 

    Process(int id, int base, int time)         //конструктор до класу
        : pid(id), 
          base_priority(base), 
          current_priority(base), 
          remaining_time(time), 
          times_skipped(0) {} 

    void print() const {        //вивід інформації про процес
        cout << "PID=" << pid 
             << ", base=" << base_priority
             << ", current=" << current_priority 
             << ", remaining=" << remaining_time; 
    }
};

class DynamicPriorityScheduler {    //клас планувальника з динамічним пріоритетом
private: 
    vector<Process> ready_queue;            //черга готових процесів 


    int selectHighestPriorityProcess() {    //повертає індекс процесу з найвищим current_priority
        int idx = 0; 
        for (size_t i = 1; i < ready_queue.size(); i++) {       //проходимо по всіх процесах
            if (ready_queue[i].current_priority >           //якщо знайшли процес з вищим пріоритетом
                ready_queue[idx].current_priority) {
                idx = i;        //запам'ятовуємо його індекс
            }
        }
        return idx; 
    }

    void increasePriorityForOthers(int executed_index) {        //піднімаємо пріоритет всім, крім виконаного
        for (size_t i = 0; i < ready_queue.size(); i++) { 
            if (i != executed_index) {              //якщо не той процес, який отримав CPU
                ready_queue[i].current_priority++;          //підвищуємо пріоритет на 1
                ready_queue[i].times_skipped++;           //збільшуємо лічильник 

                cout << "  PID=" << ready_queue[i].pid     
                     << " не отримав CPU -> пріоритет = " 
                     << ready_queue[i].current_priority << endl;
            }
        }
    }

    void executeProcess(int index) {            //виконує 1 одиницю часу процесу за індексом
        Process& p = ready_queue[index];        //посилання на процес для зручності

        cout << "\nВиконується: "; 
        p.print(); 
        cout << " (пропущено " << p.times_skipped << " раз)" << endl; 

        p.remaining_time--;     //зменшуємо залишок часу на 1 

        // після CPU скидання пріоритету
        p.current_priority = p.base_priority;   //повертаємо пріоритет до базового
        p.times_skipped = 0;        //обнуляємо лічильник пропусків

        cout << "   Після виконання: remaining=" << p.remaining_time 
             << ", priority=" << p.current_priority << endl; 

        // якщо завершився видаляємо
        if (p.remaining_time == 0) {    //якщо процес повністю виконався
            cout << "Процес PID=" << p.pid << " завершено" << endl; 
            ready_queue.erase(ready_queue.begin() + index);         //видаляємо процес з черги
        }
    }

public: 
    void addProcess(int pid, int base_priority, int time) {         //додає новий процес
        ready_queue.emplace_back(pid, base_priority, time);     //створюємо і додаємо процес в кінець черги
    }

    void printQueue() const {   //виводить початковий стан черги
        cout << "Початкова черга:\n"; 
        for (const auto& p : ready_queue) { 
            cout << "  "; 
            p.print();
            cout << endl;
        }
    }

    void run() {    //запускає планувальник
        cout << "\nДинамічний пріоритет\n";

        int step = 1;   //лічильник кроків

        while (!ready_queue.empty()) {          //поки є процеси в черзі
            cout << "\nКрок " << step++ << endl; 

            int idx = selectHighestPriorityProcess();           //обираємо процес з найвищим пріоритетом
            increasePriorityForOthers(idx);      //підвищуємо пріоритет всім іншим
            executeProcess(idx);             //виконуємо обраний процес
        }

        cout << "\nУсі процеси завершені\n"; 
    }
};

int main() { 
    DynamicPriorityScheduler scheduler;

    scheduler.addProcess(1, 2, 5);
    scheduler.addProcess(2, 1, 3); 
    scheduler.addProcess(3, 0, 4); 

    scheduler.printQueue(); 
    scheduler.run(); 

    return 0; 
}

// Результат:
// Початкова черга:
//   PID=1, base=2, current=2, remaining=5
//   PID=2, base=1, current=1, remaining=3
//   PID=3, base=0, current=0, remaining=4

// Динамічний пріоритет

// Крок 1
//   PID=2 не отримав CPU -> пріоритет = 2
//   PID=3 не отримав CPU -> пріоритет = 1

// Виконується: PID=1, base=2, current=2, remaining=5 (пропущено 0 раз)
//    Після виконання: remaining=4, priority=2

// Крок 2
//   PID=2 не отримав CPU -> пріоритет = 3
//   PID=3 не отримав CPU -> пріоритет = 2

// Виконується: PID=1, base=2, current=2, remaining=4 (пропущено 0 раз)
//    Після виконання: remaining=3, priority=2

// Крок 3
//   PID=1 не отримав CPU -> пріоритет = 3
//   PID=3 не отримав CPU -> пріоритет = 3

// Виконується: PID=2, base=1, current=3, remaining=3 (пропущено 2 раз)
//    Після виконання: remaining=2, priority=1

// Крок 4
//   PID=2 не отримав CPU -> пріоритет = 2
//   PID=3 не отримав CPU -> пріоритет = 4

// Виконується: PID=1, base=2, current=3, remaining=3 (пропущено 1 раз)
//    Після виконання: remaining=2, priority=2

// Крок 5
//   PID=1 не отримав CPU -> пріоритет = 3
//   PID=2 не отримав CPU -> пріоритет = 3

// Виконується: PID=3, base=0, current=4, remaining=4 (пропущено 4 раз)
//    Після виконання: remaining=3, priority=0

// Крок 6
//   PID=2 не отримав CPU -> пріоритет = 4
//   PID=3 не отримав CPU -> пріоритет = 1

// Виконується: PID=1, base=2, current=3, remaining=2 (пропущено 1 раз)
//    Після виконання: remaining=1, priority=2

// Крок 7
//   PID=1 не отримав CPU -> пріоритет = 3
//   PID=3 не отримав CPU -> пріоритет = 2

// Виконується: PID=2, base=1, current=4, remaining=2 (пропущено 3 раз)
//    Після виконання: remaining=1, priority=1

// Крок 8
//   PID=2 не отримав CPU -> пріоритет = 2
//   PID=3 не отримав CPU -> пріоритет = 3

// Виконується: PID=1, base=2, current=3, remaining=1 (пропущено 1 раз)
//    Після виконання: remaining=0, priority=2
// Процес PID=1 завершено

// Крок 9
//   PID=2 не отримав CPU -> пріоритет = 3

// Виконується: PID=3, base=0, current=3, remaining=3 (пропущено 3 раз)
//    Після виконання: remaining=2, priority=0

// Крок 10
//   PID=3 не отримав CPU -> пріоритет = 1

// Виконується: PID=2, base=1, current=3, remaining=1 (пропущено 2 раз)
//    Після виконання: remaining=0, priority=1
// Процес PID=2 завершено

// Крок 11

// Виконується: PID=3, base=0, current=1, remaining=2 (пропущено 1 раз)
//    Після виконання: remaining=1, priority=0

// Крок 12

// Виконується: PID=3, base=0, current=0, remaining=1 (пропущено 0 раз)
//    Після виконання: remaining=0, priority=0
// Процес PID=3 завершено

// Усі процеси завершені