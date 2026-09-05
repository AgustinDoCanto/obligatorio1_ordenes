#include <sstream>
#include "catch_amalgamated.hpp"
#include "ejercicios/listas.hpp"
#include "check_resultado.hpp"
#include "func_aux.hpp"
#include "mem_tracking_fixture.hpp"

void checkListasIguales(NodoLista *resultado, NodoLista *esperado, const char *esperadoTexto)
{
    checkResultadoSerializado(FrameworkA1::sonIgualesDatosForma(resultado, esperado),
                              resultado, esperadoTexto,
                              [](NodoLista *lista)
                              { return FrameworkA1::serializar(lista); });
}

template <typename Funcion>
void checkListaNueva(Funcion funcion, const char *inputLista, const char *expected)
{
    int largo;
    NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);
    NodoLista *copiaLista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);

    int largoSolucion;
    NodoLista *solucion = (NodoLista *)FrameworkA1::parsearColeccion(expected, largoSolucion);

    NodoLista *resultado = funcion(lista);

    bool parametrosNoModificados = FrameworkA1::sonIgualesDatosForma(lista, copiaLista);
    bool noComparteMemoria = !FrameworkA1::compartenMemoria(resultado, lista);

    checkListasIguales(resultado, solucion, expected);

    FrameworkA1::destruir(lista);
    FrameworkA1::destruir(copiaLista);
    FrameworkA1::destruir(solucion);
    FrameworkA1::destruir(resultado);

    if (!parametrosNoModificados)
        FAIL_CHECK("La función modifica los parámetros de entrada");
    if (!noComparteMemoria)
        FAIL_CHECK("El resultado comparte memoria con la lista de entrada");
}

template <typename Funcion>
void checkListaModificada(Funcion funcion, const char *inputLista, const char *expected)
{
    int largo;
    NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);

    int largoSolucion;
    NodoLista *solucion = (NodoLista *)FrameworkA1::parsearColeccion(expected, largoSolucion);

    funcion(lista);

    checkListasIguales(lista, solucion, expected);

    FrameworkA1::destruir(lista);
    FrameworkA1::destruir(solucion);
}

template <typename Funcion>
void checkDosListasNuevas(Funcion funcion, const char *inputLista1, const char *inputLista2, const char *expected)
{
    int largo1, largo2, largoSolucion;
    NodoLista *lista1 = (NodoLista *)FrameworkA1::parsearColeccion(inputLista1, largo1);
    NodoLista *lista2 = (NodoLista *)FrameworkA1::parsearColeccion(inputLista2, largo2);
    NodoLista *copiaLista1 = (NodoLista *)FrameworkA1::parsearColeccion(inputLista1, largo1);
    NodoLista *copiaLista2 = (NodoLista *)FrameworkA1::parsearColeccion(inputLista2, largo2);
    NodoLista *solucion = (NodoLista *)FrameworkA1::parsearColeccion(expected, largoSolucion);

    NodoLista *resultado = funcion(lista1, lista2);

    bool parametrosNoModificados = FrameworkA1::sonIgualesDatosForma(lista1, copiaLista1) && FrameworkA1::sonIgualesDatosForma(lista2, copiaLista2);
    bool noComparteMemoria = !FrameworkA1::compartenMemoria(resultado, lista1) && !FrameworkA1::compartenMemoria(resultado, lista2);

    checkListasIguales(resultado, solucion, expected);

    FrameworkA1::destruir(lista1);
    FrameworkA1::destruir(lista2);
    FrameworkA1::destruir(copiaLista1);
    FrameworkA1::destruir(copiaLista2);
    FrameworkA1::destruir(solucion);
    FrameworkA1::destruir(resultado);

    if (!parametrosNoModificados)
        FAIL_CHECK("La función modifica los parámetros de entrada");
    if (!noComparteMemoria)
        FAIL_CHECK("El resultado comparte memoria con las listas de entrada");
}

template <typename Funcion>
void checkMemoriaDosListas(Funcion funcion, const char *inputLista1, const char *inputLista2)
{
    int largo1, largo2;
    NodoLista *lista1 = (NodoLista *)FrameworkA1::parsearColeccion(inputLista1, largo1);
    NodoLista *lista2 = (NodoLista *)FrameworkA1::parsearColeccion(inputLista2, largo2);

    checkMemoriaEjecucion([&]
                          {
        NodoLista *resultado = funcion(lista1, lista2);
        FrameworkA1::destruir(resultado); 
        FrameworkA1::destruir(lista1);
        FrameworkA1::destruir(lista2); });
}

template <typename Funcion>
void checkPredicadoLista(Funcion funcion, const char *inputLista, bool expected)
{
    int largo;
    NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);
    NodoLista *copiaLista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);

    bool resultado = funcion(lista);
    bool parametrosNoModificados = FrameworkA1::sonIgualesDatosForma(lista, copiaLista);
    CHECK(resultado == expected);
    CHECK(parametrosNoModificados);

    FrameworkA1::destruir(lista);
    FrameworkA1::destruir(copiaLista);
}

template <typename Funcion>
void checkListaConSecuenciaModificada(Funcion funcion, const char *inputLista, const char *inputSecuencia, const char *expected)
{
    int largo, largoSolucion;
    NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);
    NodoLista *secuencia = (NodoLista *)FrameworkA1::parsearColeccion(inputSecuencia, largo);
    NodoLista *copiaSecuencia = (NodoLista *)FrameworkA1::parsearColeccion(inputSecuencia, largo);
    NodoLista *solucion = (NodoLista *)FrameworkA1::parsearColeccion(expected, largoSolucion);

    funcion(lista, secuencia);

    bool secuenciaNoModificada = FrameworkA1::sonIgualesDatosForma(secuencia, copiaSecuencia);
    checkListasIguales(lista, solucion, expected);

    FrameworkA1::destruir(lista);
    FrameworkA1::destruir(secuencia);
    FrameworkA1::destruir(copiaSecuencia);
    FrameworkA1::destruir(solucion);

    if (!secuenciaNoModificada)
        FAIL_CHECK("La función modifica la secuencia a eliminar");
}

template <typename Funcion>
void checkMemoriaListaNueva(Funcion funcion, const char *inputLista)
{
    int largo;
    NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);

    checkMemoriaEjecucion([&]
                          { NodoLista *resultado = funcion(lista);
                            FrameworkA1::destruir(resultado);
                            FrameworkA1::destruir(lista); });
}

template <typename Funcion>
void checkMemoriaLista(Funcion funcion, const char *inputLista)
{
    int largo;
    NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largo);

    checkMemoriaEjecucion([&]
                          { funcion(lista); 
                            FrameworkA1::destruir(lista); });
}

template <typename Funcion>
void checkMemoriaListaConSecuencia(Funcion funcion, const char *inputLista, const char *inputSecuencia)
{
    int largoLista, largoSecuencia;
    NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputLista, largoLista);
    NodoLista *secuencia = (NodoLista *)FrameworkA1::parsearColeccion(inputSecuencia, largoSecuencia);

    checkMemoriaEjecucion([&]
                          { funcion(lista, secuencia); 
                            FrameworkA1::destruir(lista);
                            FrameworkA1::destruir(secuencia); });
}

template <typename Funcion>
void casosMemoriaIntercalar(Funcion funcion)
{
    SECTION("both empty") { checkMemoriaDosListas(funcion, "()", "()"); }
    SECTION("one empty") { checkMemoriaDosListas(funcion, "(1,3,5)", "()"); }
    SECTION("interleaved") { checkMemoriaDosListas(funcion, "(1,3,5,7,9)", "(2,4,6,8)"); }
}

template <typename Funcion>
void casosIntercalar(Funcion funcion)
{
    auto check = [funcion](const char *inputLista1, const char *inputLista2, const char *expected)
    { checkDosListasNuevas(funcion, inputLista1, inputLista2, expected); };

    SECTION("() and ()") { check("()", "()", "()"); }
    SECTION("(1) and ()") { check("(1)", "()", "(1)"); }
    SECTION("(1) and (2)") { check("(1)", "(2)", "(1,2)"); }
    SECTION("(1,3,5,7,9) and (2,4,6,8)") { check("(1,3,5,7,9)", "(2,4,6,8)", "(1,2,3,4,5,6,7,8,9)"); }
    SECTION("(1,3,5,7,9,9) and (2,2,4,6,8)") { check("(1,3,5,7,9,9)", "(2,2,4,6,8)", "(1,2,2,3,4,5,6,7,8,9,9)"); }
    SECTION("(100,200,300,400) and same") { check("(100,200,300,400)", "(100,200,300,400)", "(100,100,200,200,300,300,400,400)"); }
    SECTION("() and (-1,0,1,5)") { check("()", "(-1,0,1,5)", "(-1,0,1,5)"); }
    SECTION("(19,24,233,2344) and (-1222,-129,-12,0,0,0,0,0,0,1,2,3,4,12)") { check("(19,24,233,2344)", "(-1222,-129,-12,0,0,0,0,0,0,1,2,3,4,12)", "(-1222,-129,-12,0,0,0,0,0,0,1,2,3,4,12,19,24,233,2344)"); }
    SECTION("(-1,-1,-1) and (-1,-1,-1,-1)") { check("(-1,-1,-1)", "(-1,-1,-1,-1)", "(-1,-1,-1,-1,-1,-1,-1)"); }
    SECTION("(-4,-1,0,5,7,10,12) and (0,1,1)") { check("(-4,-1,0,5,7,10,12)", "(0,1,1)", "(-4,-1,0,0,1,1,5,7,10,12)"); }
    SECTION("(2,2) and (2,2)") { check("(2,2)", "(2,2)", "(2,2,2,2)"); }
}

template <typename Check>
void casosOrdenamiento(Check check)
{
    SECTION("()") { check("()", "()"); }
    SECTION("(4)") { check("(4)", "(4)"); }
    SECTION("(1,1,1)") { check("(1,1,1)", "(1,1,1)"); }
    SECTION("(1,2,3)") { check("(1,2,3)", "(1,2,3)"); }
    SECTION("(1,4,2)") { check("(1,4,2)", "(1,2,4)"); }
    SECTION("(2,3,1)") { check("(2,3,1)", "(1,2,3)"); }
    SECTION("(-2,-3,-1)") { check("(-2,-3,-1)", "(-3,-2,-1)"); }
    SECTION("(1,1,4,1,3,8)") { check("(1,1,4,1,3,8)", "(1,1,1,3,4,8)"); }
    SECTION("(-2,3,1)") { check("(-2,3,1)", "(-2,1,3)"); }
    SECTION("(3,5,2,1,0)") { check("(3,5,2,1,0)", "(0,1,2,3,5)"); }
    SECTION("(9,2,2,5,1)") { check("(9,2,2,5,1)", "(1,2,2,5,9)"); }
    SECTION("(3,1,-1,1,0)") { check("(3,1,-1,1,0)", "(-1,0,1,1,3)"); }
    SECTION("(10..-2)") { check("(10,9,8,7,6,5,4,3,2,1,0,-1,-2)", "(-2,-1,0,1,2,3,4,5,6,7,8,9,10)"); }
    SECTION("(7,3,7,10,-1,1,-6,0,-10,2,1,2)") { check("(7,3,7,10,-1,1,-6,0,-10,2,1,2)", "(-10,-6,-1,0,1,1,2,2,3,7,7,10)"); }
    SECTION("(10..-2)-duplicate") { check("(10,9,8,7,6,5,4,3,2,1,0,-1,-2)", "(-2,-1,0,1,2,3,4,5,6,7,8,9,10)"); }
    SECTION("(8,7,5,2,-3,-1)") { check("(8,7,5,2,-3,-1)", "(-3,-1,2,5,7,8)"); }
    SECTION("(1,2,0,10,3,4)") { check("(1,2,0,10,3,4)", "(0,1,2,3,4,10)"); }
    SECTION("(1,2,4,3)") { check("(1,2,4,3)", "(1,2,3,4)"); }
    SECTION("(-2,0,3,1,1)") { check("(-2,0,3,1,1)", "(-2,0,1,1,3)"); }
}


TEST_CASE("PruebaInvertirParcial cases", "[PruebaInvertirParcial][file:listas]")
{
    auto check = [](const char *input, const char *expected)
    {
        ostringstream oss;
        oss << input << " -> " << expected;
        SECTION(oss.str())
        {
            checkListaNueva(invertirParcial, input, expected);
        }
    };

    check("(1)", "()");
    check("(1,2,3,4)", "(3,2,1)");
    check("(-1,-2)", "(-1)");
    check("(1,2,3,4,3,2,1,0)", "(1,2,3,4,3,2,1)");
    check("(1,2,3,4,3,2,1)", "(2,3,4,3,2,1)");
    check("(1,1,1)", "(1,1)");
    check("(0,1,2,3,4,5,6,7,8,9,10)", "(9,8,7,6,5,4,3,2,1,0)");
    check("(0,1,0,5,0,1,0)", "(1,0,5,0,1,0)");
    check("(8,9,5,1)", "(5,9,8)");
}

TEST_CASE("PruebaInvertirParcial memory cases", "[PruebaInvertirParcial][memory][file:listas]")
{
    SECTION("single") { checkMemoriaListaNueva(invertirParcial, "(1)"); }
    SECTION("ascending") { checkMemoriaListaNueva(invertirParcial, "(1,2,3,4)"); }
    SECTION("repeated") { checkMemoriaListaNueva(invertirParcial, "(0,1,0,5,0,1,0)"); }
}

TEST_CASE("PruebaEliminarNesimoDesdeElFinal cases", "[PruebaEliminarNesimoDesdeElFinal][file:listas]")
{
    auto check = [](const char *inputLista, int n, const char *expected)
    {
        auto f = [n](NodoLista *&input) mutable
        { eliminarNesimoDesdeElFinal(input, n); };
        checkListaModificada(f, inputLista, expected);
    };

    SECTION("() n=50") { check("()", 50, "()"); }
    SECTION("(1) n=50") { check("(1)", 50, "(1)"); }
    SECTION("(1) n=1") { check("(1)", 1, "()"); }
    SECTION("(1,2,3,4,5) n=2") { check("(1,2,3,4,5)", 2, "(1,2,3,5)"); }
    SECTION("(1..10) n=10") { check("(1,2,3,4,5,6,7,8,9,10)", 10, "(2,3,4,5,6,7,8,9,10)"); }
    SECTION("(1..10) n=-5") { check("(1,2,3,4,5,6,7,8,9,10)", -5, "(1,2,3,4,5,6,7,8,9,10)"); }
    SECTION("(1..10) n=0") { check("(1,2,3,4,5,6,7,8,9,10)", 0, "(1,2,3,4,5,6,7,8,9,10)"); }
    SECTION("() n=1") { check("()", 1, "()"); }
    SECTION("(1..10) n=1") { check("(1,2,3,4,5,6,7,8,9,10)", 1, "(1,2,3,4,5,6,7,8,9)"); }
    SECTION("(111,2,3,4,55,6,7,88,99,10) n=2") { check("(111,2,3,4,55,6,7,88,99,10)", 2, "(111,2,3,4,55,6,7,88,10)"); }
    SECTION("(5, 6, 7, 8, 9) n=1") { check("(5, 6, 7, 8, 9)", 1, "(5, 6, 7, 8)"); }
}

TEST_CASE("PruebaEliminarNesimoDesdeElFinal memory cases", "[PruebaEliminarNesimoDesdeElFinal][memory][file:listas]")
{
    auto check = [](const char *input, int n)
    { checkMemoriaLista([n](NodoLista *&lista) mutable
                        { eliminarNesimoDesdeElFinal(lista, n); }, input); };

    SECTION("empty") { check("()", 1); }
    SECTION("remove head") { check("(1,2,3,4)", 4); }
    SECTION("remove tail") { check("(1,2,3,4)", 1); }
}

TEST_CASE("PruebaListaOrdenadaInsertionSort cases", "[PruebaListaOrdenadaInsertionSort][file:listas]")
{
    casosOrdenamiento([](const char *input, const char *expected)
                      { checkListaNueva(listaOrdenadaInsertionSort, input, expected); });
}

TEST_CASE("PruebaListaOrdenadaInsertionSort memory cases", "[PruebaListaOrdenadaInsertionSort][memory][file:listas]")
{
    SECTION("empty") { checkMemoriaListaNueva(listaOrdenadaInsertionSort, "()"); }
    SECTION("unsorted") { checkMemoriaListaNueva(listaOrdenadaInsertionSort, "(3,1,2)"); }
    SECTION("duplicates") { checkMemoriaListaNueva(listaOrdenadaInsertionSort, "(9,2,2,5,1)"); }
}

TEST_CASE("PruebaListaOrdenadaSelectionSort cases", "[PruebaListaOrdenadaSelectionSort][file:listas]")
{
    casosOrdenamiento([](const char *input, const char *expected)
                      { checkListaModificada(listaOrdenadaSelectionSort, input, expected); });
}

TEST_CASE("PruebaListaOrdenadaSelectionSort memory cases", "[PruebaListaOrdenadaSelectionSort][memory][file:listas]")
{
    SECTION("empty") { checkMemoriaLista(listaOrdenadaSelectionSort, "()"); }
    SECTION("unsorted") { checkMemoriaLista(listaOrdenadaSelectionSort, "(3,1,2)"); }
    SECTION("duplicates") { checkMemoriaLista(listaOrdenadaSelectionSort, "(9,2,2,5,1)"); }
}

TEST_CASE("PruebaIntercalarIter cases", "[PruebaIntercalarIter][file:listas]")
{
    casosIntercalar(intercalarIter);
}

TEST_CASE("PruebaIntercalarIter memory cases", "[PruebaIntercalarIter][memory][file:listas]")
{
    casosMemoriaIntercalar(intercalarIter);
}

TEST_CASE("PruebaIntercalarRec cases", "[PruebaIntercalarRec][file:listas]")
{
    casosIntercalar(intercalarRec);
}

TEST_CASE("PruebaIntercalarRec memory cases", "[PruebaIntercalarRec][memory][file:listas]")
{
    casosMemoriaIntercalar(intercalarRec);
}

TEST_CASE("PruebaInsComFin cases", "[PruebaInsComFin][file:listas]")
{
    auto check = [](const char *inputLista, int n, const char *expected)
    { checkListaNueva([n](NodoLista *lista)
                      { return insComFin(lista, n); }, inputLista, expected); };

    SECTION("(1,2,1) x=0") { check("(1,2,1)", 0, "(0,1,2,1,0)"); }
    SECTION("(1,2,3,4) x=5") { check("(1,2,3,4)", 5, "(5,4,3,2,1,5)"); }
    SECTION("(5) x=4") { check("(5)", 4, "(4,5,4)"); }
    SECTION("() x=4") { check("()", 4, "(4,4)"); }
    SECTION("(5,4,3,2,1,2,3,4,5) x=6") { check("(5,4,3,2,1,2,3,4,5)", 6, "(6,5,4,3,2,1,2,3,4,5,6)"); }
    SECTION("(1,1,1,1,1) x=1") { check("(1,1,1,1,1)", 1, "(1,1,1,1,1,1,1)"); }
    SECTION("(3,2,1) x=0") { check("(3,2,1)", 0, "(0,1,2,3,0)"); }
    SECTION("(9,5,6,1,2,4,8,9,5,55,2) x=33") { check("(9,5,6,1,2,4,8,9,5,55,2)", 33, "(33,2,55,5,9,8,4,2,1,6,5,9,33)"); }
    SECTION("(1,2,3,4,5) x=3") { check("(1,2,3,4,5)", 3, "(3,5,4,3,2,1,3)"); }
    SECTION("(9,5,1,7,5,3) x=1000") { check("(9,5,1,7,5,3)", 1000, "(1000,3,5,7,1,5,9,1000)"); }
}

TEST_CASE("PruebaInsComFin memory cases", "[PruebaInsComFin][memory][file:listas]")
{
    auto check = [](const char *input, int n)
    { checkMemoriaListaNueva([n](NodoLista *lista)
                             { return insComFin(lista, n); }, input); };

    SECTION("empty") { check("()", 4); }
    SECTION("single") { check("(5)", 4); }
    SECTION("several") { check("(1,2,3,4)", 5); }
}

TEST_CASE("PruebaEXOR cases", "[PruebaEXOR][file:listas]")
{
    auto check = [](const char *inputLista1, const char *inputLista2, const char *expected)
    { checkDosListasNuevas(exor, inputLista1, inputLista2, expected); };

    SECTION("(100,200,300,400) xor same") { check("(100,200,300,400)", "(100,200,300,400)", "()"); }
    SECTION("(1,2,3,4) xor (2,3,5,7)") { check("(1,2,3,4)", "(2,3,5,7)", "(1,4,5,7)"); }
    SECTION("() xor ()") { check("()", "()", "()"); }
    SECTION("() xor (-1,0,1,5)") { check("()", "(-1,0,1,5)", "(-1,0,1,5)"); }
    SECTION("(19,24,233,2344) xor (-1222,-129,-12,0,0,0,0,0,0,1,2,3,4,12)") { check("(19,24,233,2344)", "(-1222,-129,-12,0,0,0,0,0,0,1,2,3,4,12)", "(-1222,-129,-12,0,1,2,3,4,12,19,24,233,2344)"); }
    SECTION("(-9,-5,-1) xor (1,6,8,9)") { check("(-9,-5,-1)", "(1,6,8,9)", "(-9,-5,-1,1,6,8,9)"); }
    SECTION("(0,2,3) xor (0,1,2,5,7,9,11)") { check("(0,2,3)", "(0,1,2,5,7,9,11)", "(1,3,5,7,9,11)"); }
    SECTION("(1,2,2,2,3,4) xor (1,3,5,7)") { check("(1,2,2,2,3,4)", "(1,3,5,7)", "(2,4,5,7)"); }
    SECTION("(100,200,300,400) xor (100,200,200,300,400)") { check("(100,200,300,400)", "(100,200,200,300,400)", "()"); }
    SECTION("(1,1,2,5) xor (2,3,3,5)") { check("(1,1,2,5)", "(2,3,3,5)", "(1,3)"); }
    SECTION("(-1,1,2,5,5) xor (2,3,3,5,5,5,7,7)") { check("(-1,1,2,5,5)", "(2,3,3,5,5,5,7,7)", "(-1,1,3,7)"); }
    SECTION("(-1,1,2,5,5) xor (2,3,3,5,5,5)") { check("(-1,1,2,5,5)", "(2,3,3,5,5,5)", "(-1,1,3)"); }
    SECTION("(-1,1,2,5,5) xor (2,3,3,5)") { check("(-1,1,2,5,5)", "(2,3,3,5)", "(-1,1,3)"); }
}

TEST_CASE("PruebaEXOR memory cases", "[PruebaEXOR][memory][file:listas]")
{
    SECTION("both empty") { checkMemoriaDosListas(exor, "()", "()"); }
    SECTION("equal") { checkMemoriaDosListas(exor, "(1,2,3)", "(1,2,3)"); }
    SECTION("overlap") { checkMemoriaDosListas(exor, "(1,2,3,4)", "(2,3,5,7)"); }
}

TEST_CASE("PruebaEliminarDuplicadosListaOrdenadaDos cases", "[PruebaEliminarDuplicadosListaOrdenadaDos][file:listas]")
{
    auto check = [](const char *inputLista, const char *expected)
    { checkListaModificada(eliminarDuplicadosListaOrdenadaDos, inputLista, expected); };

    SECTION("(100,100,200,200,300,300,400,400)") { check("(100,100,200,200,300,300,400,400)", "()"); }
    SECTION("()") { check("()", "()"); }
    SECTION("(-1,0,1,1,1,1,5,5,5,5,5)") { check("(-1,0,1,1,1,1,5,5,5,5,5)", "(-1,0)"); }
    SECTION("(-5,-5,-4,0,2,3,5,6,7,9,10,15,50,65,78,150)") { check("(-5,-5,-4,0,2,3,5,6,7,9,10,15,50,65,78,150)", "(-4,0,2,3,5,6,7,9,10,15,50,65,78,150)"); }
    SECTION("(0,1,2,2,3,3,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,10)") { check("(0,1,2,2,3,3,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,10)", "(0,1,9,10)"); }
    SECTION("(0,0,0,1,1,1)") { check("(0,0,0,1,1,1)", "()"); }
    SECTION("(1,1,2,2,2,3,3,3,3,5)") { check("(1,1,2,2,2,3,3,3,3,5)", "(5)"); }
    SECTION("(5)") { check("(5)", "(5)"); }
    SECTION("(-1,0,1)") { check("(-1,0,1)", "(-1,0,1)"); }
    SECTION("(0,1,1,2,2,2,3,3,3,3,4,4,4,4,4)") { check("(0,1,1,2,2,2,3,3,3,3,4,4,4,4,4)", "(0)"); }
}

TEST_CASE("PruebaEliminarDuplicadosListaOrdenadaDos memory cases", "[PruebaEliminarDuplicadosListaOrdenadaDos][memory][file:listas]")
{
    SECTION("empty") { checkMemoriaLista(eliminarDuplicadosListaOrdenadaDos, "()"); }
    SECTION("all duplicated") { checkMemoriaLista(eliminarDuplicadosListaOrdenadaDos, "(1,1,2,2)"); }
    SECTION("mixed") { checkMemoriaLista(eliminarDuplicadosListaOrdenadaDos, "(-1,0,1,1,1,5,5)"); }
}

TEST_CASE("PruebaPalindromo cases", "[PruebaPalindromo][file:listas]")
{
    auto check = [](const char *inputLista, bool expected)
    { checkPredicadoLista(palindromo, inputLista, expected); };

    SECTION("(1,0,1)") { check("(1,0,1)", true); }
    SECTION("()") { check("()", true); }
    SECTION("(1)") { check("(1)", true); }
    SECTION("(1,1)") { check("(1,1)", true); }
    SECTION("(1,2)") { check("(1,2)", false); }
    SECTION("(1,2,3)") { check("(1,2,3)", false); }
    SECTION("(1,2,3,4,3,3,1)") { check("(1,2,3,4,3,3,1)", false); }
    SECTION("(1,2,3,4,3,2,1)") { check("(1,2,3,4,3,2,1)", true); }
    SECTION("(1,2,3,4,3,2)") { check("(1,2,3,4,3,2)", false); }
    SECTION("(1,2,3,3,2,1)") { check("(1,2,3,3,2,1)", true); }
    SECTION("(1,1,1,1,1,1,1)") { check("(1,1,1,1,1,1,1)", true); }
}

TEST_CASE("PruebaPalindromo memory cases", "[PruebaPalindromo][memory][file:listas]")
{
    SECTION("empty") { checkMemoriaLista(palindromo, "()"); }
    SECTION("palindrome") { checkMemoriaLista(palindromo, "(1,2,3,2,1)"); }
    SECTION("not palindrome") { checkMemoriaLista(palindromo, "(1,2,3)"); }
}

TEST_CASE("PruebaEliminarSecuencia cases", "[PruebaEliminarSecuencia][file:listas]")
{
    auto check = [](const char *inputListaOriginal, const char *inputSecuenciaAEliminar, const char *expected)
    { checkListaConSecuenciaModificada(eliminarSecuencia, inputListaOriginal, inputSecuenciaAEliminar, expected); };

    SECTION("() remove ()") { check("()", "()", "()"); }
    SECTION("(1,2,3,4,5,6,7,8,9,0) remove (2,3,4)") { check("(1,2,3,4,5,6,7,8,9,0)", "(2,3,4)", "(1,5,6,7,8,9,0)"); }
    SECTION("(1,2,3,4,5,6,7,8,9,0) remove (2,4,5,6)") { check("(1,2,3,4,5,6,7,8,9,0)", "(2,4,5,6)", "(1,2,3,4,5,6,7,8,9,0)"); }
    SECTION("(1,2,3,4,5,6,7,8,9,0) remove ()") { check("(1,2,3,4,5,6,7,8,9,0)", "()", "(1,2,3,4,5,6,7,8,9,0)"); }
    SECTION("() remove (1,2,3,4,5,6,7,8,9,0)") { check("()", "(1,2,3,4,5,6,7,8,9,0)", "()"); }
    SECTION("(1,2,3,4,5,6,7,8,9,0) remove (1,2)") { check("(1,2,3,4,5,6,7,8,9,0)", "(1,2)", "(3,4,5,6,7,8,9,0)"); }
    SECTION("(1,2,3,4,5,6,7,8,9,0) remove (9,0)") { check("(1,2,3,4,5,6,7,8,9,0)", "(9,0)", "(1,2,3,4,5,6,7,8)"); }
    SECTION("(1,2,1,2,4,5,6,7,8,9,0) remove (1,2)") { check("(1,2,1,2,4,5,6,7,8,9,0)", "(1,2)", "(1,2,4,5,6,7,8,9,0)"); }
    SECTION("(4,2,1,2,5,6,4,2,2,1) remove (4,2,2)") { check("(4,2,1,2,5,6,4,2,2,1)", "(4,2,2)", "(4,2,1,2,5,6,1)"); }
    SECTION("(4,3,2,1,4,3,2,1) remove (4,3,2,1,0)") { check("(4,3,2,1,4,3,2,1)", "(4,3,2,1,0)", "(4,3,2,1,4,3,2,1)"); }
}

TEST_CASE("PruebaEliminarSecuencia memory cases", "[PruebaEliminarSecuencia][memory][file:listas]")
{
    SECTION("both empty") { checkMemoriaListaConSecuencia(eliminarSecuencia, "()", "()"); }
    SECTION("at start") { checkMemoriaListaConSecuencia(eliminarSecuencia, "(1,2,3,4)", "(1,2)"); }
    SECTION("in middle") { checkMemoriaListaConSecuencia(eliminarSecuencia, "(1,2,3,4,5)", "(2,3,4)"); }
}

TEST_CASE("PruebaMoverNodo cases", "[PruebaMoverNodo][file:listas]")
{
    auto check = [](const char *inputListaOriginal, unsigned int inicial, unsigned int final, const char *expected)
    { checkListaModificada([inicial, final](NodoLista *&lista)
                           { moverNodo(lista, inicial, final); }, inputListaOriginal, expected); };

    SECTION("move 1->2") { check("(1,2,3,4,5,6,7,8,9,0)", 1, 2, "(2,1,3,4,5,6,7,8,9,0)"); }
    SECTION("move 1->5") { check("(1,2,3,4,5,6,7,8,9,0)", 1, 5, "(2,3,4,5,1,6,7,8,9,0)"); }
    SECTION("move 5->1") { check("(1,2,3,4,5,6,7,8,9,0)", 5, 1, "(5,1,2,3,4,6,7,8,9,0)"); }
    SECTION("move 1->1") { check("(1,2,3,4,5,6,7,8,9,0)", 1, 1, "(1,2,3,4,5,6,7,8,9,0)"); }
    SECTION("move 3->11 invalid") { check("(1,2,3,4,5,6,7,8,9,0)", 3, 11, "(1,2,3,4,5,6,7,8,9,0)"); }
    SECTION("move 3->7") { check("(1,2,3,4,5,6,7,8,9,0)", 3, 7, "(1,2,4,5,6,7,3,8,9,0)"); }
    SECTION("move 7->3") { check("(1,2,3,4,5,6,7,8,9,0)", 7, 3, "(1,2,7,3,4,5,6,8,9,0)"); }
    SECTION("move 12->2 invalid") { check("(1,2,3,4,5,6,7,8,9,0)", 12, 2, "(1,2,3,4,5,6,7,8,9,0)"); }
    SECTION("move 10->9") { check("(1,2,3,4,5,6,7,8,9,0)", 10, 9, "(1,2,3,4,5,6,7,8,0,9)"); }
    SECTION("move 10->10") { check("(1,2,3,4,5,6,7,8,9,0)", 10, 10, "(1,2,3,4,5,6,7,8,9,0)"); }
    SECTION("move 0->10 invalid") { check("(1,2,3,4,5,6,7,8,9,0)", 0, 10, "(1,2,3,4,5,6,7,8,9,0)"); }
    SECTION("move 10->0 invalid") { check("(1,2,3,4,5,6,7,8,9,0)", 10, 0, "(1,2,3,4,5,6,7,8,9,0)"); }
}

TEST_CASE("PruebaMoverNodo memory cases", "[PruebaMoverNodo][memory][file:listas]")
{
    auto check = [](const char *input, unsigned int inicial, unsigned int final)
    { checkMemoriaLista([=](NodoLista *&lista)
                        { moverNodo(lista, inicial, final); }, input); };

    SECTION("forward") { check("(1,2,3,4,5)", 1, 5); }
    SECTION("backward") { check("(1,2,3,4,5)", 5, 1); }
    SECTION("invalid") { check("(1,2,3,4,5)", 8, 2); }
}

