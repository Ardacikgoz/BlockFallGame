#include "GameController.h"
#include "BlockFall.h"
#include <iostream>
#include <fstream>

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"


std::vector<std::vector<bool>> rightRotaion(std::vector<std::vector<bool>> shape)
{

    std::size_t numRows = shape.size();
    std::size_t numCols = shape[0].size();
    std::vector<std::vector<bool>> rotatedMatrix(numCols, std::vector<bool>(numRows));
    for (std::size_t i = 0; i < numRows; ++i) {
        for (std::size_t j = 0; j < numCols; ++j) {
            rotatedMatrix[j][numRows - 1 - i] = shape[i][j];
        }
    }
    return rotatedMatrix;
}
std::vector<std::vector<bool>> leftRotation(std::vector<std::vector<bool>> shape) {
    std::vector<std::vector<bool>> rotatedMatrix(shape[0].size(), std::vector<bool>(shape.size()));

    for (std::size_t i = 0; i < shape.size(); ++i) {
        for (std::size_t j = 0; j < shape[i].size(); ++j) {
            rotatedMatrix[shape[i].size() - 1 - j][i] = shape[i][j];
        }
    }
    return rotatedMatrix;
}

//thanks to emirhan
std::vector<std::string> readLinesFromFile(const std::string& fileName) {
    std::vector<std::string> lines;
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << fileName << std::endl;
        std::exit(-1);
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

bool control(BlockFall &game, std::vector<std::vector<int>> positions,std::string sentence, std::vector<std::vector<int>> &tempgrid)
{
    if (sentence == "MOVE_RIGHT")
    {
        for(auto & k : positions)
        {
            int i = k[0];
            int j = k[1];
            if(game.grid[i][j+1 + game.active_rotation->moveRight - game.active_rotation->moveLeft] == 1)

                return false;
        }
    }
    else if(sentence == "MOVE_LEFT")
    {
        for(auto & k : positions)
        {
            int i = k[0];
            int j = k[1];
            if(game.grid[i][j-1 + game.active_rotation->moveRight - game.active_rotation->moveLeft] == 1)
                return false;
        }
    }
    else if (sentence == "ROTATE_RIGHT")
    {
        std::vector<std::vector<bool>>controlShape = rightRotaion(game.active_rotation->shape);
        std::vector<std::vector<int>> controlPos;
        for(int i =0 ; i < controlShape.size(); i++)
        {
            std::vector<int> pushVector;
            for(int j =0 ; j < controlShape[0].size(); j++)
            {
                int row = i;
                int col = j + game.active_rotation->moveRight - game.active_rotation->moveLeft;
                pushVector.push_back(row);
                pushVector.push_back(col);
                controlPos.push_back(pushVector);
            }
        }


        for(auto & k : controlPos)
        {
            int i = k[0];
            int j = k[1];
            if(game.grid[i][j] == 1)
                return false;
        }

    }
    else if (sentence == "ROTATE_LEFT")
    {
        std::vector<std::vector<bool>> controlShape = leftRotation(game.active_rotation->shape);
        std::vector<std::vector<int>> controlPos;
        for(int i =0 ; i < controlShape.size(); i++)
        {
            std::vector<int> pushVector;
            for(int j =0 ; j < controlShape[0].size(); j++)
            {
                int row = i;
                int col = j + game.active_rotation->moveRight - game.active_rotation->moveLeft;
                pushVector.push_back(i);
                pushVector.push_back(j);
                controlPos.push_back(pushVector);
            }
        }

        for(auto &k : controlPos)
        {
            int i = k[0];
            int j = k[1];
            if(game.grid[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

void putMyBlock(BlockFall &game,std::vector<std::vector<int>> &tempGrid)
{
    for(int i = 0; i < game.active_rotation->shape.size(); i++)
    {
        for(int j = 0 ;j < game.active_rotation->shape[0].size(); j++)
        {
            tempGrid[i][j] = game.active_rotation->shape[i][j];
        }
    }
}

void moveRight(BlockFall& game , std::vector<std::vector<int>> &tempGrid)
{
    if(control(game,game.active_rotation->positions,"MOVE_RIGHT",tempGrid))
    {
        for(int i = 0; i < game.active_rotation->shape.size(); i++)
        {
            for(int j =0 ; j < game.active_rotation->shape[0].size(); j++)
            {
                tempGrid[i][j + game.active_rotation->moveRight - game.active_rotation->moveLeft] = 0;
            }
            for(int j =0 ; j < game.active_rotation->shape[0].size(); j++)
            {
                tempGrid[i][j + 1 + game.active_rotation->moveRight - game.active_rotation->moveLeft] = game.active_rotation->shape[i][j];
            }
        }
    }
    for(auto &k: game.active_rotation->positions)
    {
        k[1]++;
    }

    game.active_rotation->moveRight++;

}

void moveLeft(BlockFall &game,std::vector<std::vector<int>> &tempGrid)
{

    if(control(game,game.active_rotation->positions,"MOVE_LEFT",tempGrid))
    {
        for(int i = 0; i < game.active_rotation->shape.size(); i++)
        {
            for(int j = 0; j < game.active_rotation->shape[0].size(); j++)
            {
                tempGrid[i][j + game.active_rotation->moveRight - game.active_rotation->moveLeft] = 0;
            }
            for(int j = 0; j < game.active_rotation->shape[0].size(); j++)
            {
                tempGrid[i][j - 1 + game.active_rotation->moveRight - game.active_rotation->moveLeft] = game.active_rotation->shape[i][j];
            }
        }
    }
    for(auto &k : game.active_rotation->positions)
    {
        k[1]--;
    }
    game.active_rotation->moveLeft++;
}

void rotateRight(BlockFall &game ,std::vector<std::vector<int>> &tempGrid)
{
    if(control(game,game.active_rotation->positions,"ROTATE_RIGHT",tempGrid))
    {
        for(auto &k : game.active_rotation->positions)
        {
            int i = k[0];
            int j = k[1];
            tempGrid[i][j] = 0;
        }
        game.active_rotation->right_rotation->moveRight = game.active_rotation->moveRight;
        game.active_rotation->right_rotation->moveLeft = game.active_rotation->moveLeft;
        game.active_rotation = game.active_rotation->right_rotation;

        std::vector<std::vector<int>> positions;

        for(int i =0 ; i < game.active_rotation->shape.size(); i++)
        {
            for(int j =0 ; j < game.active_rotation->shape[0].size(); j++)
            {
                std::vector<int> nowPos;
                if(game.active_rotation->shape[i][j] == 1)
                {
                    int row = i;
                    int col = j + game.active_rotation->moveRight - game.active_rotation->moveLeft;
                    nowPos.push_back(row);
                    nowPos.push_back(col);
                    positions.push_back(nowPos);
                }
            }
        }

        game.active_rotation->positions = positions;

        for(auto k : positions)
        {
            int i = k[0];
            int j = k[1];
            tempGrid[i][j] = 1;
        }
    }
}

void rotateLeft(BlockFall &game , std::vector<std::vector<int>> &tempGrid)
{
    if(control(game,game.active_rotation->positions,"ROTATE_LEFT",tempGrid))
    {
        for(auto &k : game.active_rotation->positions)
        {
            int i = k[0];
            int j = k[1];
            tempGrid[i][j] = 0;
        }
        game.active_rotation->next_block->moveRight = game.active_rotation->moveRight;
        game.active_rotation->next_block->moveLeft = game.active_rotation->moveLeft;
        game.active_rotation = game.active_rotation->left_rotation;

        std::vector<std::vector<int>> positions;

        for(int i =0 ; i < game.active_rotation->shape.size(); i++)
        {
            for(int j =0; j < game.active_rotation->shape[0].size(); j++)
            {
                std::vector<int> nowPos;
                int row = i;
                int col = j + game.active_rotation->moveRight - game.active_rotation->moveLeft;
                nowPos.push_back(row);
                nowPos.push_back(col);
                positions.push_back(nowPos);
            }
        }

        game.active_rotation->positions = positions;

        for(auto &k : positions)
        {
            int i = k[0];
            int j = k[1];
            tempGrid[i][j] = 1;
        }

    }
}




void moveDown(BlockFall &game, std::vector<std::vector<int>> &tempGrid)
{
    for(auto &k : game.active_rotation->positions)
    {
        int i = k[0];
        int j = k[1];
        tempGrid[i][j] = 0;
    }
    for(auto &k : game.active_rotation->positions)
    {
        int i = k[0] + 1;
        int j = k[1];
        k[0] += 1;
        tempGrid[i][j] = 1;
    }
}

void createBlockOnGrid(BlockFall &game)
{
    std::vector<std::vector<int>> positions;
    for(int i =0 ; i < game.active_rotation->shape.size(); i++)
    {
        for(int j =0 ; j < game.active_rotation->shape[0].size() ; j++)
        {
            std::vector<int> nowPosition;
            if(game.active_rotation->shape[i][j] == true)
            {
                nowPosition.push_back(i);
                nowPosition.push_back(j + game.active_rotation->moveRight - game.active_rotation->moveLeft);
                positions.push_back(nowPosition);
            }
        }
    }
    game.active_rotation->positions = positions;
}

void printGrid(BlockFall &game,std::vector<std::vector<int>> &tempGrid)
{
    std::cout<<"Score: "<<game.current_score<<std::endl;
    if(game.leaderboard.head_leaderboard_entry == nullptr)
    {
        std::cout<<"High Score: "<<std::endl;
    }
    else{
        std::cout<<game.leaderboard.head_leaderboard_entry->score;
    }

    for(int i = 0; i < game.grid.size(); i++)
    {
        for(int j = 0 ; j < game.grid[0].size(); j++)
        {
            if(tempGrid[i][j] == 1)
            {
                std::cout<<occupiedCellChar;
            }
            else{
                std::cout<<unoccupiedCellChar;
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void beforeClearingGrid(BlockFall &game , std::vector<std::vector<int>>&tempGrid)
{
    std::cout<<"Before clearing:"<<std::endl;
    for(int i = 0; i < game.rows; i++)
    {
        for(int j =0 ; j < game.cols; j++)
        {
            if(game.grid[i][j] ==1)
            {
                std::cout<<occupiedCellChar;
            }else
                std::cout<<unoccupiedCellChar;
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void drop(BlockFall &game, std::vector<std::vector<int>> &tempGrid)
{
    int step = 0;
    int maxstep = game.grid.size() - game.active_rotation->shape.size();
    bool check = false;
    while (step < maxstep) {
        int counter = 0;
        for (int k = 0; k < game.active_rotation->positions.size(); k++) {
            int i = game.active_rotation->positions[k][0];
            int j = game.active_rotation->positions[k][1];

            if (game.grid[i + 1][j] != 1) {
                counter++;
            }
            else {
                check = true;
            }
        }
        if(check)
        {
            break;
        }
        if (game.active_rotation->positions.size() == counter) {
            moveDown(game, tempGrid);
        }
        step++;
    }
    game.current_score += (step)*game.active_rotation->positions.size();
    if(game.gravity_mode_on)
    {
        for(int i = game.active_rotation->positions.size() ; i > 0 ; i--)
        {
            int row = game.active_rotation->positions[i-1][0];
            int col = game.active_rotation->positions[i-1][1];
            int counter = 0;
            for(int z = game.grid.size(); z - i > 0; z--)
            {
                if(row!= tempGrid.size() -1  && tempGrid[row + 1][col] == 0)
                {
                    tempGrid[row][col] = 0;
                    tempGrid[row+1][col] = 1;
                    row++;
                    game.active_rotation->positions[i-1][0]++;
                }
                else{
                    break;
                }
            }

        }
    }
    game.grid = tempGrid;
    game.active_rotation = game.active_rotation->next_block;



}
bool checkForCreation(BlockFall &game,std::vector<std::vector<int>> &tempGrid)
{
    if(game.active_rotation->positions.size() == 0)
    {
        createBlockOnGrid(game);
    }
    for(int i = 0; i < game.active_rotation->shape.size() ;i++)
    {
        for(int j = 0 ; j < game.active_rotation->shape[0].size(); j++)
        {
            if(game.grid[i][j] == 1 && game.active_rotation->shape[i][j] == 1)
            {
                return true;
            }
        }
    }
    //putMyBlock(game,tempGrid);
    return false;
}


void biAndaDropAcildi(BlockFall &game, std::vector<std::vector<int>> &tempGrid)
{
    if(game.gravity_mode_on)
    {
        for(int i = game.rows - 2; i > 0; i--)
        {
            for(int j = 0; j < game.cols; j++)
            {
                if(game.grid[i][j] == 1 && game.grid[i+1][j] == 0)
                {
                    int counter = 0;
                    while(i+1+counter < game.rows && game.grid[i+1+counter][j] == 0)
                    {
                        counter++;
                    }
                    game.grid[i][j] = 0;
                    game.grid[i+counter][j] = 1;
                }
            }
        }
        tempGrid = game.grid;
    }
}


void downLine(BlockFall &game , std::vector<std::vector<int>> &tempGrid, int whichLine)
{
    for(int i = whichLine - 1 ; i > 0; i--)
    {
        for(int j = 0 ; j < game.grid[0].size(); j++)
        {
            if(game.grid[i][j] == 1)
            {
                game.grid[i][j] = 0;
                game.grid[i+1][j] = 1;
            }
            else
            {
                game.grid[i+1][j] = 0;
            }
        }
    }
    tempGrid = game.grid;
}

void checkForLine(BlockFall &game, std::vector<std::vector<int>> &tempGrid)
{
    int counter;
    for(int i = game.grid.size() - 1 ; i > 0 ; i--)
    {
        counter = 0;
        for(int j = 0 ; j < game.grid[0].size(); j++)
        {
            if(game.grid[i][j] == 1)
                counter++;
        }
        if(counter == game.grid[0].size())
        {
            beforeClearingGrid(game,tempGrid);
            downLine(game,tempGrid,i);
            game.current_score += game.cols ;
            i++;
        }
    }
}
void clearGrid(BlockFall &game,std::vector<std::vector<int>> &tempGrid)
{
    for(int i =0 ; i < game.rows; i++)
    {
        for(int j =0 ; j < game.cols; j++)
        {
            game.grid[i][j] = 0;
        }
    }
    tempGrid = game.grid;

}

void findPowerUp(BlockFall &game,std::vector<std::vector<int>> &tempGrid)
{
    int powerUpRow = game.power_up.size();
    int powerUpCol = game.power_up[0].size();
    int gridRow = game.rows;
    int gridCol = game.cols;

    for(int i = 0; i < gridRow - powerUpRow +1; i++)
    {
        for(int j = 0 ; j < gridCol - powerUpCol+ 1; j++)
        {
            int counter = 0;
            for(int a = 0; a < powerUpRow; a++)
            {
                for(int b = 0; b < powerUpCol; b++)
                {
                    if(game.grid[i+a][j+b] == game.power_up[a][b])
                        counter++;
                }
            }
            if(counter == gridRow*gridCol)
            {
                game.current_score += 1000;
                clearGrid(game,tempGrid);
            }
        }
    }
    std::cout<<"a";
}

void gameEnding(BlockFall &game)
{
    std::cout<<"Final grid and score:"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"Score: "<<game.current_score<<std::endl;
    if(game.leaderboard.head_leaderboard_entry == nullptr)
    {

    }
    else{
        std::cout<<"High Score: "<<game.leaderboard.head_leaderboard_entry->score<<std::endl;
    }

    for(int i =0 ; i < game.rows; i++)
    {
        for(int j = 0; j < game.cols; j++)
        {
            if(game.grid[i][j] == 1)
            {
                std::cout<<occupiedCellChar;
            }
            else{
                std::cout<<unoccupiedCellChar;
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"Leaderboard"<<std::endl;
    std::cout<<"-----------"<<std::endl;
    LeaderboardEntry *current = game.leaderboard.head_leaderboard_entry;
    int counter = 1;
    while(current != nullptr)
    {
        std::cout<<counter<<". "<<current->player_name<<" "<<current->score<<" "<<current->last_played<<std::endl;
        current = current->next_leaderboard_entry;
    }
}









bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    std::vector<std::string> commands = readLinesFromFile(commands_file);
    std::vector<std::vector<int>> tempGrid = game.grid;


    for(const auto& k : commands)
    {
        if(checkForCreation(game,tempGrid))
        {
            std::cout<<"GAME OVER!"<<std::endl;
            std::cout<<"Next block that couldn't fit:"<<std::endl;
            for(int i =0; i < game.active_rotation->shape.size();i++)
            {
                for(int j =0 ; j < game.active_rotation->shape[0].size(); j++)
                {
                    if(game.active_rotation->shape[i][j] == 1)
                    {
                        std::cout<<occupiedCellChar;
                    }
                    else{
                        std::cout<<unoccupiedCellChar;
                    }
                }
                std::cout<<std::endl;
            }
            std::cout<<std::endl;

            gameEnding(game);

            return false;
        }
        else if (k == "GRAVITY_SWITCH")
        {
            if(game.gravity_mode_on == true)
                game.gravity_mode_on = false;
            else{
                game.gravity_mode_on = true;
            }
            biAndaDropAcildi(game,tempGrid);
            checkForLine(game,tempGrid);
        }
        else if(k == "PRINT_GRID")
        {
            printGrid(game,tempGrid);
        }
        else if (k == "MOVE_RIGHT")
        {
            if(game.grid[0].size() > game.active_rotation->moveRight - game.active_rotation->moveLeft + game.active_rotation->shape[0].size())
                moveRight(game,tempGrid);
        }
        else if (k == "MOVE_LEFT")
        {
            if(game.active_rotation->moveRight - game.active_rotation->moveLeft > 0)
                moveLeft(game,tempGrid);
        }
        else if (k == "ROTATE_RIGHT")
        {
            rotateRight(game,tempGrid);
        }
        else if (k == "ROTATE_LEFT")
        {
            rotateLeft(game,tempGrid);
        }
        else if (k == "DROP")
        {
            createBlockOnGrid(game);
            drop(game,tempGrid);
            checkForLine(game,tempGrid);
            if(game.active_rotation == nullptr)
            {
                delete game.active_rotation;
                std::cout<<"YOU WIN!"<<std::endl;
                std::cout<<"No more blocks."<<std::endl;
                gameEnding(game);
                return true;
            }
        }
        else
        {
            std::cout<<"Unknown command: GIMME_POINTS"<<std::endl;
        }

    }

    std::cout<<"GAME FINISHED!"<<std::endl;
    std::cout<<"No more commands."<<std::endl;
    gameEnding(game);



    return true;
}



