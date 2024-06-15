## GraphBuilder

This programm is a simple plotter for math functions. Functions of the form 
y = f(x) are maintained  

## Description
You should enter your function using corresponding buttons. Then, press '=' button. 
The graph will be displayed on the plotter and the function will appear in the list.

You can manipulate functions displaying. To show/hide the graph you should 
check/uncheck the combobox of the corresponding function in the list. You can also 
delete the function by clicking the button with red cross.  

<b>Maintained functions:</b>
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

## Dependencies

You need [Qt](https://www.qt.io/) and [GSL](https://www.gnu.org/software/gsl/) libraries to be installed. Also you must have C++23

## Build & run

You can compile and run the program using CMakeLists.txt from this reposiroty:
```
git clone git@github.com:SerpentDragon/GraphBuilder-Qt.git
cd GraphBuilder-Qt/
mkdir build
cd build
cmake ..
cmake --build .
./GraphBuilder
```

Also you may need to install dependencies:  
<b>For Linux (Ubuntu):</b>  
1. [Qt](https://www.qt.io/)
2. [GSL](https://www.gnu.org/software/gsl/) or `sudo apt install gsl-bin libgsl-dev`

## Demo
![Alt text](https://github.com/SerpentDragon/GraphBuilder-Qt/blob/master/Demo/demo.png)
