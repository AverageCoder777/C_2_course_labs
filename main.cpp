#include "Base_realization.cpp"

int main()
{
    try
    {
        Game game("Hero");
        game.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
