#include <sstream>
#include "catch_amalgamated.hpp"
#include "ejercicios/arboles.hpp"
#include "check_resultado.hpp"
#include "func_aux.hpp"
#include "mem_tracking_fixture.hpp"

template <typename Type, typename Funcion, typename Esperado>
void checkArbolValor(Funcion funcion, const char *inputTree, Esperado expected)
{
    int largo;
    Type *arbol = static_cast<Type *>(FrameworkA1::parsearColeccion(inputTree, largo));
    auto resultado = funcion(arbol);
    CHECK(resultado == expected);
    FrameworkA1::destruir(arbol);
}

template <typename Nodo, typename Funcion>
void checkArbolLista(Funcion funcion, const char *inputTree, const char *expected)
{
    int largo, largoSolucion;
    Nodo *arbol = (Nodo *)FrameworkA1::parsearColeccion(inputTree, largo);
    NodoLista *solucion = (NodoLista *)FrameworkA1::parsearColeccion(expected, largoSolucion);
    NodoLista *resultado = funcion(arbol);
    auto iguales = FrameworkA1::sonIgualesDatosForma(resultado, solucion);

    checkResultadoSerializado(iguales, resultado, expected,
                              [](NodoLista *lista) { return FrameworkA1::serializar(lista); });
    FrameworkA1::destruir(arbol);
    FrameworkA1::destruir(resultado);
    FrameworkA1::destruir(solucion);
}

template <typename Nodo, typename Funcion>
void checkMemoriaArbol(Funcion funcion, const char *inputTree)
{
    int largo;
    Nodo *arbol = static_cast<Nodo *>(FrameworkA1::parsearColeccion(inputTree, largo));
    checkMemoriaEjecucion([&]
                          { funcion(arbol); });
    FrameworkA1::destruir(arbol);
}

template <typename Nodo, typename Funcion>
void checkMemoriaArbolLista(Funcion funcion, const char *inputTree)
{
    checkMemoriaArbol<Nodo>([&](Nodo *arbol)
                        {
        NodoLista *resultado = funcion(arbol);
        FrameworkA1::destruir(resultado); }, inputTree);
}

template <typename Nodo, typename Funcion, typename Esperado>
void checkDosArbolesValor(Funcion funcion, const char *inputA, const char *inputB, Esperado expected)
{
    int largoA, largoB;
    Nodo *a = static_cast<Nodo *>(FrameworkA1::parsearColeccion(inputA, largoA));
    Nodo *b = static_cast<Nodo *>(FrameworkA1::parsearColeccion(inputB, largoB));
    CHECK(funcion(a, b) == expected);
    FrameworkA1::destruir(a);
    FrameworkA1::destruir(b);
}

template <typename Nodo, typename Funcion>
void checkMemoriaDosArboles(Funcion funcion, const char *inputA, const char *inputB)
{
    int largoA, largoB;
    Nodo *a = static_cast<Nodo *>(FrameworkA1::parsearColeccion(inputA, largoA));
    Nodo *b = static_cast<Nodo *>(FrameworkA1::parsearColeccion(inputB, largoB));
    checkMemoriaEjecucion([&] { funcion(a, b); });
    FrameworkA1::destruir(a);
    FrameworkA1::destruir(b);
}

template <typename Nodo, typename Funcion, typename Esperado>
void checkArbolListaValor(Funcion funcion, const char *inputTree, const char *inputList, Esperado expected)
{
    int largoTree, largoList;
    Nodo *arbol = static_cast<Nodo *>(FrameworkA1::parsearColeccion(inputTree, largoTree));
    NodoLista *lista = static_cast<NodoLista *>(FrameworkA1::parsearColeccion(inputList, largoList));
    CHECK(funcion(arbol, lista) == expected);
    FrameworkA1::destruir(arbol);
    FrameworkA1::destruir(lista);
}

template <typename Nodo, typename Comparador>
void checkArbolesIguales(Nodo *resultado, Nodo *esperado, const char *esperadoTexto, Comparador comparar)
{
    checkResultadoSerializado(comparar(resultado, esperado), resultado, esperadoTexto,
                              [](Nodo *arbol) { return FrameworkA1::serializar(arbol); });
}

#define MEMORY_CASES(tipo, nombre, funcion, vacio, uno, varios)             \
    TEST_CASE(nombre " memory cases", "[" nombre "][memory][file:arboles]") \
    {                                                                       \
        SECTION("empty") { checkMemoriaArbol<tipo>(funcion, vacio); }       \
        SECTION("single") { checkMemoriaArbol<tipo>(funcion, uno); }        \
        SECTION("several") { checkMemoriaArbol<tipo>(funcion, varios); }    \
    }


TEST_CASE("PruebaAltura cases", "[PruebaAltura][file:arboles]")
{
    auto check = [](const char *inputTree, int expected)
    { checkArbolValor<NodoAB>(altura, inputTree, expected); };

    SECTION("{1,5,2}") { check("{1,5,2}", 2); }
    SECTION("{1,#,2,#,5}") { check("{1,#,2,#,5}", 3); }
    SECTION("{}") { check("{}", 0); }
    SECTION("{1,2,3}") { check("{1,2,3}", 2); }
    SECTION("{1,3}") { check("{1,3}", 2); }
    SECTION("{1}") { check("{1}", 1); }
    SECTION("long-case") { check("{1,2,3,1,#,#,#,#,3,1,2,1,1,#,#,#,#,#,4,1,#,2,2}", 9); }
    SECTION("all-ones") { check("{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}", 4); }
    SECTION("skewed-case") { check("{3,3,3,#,1,#,1,#,1,#,1,2,2,#,#,#,#,#,3,1,#,#,4}", 8); }
    SECTION("{1,3,#,1}") { check("{1,3,#,1}", 3); }
}

MEMORY_CASES(NodoAB, "PruebaAltura", [](NodoAB *arbol) { (void)altura(arbol); }, "{}", "{1}", "{1,2,3,4,5}")

TEST_CASE("PruebaSonIguales cases", "[PruebaSonIguales][file:arboles]")
{
    auto check = [](const char *inputA, const char *inputB, bool expected)
    { checkDosArbolesValor<NodoAB>(sonIguales, inputA, inputB, expected); };

    SECTION("equal-full") { check("{1,2,3,#,#,4,#,#,5}", "{1,2,3,#,#,4,#,#,5}", true); }
    SECTION("different-shape") { check("{1,2,3,#,#,4,#,#,5}", "{1,2,3}", false); }
    SECTION("equal-simple") { check("{1,2,3}", "{1,2,3}", true); }
    SECTION("different-values") { check("{1,3,2}", "{1,2,3}", false); }
    SECTION("empty-equals-empty") { check("{}", "{}", true); }
    SECTION("empty-vs-single") { check("{}", "{1}", false); }
    SECTION("single-vs-empty") { check("{1}", "{}", false); }
    SECTION("value-diff") { check("{1,1,#,5,7,1,5,6,1}", "{1,1,#,5,7,1,5,6,2}", false); }
    SECTION("repeated-shape") { check("{1,#,2,1,1,1,1}", "{1,#,2,1,1,1,1}", true); }
    SECTION("different-null-placement") { check("{1,#,2,1,1,1,1}", "{1,2,#,1,1,1,1}", false); }
}

TEST_CASE("PruebaSonIguales memory cases", "[PruebaSonIguales][memory][file:arboles]")
{
    auto check = [](const char *inputA, const char *inputB)
    { checkMemoriaDosArboles<NodoAB>([](NodoAB *a, NodoAB *b) { (void)sonIguales(a, b); }, inputA, inputB); };
    SECTION("empty") { check("{}", "{}"); }
    SECTION("equal") { check("{1,2,3}", "{1,2,3}"); }
    SECTION("different") { check("{1,2,3}", "{1,3,2}"); }
}

TEST_CASE("PruebaExisteCaminoConSuma cases", "[PruebaExisteCaminoConSuma][file:arboles]")
{
    auto check = [](const char *inputTree, int suma, bool expected)
    { checkArbolValor<NodoAB>([suma](NodoAB *arbol)
                              { return existeCaminoConSuma(arbol, suma); }, inputTree, expected); };

    SECTION("empty-0") { check("{}", 0, true); }
    SECTION("3-in-path") { check("{1,2,3}", 4, true); }
    SECTION("complex-true") { check("{1,2,3,#,#,4,#,#,5}", 3, true); }
    SECTION("complex-false-4") { check("{1,2,3,#,#,4,#,#,5}", 2, false); }
    SECTION("complex-false-13") { check("{1,2,3,#,#,4,#,#,5}", 4, false); }
    SECTION("complex-true-13") { check("{1,2,3,#,#,4,#,#,5}", 13, true); }
    SECTION("complex-false-14") { check("{1,1,#,5,7,1,5,6,2}", 14, false); }
    SECTION("complex-false-9") { check("{1,1,#,5,7,1,5,6,2}", 9, false); }
    SECTION("complex-true-11") { check("{1,1,#,5,7,1,5,6,2}", 11, true); }
    SECTION("complex-true-12") { check("{1,1,#,5,7,1,5,6,2}", 12, true); }
    SECTION("negative-true") { check("{-1,1,#,5,7,1,5,6,2}", 10, true); }
    SECTION("negative-zero-true") { check("{-7,1,#,5,7,1,5,6,2}", 0, true); }
}

MEMORY_CASES(NodoAB, "PruebaExisteCaminoConSuma", [](NodoAB *arbol) { (void)existeCaminoConSuma(arbol, 4); }, "{}", "{1}", "{1,2,3,4,5}")

TEST_CASE("PruebaEsArbolBalanceado cases", "[PruebaEsArbolBalanceado][file:arboles]")
{
    auto check = [](const char *inputTree, bool expected)
    { checkArbolValor<NodoAB>(esArbolBalanceado, inputTree, expected); };

    SECTION("{1,5,2}") { check("{1,5,2}", true); }
    SECTION("{1,#,2,#,3}") { check("{1,#,2,#,3}", false); }
    SECTION("{}") { check("{}", true); }
    SECTION("{1,5,2,4}") { check("{1,5,2,4}", true); }
    SECTION("{1}") { check("{1}", true); }
    SECTION("{1,2,2,4,3}") { check("{1,2,2,4,3}", true); }
    SECTION("all-ones") { check("{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}", true); }
    SECTION("complex-false") { check("{0,-3,1,5,#,#,9,#,#,#,1,#,0,#,1,4,#,#,5}", false); }
    SECTION("{1, 2, 3, 4, 5, #, 6}") { check("{1, 2, 3, 4, 5, #, 6}", true); }
    SECTION("{1, 2, 3, 4, 5, 6, 7}") { check("{1, 2, 3, 4, 5, 6, 7}", true); }
    SECTION("{1,2,3,4,#,#,5}") { check("{1,2,3,4,#,#,5}", true); }
    SECTION("{1,2,3,4,#,#,5,6,#,#,7}") { check("{1,2,3,4,#,#,5,6,#,#,7}", false); }
    SECTION("big-true") { check("{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,#,#,#,#,#,#,#,#,#,#,#,16}", true); }
    SECTION("big-false") { check("{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,#,#,#,#,#,#,#,#,#,#,#,16,#,#,#,#,#,17}", false); }
    SECTION("{1,2,3,4,#,5}") { check("{1,2,3,4,#,5}", true); }
}

MEMORY_CASES(NodoAB, "PruebaEsArbolBalanceado", [](NodoAB *arbol) { (void)esArbolBalanceado(arbol); }, "{}", "{1}", "{1,2,3,4,5}")

TEST_CASE("PruebaEnNivel cases", "[PruebaEnNivel][file:arboles]")
{
    auto check = [](const char *inputTree, int nivel, const char *expected)
    { checkArbolLista<NodoAB>([nivel](NodoAB *arbol)
                              { return enNivel(arbol, nivel); }, inputTree, expected); };

    SECTION("{} level 1") { check("{}", 1, "()"); }
    SECTION("{1,2,3,4,5,6,7} level 1") { check("{1,2,3,4,5,6,7}", 1, "(1)"); }
    SECTION("{1,2,3,4,5,6,7} level 2") { check("{1,2,3,4,5,6,7}", 2, "(2,3)"); }
    SECTION("{1,2,3,4,5,6,7} level 3") { check("{1,2,3,4,5,6,7}", 3, "(4,5,6,7)"); }
    SECTION("{1,2,3,4,5,6,7} level 4") { check("{1,2,3,4,5,6,7}", 4, "()"); }
    SECTION("big-tree level 1") { check("{1,2,3,4,#,5,6,#,7,8,9,#,10,#,#,11,#,#,#,12,13,#,#,14,#,#,15}", 1, "(1)"); }
    SECTION("big-tree level 2") { check("{1,2,3,4,#,5,6,#,7,8,9,#,10,#,#,11,#,#,#,12,13,#,#,14,#,#,15}", 2, "(2,3)"); }
    SECTION("big-tree level 3") { check("{1,2,3,4,#,5,6,#,7,8,9,#,10,#,#,11,#,#,#,12,13,#,#,14,#,#,15}", 3, "(4,5,6)"); }
    SECTION("big-tree level 4") { check("{1,2,3,4,#,5,6,#,7,8,9,#,10,#,#,11,#,#,#,12,13,#,#,14,#,#,15}", 4, "(7,8,9,10)"); }
    SECTION("big-tree level 5") { check("{1,2,3,4,#,5,6,#,7,8,9,#,10,#,#,11,#,#,#,12,13,#,#,14,#,#,15}", 5, "(11,12,13)"); }
    SECTION("big-tree level 6") { check("{1,2,3,4,#,5,6,#,7,8,9,#,10,#,#,11,#,#,#,12,13,#,#,14,#,#,15}", 6, "(14,15)"); }
}

TEST_CASE("PruebaEnNivel memory cases", "[PruebaEnNivel][memory][file:arboles]")
{
    SECTION("empty")
    {
        checkMemoriaArbolLista<NodoAB>([](NodoAB *a)
                                 { return enNivel(a, 1); }, "{}");
    }
    SECTION("root")
    {
        checkMemoriaArbolLista<NodoAB>([](NodoAB *a)
                                 { return enNivel(a, 1); }, "{1,2,3}");
    }
    SECTION("deep")
    {
        checkMemoriaArbolLista<NodoAB>([](NodoAB *a)
                                 { return enNivel(a, 3); }, "{1,2,3,4,5,6,7}");
    }
}

TEST_CASE("PruebaCantNodosEntreNiveles cases", "[PruebaCantNodosEntreNiveles][file:arboles]")
{
    auto check = [](const char *inputTree, int desde, int hasta, int expected)
    { checkArbolValor<NodoAB>([desde, hasta](NodoAB *arbol)
                              { return cantNodosEntreNiveles(arbol, desde, hasta); }, inputTree, expected); };

    SECTION("{1,2,3,4,5,6,7},1,1") { check("{1,2,3,4,5,6,7}", 1, 1, 1); }
    SECTION("{1,2,3,4,5,6,7},1,2") { check("{1,2,3,4,5,6,7}", 1, 2, 3); }
    SECTION("{1,2,3,4,5,6,7},1,4") { check("{1,2,3,4,5,6,7}", 1, 4, 7); }
    SECTION("{1,2,3,4,5,6,7},2,4") { check("{1,2,3,4,5,6,7}", 2, 4, 6); }
    SECTION("complex-1") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 1, 6, 9); }
    SECTION("complex-empty") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 5, 4, 0); }
    SECTION("{} empty") { check("{}", 1, 3, 0); }
    SECTION("{} one") { check("{}", 1, 1, 0); }
    SECTION("{} three") { check("{}", 3, 1, 0); }
    SECTION("{1,2,3,4,#,#,#,5},3,3") { check("{1,2,3,4,#,#,#,5}", 3, 3, 1); }
}

MEMORY_CASES(NodoAB, "PruebaCantNodosEntreNiveles", [](NodoAB *arbol) { (void)cantNodosEntreNiveles(arbol, 1, 3); }, "{}", "{1}", "{1,2,3,4,5}")

TEST_CASE("PruebaCamino cases", "[PruebaCamino][file:arboles]")
{
    auto check = [](const char *inputTree, int dato, const char *expected)
    { checkArbolLista<NodoAB>([dato](NodoAB *arbol)
                              { return camino(arbol, dato); }, inputTree, expected); };

    SECTION("9") { check("{8,3,10,1,5,9,13}", 9, "(8,10,9)"); }
    SECTION("root") { check("{4}", 4, "(4)"); }
    SECTION("left-root") { check("{1,#,2,#,3,#,4,#,5}", 1, "(1)"); }
    SECTION("parent") { check("{10,5,13,#,6,#,15,#,9,#,30,7}", 13, "(10,13)"); }
    SECTION("negative") { check("{-80,-130,-1,#,-90,-5,10}", -90, "(-80,-130,-90)"); }
    SECTION("big-left") { check("{120,53,180,33,88,150,300,15,#,66,100,#,#,#,#,#,18,#,#,#,#,#,31}", 180, "(120, 180)"); }
    SECTION("zero") { check("{0,-5,8,-10,-4,5,1000000}", 0, "(0)"); }
    SECTION("deep") { check("{120,53,180,33,88,150,300,15,#,66,100,#,#,#,#,#,18,#,#,#,#,#,31}", 18, "(120, 53, 33, 15, 18)"); }
    SECTION("deep-right") { check("{10,5,13,#,6,#,15,#,9,#,30,7}", 7, "(10, 5, 6, 9, 7)"); }
    SECTION("small-right") { check("{0,-5,8,-10,-4,5,1000000}", 5, "(0, 8, 5)"); }
}

TEST_CASE("PruebaCamino memory cases", "[PruebaCamino][memory][file:arboles]")
{
    SECTION("empty")
    {
        checkMemoriaArbolLista<NodoAB>([](NodoAB *a)
                                 { return camino(a, 1); }, "{}");
    }
    SECTION("root")
    {
        checkMemoriaArbolLista<NodoAB>([](NodoAB *a)
                                 { return camino(a, 1); }, "{1}");
    }
    SECTION("deep")
    {
        checkMemoriaArbolLista<NodoAB>([](NodoAB *a)
                                 { return camino(a, 5); }, "{1,2,3,4,5}");
    }
}

TEST_CASE("PruebaInvertirHastak cases", "[PruebaInvertirHastak][file:arboles]")
{
    auto check = [](const char *inputTree, int k, const char *expected)
    {
        int largo;
        NodoAB *arbol = (NodoAB *)FrameworkA1::parsearColeccion(inputTree, largo);
        int largoSolucion;
        NodoAB *solucion = (NodoAB *)FrameworkA1::parsearColeccion(expected, largoSolucion);
        NodoAB *resultado = invertirHastak(arbol, k);
        checkArbolesIguales(resultado, solucion, expected,
                             [](NodoAB *a, NodoAB *b) { return FrameworkA1::sonIgualesDatosForma(a, b); });
        FrameworkA1::destruir(arbol);
        FrameworkA1::destruir(resultado);
        FrameworkA1::destruir(solucion);
    };

    SECTION("empty") { check("{}", 5, "{}"); }
    SECTION("k=10") { check("{1,2,3}", 10, "{1,3,2}"); }
    SECTION("k=0") { check("{1,2,3,4,5,6,7}", 0, "{}"); }
    SECTION("k=1") { check("{1,2,3,4,5,6,7}", 1, "{1}"); }
    SECTION("k=2") { check("{1,2,3,4,5,6,7}", 2, "{1,3,2}"); }
    SECTION("k=3") { check("{1,2,3,4,5,6,7}", 3, "{1,3,2,7,6,5,4}"); }
    SECTION("skewed-1") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 1, "{1}"); }
    SECTION("skewed-2") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 2, "{1,3,2}"); }
    SECTION("skewed-3") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 3, "{1,3,2,6,5,4}"); }
    SECTION("skewed-4") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 4, "{1,3,2,6,5,4,#,8,7}"); }
    SECTION("skewed-5") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 5, "{1,3,2,6,5,4,#,8,7,#,#,#,#,9}"); }
}

TEST_CASE("PruebaInvertirHastak memory cases", "[PruebaInvertirHastak][memory][file:arboles]")
{
    auto check = [](const char *input, int k)
    { checkMemoriaArbol<NodoAB>([=](NodoAB *a)
                          { NodoAB *resultado = invertirHastak(a, k); FrameworkA1::destruir(resultado); }, input); };
    SECTION("empty") { check("{}", 3); }
    SECTION("zero") { check("{1,2,3}", 0); }
    SECTION("several levels") { check("{1,2,3,4,5,6,7}", 3); }
}

TEST_CASE("PruebaBorrarNodoRaiz cases", "[PruebaBorrarNodoRaiz][file:arboles]")
{
    auto check = [](const char *inputTree, const char *expected)
    {
        int largo;
        NodoAB *arbol = (NodoAB *)FrameworkA1::parsearColeccion(inputTree, largo);
        NodoAB *solucion = (NodoAB *)FrameworkA1::parsearColeccion(expected, largo);
        borrarNodoRaiz(arbol);
        bool esAbb = FrameworkA1::esABB(arbol);
        checkArbolesIguales(arbol, solucion, expected,
                             [](NodoAB *a, NodoAB *b) { return FrameworkA1::sonIgualesDatos(a, b); });
        if (!esAbb)
            FAIL_CHECK("El resultado no es un ABB");
        else
            CHECK(true);
        FrameworkA1::destruir(arbol);
        FrameworkA1::destruir(solucion);
    };

    SECTION("leaf") { check("{1}", "{}"); }
    SECTION("root-with-right") { check("{1,#,2}", "{2}"); }
    SECTION("root-with-left-right") { check("{3,2,6}", "{2,#,6}"); }
    SECTION("full-tree") { check("{8,3,10,1,5,9,13}", "{1,3,5,9,10,13}"); }
    SECTION("balanced") { check("{5,3,7,2,4,6}", "{2,3,6,7,4}"); }
    SECTION("two-level") { check("{5,3,6,1,4}", "{3,6,1,4}"); }
    SECTION("skewed-right") { check("{1,#,2,#,3,#,4,#,5}", "{2,3,4,5}"); }
    SECTION("complex") { check("{10,5,13,#,6,#,15,#,9,#,30,7}", "{5,6,7,9,13,15,30}"); }
    SECTION("negative") { check("{0,-5,8,-10,-4,5,1000000}", "{-10,-5,-4,5,8,1000000}"); }
    SECTION("big") { check("{120,53,180,33,88,130,300,15,#,66,100,#,#,#,#,#,18,#,#,#,#,#,31}", "{100,53,180,33,88,130,300,15,#,66,#,#,#,#,#,#,18,#,#,#,31}"); }
}

TEST_CASE("PruebaBorrarNodoRaiz memory cases", "[PruebaBorrarNodoRaiz][memory][file:arboles]")
{
    SECTION("leaf")
    {
        checkMemoriaArbol<NodoAB>([](NodoAB *&a)
                            { borrarNodoRaiz(a); }, "{1}");
    }
    SECTION("one child")
    {
        checkMemoriaArbol<NodoAB>([](NodoAB *&a)
                            { borrarNodoRaiz(a); }, "{1,#,2}");
    }
    SECTION("two children")
    {
        checkMemoriaArbol<NodoAB>([](NodoAB *&a)
                            { borrarNodoRaiz(a); }, "{3,2,6}");
    }
}

TEST_CASE("PruebaSumaABB cases", "[PruebaSumaABB][file:arboles]")
{
    auto check = [](const char *inputTree, int n, bool expected)
    { checkArbolValor<NodoAB>([n](NodoAB *arbol) { return sumaABB(arbol, n); }, inputTree, expected); };

    SECTION("{3,1,4,0,2},7") { check("{3,1,4,0,2}", 7, true); }
    SECTION("{},0") { check("{}", 0, false); }
    SECTION("{2},3") { check("{2}", 3, false); }
    SECTION("{1},1") { check("{1}", 1, false); }
    SECTION("{2,#,4},3") { check("{2,#,4}", 3, false); }
    SECTION("{2,#,4},6") { check("{2,#,4}", 6, true); }
    SECTION("{5,3,8,2,4,7,10},10") { check("{5,3,8,2,4,7,10}", 10, true); }
    SECTION("{5,3,8,2,4,7,10},1024") { check("{5,3,8,2,4,7,10}", 1024, false); }
    SECTION("{5,3,8,2,4,7,10},39") { check("{5,3,8,2,4,7,10}", 39, false); }
    SECTION("{5,3,8,2,4,7,10},18") { check("{5,3,8,2,4,7,10}", 18, true); }
    SECTION("{5,3,8,2,4,7,10},12") { check("{5,3,8,2,4,7,10}", 12, true); }
    SECTION("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20},30") { check("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20}", 30, true); }
    SECTION("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20},21") { check("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20}", 21, true); }
    SECTION("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20},23") { check("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20}", 23, true); }
    SECTION("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20},20") { check("{5,3,8,2,4,7,10,1,#,#,#,6,#,#,20}", 20, false); }
}

MEMORY_CASES(NodoAB, "PruebaSumaABB", [](NodoAB *arbol) { (void)sumaABB(arbol, 4); }, "{}", "{1}", "{1,2,3,4,5}")

TEST_CASE("PruebaSucesorABB cases", "[PruebaSucesorABB][file:arboles]")
{
    auto check = [](const char *inputTree, int n, int expected)
    {
        int largo;
        NodoAB *arbol = (NodoAB *)FrameworkA1::parsearColeccion(inputTree, largo);
        NodoAB *copiaArbol = (NodoAB *)FrameworkA1::parsearColeccion(inputTree, largo);
        CHECK(sucesor(arbol, n) == expected);

        if (!FrameworkA1::sonIgualesDatosForma(arbol, copiaArbol))
            FAIL_CHECK("La función modifica el parámetro de entrada");
        FrameworkA1::destruir(arbol);
        FrameworkA1::destruir(copiaArbol);
    };

    SECTION("{2,#,4},2") { check("{2,#,4}", 2, 4); }
    SECTION("{3,1,4},1") { check("{3,1,4}", 1, 3); }
    SECTION("{5,3,8,2,4,7,10},5") { check("{5,3,8,2,4,7,10}", 5, 7); }
    SECTION("{5,3,8,2,4,7,10},3") { check("{5,3,8,2,4,7,10}", 3, 4); }
    SECTION("complex-10") { check("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4}", 10, 20); }
    SECTION("complex-3") { check("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4}", 3, 4); }
    SECTION("complex-7") { check("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4}", 7, 8); }
    SECTION("complex-8") { check("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4}", 8, 10); }
    SECTION("{5,2},3") { check("{5,2}", 3, 5); }
    SECTION("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4},3") { check("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4}", 3, 4); }
    SECTION("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4},11") { check("{10,7,20,5,8,#,25,3,6,#,#,#,#,1,4}", 11, 20); }
    SECTION("{5,2},7") { check("{5,2}", 7, -1); }
    SECTION("{5,2,#,1,3},5") { check("{5,2,#,1,3}", 5, -1); }
}

MEMORY_CASES(NodoAB, "PruebaSucesorABB", [](NodoAB *arbol) { (void)sucesor(arbol, 1); }, "{}", "{1}", "{1,2,3,4,5}")

TEST_CASE("PruebaNivelMasNodos cases", "[PruebaNivelMasNodos][file:arboles]")
{
    auto check = [](const char *inputTree, int nivelHasta, int expected)
    { checkArbolValor<NodoAB>([nivelHasta](NodoAB *arbol) { return nivelMasNodos(arbol, nivelHasta); }, inputTree, expected); };

    SECTION("{}") { check("{}", 0, 0); }
    SECTION("{1}") { check("{1}", 1, 1); }
    SECTION("{1,2,3,4,5,6,7},1") { check("{1,2,3,4,5,6,7}", 1, 1); }
    SECTION("{1,2,3,4,5,6,7},2") { check("{1,2,3,4,5,6,7}", 2, 2); }
    SECTION("{1,2,3,4,5,6,7},3") { check("{1,2,3,4,5,6,7}", 3, 3); }
    SECTION("{1,2,3,4,5,6,7},5") { check("{1,2,3,4,5,6,7}", 5, 3); }
    SECTION("complex1") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 5, 3); }
    SECTION("complex2") { check("{1,2,3,#,4,5,6,#,#,#,#,7,8,#,#,#,9}", 2, 2); }
    SECTION("complex3") { check("{1,2,3,#,4,5,6,7,8,9,10,11,12,#,#,#,13}", 5, 4); }
    SECTION("complex4") { check("{1,2,3,#,4,5,6,7,8,9,10,11,12,#,#,#,13}", 3, 3); }
    SECTION("complex5") { check("{1,2,3,#,#,4,6}", 3, 2); }
    SECTION("complex6") { check("{1,2,3,#,#,4,6,5,11}", 3, 2); }
}

MEMORY_CASES(NodoAB, "PruebaNivelMasNodos", [](NodoAB *arbol) { (void)nivelMasNodos(arbol, 3); }, "{}", "{1}", "{1,2,3,4,5}")

TEST_CASE("PruebaAlturaAG cases", "[PruebaAlturaAG][file:arboles]")
{
    auto check = [](const char *inputTree, int expected)
    { checkArbolValor<NodoAG>(alturaAG, inputTree, expected); };

    SECTION("{{}}") { check("{{}}", 0); }
    SECTION("{{1,2,5,#,#,3,#,4}}") { check("{{1,2,5,#,#,3,#,4}}", 3); }
    SECTION("{{1,2,5,#,6,11,#,#,#,3,#,4,7,#,8,#,9,#,10}}") { check("{{1,2,5,#,6,11,#,#,#,3,#,4,7,#,8,#,9,#,10}}", 4); }
    SECTION("{{10}}") { check("{{10}}", 1); }
    SECTION("{{1,2,#,3,#,4,5,#,6,#,7,8}}") { check("{{1,2,#,3,#,4,5,#,6,#,7,8}}", 4); }
    SECTION("{{1,2,#,3,#,4,#,5,#,6,#,7,#,8,#,9}}") { check("{{1,2,#,3,#,4,#,5,#,6,#,7,#,8,#,9}}", 2); }
    SECTION("{{1,2,3,4,5}}") { check("{{1,2,3,4,5}}", 5); }
    SECTION("{{1,2,3,4,5,#,6}}") { check("{{1,2,3,4,5,#,6}}", 5); }
    SECTION("{{1,2,3,4,#,#,#,5,6,7}}") { check("{{1,2,3,4,#,#,#,5,6,7}}", 4); }
}

MEMORY_CASES(NodoAG, "PruebaAlturaAG", [](NodoAG *arbol) { (void)alturaAG(arbol); }, "{{}}", "{{1}}", "{{1,2,3,#,4}}")

TEST_CASE("PruebaSumaPorNiveles cases", "[PruebaSumaPorNiveles][file:arboles]")
{
    auto check = [](const char *inputTree, int expected)
    { checkArbolValor<NodoAG>(sumaPorNiveles, inputTree, expected); };

    SECTION("{{}}") { check("{{}}", 0); }
    SECTION("{{6}}") { check("{{6}}", -6); }
    SECTION("{{6,2}}") { check("{{6,2}}", -4); }
    SECTION("{{6,#,4,#,1}}") { check("{{6,#,4,#,1}}", -11); }
    SECTION("{{90,60,10,40,#,45}}") { check("{{90,60,10,40,#,45}}", 45); }
    SECTION("{{90,60,10,40,#,45,12,#,#,3,#,2,#,43}}") { check("{{90,60,10,40,#,45,12,#,#,3,#,2,#,43}}", 81); }
    SECTION("{{1,2,3,#,1}}") { check("{{1,2,3,#,1}}", -3); }
    SECTION("{{1,2,5,#,#,3,#,4}}") { check("{{1,2,5,#,#,3,#,4}}", 3); }
    SECTION("{{1,2,5,#,20,#,#,3,#,4,7}}") { check("{{1,2,5,#,20,#,#,3,#,4,7}}", -24); }
    SECTION("{{1,-7,5,#,20,#,#,3,#,4,7,33}}") { check("{{1,-7,5,#,20,#,#,3,#,4,7,33}}", 0); }
    SECTION("{{1,-7,5,#,15,#,#,3,#,4,8,34}}") { check("{{1,-7,5,#,15,#,#,3,#,4,8,34}}", 5); }
}

MEMORY_CASES(NodoAG, "PruebaSumaPorNiveles", [](NodoAG *arbol) { (void)sumaPorNiveles(arbol); }, "{{}}", "{{1}}", "{{1,2,3,#,4}}")

TEST_CASE("PruebaEsPrefijo cases", "[PruebaEsPrefijo][file:arboles]")
{
    auto check = [](const char *inputTree, const char *inputList, bool expected)
    { checkArbolListaValor<NodoAG>(esPrefijo, inputTree, inputList, expected); };

    SECTION("empty-tree-empty-list") { check("{{}}", "()", true); }
    SECTION("single-tree-empty-list") { check("{{1}}", "()", true); }
    SECTION("empty-tree-nonempty") { check("{{}}", "(1,2,3,4)", false); }
    SECTION("prefix-true") { check("{{1,2,5,#,#,3,7,4}}", "(1,2,5)", true); }
    SECTION("prefix-false-1") { check("{{2,4,6,#,8}}", "(2,3,6,8)", false); }
    SECTION("prefix-false-2") { check("{{2,4,6,#,8}}", "(2,4,6,8)", false); }
    SECTION("prefix-true-3") { check("{{1,2,3,#,#,#,1}}", "(1,2,3)", true); }
    SECTION("prefix-false-4") { check("{{1,2,3,#,1}}", "(1,2,3,4)", false); }
    SECTION("empty-prefix-true") { check("{{1,-7,5,#,20,#,#,3,#,4,7,33}}", "()", true); }
    SECTION("false-again") { check("{{1,-7,5,#,20,#,#,3,#,4,7,33}}", "(1,-7,4)", false); }
    SECTION("true-mid") { check("{{1,-7,5,#,20,#,3,4,7,33}}", "(1,-7,3,4,7)", true); }
    SECTION("true-tail") { check("{{1,-7,5,#,20,#,#,3,#,4,7,33}}", "(1,4,7,33)", true); }
    SECTION("false-tail") { check("{{1,-7,5,#,20,#,#,3,#,4,7,33}}", "(1,-7,5,20)", false); }
    SECTION("true-late") { check("{{1,-7,5,#,20,#,#,3,#,#,4,7,33}}", "(4,7,33)", true); }
    SECTION("true-deep") { check("{{1,-7,5,#,20,#,#,3,#,#,4,7,33,#,88,45}}", "(4,7,88,45)", true); }
    SECTION("true-nested") { check("{{5,7,1,#,#,3,8,#,4,#,#,1,#,#,10,9,1,#,#,3,2}}", "(10,3,2)", true); }
    SECTION("false-nested") { check("{{5,7,1,#,#,3,8,#,4,#,#,1,#,#,10,9,1,#,#,3,2}}", "(10,9,1,1)", false); }
}

TEST_CASE("PruebaEsPrefijo memory cases", "[PruebaEsPrefijo][memory][file:arboles]")
{
    auto check = [](const char *inputTree, const char *inputList)
    {
        int largoLista;
        NodoLista *lista = (NodoLista *)FrameworkA1::parsearColeccion(inputList, largoLista);
        checkMemoriaArbol<NodoAG>([&](NodoAG *a)
                            { (void)esPrefijo(a, lista); }, inputTree);
        FrameworkA1::destruir(lista);
    };
    SECTION("empty path") { check("{{1}}", "()"); }
    SECTION("prefix") { check("{{1,2,3,#,4}}", "(1,2,4)"); }
    SECTION("not prefix") { check("{{1,2,3,#,4}}", "(1,4)"); }
}

TEST_CASE("PruebaCaminoAG cases", "[PruebaCaminoAG][file:arboles]")
{
    auto check = [](const char *inputTree, int dato, const char *expected)
    { checkArbolLista<NodoAG>([dato](NodoAG *arbol)
                              { return caminoAG(arbol, dato); }, inputTree, expected); };

    SECTION("single-1") { check("{{1}}", 1, "(1)"); }
    SECTION("single-5") { check("{{1}}", 5, "()"); }
    SECTION("small") { check("{{1,2,3,#,4}}", 4, "(1,2,4)"); }
    SECTION("simple") { check("{{1,2,4}}", 4, "(1,2,4)"); }
    SECTION("path-1") { check("{{1,2,4,#,#,3,5}}", 5, "(1,3,5)"); }
    SECTION("path-2") { check("{{1,2,4,#,#,3,5,#,#,6,7}}", 6, "(1,6)"); }
    SECTION("deep-9") { check("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9}}", 9, "(1,7,9)"); }
    SECTION("deep-4") { check("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9}}", 4, "(1,2,4)"); }
    SECTION("big-13") { check("{{0,1,2,3,#,4,5,#,#,#,6,#,7,8,#,9,#,#,#,10,11,12,#,13}}", 13, "(0,10,11,13)"); }
    SECTION("big-5") { check("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9}}", 5, "(1,2,4,5)"); }
    SECTION("big-7") { check("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9}}", 7, "(1,7)"); }
    SECTION("nested") { check("{{5,7,1,#,#,3,8,#,4,#,#,1,#,#,10,9,1,#,#,3,2}}", 2, "(10,3,2)"); }
    SECTION("repeat-9") { check("{{1,2,3,#,4,9,#,#,#,6,#,7,8,#,9}}", 9, "(1,2,4,9)"); }
    SECTION("repeat-4") { check("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,4}}", 4, "(1,2,4)"); }
}

TEST_CASE("PruebaCaminoAG memory cases", "[PruebaCaminoAG][memory][file:arboles]")
{
    SECTION("empty")
    {
        checkMemoriaArbolLista<NodoAG>([](NodoAG *a)
                                 { return caminoAG(a, 1); }, "{{}}");
    }
    SECTION("root")
    {
        checkMemoriaArbolLista<NodoAG>([](NodoAG *a)
                                 { return caminoAG(a, 1); }, "{{1}}");
    }
    SECTION("deep")
    {
        checkMemoriaArbolLista<NodoAG>([](NodoAG *a)
                                 { return caminoAG(a, 4); }, "{{1,2,3,#,4}}");
    }
}

TEST_CASE("PruebaNivelConMasNodosAG cases", "[PruebaNivelConMasNodosAG][file:arboles]")
{
    auto check = [](const char *inputTree, int expected)
    { checkArbolValor<NodoAG>(nivelConMasNodosAG, inputTree, expected); };

    SECTION("{{1}}") { check("{{1}}", 1); }
    SECTION("{{1,2,#,3}}") { check("{{1,2,#,3}}", 2); }
    SECTION("{{1,2,4,#,#,3,5}}") { check("{{1,2,4,#,#,3,5}}", 2); }
    SECTION("{{1,2,3,#,4,#,5,#,6}}") { check("{{1,2,3,#,4,#,5,#,6}}", 3); }
    SECTION("{{1,2,4,#,#,3,5,#,#,6,#,7,8,#,9,#,10,#,11}}") { check("{{1,2,4,#,#,3,5,#,#,6,#,7,8,#,9,#,10,#,11}}", 3); }
    SECTION("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9}}") { check("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9}}", 3); }
    SECTION("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9,#,#,10,#,11}}") { check("{{1,2,3,#,4,5,#,#,#,6,#,7,8,#,9,#,#,10,#,11}}", 2); }
    SECTION("{{1,2,3,#,4,#,5,#,6,#,#,7,#,9,#,10}}") { check("{{1,2,3,#,4,#,5,#,6,#,#,7,#,9,#,10}}", 2); }
    SECTION("{{1,2,3,#,4,#,5,#,6,#,#,7,8,#,9,#,10}}") { check("{{1,2,3,#,4,#,5,#,6,#,#,7,8,#,9,#,10}}", 3); }
    SECTION("{{1,2,4,#,#,3,5,11,#,12,#,13,#,14,#,15,#,#,#,6,#,7,8,#,9}}") { check("{{1,2,4,#,#,3,5,11,#,12,#,13,#,14,#,15,#,#,#,6,#,7,8,#,9}}", 4); }
    SECTION("{{1,2,4,#,#,3,5,#,#,#,6,7,#,#,8,#,9}}") { check("{{1,2,4,#,#,3,5,#,#,#,6,7,#,#,8,#,9}}", 1); }
}

MEMORY_CASES(NodoAG, "PruebaNivelConMasNodosAG", [](NodoAG *arbol) { (void)nivelConMasNodosAG(arbol); }, "{{}}", "{{1}}", "{{1,2,3,#,4}}")

#undef MEMORY_CASES
