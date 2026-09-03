#include "catch_amalgamated.hpp"

int main(int argc, char *argv[])
{
    Catch::Session session;
    session.configData().runOrder = Catch::TestRunOrder::Declared;

    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0)
        return returnCode;

    return session.run();
}
