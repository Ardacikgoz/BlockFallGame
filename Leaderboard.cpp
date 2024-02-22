#include "Leaderboard.h"
#include "LeaderboardEntry.h"
#include <iostream>
#include <fstream>
#include <string>



void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).

    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry* current = head_leaderboard_entry;
        while (current->next_leaderboard_entry && new_entry->score <= current->next_leaderboard_entry->score) {
            current = current->next_leaderboard_entry;
        }

        new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
        current->next_leaderboard_entry = new_entry;
    }
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: a" << filename << std::endl;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;

    while (current != nullptr) {
        file << current->score << " " << current->last_played << " " << current->player_name << std::endl;
        current = current->next_leaderboard_entry;
    }

    delete current;

    file.close();
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    std::string name;
    int score;
    time_t timestamp;
    while (file >> name >> score >> timestamp) {
        if(name.empty())
        {
            continue;
        }
        else
        {
            LeaderboardEntry* new_entry = new LeaderboardEntry(score, timestamp, name);
            insert_new_entry(new_entry);
        }
    }

    file.close();

}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    LeaderboardEntry* current = head_leaderboard_entry;

    std::cout << "Leaderboard:" << std::endl;

    while (current != nullptr) {
        std::cout << "Score: " << current->score
                  << " | Last Played: " << current->last_played
                  << " | Player Name: " << current->player_name << std::endl;
        current = current->next_leaderboard_entry;
    }

    delete current;
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* next;

    while (current != nullptr) {
        next = current->next_leaderboard_entry;
        delete current;
        current = next;
    }

    head_leaderboard_entry = nullptr;

}
