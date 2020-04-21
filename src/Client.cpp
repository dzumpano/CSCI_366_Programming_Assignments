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
#include "Client.hpp"

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    this->player = player; //so we can call player later
    string actBoard = "player_" + to_string(player)+ ".action_board.json"; //action board


    vector<vector<int>> board(board_size, vector<int> (board_size, 0)); //   board: [
    ofstream output;                                                    //              [
    output.open(actBoard, ofstream::out);                               //                  0
    if(output){ //cereal stuff                                          //                  0
        {                                                                  //           ]
            cereal::JSONOutputArchive arc(output);                         //etc    [
            arc(CEREAL_NVP(board));                                                 //0
        }
        output.close();
        initialized = true;
    }
}


void Client::fire(unsigned int x, unsigned int y) {

    string fName = "player_" + to_string(player) + ".shot.json";

    ofstream file;
    file.open(fName, ofstream::out);
    if(file) { //writing it out
        {
            cereal::JSONOutputArchive arc(file);
            arc(CEREAL_NVP(x));
            arc(CEREAL_NVP(y));
        }
        file.close();
    }

}


bool Client::result_available() {
    ifstream p1_result_file("player_1.result.json");
    ifstream p2_result_file("player_2.result.json");
    if (p1_result_file || p2_result_file)
    {
        return true;
    }
    else{
        return false;
    }
}


int Client::get_result() {  //pretty much cut and pasted fire and then switched all things out to in

    string fName = "player_" + to_string(player) + ".result.json";
    ifstream file;
    file.open(fName, ifstream::in);
    int res;
    remove(fName.c_str()); //cleanup
    if(file) {
        {
            cereal::JSONInputArchive arc(file);
            arc(res);
        }
        file.close();
    }
    if(res > 1 || res < -1){
        throw new ClientException("Bad Result");
    }
    return res;
}


void Client::update_action_board(int result, unsigned int x, unsigned int y) {

    //Had to tinker with this a little to not get a runtime
    //error for some reason, even though my pa1 code was fine
    string fName = "player_"+to_string(player)+".action_board.json";
    ifstream file(fName);
    vector< vector<int>> board;
    if(file) {
        {
            cereal::JSONInputArchive arc(file);
            arc(board);
        }
    }
    file.close();

    board[y][x] = result;
    ofstream os(fName);
    //Basically just took this part out of brackets
    cereal::JSONOutputArchive arc2(os);
    arc2(cereal::make_nvp("board", board));

}

string Client::render_action_board(){
    //added this
    string board;

    string fName = "player_"+to_string(player)+".action_board.json";
        ofstream output("player_1.action_board.json");
        cereal::JSONOutputArchive arc(output);
        arc(cereal::make_nvp("board", board));
        output.close();

    //prints
    //cout << board
    return board;
}