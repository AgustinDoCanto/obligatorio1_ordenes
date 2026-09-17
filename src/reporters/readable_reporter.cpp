#include "catch_amalgamated.hpp"

#include <cstdint>
#include <iomanip>
#include <string>
#include <vector>

namespace {

struct FailureDetail {
    std::string location;
    std::string sectionPath;
    std::string expression;
    std::string expandedExpression;
    std::string message;
    bool hasExpression = false;
    bool hasExpandedExpression = false;
    bool hasMessage = false;
};

class ReadableReporter final : public Catch::StreamingReporterBase {
public:
    explicit ReadableReporter(Catch::ReporterConfig&& config)
        : StreamingReporterBase(CATCH_MOVE(config)) {
        m_preferences.shouldRedirectStdOut = true;
    }

    static std::string getDescription() {
        return "Prints a human-readable summary of test cases and sections";
    }

    void testRunStarting(Catch::TestRunInfo const& testRunInfo) override {
        StreamingReporterBase::testRunStarting(testRunInfo);
        {
            auto guard = m_colour->guardColour(Catch::Colour::Headers).engage(m_stream);
            m_stream << "Pruebas del obligatorio\n";
        }
        m_stream << "=======================\n";
    }

    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override {
        StreamingReporterBase::testCaseStarting(testInfo);
        if (!m_isFirstTestCase) {
            m_stream << "\n";
        }
        m_isFirstTestCase = false;
        m_stream << testInfo.name << "\n";
    }

    void assertionEnded(Catch::AssertionStats const& assertionStats) override {
        Catch::AssertionResult const& result = assertionStats.assertionResult;
        if (result.succeeded()) {
            return;
        }

        FailureDetail detail;
        detail.location = std::string(result.getSourceInfo().file) + ":" +
                           std::to_string(result.getSourceInfo().line);

        // Catch2 no registra el nombre de la funcion C++ donde ocurre el
        // assert; lo mas cercano es la cadena de SECTION anidadas (la
        // primera, de profundidad 1, coincide con el nombre del TEST_CASE).
        for (std::size_t i = 1; i < m_sectionStack.size(); ++i) {
            if (!detail.sectionPath.empty()) {
                detail.sectionPath += " > ";
            }
            detail.sectionPath += m_sectionStack[i].name;
        }

        // Las excepciones no capturadas (p. ej. NoImplementado) generan una
        // assertion sintetica sin macro real ni expresion utilizable.
        const bool isRealAssertion = !result.getTestMacroName().empty();
        detail.hasExpression = result.hasExpression() && isRealAssertion;
        if (detail.hasExpression) {
            detail.expression = result.getExpression();
            detail.hasExpandedExpression = result.hasExpandedExpression();
            if (detail.hasExpandedExpression) {
                detail.expandedExpression = result.getExpandedExpression();
            }
        }

        detail.hasMessage = result.hasMessage();
        if (detail.hasMessage) {
            detail.message = static_cast<std::string>(result.getMessage());
        }

        m_pendingFailures.push_back(CATCH_MOVE(detail));
    }

    void sectionEnded(Catch::SectionStats const& sectionStats) override {
        // Catch2 abre una seccion raiz implicita con el nombre del TEST_CASE
        // (profundidad 1); el primer SECTION real queda en profundidad 2.
        // Solo reportamos ese nivel para no duplicar salida con eventuales
        // SECTION anidadas mas profundas.
        if (m_sectionStack.size() == 2) {
            const bool ok = sectionStats.assertions.failed == 0;
            m_stream << "  [";
            {
                auto guard = m_colour
                                 ->guardColour(ok ? Catch::Colour::ResultSuccess
                                                   : Catch::Colour::ResultError)
                                 .engage(m_stream);
                m_stream << (ok ? "OK" : "FAIL");
            }
            m_stream << "] " << sectionStats.sectionInfo.name << "\n";
            flushPendingFailures();
        }

        StreamingReporterBase::sectionEnded(sectionStats);
    }

    void testCaseEnded(Catch::TestCaseStats const& testCaseStats) override {
        // Cubre asserts hechos fuera de cualquier SECTION.
        flushPendingFailures();
        StreamingReporterBase::testCaseEnded(testCaseStats);
    }

    void testRunEnded(Catch::TestRunStats const& testRunStats) override {
        Catch::Totals const& totals = testRunStats.totals;
        const auto testCasesTotal = totals.testCases.total();
        const double testCasesPercentage = testCasesTotal == 0
            ? 0.0
            : 100.0 * static_cast<double>(totals.testCases.passed) /
                  static_cast<double>(testCasesTotal);

        m_stream << "\n";
        {
            auto guard = m_colour->guardColour(Catch::Colour::Headers).engage(m_stream);
            m_stream << "Resumen\n";
        }
        m_stream << "=======\n";

        m_stream << "Tests: ";
        printColoured(Catch::Colour::ResultSuccess, totals.testCases.passed);
        m_stream << " OK, ";
        printColoured(Catch::Colour::ResultError, totals.testCases.failed);
        m_stream << " fallidos, ";
        printColoured(Catch::Colour::Skip, totals.testCases.skipped);
        m_stream << " omitidos, " << testCasesTotal << " total ("
                  << std::fixed << std::setprecision(2) << testCasesPercentage << "%)\n";

        m_stream << "Checks: ";
        printColoured(Catch::Colour::ResultSuccess, totals.assertions.passed);
        m_stream << " OK, ";
        printColoured(Catch::Colour::ResultError, totals.assertions.failed);
        m_stream << " fallidos, ";
        printColoured(Catch::Colour::Skip, totals.assertions.skipped);
        m_stream << " omitidos, " << totals.assertions.total() << " total\n";

        StreamingReporterBase::testRunEnded(testRunStats);
    }

private:
    void printColoured(Catch::Colour::Code code, std::uint64_t value) {
        auto guard = m_colour->guardColour(code).engage(m_stream);
        m_stream << value;
    }

    void flushPendingFailures() {
        for (auto const& failure : m_pendingFailures) {
            m_stream << "    Fallo en ";
            {
                auto guard = m_colour->guardColour(Catch::Colour::FileName).engage(m_stream);
                m_stream << failure.location;
            }
            m_stream << "\n";

            if (!failure.sectionPath.empty()) {
                m_stream << "    Seccion: ";
                {
                    auto guard = m_colour->guardColour(Catch::Colour::FileName).engage(m_stream);
                    m_stream << failure.sectionPath;
                }
                m_stream << "\n";
            }

            if (failure.hasExpression) {
                m_stream << "    Expresion: ";
                {
                    auto guard = m_colour->guardColour(Catch::Colour::OriginalExpression)
                                     .engage(m_stream);
                    m_stream << failure.expression;
                }
                m_stream << "\n";

                if (failure.hasExpandedExpression) {
                    m_stream << "    Valores: ";
                    {
                        auto guard = m_colour->guardColour(Catch::Colour::ReconstructedExpression)
                                         .engage(m_stream);
                        m_stream << failure.expandedExpression;
                    }
                    m_stream << "\n";
                }
            }

            if (failure.hasMessage) {
                m_stream << "    Mensaje: ";
                {
                    auto guard = m_colour->guardColour(Catch::Colour::Warning).engage(m_stream);
                    m_stream << failure.message;
                }
                m_stream << "\n";
            }
        }
        m_pendingFailures.clear();
    }

    std::vector<FailureDetail> m_pendingFailures;
    bool m_isFirstTestCase = true;
};

} // namespace

CATCH_REGISTER_REPORTER("readable", ReadableReporter)
