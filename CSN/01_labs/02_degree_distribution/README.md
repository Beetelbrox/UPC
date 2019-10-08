# Analysis of Degree Distribution

## Trampas

Las distintas funciones de log-likelihood dependen para su ejecución, en mayor o menor medida, de tres parámetros:
* **M**
* **M_prime**
* **N**

Esto es un problema a la hora de definir estas funciones, ya que necesitan que estas variables existan a priori para que el compiler permita definirlas.

La primera opción que intenté, era parametrizar estas funciones de likelihood con los valores _N_, _M_ y _M_prime_ según las usasen o no. Esta solución me llevó a infinitos errores con los solvers así que la descarté.

La segunda solución (actual), pasa por hacer el código un poco más sucio, aunque deberíamos ser capaces de encontrar otra solución para que el código funcione y quede más limpio. La solución actual es inicializar a un valor arbitrario (0 en este caso) estos tres parámetros antes de definir las funciones. Esto no afecta para nada al resultado final, ya que estos valores se van a actualizar antes de que las funciones se ejecuten en cada iteración (un lenguaje por cada vez).

Y ya está.


## Cambios en el código

No hay muchos salvo la posibilidad de generar esa tabla y he eliminado los *code chunks* que había por el medio que parecían más de ejemplo que otra cosa. Si son necesarios y no me he dado cuenta de su importancia, ruego disculpes mi falta de conocimiento, y de todas formas se pueden recuperar de commits anteriores. Ejecutando el código al completo no debería dar ningún error y los valores para la tabla se guardan en un dataframe que se llama **parameters**.