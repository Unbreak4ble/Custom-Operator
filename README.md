# Custom-Operator
Sem nada para fazer, resolvi desenvolver um sistema de operadores customizados 

# uso
```cpp
double result = Custor::input(<string expression>); //apenas para calcular a expressão
double result = Custor::input(<string expression>, <vector operators>); //calcular expressão com operadores customizados
```

# exemplo de operadores customizados
```cpp
#include <iostream>
#include <vector>
#include "customOperator.h"

double calc (double right, double left){
	return right+left;
}

int main() {
  
	std::vector<Custor::operators> ops{{'~', 0, calc}}; //nescessário o operador, a posição, e a função que irá lidar com o cálculo
  
	try{

		std::string str = "2.5~2";
		double out = Custor::input(str, ops);
    
		std::cout << out << std::endl;
    
	}
        catch(std::string err)
        {
		std::cout << err << std::endl;
	}
	
	return 0;
}
```
