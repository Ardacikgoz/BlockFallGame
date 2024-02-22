#include "BlockFall.h"


BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}
std::vector<std::vector<bool>> rightRotaion1(std::vector<std::vector<bool>> shape)
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
std::vector<std::string> readLinesFromFile1(const std::string& fileName) {
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


std::vector<std::vector<bool>> stringToBool2DVector(const std::string& input) {
    std::vector<std::vector<bool>> bool2DVector;

    std::vector<bool> currentVector;
    int counter = 0;
    for (char ch : input) {
        if (ch == '0' || ch == '1') {
            currentVector.push_back(ch == '1');
        }
        else if (ch == '\r') {
            if(counter == 0){
                continue;
            }
            else{
                bool2DVector.push_back(currentVector);
                currentVector.clear();
            }
        }
        counter++;
    }
    bool2DVector.push_back(currentVector);
    return bool2DVector;
}

void fonsk(Block* &initial_block,std::vector<std::vector<bool>> shape, Block* beforeBlock)
{

    initial_block = new Block;
    initial_block->shape = shape;
    std::vector<Block*> pointers;
    int i = 0;
    pointers.push_back(initial_block);
    while(initial_block->shape != rightRotaion1(pointers[i]->shape))
    {
        Block *temp = new Block;
        temp->shape = rightRotaion1(pointers[i]->shape);
        pointers[i]->right_rotation = temp;
        temp->left_rotation = pointers[i];
        pointers.push_back(temp);
        i++;
    }
    pointers[i]->right_rotation = initial_block;
    initial_block->left_rotation = pointers[i];

    if(beforeBlock != nullptr)
    {
        Block* control = beforeBlock->right_rotation;
        beforeBlock->next_block = initial_block;
        beforeBlock->next_block = initial_block;
        while(beforeBlock->shape != control->shape)
        {
            control->next_block = initial_block;
            control = control->right_rotation;
        }
    }
}



void fonsk2(Block* &head,std::vector<std::string> input)
{
    int row = 0;
    int col = 0;

    std::vector<Block*> pointers;
    int counter = 0;
    while(row < input.size())
    {
        if(input[row] == "\r" || input[row] == "\n")
        {
            row++;
            continue;
        }
        std::string sentence = "";
        while(col < input[row].size() || col == input[row].size())
        {
            if(static_cast<int> (input[row][col]) == 91)
            {
                col++;
            }
            else if (static_cast<int>(input[row][col] == 93))
            {
                break;
            }
            else if(static_cast<int> (input[row][col]) != 91 && col < input[row].size())
            {
                sentence += input[row][col++];
            }
            else if(col == input[row].size())
            {
                sentence += "\r";
                col = 0;
                row++;
            }
        }
        col = 0;
        row++;
        if(pointers.size() == 0)
        {
            fonsk(head,stringToBool2DVector(sentence), nullptr);
            pointers.push_back(head);
        }
        else
        {
            Block *temp = nullptr;
            fonsk(temp, stringToBool2DVector(sentence),pointers[counter]);
            pointers.push_back(temp);
            counter++;
        }
    }

}


void BlockFall::read_blocks(const string &input_file){
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)

    std::vector<std::string> input = readLinesFromFile1(input_file);
    int row = 0;
    int col = 0;
    fonsk2(initial_block,input);
    Block* temp = initial_block;
    Block* prev = nullptr;
    active_rotation = initial_block;

    while(temp->next_block != nullptr)
    {
        prev = temp;
        temp = prev->next_block;
    }
    prev->next_block = nullptr;
    power_up = temp->shape;
    delete temp;


}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main


    std::vector<std::string> input = readLinesFromFile1(input_file);

    rows = input.size();
    cols = (input[0].size() + 1) / 2;

    for(int i = 0; i < rows ; i++)
    {
        std::vector<int> nowRow;
        for(int j = 0; j < input[0].size(); j++)
        {
            if(static_cast<int>(input[i][j]) == 48)
            {
                nowRow.push_back(0);
            }
            else if(static_cast<int>(input[i][j]) == 49)
            {
                nowRow.push_back(1);
            }
        }
        grid.push_back(nowRow);
    }
}



BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    while(initial_block != nullptr)
    {
        while(initial_block != initial_block->right_rotation)
        {
            Block* temp = initial_block->right_rotation;
            initial_block->right_rotation = temp->right_rotation;
            delete temp;
        }
        Block* temp = initial_block;
        initial_block = initial_block->next_block;
        delete temp;
    }
    delete initial_block;
}



