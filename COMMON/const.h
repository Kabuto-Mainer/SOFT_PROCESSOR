#ifndef CONST_H
#define CONST_H

#define OFF 0
#define ON 1

#define PROCESSOR 0
#define DEBUG 1


#define MOD_WORK PROCESSOR

// Канареечная зашита
#define BIRD_SECURE OFF

// Хеш зашиты
#define HASH_SECURE OFF

// Автоматическое увеличение стека
#define AUTO_REALLOC OFF

// Заполнение стека
//* Необходимо для работы Канареечной и Хеш зашиты
#define COMPLETION_DATA OFF


// Тип данных стека. В режиме процессора должен быть <int>
typedef int stmn_t;
const char name_type[] = "int";
#define PRINT_ELEMENT(num) { \
   printf("%d", num); \
}

// Множитель realloc
const int MOD_REALLOC = 2;

// Множитель для увеличения точности
// Работает не самым лучшим образом
const int MODE_DECISION = 1;

// Начальное количество команд
const size_t START_AMOUNT_CMD = 10;

// Минимальное и максимальное значение аргументов
const stmn_t MAX_MEAN = 1e4;
const stmn_t MIN_MEAN = -1e4;

// Сид для хеш-функции
const size_t START_HASH_CODE = 5318;

// Ядовитое значение для стека
const stmn_t POISON_NUM = -3333;

// Минимальные рабочий адрес
const int MIN_ADDRESS = 8000;

// Количество элементов, которое выведется, если size > этого значения
const int AMOUNT_PRINT_ELEMENT = 10;

// Куропатка или Канарейка
const stmn_t BIRD_NUM = 7777;















#endif // CONST_H
