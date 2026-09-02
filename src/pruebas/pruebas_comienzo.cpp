#include <sstream>
#include "catch_amalgamated.hpp"

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "comienzo.hpp"
#include "check_resultado.hpp"
#include "func_aux.hpp"
#include "mem_tracking_fixture.hpp"

class RedireccionCout
{
    std::streambuf *anterior;

public:
    explicit RedireccionCout(std::ostream &destino)
        : anterior(std::cout.rdbuf(destino.rdbuf())) {}

    ~RedireccionCout()
    {
        std::cout.rdbuf(anterior);
    }
};

template <typename Funcion>
void checkSalida(Funcion funcion, const std::string &expected)
{
    std::ostringstream salida;
    RedireccionCout redireccion(salida);
    funcion();

    std::string resultado = salida.str();
    if (!resultado.empty() && resultado.back() == '\n')
        resultado.pop_back();
    CHECK(resultado == expected);
}

template <typename Funcion>
void checkVectorIntModificado(Funcion funcion, const char *input, const std::string &expected)
{
    int largo, largoEsperado;
    int *vector = (int *)FrameworkA1::parsearColeccion(input, largo);
    int *esperado = (int *)FrameworkA1::parsearColeccion(expected.c_str(), largoEsperado);

    funcion(vector, largo);
    bool iguales = FrameworkA1::sonIguales(vector, esperado, largoEsperado);
    checkResultadoSerializado(iguales, vector, expected,
                              [largo](int *resultado)
                              { return FrameworkA1::serializar(resultado, largo); });
    FrameworkA1::destruir(vector);
    FrameworkA1::destruir(esperado);
}

TEST_CASE("PruebaOrdenarVecInt memory cases", "[PruebaOrdenarVecInt][memory][file:comienzo]")
{
    auto check = [](const char *input)
    {
        int largo;
        int *vector = (int *)FrameworkA1::parsearColeccion(input, largo);
        checkMemoriaEjecucion([&]
                              { ordenarVecInt(vector, largo); });
        FrameworkA1::destruir(vector);
    };
    SECTION("empty") { check("[]"); }
    SECTION("unsorted") { check("[9,5,1]"); }
    SECTION("duplicates") { check("[1,6,4,9,2,4,9,1]"); }
}

TEST_CASE("PruebaIntercalarVector memory cases", "[PruebaIntercalarVector][memory][file:comienzo]")
{
    auto check = [](const char *input1, const char *input2)
    {
        int largo1, largo2;
        int *vector1 = (int *)FrameworkA1::parsearColeccion(input1, largo1);
        int *vector2 = (int *)FrameworkA1::parsearColeccion(input2, largo2);
        checkMemoriaEjecucion([&]
                              {
            int *resultado = intercalarVector(vector1, vector2, largo1, largo2);
            FrameworkA1::destruir(resultado); });
        FrameworkA1::destruir(vector1);
        FrameworkA1::destruir(vector2);
    };
    SECTION("both empty") { check("[]", "[]"); }
    SECTION("one empty") { check("[1,2,3]", "[]"); }
    SECTION("interleaved") { check("[1,3,5]", "[2,4]"); }
}

TEST_CASE("PruebaSubconjuntoVector memory cases", "[PruebaSubconjuntoVector][memory][file:comienzo]")
{
    auto check = [](const char *input1, const char *input2)
    {
        int largo1, largo2;
        int *vector1 = (int *)FrameworkA1::parsearColeccion(input1, largo1);
        int *vector2 = (int *)FrameworkA1::parsearColeccion(input2, largo2);
        checkMemoriaEjecucion([&]
                              { (void)subconjuntoVector(vector1, vector2, largo1, largo2); });
        FrameworkA1::destruir(vector1);
        FrameworkA1::destruir(vector2);
    };
    SECTION("both empty") { check("[]", "[]"); }
    SECTION("subset") { check("[1,2,3]", "[1,2,3,4]"); }
    SECTION("not subset") { check("[1,5]", "[1,2,3]"); }
}

TEST_CASE("PruebaInvertirCase memory cases", "[PruebaInvertirCase][memory][file:comienzo]")
{
    auto check = [](const char *input)
    {
        char *copia = FrameworkA1::copioString(input);
        checkMemoriaEjecucion([&]
                              {
            char *resultado = invertirCase(copia);
            delete[] resultado; });
        delete[] copia;
    };
    SECTION("empty") { check(""); }
    SECTION("lowercase") { check("hola"); }
    SECTION("mixed") { check("Write in C"); }
}

TEST_CASE("PruebaOcurrenciasSubstring memory cases", "[PruebaOcurrenciasSubstring][memory][file:comienzo]")
{
    auto check = [](const char *input, const char *substring)
    {
        int largo;
        char **vector = (char **)FrameworkA1::parsearColeccion(input, largo);
        char *sub = FrameworkA1::copioString(substring);
        checkMemoriaEjecucion([&]
                              { (void)ocurrenciasSubstring(vector, largo, sub); });
        FrameworkA1::destruir(vector, largo);
        delete[] sub;
    };
    SECTION("empty") { check("[]", "a"); }
    SECTION("one match") { check("['hola']", "hola"); }
    SECTION("several") { check("['abc','ab','ab']", "ab"); }
}

TEST_CASE("PruebaOrdenarVecStr memory cases", "[PruebaOrdenarVecStr][memory][file:comienzo]")
{
    auto check = [](const char *input)
    {
        int largo;
        char **vector = (char **)FrameworkA1::parsearColeccion(input, largo);
        checkMemoriaEjecucion([&]
                              {
            char **resultado = ordenarVecStrings(vector, largo);
            FrameworkA1::destruir(resultado, largo); });
        FrameworkA1::destruir(vector, largo);
    };
    SECTION("empty") { check("[]"); }
    SECTION("single") { check("['hola']"); }
    SECTION("unsorted") { check("['paso','pasa','pasado','pasaron']"); }
}

TEST_CASE("PruebaSuma cases", "[PruebaSuma][file:comienzo]")
{
    auto checkSuma = [](int a, int b, int expected)
    {
        CHECK(suma(a, b) == expected);
    };
    SECTION("2 + 7 = 9") { checkSuma(2, 7, 9); }
    SECTION("-8 + 6 = -2") { checkSuma(-8, 6, -2); }
}

TEST_CASE("PruebaTablaDel cases", "[PruebaTablaDel][file:comienzo]")
{
    auto checkTablaDel = [](unsigned int tablaDelN, unsigned int desde, unsigned int hasta, const std::string &expected)
    { checkSalida([=]
                  { tablaDel(tablaDelN, desde, hasta); }, expected); };

    SECTION("7,0,6")
    {
        checkTablaDel(7, 0, 6, "0*7=0;1*7=7;2*7=14;3*7=21;4*7=28;5*7=35;6*7=42");
    }
    SECTION("11,1,5") { checkTablaDel(11, 1, 5, "1*11=11;2*11=22;3*11=33;4*11=44;5*11=55"); }
    SECTION("0,0,0") { checkTablaDel(0, 0, 0, "0*0=0"); }
    SECTION("12,10,15") { checkTablaDel(12, 10, 15, "10*12=120;11*12=132;12*12=144;13*12=156;14*12=168;15*12=180"); }
    SECTION("5,9,9") { checkTablaDel(5, 9, 9, "9*5=45"); }
    SECTION("10,0,1") { checkTablaDel(10, 0, 1, "0*10=0;1*10=10"); }
}

TEST_CASE("PruebaSimplificar cases", "[PruebaSimplificar][file:comienzo]")
{
    auto check = [](int n, int d, const std::string &expected)
    { checkSalida([=]
                  { simplificar(n, d); }, expected); };

    SECTION("2,7") { check(2, 7, "2/7"); }
    SECTION("30,6") { check(30, 6, "5/1"); }
    SECTION("6,30") { check(6, 30, "1/5"); }
    SECTION("1024,32") { check(1024, 32, "32/1"); }
    SECTION("1,1") { check(1, 1, "1/1"); }
    SECTION("-1,1") { check(-1, 1, "-1/1"); }
    SECTION("1,-1") { check(1, -1, "-1/1"); }
    SECTION("-1024,32") { check(-1024, 32, "-32/1"); }
    SECTION("1024,-32") { check(1024, -32, "-32/1"); }
    SECTION("-1024,-32") { check(-1024, -32, "32/1"); }
}

TEST_CASE("PruebaOcurrencias123Repetidos cases", "[PruebaOcurrencias123Repetidos][file:comienzo]")
{
    auto check = [](const char *vecStr, int expected)
    {
        int largo;
        int *vec = (int *)FrameworkA1::parsearColeccion(vecStr, largo);
        int *copia = (int *)FrameworkA1::parsearColeccion(vecStr, largo);
        CHECK(ocurrencias123Repetidos(vec, largo) == expected);
        if (!FrameworkA1::sonIguales(vec, copia, largo))
            FAIL_CHECK("La función modifica el parámetro de entrada");
        FrameworkA1::destruir(vec);
        FrameworkA1::destruir(copia);
    };

    SECTION("[1]") { check("[1]", 0); }
    SECTION("[1,3]") { check("[1,3]", 0); }
    SECTION("[3]") { check("[3]", 0); }
    SECTION("[1,3,1]") { check("[1,3,1]", 0); }
    SECTION("[1,2,3]") { check("[1,2,3]", 1); }
    SECTION("[1,2,2,2,3]") { check("[1,2,2,2,3]", 1); }
    SECTION("[1,1,2,3]") { check("[1,1,2,3]", 1); }
    SECTION("[1,1,2,1,3]") { check("[1,1,2,1,3]", 0); }
    SECTION("[1,2,3,1,2,3,1,2,3,2,3]") { check("[1,2,3,1,2,3,1,2,3,2,3]", 3); }
    SECTION("long-case-1") { check("[1,2,1,2,1,2,1,3,2,3,1,2,3,1,2,2,3,2,3,1,2,3,1,1,2]", 3); }
}

TEST_CASE("PruebaMaximoNumero cases", "[PruebaMaximoNumero][file:comienzo]")
{
    auto check = [](unsigned int n, const char *inputVec, int expected)
    {
        std::istringstream iss(inputVec);
        auto old = std::cin.rdbuf(iss.rdbuf());
        int res = maximoNumero(n);
        std::cin.rdbuf(old);
        CHECK(res == expected);
    };

    SECTION("3:2 4 8") { check(3, "2 4 8", 8); }
    SECTION("2:1 1") { check(2, "1 1", 1); }
    SECTION("6:mixed") { check(6, "2 2 8 5 6 7", 8); }
    SECTION("1:single") { check(1, "2", 2); }
    SECTION("10:big") { check(10, "2 11 20 90 465 45 34 767 32 1", 767); }
    SECTION("4:neg") { check(4, "-17 89 12 11", 89); }
    SECTION("3:negatives") { check(3, "-28923 -2229 -1122", -1122); }
}

TEST_CASE("PruebaOrdenarVecInt cases", "[PruebaOrdenarVecInt][file:comienzo]")
{
    auto check = [](const char *vecStr, const std::string &expected)
    { checkVectorIntModificado(ordenarVecInt, vecStr, expected); };

    SECTION("empty") { check("[]", "[]"); }
    SECTION("single") { check("[5]", "[5]"); }
    SECTION("sorted") { check("[1,2,3,4,5]", "[1,2,3,4,5]"); }
    SECTION("unsorted-small") { check("[9,5,1]", "[1,5,9]"); }
    SECTION("duplicates") { check("[1,6,4,9,2,4,9,1]", "[1,1,2,4,4,6,9,9]"); }
    SECTION("many-duplicates") { check("[9,9,5,5,5,1,1]", "[1,1,5,5,5,9,9]"); }
}

TEST_CASE("PruebaIntercalarVector cases", "[PruebaIntercalarVector][file:comienzo]")
{
    auto check = [](const char *v1s, const char *v2s, const std::string &expected)
    {
        int l1 = 0;
        int *v1 = (int *)FrameworkA1::parsearColeccion(v1s, l1);
        int l2 = 0;
        int *v2 = (int *)FrameworkA1::parsearColeccion(v2s, l2);
        int lExp = 0;
        int *exp = (int *)FrameworkA1::parsearColeccion(expected.c_str(), lExp);
        int *res = intercalarVector(v1, v2, l1, l2);
        bool ok = FrameworkA1::sonIguales(res, exp, lExp);
        checkResultadoSerializado(ok, res, expected,
                                  [largo = l1 + l2](int *resultado)
                                  { return FrameworkA1::serializar(resultado, largo); });

        FrameworkA1::destruir(v1);
        FrameworkA1::destruir(v2);
        FrameworkA1::destruir(exp);
        if (res)
            FrameworkA1::destruir(res);
    };

    SECTION("inter1") { check("[1,2,3]", "[1,2,3]", "[1,1,2,2,3,3]"); }
    SECTION("inter2") { check("[1,3,5]", "[2,4]", "[1,2,3,4,5]"); }
    SECTION("inter3") { check("[1,100]", "[2,53]", "[1,2,53,100]"); }
    SECTION("inter-empty") { check("[]", "[]", "[]"); }
    SECTION("inter-single") { check("[2]", "[4]", "[2,4]"); }
    SECTION("inter-append") { check("[1,2,3,4,5]", "[6,7,8]", "[1,2,3,4,5,6,7,8]"); }
    SECTION("inter-reverse") { check("[6,7,8]", "[1,2,3,4,5]", "[1,2,3,4,5,6,7,8]"); }
    SECTION("inter-left-empty") { check("[]", "[1,2,3]", "[1,2,3]"); }
    SECTION("inter-right-empty") { check("[1,2,3]", "[]", "[1,2,3]"); }
}

TEST_CASE("PruebaIntercalarVector order", "[PruebaIntercalarVector][file:comienzo]")
{
    auto benchmark = []()
    {
        ankerl::nanobench::Bench bench;
        bench.output(nullptr);                             // que no imprima nada
        bench.minEpochTime(std::chrono::milliseconds(10)); // mejoramos la precisión de la medición
        std::vector<int> arr1;
        std::vector<int> arr2;

        for (uint64_t n = 100; n <= 5'000; n += 200)
        {
            // Set up
            arr1.clear();
            arr1.resize(n);
            arr2.clear();
            arr2.resize(n);
            for (int i = 0; i < n; i++)
            {
                arr1[i] = i;
                arr2[i] = i;
            }

            // Benchmark
            bench.complexityN(arr1.size()).run("IntercalarVector", [&]
                                               {
            auto ret = intercalarVector(arr1.data(), arr2.data(), arr1.size(), arr2.size());
            ankerl::nanobench::doNotOptimizeAway(ret);
            delete[] ret; });
        }
        return bench;
    };

    auto bench = benchmark();
    auto bigO = bench.complexityBigO()[0];

    REQUIRE(bigO.name() == "O(n)");
}

TEST_CASE("PruebaSubconjuntoVector cases", "[PruebaSubconjuntoVector][file:comienzo]")
{
    auto check = [](const char *v1s, const char *v2s, bool expected)
    {
        int l1 = 0;
        int *v1 = (int *)FrameworkA1::parsearColeccion(v1s, l1);
        int *v1c = (int *)FrameworkA1::parsearColeccion(v1s, l1);
        int l2 = 0;
        int *v2 = (int *)FrameworkA1::parsearColeccion(v2s, l2);
        int *v2c = (int *)FrameworkA1::parsearColeccion(v2s, l2);

        CHECK(subconjuntoVector(v1, v2, l1, l2) == expected);
        if (!FrameworkA1::sonIguales(v1, v1c, l1) || !FrameworkA1::sonIguales(v2, v2c, l2))
            FAIL_CHECK("La función modifica los parámetros de entrada");
        FrameworkA1::destruir(v1);
        FrameworkA1::destruir(v1c);
        FrameworkA1::destruir(v2);
        FrameworkA1::destruir(v2c);
    };

    SECTION("sub2") { check("[1,2,3]", "[1,2,3,4,5,6]", true); }
    SECTION("sub1") { check("[100,400,200,300,300,400,400]", "[100,400,200]", false); }
    SECTION("sub3") { check("[2,2,2,3,54,6,7]", "[1,2,3,4]", false); }
    SECTION("sub4") { check("[2,2,2,3,54,6,7]", "[7,6,54,6,3,2]", true); }
    SECTION("sub5") { check("[1,1,1,4,2]", "[1,2,5]", false); }
    SECTION("sub-empty") { check("[]", "[]", true); }
    SECTION("sub-empty-left") { check("[]", "[2,3,1]", true); }
    SECTION("sub-empty-right") { check("[2,3,4]", "[]", false); }
    SECTION("sub-single") { check("[1]", "[1]", true); }
}

TEST_CASE("PruebaInvertirCase cases", "[PruebaInvertirCase][file:comienzo]")
{
    auto check = [](const char *in, const char *expected)
    {
        char *copia = FrameworkA1::copioString(in);

        char *res = invertirCase(copia);
        bool ok = FrameworkA1::sonIguales(res, expected);
        if (!ok)
        {
            FAIL_CHECK("Esperado: \"" << expected << "\" -- Recibido: \"" << (res ? res : "NULL") << "\"");
        }
        else
            CHECK(true);
        delete[] copia;
        if (res)
            delete[] res;
    };

    SECTION("empty") { check("", ""); }
    SECTION("hola") { check("hola", "HOLA"); }
    SECTION("write") { check("Write in C", "wRITE IN c"); }
    SECTION("mixed") { check("es UNA cosa DE locos", "ES una COSA de LOCOS"); }
    SECTION("weird") { check("EsCrIbIR asi Es MoLeSTOOO", "eScRiBir ASI eS mOlEstooo"); }
    SECTION("digits") { check("1434", "1434"); }
    SECTION("symbols") { check("#&/()|A", "#&/()|a"); }
}

TEST_CASE("PruebaOcurrenciasSubstring cases", "[PruebaOcurrenciasSubstring][file:comienzo]")
{
    auto check = [](const char *vecStr, const char *substr, int expected)
    {
        int largo;
        char **vec = (char **)FrameworkA1::parsearColeccion(vecStr, largo);
        char **copia = (char **)FrameworkA1::parsearColeccion(vecStr, largo);
        char *subc = FrameworkA1::copioString(substr);
        char *subc2 = FrameworkA1::copioString(substr);

        unsigned int res = ocurrenciasSubstring(vec, largo, subc2);
        CHECK((int)res == expected);
        if (!FrameworkA1::sonIguales(vec, copia, largo) || strcmp(subc, subc2) != 0)
            FAIL_CHECK("La función modifica los parámetros de entrada");
        FrameworkA1::destruir(vec, largo);
        FrameworkA1::destruir(copia, largo);
        delete[] subc;
        delete[] subc2;
    };

    SECTION("['hola']") { check("['hola']", "hola", 1); };
    SECTION("['abc','ab','ab']") { check("['abc','ab','ab']", "ab", 3); };
    SECTION("['holaho','holahol','holahola','holaholahola','holaholaholaho']") { check("['holaho','holahol','holahola','holaholahola','holaholaholaho']", "holahola", 3); };
    SECTION("['abcjsdf','sagsdfg','afgadfg','afgadfg12','wertewrt','tyafgadfgsegfdxv','jo']") { check("['abcjsdf','sagsdfg','afgadfg','afgadfg12','wertewrt','tyafgadfgsegfdxv','jo']", "afgadfg", 3); };
    SECTION("['solsol']") { check("['solsol']", "sol", 1); };
    SECTION("['ababac','aabaca','abaabaca']") { check("['ababac','aabaca','abaabaca']", "abac", 3); };
}

TEST_CASE("PruebaOrdenarVecStr cases", "[PruebaOrdenarVecStr][file:comienzo]")
{
    auto check = [](const char *vecStr, const char *expectedStr)
    {
        int l;
        char **vec = (char **)FrameworkA1::parsearColeccion(vecStr, l);
        char **copia = (char **)FrameworkA1::parsearColeccion(vecStr, l);
        int le;
        char **exp = (char **)FrameworkA1::parsearColeccion(expectedStr, le);

        char **res = ordenarVecStrings(vec, l);
        bool ok = FrameworkA1::sonIguales(res, exp, le);
        checkResultadoSerializado(ok, res, expectedStr,
                                  [l](char **resultado)
                                  { return FrameworkA1::serializar(resultado, l); });
        FrameworkA1::destruir(vec, l);
        FrameworkA1::destruir(copia, l);
        FrameworkA1::destruir(exp, le);
        if (res)
            FrameworkA1::destruir(res, l);
    };

    SECTION("[]") { check("[]", "[]"); }
    SECTION("['paso','pasa','pasado','pasaron']") { check("['paso','pasa','pasado','pasaron']", "['pasa','pasado','pasaron','paso']"); }
    SECTION("['hola']") { check("['hola']", "['hola']"); }
    SECTION("['string','strings','substring','strong','strength']") { check("['string','strings','substring','strong','strength']", "['strength', 'string', 'strings', 'strong','substring']"); }
    SECTION("['1234','15324567','1563248','5132135']") { check("['1234','15324567','1563248','5132135']", "['1234', '15324567', '1563248', '5132135']"); }
    SECTION("['153243','1532468','153246','15324679']") { check("['153243','1532468','153246','15324679']", "['153243', '153246', '15324679', '1532468']"); }
    SECTION("['algoritmos','algoritmia','algoritmo','algo']") { check("['algoritmos','algoritmia','algoritmo','algo']", "['algo', 'algoritmia', 'algoritmo', 'algoritmos']"); }
    SECTION("['123456','123546','123645','123598','123754','12365498','1232','12','123','12911']") { check("['123456','123546','123645','123598','123754','12365498','1232','12','123','12911']", "['12','123', '1232', '123456', '123546','123598', '123645', '12365498', '123754', '12911']"); }
}

TEST_CASE("PruebaSplitStr cases", "[PruebaSplitStr][file:comienzo]")
{
    auto check = [](const char *in, char sep, const char *expected)
    {
        char *inCopy = FrameworkA1::copioString(in);
        int le;
        char **exp = (char **)FrameworkA1::parsearColeccion(expected, le);
        int lo;
        char **got = splitStr(inCopy, sep, lo);
        bool ok = (le == lo) && FrameworkA1::sonIguales(got, exp, le);
        checkResultadoSerializado(ok, got, expected,
                                  [lo](char **resultado)
                                  { return FrameworkA1::serializar(resultado, lo); });
        FrameworkA1::destruir(got, lo);
        FrameworkA1::destruir(exp, le);
        delete[] inCopy;
    };

    SECTION("") { check("", '@', "[]"); }
    SECTION(" ") { check(" ", '@', "[' ']"); }
    SECTION("@") { check("@", '@', "[]"); }
    SECTION("@@ @@ @@ @@ @@") { check("@@ @@ @@ @@ @@", '@', "[' ', ' ', ' ', ' ']"); }
    SECTION("A@B") { check("A@B", '@', "['A','B']"); }
    SECTION("A@B@C") { check("A@B@C", '@', "['A','B', 'C']"); }
    SECTION("A@") { check("A@", '@', "['A']"); }
    SECTION("@B") { check("@B", '@', "['B']"); }
    SECTION("@@@") { check("@@@", '@', "[]"); }
    SECTION("@@@A") { check("@@@A", '@', "['A']"); }
    SECTION("A@@@B") { check("A@@@B", '@', "['A','B']"); }
    SECTION("The-Lannisters-send-their-regards") { check("The-Lannisters-send-their-regards", '-', "['The','Lannisters','send','their','regards']"); }
    SECTION("Hola@Mundo") { check("Hola@Mundo", '-', "['Hola@Mundo']"); }
}

TEST_CASE("PruebaOrdenarVecIntMergeSort cases", "[PruebaOrdenarVecIntMergeSort][file:comienzo]")
{
    auto check = [](const char *in, const char *expected)
    { checkVectorIntModificado(ordenarVecIntMergeSort, in, expected); };

    SECTION("[]") { check("[]", "[]"); }
    SECTION("[4]") { check("[4]", "[4]"); }
    SECTION("[1,1,1]") { check("[1,1,1]", "[1,1,1]"); }
    SECTION("[1,2,3]") { check("[1,2,3]", "[1,2,3]"); }
    SECTION("[1,4,2]") { check("[1,4,2]", "[1,2,4]"); }
    SECTION("[2,3,1]") { check("[2,3,1]", "[1,2,3]"); }
    SECTION("[-2,-3,-1]") { check("[-2,-3,-1]", "[-3,-2,-1]"); }
    SECTION("[1,1,4,1,3,8]") { check("[1,1,4,1,3,8]", "[1,1,1,3,4,8]"); }
    SECTION("[-2,3,1]") { check("[-2,3,1]", "[-2,1,3]"); }
    SECTION("[3,5,2,1,0]") { check("[3,5,2,1,0]", "[0,1,2,3,5]"); }
    SECTION("[9,2,2,5,1]") { check("[9,2,2,5,1]", "[1,2,2,5,9]"); }
    SECTION("[3,1,-1,1,0]") { check("[3,1,-1,1,0]", "[-1,0,1,1,3]"); }
    SECTION("[10,9,8,7,6,5,4,3,2,1,0,-1,-2]") { check("[10,9,8,7,6,5,4,3,2,1,0,-1,-2]", "[-2,-1,0,1,2,3,4,5,6,7,8,9,10]"); }
    SECTION("[7,3,7,10,-1,1,-6,0,-10,2,1,2]") { check("[7,3,7,10,-1,1,-6,0,-10,2,1,2]", "[-10,-6,-1,0,1,1,2,2,3,7,7,10]"); }
    SECTION("[10,9,8,7,6,5,4,3,2,1,0,-1,-2]") { check("[10,9,8,7,6,5,4,3,2,1,0,-1,-2]", "[-2,-1,0,1,2,3,4,5,6,7,8,9,10]"); }
    SECTION("[8,7,5,2,-3,-1]") { check("[8,7,5,2,-3,-1]", "[-3,-1,2,5,7,8]"); }
    SECTION("[1,2,0,10,3,4]") { check("[1,2,0,10,3,4]", "[0,1,2,3,4,10]"); }
    SECTION("[1,2,4,3]") { check("[1,2,4,3]", "[1,2,3,4]"); }
    SECTION("[-2,0,3,1,1]") { check("[-2,0,3,1,1]", "[-2,0,1,1,3]"); }
}

TEST_CASE("PruebaOrdenarVecIntMergeSort order", "[PruebaOrdenarVecIntMergeSort][file:comienzo]")
{
    auto benchmark = []()
    {
        ankerl::nanobench::Bench bench;
        bench.output(nullptr);                             // que no imprima nada
        bench.minEpochTime(std::chrono::milliseconds(10)); // mejoramos la precisión de la medición
        std::vector<int> arr;

        for (uint64_t n = 100; n <= 5'000; n += 200)
        {
            // Set up
            arr.clear();
            arr.resize(n);
            for (int i = 0; i < n; i++)
                arr[i] = n - i;

            // Benchmark
            bench.complexityN(arr.size()).run("OrdenarVecIntMergeSort", [&]
                                              {
            ordenarVecIntMergeSort(arr.data(), arr.size());
            ankerl::nanobench::doNotOptimizeAway(arr.data()); });
        }
        return bench;
    };

    auto bench = benchmark();
    auto bigO = bench.complexityBigO()[0];

    REQUIRE((bigO.name() == "O(n log n)" || bigO.name() == "O(n)"));
}
