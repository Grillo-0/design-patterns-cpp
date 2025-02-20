/**
     * EN: Have in mind it is an ilustrative trivial example, in real world
     *     you may have in mind some more possible issues.
     *
     * RU: 
     */

#include <iostream>
#include <mutex>
#include <thread>

/**
 * EN: Singleton Design Pattern
 *
 * Intent: Lets you ensure that a class has only one instance, while providing a
 * global access point to this instance.
 *
 * RU: Паттерн Одиночка
 *
 * Назначение: Гарантирует, что у класса есть только один экземпляр, и
 * предоставляет к нему глобальную точку доступа.
 */
/**
 * EN: The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 *
 * RU: Класс Одиночка предоставляет метод `GetInstance`, который ведёт себя как
 * альтернативный конструктор и позволяет клиентам получать один и тот же
 * экземпляр класса при каждом вызове.
 */
class Singleton
{

    /**
     * EN: The Singleton's constructor/destructor should always be private to prevent
     * direct construction/desctruction calls with the `new`/`delete` operator.
     *
     * RU: Конструктор Одиночки всегда должен быть скрытым, чтобы предотвратить
     * создание объекта через оператор new.
     */
private:
    static Singleton * pinstance_;
    static std::mutex mutex_;

protected:
    Singleton(const std::string value): value_(value)
    {
    }
    ~Singleton() {}
    std::string value_;

public:
    /**
     * EN: Singletons should not be cloneable.
     *
     * RU: Одиночки не должны быть клонируемыми.
     */
    Singleton(Singleton &other) = delete;
    /**
     * EN: Singletons should not be assignable.
     *
     * RU:
     */
    void operator=(const Singleton &) = delete;
    /**
     * EN: This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     *
     *
     * RU: Это статический метод, управляющий доступом к экземпляру одиночки.
     * При первом запуске, он создаёт экземпляр одиночки и помещает его в
     * статическое поле. При последующих запусках, он возвращает клиенту объект,
     * хранящийся в статическом поле.
     *
     */

    static Singleton &GetInstance(const std::string& value);
    /**
     * EN: Finally, any singleton should define some business logic, which can
     * be executed on its instance.
     *
     * RU: Наконец, любой одиночка должен содержать некоторую бизнес-логику,
     * которая может быть выполнена на его экземпляре.
     */
    void SomeBusinessLogic()
    {
        // ...
    }
    
    std::string value() const{
        return value_;
    } 
};

/**
     * EN: Static methods should be defined outside the class.
     *
     * RU: 
     */

Singleton* Singleton::pinstance_{nullptr};
std::mutex Singleton::mutex_;

/**
     * EN: The first time we call GetInstance we will lock the storage location
     *      and then we make sure again that the variable is null and then we 
     *      set the value.
     * RU: 
     */
Singleton &Singleton::GetInstance(const std::string& value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr)
    {
        pinstance_ = new Singleton(value);
    }
    return *pinstance_;
}

void ThreadFoo(){
    // EN: Following code emulates slow initialization.
    //
    // RU: Этот код эмулирует медленную инициализацию.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton& singleton = Singleton::GetInstance("FOO");
    std::cout << singleton.value() << "\n";
}

void ThreadBar(){
    // EN: Following code emulates slow initialization.
    //
    // RU: Этот код эмулирует медленную инициализацию.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton& singleton = Singleton::GetInstance("BAR");
    std::cout << singleton.value() << "\n";
}

int main()
{   
    std::cout <<"If you see the same value, then singleton was reused (yay!\n" <<
                "If you see different values, then 2 singletons were created (booo!!)\n\n" <<
                "RESULT:\n";   
    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);
    t1.join();
    t2.join();
    
    return 0;
}
