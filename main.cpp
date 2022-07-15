#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "CommandsTask1.h"
#include "CommandsTask2.h"
#include "AdvisorBot.h"
#include "config.h"

int main() {
    std::string dataPath = "./20200317.csv";
    AdvisorBot advisorBot(dataPath, PREDICTION_WINDOW_SIZE, VISUALIZE_N, EPSILON);
    advisorBot.addCommand(new HelpCommand());
    advisorBot.addCommand(new HelpCmdCommand());
    advisorBot.addCommand(new ProdCommand());
    advisorBot.addCommand(new MinCommand());
    advisorBot.addCommand(new MaxCommand());
    advisorBot.addCommand(new AvgCommand());
    advisorBot.addCommand(new PredictCommand());
    advisorBot.addCommand(new TimeCommand());
    advisorBot.addCommand(new StepCommand());
    advisorBot.addCommand(new ExitCommand());
    advisorBot.addCommand(new SpreadCommand());
    advisorBot.addCommand(new VisualizeCommand());
    advisorBot.addCommand(new PrintCommand());

    advisorBot.run();
}