# Домашнее задание №2

## Содержание
- [Домашнее задание №2](#домашнее-задание-2)
  - [Содержание](#содержание)
  - [Задание](#задание)
  - [Требования к корректности решения](#требования-к-корректности-решения)
    - [Базовые требования](#базовые-требования)
    - [Ограничения на ввод и вывод](#ограничения-на-ввод-и-вывод)
    - [Требования к реализации](#требования-к-реализации)
    - [Консольное приложение](#консольное-приложение)
    - [Разделение обязанностей](#разделение-обязанностей)
    - [Структура репозитория](#структура-репозитория)
  - [Сроки сдачи](#сроки-сдачи)
  - [Система оценки](#система-оценки)


## Задание

Напишите программу `hw-02_huffman`, выполняющую сжатие и разжатие двухпроходным алгоритмом Хаффмана.
Также реализуйте тесты написанного алгоритма.

## Требования к корректности решения

### Базовые требования
В этом задании действуют стандартные и дополнительные требования:

* Программа обязана проверить корректность аргументов (смотри
  [консольное приложение](#консольное-приложение)).
  * Достаточно проверить относительно тривиальные ошибки, например, нехватку аргументов.
* При проблемах с аргументами, открытием файла, выделением памяти и прочим, программа должна
  корректно завершить работу и вернуть ненулевой код возврата.

### Ограничения на ввод и вывод

Хотя программа не должна опираться на это, при проверке входные файлы ограничены в 5MiB. Многие
описывают алогритм Хаффмана для сжатия текста, но входной файл может содержать любые байты. У
программы есть ограничение времени работы в 5 секунд, но на деле она должна работать заметно
быстрее.

### Требования к реализации

* Реализация должна быть выполнена в объектно-ориентированном стиле (например, включать классы 
  `HuffmanTreeNode`, `HuffmanTree`, `HuffmanArchiver`, но данные имена лишь пример).
* Для хранения элементов, там, где это уместно, должна быть использована библиотека STL (например,
  `std::vector`, а не динамический массив).
* Ввод-вывод должен быть выполнен с помощью классов STL (`iostream`).
* Должны использоваться исключения.
* Должен быть реализован свой класс для автоматического тестирования или использована одна из
  библиотек (`google test`, `doctest`, …).
* Для методов должны быть написаны автотесты.


*Подсказки*:
* Рекомендуем удостовериться, что сжатый, а затем разжатый Вами файл абсолютно эквивалентен
  исходному, не проверяйте на глаз (`diff` и аналоги).
* Рекомендуем протестировать на больших файлах, файлах разного типа (текстовые, бинарные) и на
  пустом файле.
* Рекомендуем проверить свою программу на синтетических тестах, заведомо плохо кодируемых алгоритмом
  Хаффмана.
* Рекомендуем проверить свою программу на утечки памяти.

### Консольное приложение

Программа запускается с флагами:
```
./hw-02_huffman [flags]
```
где флаги:
* `-c`: сжатие,
* `-u`: разжатие,
* `-f <path>`, `--file <path>`: имя входного файла,
* `-o <path>`, `--output <путь>`: имя результирующего файла.
Флаги могут указываться в любом порядке.

Программа должна выводить на экран статистику сжатия/распаковки: размер исходных данных, размер
полученных данных и размер, который был использован для хранения вспомогательных данных в выходном
файле (например, таблицы). Не должно выводиться никакого дополнительного текста, только размеры.
Все размеры в байтах.

Например:
```
$ ./huffman -c -f myfile.txt -o result.bin
15678
6172
482
```

Размер исходного файла (исходные данные): 15678 байт, размер сжатых данных (без дополнительной
информации): 6172 байта, размер дополнительных данных: 482 байта. Размер всего сжатого файла: 6172 +
482 = 6654 байта.

```
$ ./huffman -u -f result.bin -o myfile_new.txt
6172
15678
482
```

Размер распакованного файла (полученные данные): 15678 байт, размер сжатых данных (без 
дополнительной информации): 6172 байта, размер дополнительных данных: 482 байта. Размер всего
исходного сжатого файла: 6172 + 482 = 6654 байта.

### Разделение обязанностей
* Работу с аргументами и точку входа реализовать в `main.c`.
* Классы реализации алгоритма поместить в `src/` и `include/`.
* Файлы с тестами поместить в `test/`. Точкой входа должен быть `test.cpp`.

### Структура репозитория
Если вам нужны дополнительные файлы (например, тестовые файлы), кладите их в папку `samples`.

```
<корень-личного-репозитория>
|--hw-02_huffman
   |--include
   |  |-- ...
   |--samples
   |  |-- ...
   |--src
   |  |-- main.cpp
   |  |-- ...
   |--test
   |  |-- test.cpp
   |  |-- huffman_test.cpp (?)
   |  |-- huffman_test.hpp (?)
   |--Makefile / CMakeLists.txt
```

Папку `obj`, объектные и исполняемые файлы класть в репозиторий не разрешается.

## Сроки сдачи
На задание даётся **ровно три** попытки сдачи. Сроки попыток уточняйте у преподавателя.

Учтите, что в отличие от лабораторных:

* Это домашнее задание больше по сложности и объёму кода. Пожалуйста, не откладывайте даже до
  промежуточного срока сдачи.
* Вы можете сделать **не больше трёх** полноценных попыток за всё время сдачи. На усмотрение
  преподавателя могут добавляться промежуточные попытки, которые будут проверяться частично.
* Из-за объёма кода решения могут проверяться с существенной задержкой, особенно при наличии
  стилистических замечаний.

## Система оценки
* Задание оценивается в 30 баллов: 15 за корректность, 5 за тесты, 10 за стиль.
