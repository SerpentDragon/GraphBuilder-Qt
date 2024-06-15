## GraphBuilder

Данная программа предназначена для построения графиков математических функций. Поддерживаются функции вида y = f(x).

## Описание
Для работы с приложением необходимо ввести Вашу функцию, используя соответствующие кнопки. После чего, нажать кнопку '='.
График будет нарисован на плоттере, а функция появится в общем списке.

Вы можете управлять отображением функций. Для того, чтобы показать/спрятать график, необходимо установить/снять флажок для соответствующей функции из списка. Также, Вы можете удалить функцию из списка, нажав на кнопку с красным крестиком.

<b>Поддерживаемые функции:</b>
- sin(x)
- cos(x)
- tg(x)
- ctg(x)
- sinh(x)
- cosh(x)
- tgh(x)
- ctg(x)
- asin(x)
- acos(x)
- atg(x)
- actg(x)
- ln(x)
- lg(x)/log(x)
- abs(x)
- x^y
- sqrt(x)
- e
- pi

## Зависимости

Вам потребуются установленные фреймворк [Qt](https://www.qt.io/) и библиотека [GSL](https://www.gnu.org/software/gsl/). Также необходимо иметь компилятор, поддерживающий C++23

## Сборка и запуск

Вы можете собрать и запустить приложение с помощью CMakeLists.txt из данного репозитория
```
git clone git@github.com:SerpentDragon/GraphBuilder-Qt.git
cd GraphBuilder-Qt/
mkdir build
cd build
cmake ..
cmake --build .
./GraphBuilder
```

Также, возможно, Вам потребуется установить зависимости   
<b>For Linux (Ubuntu):</b>  
1. [Qt](https://www.qt.io/)
2. [GSL](https://www.gnu.org/software/gsl/) или `sudo apt install gsl-bin libgsl-dev`

## Демо
![Alt text](https://github.com/SerpentDragon/GraphBuilder-Qt/blob/master/Demo/demo.png)
