//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include "common.hpp"
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(ifstream *file){
}


void Server::initialize(unsigned int board_size, string p1_setup_board, string p2_setup_board){
    this->board_size = board_size; //so we can use this later
    if(board_size != BOARD_SIZE){
        throw new ServerException("Wrong Board Size");
    }                                                   //for empty file names
    if(p1_setup_board.length() == 0 && p2_setup_board.length() == 0){
        throw new ServerException("Invalid file name");
    }

}


Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){

}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if(player > 2 || player < 1){
        throw new ServerException("Wrong player number!");
    }
    if(x > BOARD_SIZE || y > BOARD_SIZE){
        return OUT_OF_BOUNDS;
    }

    string player_board;
    if(player == 1){ //player 1s shot lands on player 2s board
        player_board = "player_2.setup_board.txt";
    }else{
        player_board = "player_1.setup_board.txt";
    }

    ifstream in;

    char temp;

    cout<< board_size;
    in.open(player_board);

    //the logic that parses the file in and evaluates if the shots a hit or miss
    for (int col = 0; col < board_size; col++) { //nested for loop
        for (int row = 0; row < board_size; row++) { //made more sense than a vector to me.
            in >> temp;
            //cout << temp;
            if (x == row && y == col) {
                //cout << x;
                //cout << y;
                //cout << row;
                // cout << col;
                // cout << temp;
                if (temp != '_') {
                    return HIT;
                } else {
                    return MISS;
                }
            }

        }
    }



    in.close();
};


int Server::process_shot(unsigned int player) { //once again, cut and pasted from my other functions.
    string player_board;
    string fName;

    if(player == 1){ //fName is the name of the shot file, player board is the name of the result file
        fName = "player_1.shot.json";
        player_board = "player_1.result.json";
    }else{
        fName = "player_2.shot.json";
        player_board = "player_2.result.json";
    }
    //shot = read
    //result = write
    int x;
    int y;
    //DESERIALIZE
    ifstream file1;
    file1.open(fName, ifstream::in);
    if(file1) {
        {
            cereal::JSONInputArchive arc(file1);
            arc(CEREAL_NVP(x));
            arc(CEREAL_NVP(y));
        }
        file1.close();
    }
    remove(fName.c_str()); //for the clean up test


    int result = evaluate_shot(player, x, y); //either hit, miss, or OOB.
    //SERIALIZE
    ofstream file;
    file.open(player_board, ofstream::out);
    if(file){
        {
            cereal::JSONOutputArchive arc(file);
            arc(CEREAL_NVP(result));
        }
        file.close();
        return SHOT_FILE_PROCESSED;
    }
    else{
        return NO_SHOT_FILE;
    }

}